#include "Proximity.h"
#ifndef DEF_SOLVER
#define DEF_SOLVER


//********************************************//
//** EULER STEP ******************************//
//********************************************//

void Euler_step(
	int Nb_bodies ,
	vector<Body>& Bodies ,
	int Nb_materials ,
	vector<Material> Materials ,
	int Nb_contact_laws ,
	vector<Contact_law> Contact_laws ,
	vector<vector<int>>& Contacts_Table ,
	double Tend ,
	double Xmin_period ,
	double Xmax_period ,
	double Penalty ,
	double Xgravity ,
	double Ygravity ,
	double& Time ,
	double Deltat ,
	int& Number_iteration )
{
	cout << endl ;
	cout << "Step " << Number_iteration << " ; Simulated time : " << Time << " s" << endl ;

	// Loops on bodies
	for (int i(0) ; i<Nb_bodies ; i++)
	{
		Bodies[i].Initialize_contact_forces() ;
		Bodies[i].Update_bc(Time) ;
		Bodies[i].Update_borders(Xmin_period , Xmax_period) ;
		Bodies[i].Update_contacts(Bodies , Xmin_period , Xmax_period) ;
	}
    for (int i(0) ; i<Nb_bodies ; i++)
    {
        Bodies[i].Update_bc_forces() ;
        Bodies[i].Update_body_forces(Xgravity , Ygravity) ;
        for (int region=0. ; region<Bodies[i].nb_regions ; region++)
        {
            Bodies[i].Update_internal_forces(region , Nb_materials , Materials) ;
        }
        Bodies[i].Update_damping_forces() ;
        Bodies[i].Update_contact_forces(Deltat , Bodies , Nb_contact_laws , Contact_laws , Contacts_Table , Xmin_period , Xmax_period) ;
    }
	for (int i(0) ; i<Nb_bodies ; i++)
	{
		Bodies[i].Sum_up_forces() ;
		Bodies[i].Apply_Newton() ;
		Bodies[i].Apply_Euler(Deltat) ;
		Bodies[i].Update_kinematics() ;
		Bodies[i].Update_current_positions() ;
	}
}

//********************************************//
//** ADAPTIVE EULER STEP *********************//
//********************************************//

void Adaptive_Euler_step(
	int Nb_bodies ,
	vector<Body>& Bodies ,
	int Nb_regions ,
	vector<vector<int>> Regions ,
	int Nb_materials ,
	vector<Material> Materials ,
	int Nb_contact_laws ,
	vector<Contact_law> Contact_laws ,
	vector<vector<int>>& Contacts_Table ,
	double Tend ,
	double Xmin_period ,
	double Xmax_period ,
	double Penalty ,
	double Xgravity ,
	double Ygravity ,
	double& Time ,
	double& Deltat ,
	int& Number_iteration ,
	double& Next_contact_update ,
	double& Contact_update_period ,
	double Target_error ,
	double Control_parameter ,
	double Accepted_ratio ,
	int& neval ,
	double& max_error ,
	double& mean_error ,
	vector<int>& flags)
{
    if (flags[7]==0)                                                                                    // SEQUENTIAL //
    {
        cout << endl ;                                                                                  // SEQUENTIAL //
        cout << Number_iteration << " ; t " << Time ;                                                   // SEQUENTIAL //
    }
    #pragma omp parallel
    {
        #pragma omp for schedule(dynamic)
        for (int i=0 ; i<Nb_bodies ; i++) Bodies[i].Store() ;
    }
	//int nthreads = omp_get_num_thread();
	//double nnodes = 0. ;
	//double ngauss = 0. ;
	//double nbnodes = 0. ;
    //for (int i=0 ; i<Nb_bodies ; i++)
    //{
    //    nnodes += Bodies[i].nb_nodes ;
    //    ngauss += Bodies[i].nb_gauss ;
    //    nbnodes += Bodies[i].nb_border_nodes ;
    //}
    //double nnodes_per_thread = nnodes / nthreads ;
    //double ngauss_per_thread = ngauss / nthreads ;
    //double nbnodes_per_thread = nbnodes / nthreads ;
	int bodyformax, nodeformax ;                                                                        // SEQUENTIAL //
	int flag_success = 0 ;                                                                              // SEQUENTIAL //
	neval = 0 ;                                                                                         // SEQUENTIAL //
    while (flag_success == 0)
    {
        neval++ ;                                                                                       // SEQUENTIAL //
        #pragma omp parallel
        {
            #pragma omp for schedule(dynamic)
            for (int i=0 ; i<Nb_bodies ; i++)
            {
                if (Bodies[i].status == "inactive") continue ;
                //cout << "00 " << i << endl ;
                Bodies[i].Initialize_contact_forces() ;
                //cout << "01 " << i << endl ;
                Bodies[i].Update_bc(Time) ;
                //cout << "02 " << i << endl ;
                Bodies[i].Update_borders(Xmin_period , Xmax_period) ;
                //cout << "03 " << i << endl ;
                Bodies[i].Update_contacts(Bodies , Xmin_period , Xmax_period) ;
                //cout << "04 " << i << endl ;
            }
            #pragma omp for schedule(dynamic)
            for (int i=0 ; i<Nb_bodies ; i++)
            {

                if (Bodies[i].status == "inactive") continue ;
                //cout << "05 " << i << endl ;
                Bodies[i].Update_bc_forces() ;
                //cout << "06 " << i << endl ;
                Bodies[i].Update_body_forces(Xgravity , Ygravity) ;
                //cout << "07 " << i << endl ;
                Bodies[i].Update_damping_forces() ;
                //cout << "08 " << i << endl ;
                Bodies[i].Update_alid_forces(Time) ;
                //cout << "09 " << i << endl ;
                Bodies[i].Update_contact_forces(Deltat , Bodies , Nb_contact_laws , Contact_laws , Contacts_Table , Xmin_period , Xmax_period) ;
                //cout << "10 " << i << endl ;
            }
            #pragma omp for schedule(dynamic)
            for (int i=0 ; i<Nb_regions ; i++)
            {
                if (Bodies[Regions[i][0]].status == "inactive") continue ;
                //cout << "11 " << i << endl ;
                Bodies[Regions[i][0]].Update_internal_forces(Regions[i][1] , Nb_materials , Materials) ;
                //cout << "12 " << i << endl ;
            }
            #pragma omp for schedule(dynamic)
            for (int i=0 ; i<Nb_bodies ; i++)
            {
                if (Bodies[i].status == "inactive") continue ;
                //cout << "13 " << i << endl ;
                Bodies[i].Sum_up_forces() ;
                //cout << "14 " << i << endl ;
                Bodies[i].Apply_Newton() ;
                //cout << "15 " << i << endl ;
                Bodies[i].Apply_Euler_temporary(Deltat) ;
                //cout << "16 " << i << endl ;
                Bodies[i].Update_kinematics_temporary() ;
                //cout << "17 " << i << endl ;
                Bodies[i].Apply_Euler(Deltat*0.5) ;
                //cout << "18 " << i << endl ;
                Bodies[i].Update_kinematics() ;
                //cout << "19 " << i << endl ;
                Bodies[i].Update_current_positions() ;
                //cout << "20 " << i << endl ;
            }
            #pragma omp for schedule(dynamic)
            for (int i=0 ; i<Nb_bodies ; i++)
            {
                if (Bodies[i].status == "inactive") continue ;
                //cout << "21 " << i << endl ;
                Bodies[i].Initialize_contact_forces() ;
                //cout << "22 " << i << endl ;
                Bodies[i].Update_bc(Time+Deltat*0.5) ;
                //cout << "23 " << i << endl ;
                Bodies[i].Update_borders(Xmin_period , Xmax_period) ;
                //cout << "24 " << i << endl ;
                Bodies[i].Update_contacts(Bodies , Xmin_period , Xmax_period) ;
                //cout << "25 " << i << endl ;
            }
            #pragma omp for schedule(dynamic)
            for (int i=0 ; i<Nb_bodies ; i++)
            {
                if (Bodies[i].status == "inactive") continue ;
                //cout << "26 " << i << endl ;
                Bodies[i].Update_bc_forces() ;
                //cout << "27 " << i << endl ;
                Bodies[i].Update_body_forces(Xgravity , Ygravity) ;
                //cout << "28 " << i << endl ;
                Bodies[i].Update_damping_forces() ;
                //cout << "29 " << i << endl ;
                Bodies[i].Update_alid_forces(Time+Deltat*0.5) ;
                //cout << "30 " << i << endl ;
                Bodies[i].Update_contact_forces(Deltat , Bodies , Nb_contact_laws , Contact_laws , Contacts_Table , Xmin_period , Xmax_period) ;
                //cout << "31 " << i << endl ;
            }

            #pragma omp for schedule(dynamic)
            for (int i=0 ; i<Nb_regions ; i++)
            {
                if (Bodies[Regions[i][0]].status == "inactive") continue ;
                //cout << "32 " << i << endl ;
                Bodies[Regions[i][0]].Update_internal_forces(Regions[i][1] , Nb_materials , Materials) ;
                //cout << "33 " << i << endl ;
            }
            #pragma omp for schedule(dynamic)
            for (int i=0 ; i<Nb_bodies ; i++)
            {
                if (Bodies[i].status == "inactive") continue ;
                //cout << "34 " << i << endl ;
                Bodies[i].Sum_up_forces() ;
                //cout << "35 " << i << endl ;
                Bodies[i].Apply_Newton() ;
                //cout << "36 " << i << endl ;
                Bodies[i].Apply_Euler(Deltat*0.5) ;
                //cout << "37 " << i << endl ;
                Bodies[i].Update_kinematics() ;
                //cout << "38 " << i << endl ;
                Bodies[i].Update_current_positions() ;
                //cout << "39 " << i << endl ;
                Bodies[i].Compute_error() ;
                //cout << "40 " << i << endl ;
            }
        }
        mean_error = 0. ;                                                                               // SEQUENTIAL //
        max_error = 0. ;                                                                                // SEQUENTIAL //
        int nnodes = 0 ;                                                                                // SEQUENTIAL //
        for (int i=0 ; i<Nb_bodies ; i++)                                                               // SEQUENTIAL //
        {
            if (Bodies[i].status == "inactive") continue ;                                              // SEQUENTIAL //
            mean_error += Bodies[i].total_error ;                                                       // SEQUENTIAL //
            nnodes += Bodies[i].nb_nodes ;                                                              // SEQUENTIAL //
            if (Bodies[i].max_error > max_error)                                                        // SEQUENTIAL //
            {
                max_error = Bodies[i].max_error ;                                                       // SEQUENTIAL //
                bodyformax = i ;                                                                        // SEQUENTIAL //
                nodeformax = Bodies[i].node_for_max_error ;                                             // SEQUENTIAL //
            }
        }
        mean_error /= nnodes ;                                                                          // SEQUENTIAL //

        if (flags[7]==0) cout << " ; dt " << Deltat << " ; err " << max_error << " -" << bodyformax << "-" << nodeformax << "- (" << mean_error << ")" << endl ;
        double error = max_error ;                                                                      // SEQUENTIAL //
        if (error > Target_error * Accepted_ratio)                                                      // SEQUENTIAL //
        {
            flag_success = 0 ;                                                                          // SEQUENTIAL //
            #pragma omp parallel
            {
                #pragma omp for schedule(dynamic)
                for (int i=0 ; i<Nb_bodies ; i++) Bodies[i].Restore() ;
            }
            double correction1 = error / Target_error ;                                                 // SEQUENTIAL //
            double correction2 = 1. / Accepted_ratio ;                                                  // SEQUENTIAL //
            if (correction1>correction2)    Deltat = Deltat / pow(correction1 , Control_parameter) ;    // SEQUENTIAL //
            else                            Deltat = Deltat / pow(correction2 , Control_parameter) ;    // SEQUENTIAL //
            if (Deltat>Contact_update_period)   Deltat = Contact_update_period ;                        // SEQUENTIAL //
            if (flags[7]==0) cout << "                 Failed" ;                                        // SEQUENTIAL //
        }
        else                                                                                            // SEQUENTIAL //
        {
            //cout << "41" << endl ;
            flag_success = 1 ;                                                                          // SEQUENTIAL //
            Time += Deltat ;                                                                            // SEQUENTIAL //
            double correction1 = error / Target_error ;                                                 // SEQUENTIAL //
            double correction2 = 1. / Accepted_ratio ;                                                  // SEQUENTIAL //
            if (correction1>correction2)    Deltat = Deltat / pow(correction1 , Control_parameter) ;    // SEQUENTIAL //
            else                            Deltat = Deltat / pow(correction2 , Control_parameter) ;    // SEQUENTIAL //
            if (Deltat>Contact_update_period)   Deltat = Contact_update_period ;                        // SEQUENTIAL //
            //cout << "42" << endl ;
            Update_contact_pressures( Nb_bodies , Bodies ) ;                                            // SEQUENTIAL //
            //cout << "43" << endl ;
        }
	}
}

#endif