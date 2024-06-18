#ifndef DEF_GRAPHIC
#define DEF_GRAPHIC

//********************************************//
//** PROTOTYPES ******************************//
//********************************************//
void Shift_body_graphic(int i,
                        vector<Body>& Bodies,
                        double xmin,
                        double xmax,
                        vector<double>& x,
                        vector<double>& y,
                        vector<int>& indices,
                        vector<vector<int>>& cells,
                        int& n,
                        int& nc) ;

//********************************************//
//** WRITE GRAINS ****************************//
//********************************************//
void Write_grains(int Nb_bodies,
                  vector<Body>& Bodies,
                  int Number_iteration,
                  int Number_save,
                  int Number_print,
                  double Time,
                  double Xmin_period,
                  double Xmax_period,
                  int Nb_materials,
                  vector<Material>& Materials,
                  vector<int> To_Plot)
{
    cout << endl ;
    cout << "Writing grains file " << Number_print << endl ;
    string filename ;
    stringstream sfilename ;
    sfilename << Number_print;
    if      (Number_print<10)
        filename="GRAINS_0000"+sfilename.str()+".vtk" ;
    else if (Number_print<100)
        filename="GRAINS_000"+sfilename.str()+".vtk" ;
    else if (Number_print<1000)
        filename="GRAINS_00"+sfilename.str()+".vtk" ;
    else if (Number_print<10000)
        filename="GRAINS_0"+sfilename.str()+".vtk" ;
    else if (Number_print<100000)
        filename="GRAINS_"+sfilename.str()+".vtk" ;
    ofstream Graphic_file (filename) ;

    Graphic_file << setprecision (12) ;
    Graphic_file << "# vtk DataFile Version 2.0" << endl ;
    Graphic_file << "MELODY 2D grains file ; Iteration " << Number_iteration
                 << " ; Save " << Number_save
                 << " ; Print " << Number_print
                 << " ; Time " << Time << endl ;
    Graphic_file << "ASCII" << endl ;
    Graphic_file << endl ;
    Graphic_file << "DATASET POLYDATA" << endl ;
    Graphic_file << endl ;

    vector<double> xtot, ytot ;
    vector<vector<int>> indicestot ;
    vector<vector<int>> cellstot ;
    int ntot=0, nctot=0 ;
    int n=0, nc=0 ;
    for (int i=0 ; i<Nb_bodies ; i++)
    {
        if (Bodies[i].status == "inactive")
            continue ;
        vector<double> x, y ;
        vector<int> indices ;
        vector<vector<int>> cells ;
        Shift_body_graphic(i, Bodies, Xmin_period, Xmax_period, x, y, indices, cells, n, nc) ;
        for (int j(0) ; j<n ; j++)
        {
            xtot.push_back( x[j] ) ;
            ytot.push_back( y[j] ) ;
            indicestot.push_back({ i, indices[j] }) ;
        }
        for (int j(0) ; j<nc ; j++)
        {
            cellstot.push_back({ cells[j][0]+ntot, cells[j][1]+ntot, cells[j][2]+ntot }) ;
        }
        ntot = ntot + n ;
        nctot = nctot + nc ;
    }
    #pragma omp parallel
    {
        #pragma omp for schedule(dynamic)
        for (int i=0 ; i<Nb_bodies ; i++)
            Bodies[i].Compute_nodal_stresses( Nb_materials, Materials ) ;
    }
    vector<vector<int>> segmentstot(3*nctot) ;
    for (int i=0 ; i<nctot ; i++)
    {
        if (cellstot[i][0]<cellstot[i][1])
            segmentstot[0+3*i] = {cellstot[i][0], cellstot[i][1]} ;
        else
            segmentstot[0+3*i] = {cellstot[i][1], cellstot[i][0]} ;
        if (cellstot[i][1]<cellstot[i][2])
            segmentstot[1+3*i] = {cellstot[i][1], cellstot[i][2]} ;
        else
            segmentstot[1+3*i] = {cellstot[i][2], cellstot[i][1]} ;
        if (cellstot[i][0]<cellstot[i][2])
            segmentstot[2+3*i] = {cellstot[i][0], cellstot[i][2]} ;
        else
            segmentstot[2+3*i] = {cellstot[i][2], cellstot[i][0]} ;
    }
    sort( segmentstot.begin(), segmentstot.end() );
    vector<vector<int>> borders ;
    for (int i=1 ; i<3*nctot-1 ; i++)
    {
        if ( (segmentstot[i]!=segmentstot[i-1]) &&
                (segmentstot[i]!=segmentstot[i+1]) )
        {
            borders.push_back(segmentstot[i]) ;
        }
    }

    vector<int> bordernodestot(2*(int)borders.size()) ;
    for (int i=0 ; i<(int)borders.size() ; i++)
    {
        bordernodestot[2*i] = borders[i][0] ;
        bordernodestot[1+2*i] = borders[i][1] ;
    }
    sort( bordernodestot.begin(), bordernodestot.end() );
    vector<int> bordernodes ;
    for (int i=1 ; i<2*(int)borders.size() ; i++)
    {
        if (bordernodestot[i]!=bordernodestot[i-1])
            bordernodes.push_back(bordernodestot[i]) ;
    }

    double xvar, yvar ;
    Graphic_file << "POINTS " << ntot << " float" << endl ;
    for (int j(0) ; j<ntot ; j++)
    {
        if ( abs(xtot[j]) < 1.e-16 )
            xvar = 0. ;
        else
            xvar = xtot[j] ;
        if ( abs(ytot[j]) < 1.e-16 )
            yvar = 0. ;
        else
            yvar = ytot[j] ;
        Graphic_file << xvar << ' '
                     << yvar << ' '
                     << '0' << endl ;
    }
    Graphic_file << endl ;

    Graphic_file << setprecision (6) ;
    Graphic_file << "POLYGONS " << nctot << ' ' << nctot*4 << endl ;
    for (int j(0) ; j<nctot ; j++)
    {
        Graphic_file << "3 "
                     << cellstot[j][0] << ' '
                     << cellstot[j][1] << ' '
                     << cellstot[j][2] << endl ;
    }
    Graphic_file << endl ;

    int bo, no ;
    Graphic_file << "POINT_DATA " << ntot << endl ;

    if ( To_Plot[0] == 1 )
    {
        Graphic_file << "SCALARS 00_Body_index float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            Graphic_file << bo << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[1] == 1 )
    {
        Graphic_file << "SCALARS 01_Initial_position float 2" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].x_initial) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].x_initial ;
            if ( abs(Bodies[bo].nodes[no].y_initial) < 1.e-16 )
                yvar = 0. ;
            else
                yvar = Bodies[bo].nodes[no].y_initial ;
            Graphic_file << xvar << ' '
                         << yvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[2] == 1 )
    {
        Graphic_file << "SCALARS 02_Current_position float 2" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].x_current) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].x_current ;
            if ( abs(Bodies[bo].nodes[no].y_current) < 1.e-16 )
                yvar = 0. ;
            else
                yvar = Bodies[bo].nodes[no].y_current ;
            Graphic_file << xvar << ' '
                         << yvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[3] == 1 )
    {
        Graphic_file << "SCALARS 03_Displacement float 2" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].x_displacement) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].x_displacement ;
            if ( abs(Bodies[bo].nodes[no].y_displacement) < 1.e-16 )
                yvar = 0. ;
            else
                yvar = Bodies[bo].nodes[no].y_displacement ;
            Graphic_file << xvar << ' '
                         << yvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[4] == 1 )
    {
        Graphic_file << "SCALARS 04_Velocity float 2" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].x_velocity) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].x_velocity ;
            if ( abs(Bodies[bo].nodes[no].y_velocity) < 1.e-16 )
                yvar = 0. ;
            else
                yvar = Bodies[bo].nodes[no].y_velocity ;
            Graphic_file << xvar << ' '
                         << yvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[5] == 1 )
    {
        Graphic_file << "SCALARS 05_Acceleration float 2" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].x_acceleration) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].x_acceleration ;
            if ( abs(Bodies[bo].nodes[no].y_acceleration) < 1.e-16 )
                yvar = 0. ;
            else
                yvar = Bodies[bo].nodes[no].y_acceleration ;
            Graphic_file << xvar << ' '
                         << yvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[6] == 1 )
    {
        Graphic_file << "SCALARS 06_Force float 2" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].x_force) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].x_force ;
            if ( abs(Bodies[bo].nodes[no].y_force) < 1.e-16 )
                yvar = 0. ;
            else
                yvar = Bodies[bo].nodes[no].y_force ;
            Graphic_file << xvar << ' '
                         << yvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[7] == 1 )
    {
        Graphic_file << "SCALARS 07_Internal_force float 2" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].x_internal_force) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].x_internal_force ;
            if ( abs(Bodies[bo].nodes[no].y_internal_force) < 1.e-16 )
                yvar = 0. ;
            else
                yvar = Bodies[bo].nodes[no].y_internal_force ;
            Graphic_file << xvar << ' '
                         << yvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[8] == 1 )
    {
        Graphic_file << "SCALARS 08_Contact_force float 2" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].x_contact_force) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].x_contact_force ;
            if ( abs(Bodies[bo].nodes[no].y_contact_force) < 1.e-16 )
                yvar = 0. ;
            else
                yvar = Bodies[bo].nodes[no].y_contact_force ;
            Graphic_file << xvar << ' '
                         << yvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[9] == 1 )
    {
        Graphic_file << "SCALARS 09_Body_force float 2" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].x_body_force) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].x_body_force ;
            if ( abs(Bodies[bo].nodes[no].y_body_force) < 1.e-16 )
                yvar = 0. ;
            else
                yvar = Bodies[bo].nodes[no].y_body_force ;
            Graphic_file << xvar << ' '
                         << yvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[10] == 1 )
    {
        Graphic_file << "SCALARS 10_Dirichlet_force float 2" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].x_dirichlet_force) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].x_dirichlet_force ;
            if ( abs(Bodies[bo].nodes[no].y_dirichlet_force) < 1.e-16 )
                yvar = 0. ;
            else
                yvar = Bodies[bo].nodes[no].y_dirichlet_force ;
            Graphic_file << xvar << ' '
                         << yvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[11] == 1 )
    {
        Graphic_file << "SCALARS 11_Neumann_force float 2" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].x_neumann_force) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].x_neumann_force ;
            if ( abs(Bodies[bo].nodes[no].y_neumann_force) < 1.e-16 )
                yvar = 0. ;
            else
                yvar = Bodies[bo].nodes[no].y_neumann_force ;
            Graphic_file << xvar << ' '
                         << yvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[12] == 1 )
    {
        Graphic_file << "SCALARS 12_Damping_force float 2" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].x_damping_force) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].x_damping_force ;
            if ( abs(Bodies[bo].nodes[no].y_damping_force) < 1.e-16 )
                yvar = 0. ;
            else
                yvar = Bodies[bo].nodes[no].y_damping_force ;
            Graphic_file << xvar << ' '
                         << yvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[13] == 1 )
    {
        Graphic_file << "SCALARS 13_Alid_force float 2" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].x_alid_force) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].x_alid_force ;
            if ( abs(Bodies[bo].nodes[no].y_alid_force) < 1.e-16 )
                yvar = 0. ;
            else
                yvar = Bodies[bo].nodes[no].y_alid_force ;
            Graphic_file << xvar << ' '
                         << yvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[14] == 1 )
    {
        Graphic_file << "SCALARS 14_Jacobian float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            Graphic_file << Bodies[bo].nodes[no].jacobian << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[15] == 1 )
    {
        Graphic_file << "SCALARS 15_Cauchy_XX_stress float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].Sigmaxx) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].Sigmaxx ;
            Graphic_file << xvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[16] == 1 )
    {
        Graphic_file << "SCALARS 16_Cauchy_YY_stress float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].Sigmayy) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].Sigmayy ;
            Graphic_file << xvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[17] == 1 )
    {
        Graphic_file << "SCALARS 17_Cauchy_XY_stress float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].Sigmaxy) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].Sigmaxy ;
            Graphic_file << xvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[18] == 1 )
    {
        Graphic_file << "SCALARS 18_Cauchy_ZZ_stress float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].Sigmazz) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].Sigmazz ;
            Graphic_file << xvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[19] == 1 )
    {
        Graphic_file << "SCALARS 19_Tresca_stress float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].SigmaTresca) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].SigmaTresca ;
            Graphic_file << xvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[20] == 1 )
    {
        Graphic_file << "SCALARS 20_Von_Mises_stress float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].SigmaVM) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].SigmaVM ;
            Graphic_file << xvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[21] == 1 )
    {
        Graphic_file << "SCALARS 21_Major_principal_stress float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].SigmaI) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].SigmaI ;
            Graphic_file << xvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[22] == 1 )
    {
        Graphic_file << "SCALARS 22_Intermediate_principal_stress float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].SigmaII) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].SigmaII ;
            Graphic_file << xvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[23] == 1 )
    {
        Graphic_file << "SCALARS 23_Minor_principal_stress float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].SigmaIII) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].SigmaIII ;
            Graphic_file << xvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[24] == 1 )
    {
        Graphic_file << "SCALARS 24_Spherical_stress float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].SigmaSph) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].SigmaSph ;
            Graphic_file << xvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[25] == 1 )
    {
        Graphic_file << "SCALARS 25_Green-Lagrange_XX_Strain float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].Exx) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].Exx ;
            Graphic_file << xvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[26] == 1 )
    {
        Graphic_file << "SCALARS 26_Green-Lagrange_YY_Strain float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].Eyy) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].Eyy ;
            Graphic_file << xvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[27] == 1 )
    {
        Graphic_file << "SCALARS 27_Green-Lagrange_XY_Strain float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].Exy) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].Exy ;
            Graphic_file << xvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[28] == 1 )
    {
        Graphic_file << "SCALARS 28_Green-Lagrange_Norm float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            if ( abs(Bodies[bo].nodes[no].NormE) < 1.e-16 )
                xvar = 0. ;
            else
                xvar = Bodies[bo].nodes[no].NormE ;
            Graphic_file << xvar << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[29] == 1 )
    {
        Graphic_file << "SCALARS 29_Body_Damage float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            Graphic_file << Bodies[bo].damage << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[30] == 1 )
    {
        Graphic_file << "SCALARS 30_Body_Relative_Damage float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            Graphic_file << (Bodies[bo].damage-Bodies[bo].initial_damage)/(1.-Bodies[bo].initial_damage) << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[31] == 1 )
    {
        Graphic_file << "SCALARS 31_Normalized_displacement_error float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            Graphic_file << log10( Bodies[bo].nodes[no].error_norm ) << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[32] == 1 )
    {
        Graphic_file << "SCALARS 32_Displacement_error float 2" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            Graphic_file << Bodies[bo].nodes[no].x_error << ' '
                         << Bodies[bo].nodes[no].y_error << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[33] == 1 )
    {
        Graphic_file << "SCALARS 33_Internal_work float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            Graphic_file << Bodies[bo].internal_work << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[34] == 1 )
    {
        Graphic_file << "SCALARS 34_Contact_work float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            Graphic_file << Bodies[bo].contact_work << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[35] == 1 )
    {
        Graphic_file << "SCALARS 35_Body_work float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            Graphic_file << Bodies[bo].body_work << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[36] == 1 )
    {
        Graphic_file << "SCALARS 36_Dirichlet_work float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            Graphic_file << Bodies[bo].dirichlet_work << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[37] == 1 )
    {
        Graphic_file << "SCALARS 37_Neumann_work float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            Graphic_file << Bodies[bo].neumann_work << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[38] == 1 )
    {
        Graphic_file << "SCALARS 38_Damping_work float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            Graphic_file << Bodies[bo].damping_work << endl ;
        }
        Graphic_file << endl ;
    }


    if ( To_Plot[39] == 1 )
    {
        Graphic_file << "SCALARS 39_Alid_work float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            Graphic_file << Bodies[bo].alid_work << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[40] == 1 )
    {
        Graphic_file << "SCALARS 40_Temperature float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            Graphic_file << Bodies[bo].temperature << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[41] == 1 )
    {
        Graphic_file << "SCALARS 41-Epsilon_Mass_Scaling float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            no = indicestot[j][1] ;
            Graphic_file << Bodies[bo].nodes[no].factor_mass_scaling << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[42] == 1 )
    {
        Graphic_file << "SCALARS 42-Active_Contacts float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            Graphic_file << Bodies[bo].nb_active_contacts << endl ;
        }
        Graphic_file << endl ;
    }

    if ( To_Plot[43] == 1 )
    {
        Graphic_file << "SCALARS 43-Contacting-Bodies float 1" << endl ;
        Graphic_file << "LOOKUP_TABLE default" << endl ;
        for (int j(0) ; j<ntot ; j++)
        {
            bo = indicestot[j][0] ;
            Graphic_file << Bodies[bo].nb_contacting_bodies << endl ;
        }
        Graphic_file << endl ;
    }

    Graphic_file.close () ;
}





//********************************************//
//** SHIFT BODY GRAPHIC **********************//
//********************************************//
void Shift_body_graphic(int i,
                        vector<Body>& Bodies,
                        double xmin,
                        double xmax,
                        vector<double>& x,
                        vector<double>& y,
                        vector<int>& indices,
                        vector<vector<int>>& cells,
                        int& n,
                        int& nc)
{
    double period = xmax - xmin ;
    double ymin = 1.e6, ymax = -1.e6 ;
    for (int j(0) ; j<Bodies[i].nb_nodes ; j++)
    {
        x.push_back( Bodies[i].nodes[j].x_current - period * floor( ( Bodies[i].nodes[j].x_current - xmin ) / period ) ) ;
        y.push_back( Bodies[i].nodes[j].y_current ) ;
        if (y[j] < ymin)
            ymin = y[j] ;
        if (y[j] > ymax)
            ymax = y[j] ;
        indices.push_back( j ) ;
    }
    double dist = 6 * sqrt ( ( period * ( ymax - ymin ) ) / Bodies[i].nb_nodes ) ;

    n = Bodies[i].nb_nodes ;
    nc = 0 ;
    for (int j(0) ; j<Bodies[i].nb_cells ; j++)
    {
        if (x[Bodies[i].triangulation[j][0]] <= xmin+dist)
        {
            if ( (x[Bodies[i].triangulation[j][1]] >= xmax-dist) && (x[Bodies[i].triangulation[j][2]] <= xmax-dist) )
            {
                cells.push_back({ Bodies[i].triangulation[j][0], n+1, Bodies[i].triangulation[j][2] }) ;
                cells.push_back({ n+0, Bodies[i].triangulation[j][1], n+2 }) ;
                x.push_back( x[Bodies[i].triangulation[j][0]] + period ) ;
                y.push_back( y[Bodies[i].triangulation[j][0]] ) ;
                indices.push_back( Bodies[i].triangulation[j][0] ) ;
                x.push_back( x[Bodies[i].triangulation[j][1]] - period ) ;
                y.push_back( y[Bodies[i].triangulation[j][1]] ) ;
                indices.push_back( Bodies[i].triangulation[j][1] ) ;
                x.push_back( x[Bodies[i].triangulation[j][2]] + period ) ;
                y.push_back( y[Bodies[i].triangulation[j][2]] ) ;
                indices.push_back( Bodies[i].triangulation[j][2] ) ;
                n = n + 3 ;
                nc = nc + 2 ;
                continue ;
            }
            else if ( (x[Bodies[i].triangulation[j][1]] <= xmax-dist) && (x[Bodies[i].triangulation[j][2]] >= xmax-dist) )
            {
                cells.push_back({ Bodies[i].triangulation[j][0], Bodies[i].triangulation[j][1], n+2 }) ;
                cells.push_back({ n+0, n+1, Bodies[i].triangulation[j][2] }) ;
                x.push_back( x[Bodies[i].triangulation[j][0]] + period ) ;
                y.push_back( y[Bodies[i].triangulation[j][0]] ) ;
                indices.push_back( Bodies[i].triangulation[j][0] ) ;
                x.push_back( x[Bodies[i].triangulation[j][1]] + period ) ;
                y.push_back( y[Bodies[i].triangulation[j][1]] ) ;
                indices.push_back( Bodies[i].triangulation[j][1] ) ;
                x.push_back( x[Bodies[i].triangulation[j][2]] - period ) ;
                y.push_back( y[Bodies[i].triangulation[j][2]] ) ;
                indices.push_back( Bodies[i].triangulation[j][2] ) ;
                n = n + 3 ;
                nc = nc + 2 ;
                continue ;
            }
            else if ( (x[Bodies[i].triangulation[j][1]] >= xmax-dist) && (x[Bodies[i].triangulation[j][2]] >= xmax-dist) )
            {
                cells.push_back({ Bodies[i].triangulation[j][0], n+1, n+2 }) ;
                cells.push_back({ n+0, Bodies[i].triangulation[j][1], Bodies[i].triangulation[j][2] }) ;
                x.push_back( x[Bodies[i].triangulation[j][0]] + period ) ;
                y.push_back( y[Bodies[i].triangulation[j][0]] ) ;
                indices.push_back( Bodies[i].triangulation[j][0] ) ;
                x.push_back( x[Bodies[i].triangulation[j][1]] - period ) ;
                y.push_back( y[Bodies[i].triangulation[j][1]] ) ;
                indices.push_back( Bodies[i].triangulation[j][1] ) ;
                x.push_back( x[Bodies[i].triangulation[j][2]] - period ) ;
                y.push_back( y[Bodies[i].triangulation[j][2]] ) ;
                indices.push_back( Bodies[i].triangulation[j][2] ) ;
                n = n + 3 ;
                nc = nc + 2 ;
                continue ;
            }
        }
        else if (x[Bodies[i].triangulation[j][1]] <= xmin+dist)
        {
            if ( (x[Bodies[i].triangulation[j][2]] >= xmax-dist) && (x[Bodies[i].triangulation[j][0]] <= xmax-dist) )
            {
                cells.push_back({ Bodies[i].triangulation[j][1], n+1, Bodies[i].triangulation[j][0] }) ;
                cells.push_back({ n+0, Bodies[i].triangulation[j][2], n+2 }) ;
                x.push_back( x[Bodies[i].triangulation[j][1]] + period ) ;
                y.push_back( y[Bodies[i].triangulation[j][1]] ) ;
                indices.push_back( Bodies[i].triangulation[j][1] ) ;
                x.push_back( x[Bodies[i].triangulation[j][2]] - period ) ;
                y.push_back( y[Bodies[i].triangulation[j][2]] ) ;
                indices.push_back( Bodies[i].triangulation[j][2] ) ;
                x.push_back( x[Bodies[i].triangulation[j][0]] + period ) ;
                y.push_back( y[Bodies[i].triangulation[j][0]] ) ;
                indices.push_back( Bodies[i].triangulation[j][0] ) ;
                n = n + 3 ;
                nc = nc + 2 ;
                continue ;
            }
            else if ( (x[Bodies[i].triangulation[j][2]] <= xmax-dist) && (x[Bodies[i].triangulation[j][0]] >= xmax-dist) )
            {
                cells.push_back({ Bodies[i].triangulation[j][1], Bodies[i].triangulation[j][2], n+2 }) ;
                cells.push_back({ n+0, n+1, Bodies[i].triangulation[j][0] }) ;
                x.push_back( x[Bodies[i].triangulation[j][1]] + period ) ;
                y.push_back( y[Bodies[i].triangulation[j][1]] ) ;
                indices.push_back( Bodies[i].triangulation[j][1] ) ;
                x.push_back( x[Bodies[i].triangulation[j][2]] + period ) ;
                y.push_back( y[Bodies[i].triangulation[j][2]] ) ;
                indices.push_back( Bodies[i].triangulation[j][2] ) ;
                x.push_back( x[Bodies[i].triangulation[j][0]] - period ) ;
                y.push_back( y[Bodies[i].triangulation[j][0]] ) ;
                indices.push_back( Bodies[i].triangulation[j][0] ) ;
                n = n + 3 ;
                nc = nc + 2 ;
                continue ;
            }
            else if ( (x[Bodies[i].triangulation[j][2]] >= xmax-dist) && (x[Bodies[i].triangulation[j][0]] >= xmax-dist) )
            {
                cells.push_back({ Bodies[i].triangulation[j][1], n+1, n+2 }) ;
                cells.push_back({ n+0, Bodies[i].triangulation[j][2], Bodies[i].triangulation[j][0] }) ;
                x.push_back( x[Bodies[i].triangulation[j][1]] + period ) ;
                y.push_back( y[Bodies[i].triangulation[j][1]] ) ;
                indices.push_back( Bodies[i].triangulation[j][1] ) ;
                x.push_back( x[Bodies[i].triangulation[j][2]] - period ) ;
                y.push_back( y[Bodies[i].triangulation[j][2]] ) ;
                indices.push_back( Bodies[i].triangulation[j][2] ) ;
                x.push_back( x[Bodies[i].triangulation[j][0]] - period ) ;
                y.push_back( y[Bodies[i].triangulation[j][0]] ) ;
                indices.push_back( Bodies[i].triangulation[j][0] ) ;
                n = n + 3 ;
                nc = nc + 2 ;
                continue ;
            }
        }
        else if (x[Bodies[i].triangulation[j][2]] <= xmin+dist)
        {
            if ( (x[Bodies[i].triangulation[j][0]] >= xmax-dist) && (x[Bodies[i].triangulation[j][1]] <= xmax-dist) )
            {
                cells.push_back({ Bodies[i].triangulation[j][2], n+1, Bodies[i].triangulation[j][1] }) ;
                cells.push_back({ n+0, Bodies[i].triangulation[j][0], n+2 }) ;
                x.push_back( x[Bodies[i].triangulation[j][2]] + period ) ;
                y.push_back( y[Bodies[i].triangulation[j][2]] ) ;
                indices.push_back( Bodies[i].triangulation[j][2] ) ;
                x.push_back( x[Bodies[i].triangulation[j][0]] - period ) ;
                y.push_back( y[Bodies[i].triangulation[j][0]] ) ;
                indices.push_back( Bodies[i].triangulation[j][0] ) ;
                x.push_back( x[Bodies[i].triangulation[j][1]] + period ) ;
                y.push_back( y[Bodies[i].triangulation[j][1]] ) ;
                indices.push_back( Bodies[i].triangulation[j][1] ) ;
                n = n + 3 ;
                nc = nc + 2 ;
                continue ;
            }
            else if ( (x[Bodies[i].triangulation[j][0]] <= xmax-dist) && (x[Bodies[i].triangulation[j][1]] >= xmax-dist) )
            {
                cells.push_back({ Bodies[i].triangulation[j][2], Bodies[i].triangulation[j][0], n+2 }) ;
                cells.push_back({ n+0, n+1, Bodies[i].triangulation[j][1] }) ;
                x.push_back( x[Bodies[i].triangulation[j][2]] + period ) ;
                y.push_back( y[Bodies[i].triangulation[j][2]] ) ;
                indices.push_back( Bodies[i].triangulation[j][2] ) ;
                x.push_back( x[Bodies[i].triangulation[j][0]] + period ) ;
                y.push_back( y[Bodies[i].triangulation[j][0]] ) ;
                indices.push_back( Bodies[i].triangulation[j][0] ) ;
                x.push_back( x[Bodies[i].triangulation[j][1]] - period ) ;
                y.push_back( y[Bodies[i].triangulation[j][1]] ) ;
                indices.push_back( Bodies[i].triangulation[j][1] ) ;
                n = n + 3 ;
                nc = nc + 2 ;
                continue ;
            }
            else if ( (x[Bodies[i].triangulation[j][0]] >= xmax-dist) && (x[Bodies[i].triangulation[j][1]] >= xmax-dist) )
            {
                cells.push_back({ Bodies[i].triangulation[j][2], n+1, n+2 }) ;
                cells.push_back({ n+0, Bodies[i].triangulation[j][0], Bodies[i].triangulation[j][1] }) ;
                x.push_back( x[Bodies[i].triangulation[j][2]] + period ) ;
                y.push_back( y[Bodies[i].triangulation[j][2]] ) ;
                indices.push_back( Bodies[i].triangulation[j][2] ) ;
                x.push_back( x[Bodies[i].triangulation[j][0]] - period ) ;
                y.push_back( y[Bodies[i].triangulation[j][0]] ) ;
                indices.push_back( Bodies[i].triangulation[j][0] ) ;
                x.push_back( x[Bodies[i].triangulation[j][1]] - period ) ;
                y.push_back( y[Bodies[i].triangulation[j][1]] ) ;
                indices.push_back( Bodies[i].triangulation[j][1] ) ;
                n = n + 3 ;
                nc = nc + 2 ;
                continue ;
            }
        }
        cells.push_back( Bodies[i].triangulation[j] ) ;
        nc = nc + 1 ;
    }
}


//********************************************//
//** WRITE CHAINS ****************************//
//********************************************//
void Write_chains(int Nb_bodies,
                  vector<Body>& Bodies,
                  int Number_iteration,
                  int Number_save,
                  int Number_print,
                  double Time,
                  double Xmin_period,
                  double Xmax_period,
                  double Chains_typical_pressure,
                  double Chains_size_ratio)
{
    cout << endl ;
    cout << "Writing chains file " << Number_print << endl ;
    string filename1 ;
    stringstream sfilename ;
    sfilename << Number_print;
    if      (Number_print<10)
        filename1="CHAINS_0000"+sfilename.str()+".vtk" ;
    else if (Number_print<100)
        filename1="CHAINS_000"+sfilename.str()+".vtk" ;
    else if (Number_print<1000)
        filename1="CHAINS_00"+sfilename.str()+".vtk" ;
    else if (Number_print<10000)
        filename1="CHAINS_0"+sfilename.str()+".vtk" ;
    else if (Number_print<100000)
        filename1="CHAINS_"+sfilename.str()+".vtk" ;
    ofstream Graphic_file (filename1) ;

    Graphic_file << setprecision (12) ;
    Graphic_file << "# vtk DataFile Version 2.0" << endl ;
    Graphic_file << "MELODY 2D chains file ; Iteration " << Number_iteration
                 << " ; Save " << Number_save
                 << " ; Print " << Number_print
                 << " ; Time " << Time
                 << " ; Typical pressure " << Chains_typical_pressure
                 << " ; Chains thickness ratio " << Chains_size_ratio
                 << endl ;
    Graphic_file << "ASCII" << endl ;
    Graphic_file << endl ;
    Graphic_file << "DATASET POLYDATA" << endl ;
    Graphic_file << endl ;

    vector<vector<double>> points ;
    vector<vector<double>> forces ;
    double xs, ys, xm, ym, xc, yc, fx, fy, f, fn, ft ;
    double shifts, shiftm, period, length, width ;
    double x1, y1, x2, y2, x3, y3, x4, y4, ux, uy ;
    int m, nrect ;
    period = Xmax_period - Xmin_period ;
    nrect = 0 ;
    for (int i=0 ; i<Nb_bodies ; i++)
    {
        if (Bodies[i].status == "inactive")
            continue ;
        xs = Bodies[i].x_current ;
        ys = Bodies[i].y_current ;
        for (int j=0 ; j<Bodies[i].nb_contact_elements ; j++)
        {
            fx = Bodies[i].contact_elements[j].fx ;
            fy = Bodies[i].contact_elements[j].fy ;
            if ((fx == 0.) && (fy == 0.))
                continue ;
            f = sqrt( fx * fx + fy * fy ) ;
            fn = fx * Bodies[i].contact_elements[j].xnorm + fy * Bodies[i].contact_elements[j].ynorm ;
            ft = fx * Bodies[i].contact_elements[j].xtan + fy * Bodies[i].contact_elements[j].ytan ;
            m = Bodies[i].contact_elements[j].bodyM ;
            xc = Bodies[i].nodes[Bodies[i].contact_elements[j].nodeS].x_current ;
            yc = Bodies[i].nodes[Bodies[i].contact_elements[j].nodeS].y_current ;
            width = Chains_size_ratio * 0.05 * f / Chains_typical_pressure ;

            if (Bodies[i].periodicity != "Periodic" && Bodies[i].type == "rigid")
            {
                //cout << "Slave " << i << " rectangle " << nrect << endl ;
                shifts = - period * floor( ( xc - Xmin_period ) / period ) ;
                length = sqrt( (xs - xc) * (xs - xc) + (ys - yc) * (ys - yc) ) ;
                //width = 0.1 * length ;
                ux = (xc - xs) / length ;
                uy = (yc - ys) / length ;
                x1 = xs + width * uy + shifts ;
                x2 = xc + width * uy + shifts ;
                x3 = xc - width * uy + shifts ;
                x4 = xs - width * uy + shifts ;
                y1 = ys - width * ux ;
                y2 = yc - width * ux ;
                y3 = yc + width * ux ;
                y4 = ys + width * ux ;
                points.push_back({ x1, y1 }) ;
                points.push_back({ x2, y2 }) ;
                points.push_back({ x3, y3 }) ;
                points.push_back({ x4, y4 }) ;
                forces.push_back({ fx, fy, fn, abs(ft), abs(ft)/fn }) ;
                nrect++ ;
            }

            if (Bodies[m].periodicity != "Periodic" && Bodies[m].type == "rigid")
            {
                //cout << "Master " << m << " rectangle " << nrect << endl ;
                xm = Bodies[m].x_current ;
                ym = Bodies[m].y_current ;
                shiftm = - period * floor( ( xm - Xmin_period ) / period ) ;
                xm = xm + shiftm ;
                shifts = - period * floor( ( xc - Xmin_period ) / period ) ;
                xc = xc + shifts ;
                length = sqrt( (xm - xc) * (xm - xc) + (ym - yc) * (ym - yc) ) ;
                //width = 0.1 * length ;
                ux = (xc - xm) / length ;
                uy = (yc - ym) / length ;
                x1 = xm + width * uy ;
                x2 = xc + width * uy ;
                x3 = xc - width * uy ;
                x4 = xm - width * uy ;
                y1 = ym - width * ux ;
                y2 = yc - width * ux ;
                y3 = yc + width * ux ;
                y4 = ym + width * ux ;
                if ( sqrt( (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) ) < 0.5 * period )
                {
                    points.push_back({ x1, y1 }) ;
                    points.push_back({ x2, y2 }) ;
                    points.push_back({ x3, y3 }) ;
                    points.push_back({ x4, y4 }) ;
                    forces.push_back({ fx, fy, fn, abs(ft), abs(ft)/fn }) ;
                    nrect++ ;
                }
            }
        }
    }

    Graphic_file << "POINTS " << nrect * 4 << " float" << endl ;
    for (int j(0) ; j<nrect * 4 ; j++)
    {
        Graphic_file << points[j][0] << ' '
                     << points[j][1] << ' '
                     << '1' << endl ;
    }
    Graphic_file << endl ;

    Graphic_file << setprecision (6) ;
    Graphic_file << "POLYGONS " << nrect << ' ' << nrect*5 << endl ;
    for (int j(0) ; j<nrect ; j++)
    {
        Graphic_file << "4 "
                     << j * 4 << ' '
                     << j * 4 + 1 << ' '
                     << j * 4 + 2 << ' '
                     << j * 4 + 3 << endl ;
    }
    Graphic_file << endl ;

    Graphic_file << "CELL_DATA " << nrect << endl ;
    Graphic_file << "SCALARS 00_Contact_force float 2" << endl ;
    Graphic_file << "LOOKUP_TABLE default" << endl ;
    for (int j(0) ; j<nrect ; j++)
    {
        Graphic_file << forces[j][0] << ' ' << forces[j][1] << endl ;
    }
    Graphic_file << endl ;

    Graphic_file << "SCALARS 01_Normal float 1" << endl ;
    Graphic_file << "LOOKUP_TABLE default" << endl ;
    for (int j(0) ; j<nrect ; j++)
    {
        Graphic_file << forces[j][2] << endl ;
    }
    Graphic_file << endl ;

    Graphic_file << "SCALARS 02_Absolute_tangential float 1" << endl ;
    Graphic_file << "LOOKUP_TABLE default" << endl ;
    for (int j(0) ; j<nrect ; j++)
    {
        Graphic_file << forces[j][3] << endl ;
    }
    Graphic_file << endl ;

    Graphic_file << "SCALARS 03_Tangential_to_normal float 1" << endl ;
    Graphic_file << "LOOKUP_TABLE default" << endl ;
    for (int j(0) ; j<nrect ; j++)
    {
        Graphic_file << forces[j][4] << endl ;
    }
    Graphic_file << endl ;

    Graphic_file.close () ;
}




//********************************************//
//** WRITE FIELDS ****************************//
//********************************************//
void Write_fields(int Nb_bodies,
                  vector<Body>& Bodies,
                  int Number_iteration,
                  int Number_save,
                  int Number_print,
                  double Time,
                  double Xmin_period,
                  double Xmax_period,
                  double xmin,
                  double xmax,
                  double ymin,
                  double ymax,
                  double step,
                  double dist)
{
    cout << endl ;
    cout << "Writing fields file " << Number_print << endl ;
    string filename1 ;
    stringstream sfilename ;
    sfilename << Number_print;
    if      (Number_print<10)
        filename1="FIELDS_0000"+sfilename.str()+".vtk" ;
    else if (Number_print<100)
        filename1="FIELDS_000"+sfilename.str()+".vtk" ;
    else if (Number_print<1000)
        filename1="FIELDS_00"+sfilename.str()+".vtk" ;
    else if (Number_print<10000)
        filename1="FIELDS_0"+sfilename.str()+".vtk" ;
    else if (Number_print<100000)
        filename1="FIELDS_"+sfilename.str()+".vtk" ;
    ofstream Graphic_file (filename1) ;

    double xa, ya, xb, yb, xc, yc, xd, yd, xs, Cab, Cac, Cbc, ystart, yend ;
    int ixstart, ixend, iystart, iyend ;
    double period = Xmax_period - Xmin_period ;
    double invperiod = 1. / period ;
    double invstep = 1. / step ;
    int nx = (int)((xmax - xmin) / step + 1) ;
    int ny = (int)((ymax - ymin) / step + 1) ;
    vector<double> X(nx) ;
    vector<double> Y(ny) ;
    for (int j(0) ; j<nx ; j++)
        X[j] = xmin + j * step ;
    for (int j(0) ; j<ny ; j++)
        Y[j] = ymin + j * step ;
    vector<int> EmptyColumnInt(ny) ;
    for (int j(0) ; j<ny ; j++)
        EmptyColumnInt[j] = 0 ;
    vector<vector<int>> EmptyImageInt(nx) ;
    for (int j(0) ; j<nx ; j++)
        EmptyImageInt[j] = EmptyColumnInt ;
    vector<double> EmptyColumnDouble(ny) ;
    for (int j(0) ; j<ny ; j++)
        EmptyColumnDouble[j] = 0. ;
    vector<vector<double>> EmptyImageDouble(nx) ;
    for (int j(0) ; j<nx ; j++)
        EmptyImageDouble[j] = EmptyColumnDouble ;
    vector<vector<int>> ImageA=EmptyImageInt ;
    vector<vector<double>> ImageB=EmptyImageDouble ;
    vector<vector<int>> ImageC=EmptyImageInt ;
    vector<vector<int>> ImageD=EmptyImageInt ;
    vector<vector<double>> ImageE=EmptyImageDouble ;
    vector<vector<double>> ImageF=EmptyImageDouble ;

    for (int body(0) ; body<Nb_bodies ; body++)
    {
        for (int j(0) ; j<(int)Bodies[body].triangulation.size() ; j++)
        {
            xa = Bodies[body].nodes[Bodies[body].triangulation[j][0]].x_current ;
            ya = Bodies[body].nodes[Bodies[body].triangulation[j][0]].y_current ;
            xb = Bodies[body].nodes[Bodies[body].triangulation[j][1]].x_current ;
            yb = Bodies[body].nodes[Bodies[body].triangulation[j][1]].y_current ;
            xc = Bodies[body].nodes[Bodies[body].triangulation[j][2]].x_current ;
            yc = Bodies[body].nodes[Bodies[body].triangulation[j][2]].y_current ;
            xs = (xa + xb + xc) * 0.333333333333 ;
            xa = xa - period * floor( ( xs - Xmin_period ) * invperiod ) ;
            xb = xb - period * floor( ( xs - Xmin_period ) * invperiod ) ;
            xc = xc - period * floor( ( xs - Xmin_period ) * invperiod ) ;

            if (xc<=xb && xb<=xa)
            {
                swap(xa,xc) ;
                swap(ya,yc) ;
            }
            else if (xa<=xc && xc<=xb)
            {
                swap(xb,xc) ;
                swap(yb,yc) ;
            }
            else if (xb<=xa && xa<=xc)
            {
                swap(xa,xb) ;
                swap(ya,yb) ;
            }
            else if (xc<=xa && xa<=xb)
            {
                xd = xb ;
                xb = xa ;
                xa = xc ;
                xc = xd ;
                yd = yb ;
                yb = ya ;
                ya = yc ;
                yc = yd ;
            }
            else if (xb<=xc && xc<=xa)
            {
                xd = xc ;
                xc = xa ;
                xa = xb ;
                xb = xd ;
                yd = yc ;
                yc = ya ;
                ya = yb ;
                yb = yd ;
            }
            if (xa > xmax || xc < xmin || min(ya, min(yb, yc)) > ymax || max(ya, max(yb, yc)) < ymin)
                continue ;

            Cab = (yb-ya) / (xb-xa) ;
            Cac = (yc-ya) / (xc-xa) ;
            Cbc = (yc-yb) / (xc-xb) ;
            xd = xb ;
            yd = ya + (xd - xa) * Cac ;

            ixstart = ceil( (xa - xmin) * invstep ) ;
            ixend = floor( (xb - xmin) * invstep ) ;
            if (ixstart < 0)
                ixstart = 0 ;
            if (ixend > nx - 1)
                ixend = nx - 1 ;
            for (int ix(ixstart) ; ix<=ixend ; ix++)
            {
                if (yd > yb)
                {
                    ystart = ya + (X[ix] - xa) * Cab ;
                    yend = ya + (X[ix] - xa) * Cac ;
                }
                else
                {
                    ystart = ya + (X[ix] - xa) * Cac ;
                    yend = ya + (X[ix] - xa) * Cab ;
                }
                iystart = ceil( (ystart - ymin) * invstep ) ;
                iyend = floor( (yend - ymin) * invstep ) ;
                if (iystart < 0)
                    iystart = 0 ;
                if (iyend > ny - 1)
                    iyend = ny - 1 ;
                for (int iy(iystart) ; iy<=iyend ; iy++)
                    ImageA[ix][iy] = 1 ;
            }

            ixstart = ixend + 1 ;
            ixend = floor( (xc - xmin) * invstep ) ;
            if (ixstart > nx)
                continue ;
            if (ixend < 0)
                continue ;
            if (ixstart < 0)
                ixstart = 0 ;
            if (ixend > nx - 1)
                ixend = nx - 1 ;
            for (int ix(ixstart) ; ix<=ixend ; ix++)
            {
                if (yd > yb)
                {
                    ystart = yb + (X[ix] - xb) * Cbc ;
                    yend = ya + (X[ix] - xa) * Cac ;
                }
                else
                {
                    ystart = ya + (X[ix] - xa) * Cac ;
                    yend = yb + (X[ix] - xb) * Cbc ;
                }
                iystart = ceil( (ystart - ymin) * invstep ) ;
                iyend = floor( (yend - ymin) * invstep ) ;
                if (iystart < 0)
                    iystart = 0 ;
                if (iyend > ny - 1)
                    iyend = ny - 1 ;
                for (int iy(iystart) ; iy<=iyend ; iy++)
                    ImageA[ix][iy] = 1 ;
            }
        }
    }

    int Msize = floor(dist*invstep) ;
    Convolute(ImageA, ImageB, "Gauss", Msize) ;
    Dilate(ImageA, ImageC, Msize) ;
    Erode(ImageC, ImageD, Msize) ;
    Convolute(ImageD, ImageE, "Gauss", Msize) ;
    for (int iy(0) ; iy<ny ; iy++)
    {
        for (int ix(0) ; ix<nx ; ix++)
        {
            if (ImageE[ix][iy] != 0.)
                ImageF[ix][iy] = ImageB[ix][iy] / ImageE[ix][iy] * ImageD[ix][iy] ;
            if (ImageF[ix][iy] > 1.)
                ImageF[ix][iy] = 1. ;
            if (ImageF[ix][iy] < 0.)
                ImageF[ix][iy] = 0. ;
        }
    }

    Graphic_file << setprecision (12) ;
    Graphic_file << "# vtk DataFile Version 2.0" << endl ;
    Graphic_file << "MELODY 2D fields file ; Iteration " << Number_iteration
                 << " ; Save " << Number_save
                 << " ; Print " << Number_print
                 << " ; Time " << Time
                 << " ; Window " << xmin << ' ' << xmax << ' ' << ymin << ' ' << ymax
                 << " ; Step " << step
                 << endl ;
    Graphic_file << "ASCII" << endl ;
    Graphic_file << "DATASET STRUCTURED_POINTS" << endl ;
    Graphic_file << "DIMENSIONS " << nx << ' ' << ny << " 1" << endl ;
    Graphic_file << "ASPECT_RATIO " << step << ' ' << step << " 1" << endl ;
    Graphic_file << "ORIGIN " << xmin << ' ' << ymin << " -1" << endl ;
    Graphic_file << "POINT_DATA " << nx * ny << endl ;
    Graphic_file << endl ;
    Graphic_file << "SCALARS 00_Solid_Fraction float 1" << endl ;
    Graphic_file << "LOOKUP_TABLE default" << endl ;
    for (int iy(0) ; iy<ny ; iy++)
    {
        for (int ix(0) ; ix<nx ; ix++)
        {
            Graphic_file << ImageF[ix][iy] << ' ' ;
        }
        Graphic_file << endl ;
    }
    Graphic_file << endl ;
    Graphic_file.close () ;
}



//********************************************//
//** WRITE CONTOURS **************************//
//********************************************//
void Write_contours(int Nb_bodies,
                    vector<Body>& Bodies,
                    int Number_iteration,
                    int Number_save,
                    int Number_print,
                    double Time,
                    double Xmin_period,
                    double Xmax_period)
{
    cout << endl ;
    cout << "Writing contours file " << Number_print << endl ;
    stringstream sfilename ;
    sfilename << Number_print;
    string filename ;
    if      (Number_print<10)
        filename="CONTOURS_0000"+sfilename.str()+".vtk" ;
    else if (Number_print<100)
        filename="CONTOURS_000"+sfilename.str()+".vtk" ;
    else if (Number_print<1000)
        filename="CONTOURS_00"+sfilename.str()+".vtk" ;
    else if (Number_print<10000)
        filename="CONTOURS_0"+sfilename.str()+".vtk" ;
    else if (Number_print<100000)
        filename="CONTOURS_"+sfilename.str()+".vtk" ;
    ofstream Borders_file (filename) ;

    Borders_file << setprecision (12) ;
    Borders_file << "# vtk DataFile Version 2.0" << endl ;
    Borders_file << "MELODY 2D contours file ; Iteration " << Number_iteration
    			 << " ; Save " << Number_save
    			 << " ; Print " << Number_print
    			 << " ; Time " << Time << endl ;
    Borders_file << "ASCII" << endl ;
    Borders_file << "DATASET POLYDATA" << endl ;
    Borders_file << endl ;

    double xc ;
    int c ;
    vector<double> xtot, ytot, xint, yint ;
    vector<vector<int>> indicestot ;
    double period = Xmax_period - Xmin_period ;
    double invperiod = 1. / period ;
    int n=0, nc=0 ;
    for (int i=0 ; i<Nb_bodies ; i++)
    {
        xint = {} ;
        yint = {} ;
        xc = 0. ;
        c = 0 ;
        if (Bodies[i].status == "inactive" || Bodies[i].periodicity == "Periodic")
            continue ;
        for (int j=0 ; j<Bodies[i].nb_borders ; j++)
        {
            indicestot.push_back({}) ;
            for (int k=0 ; k<Bodies[i].borders[j].number_border_nodes ; k++)
            {
                xint.push_back(Bodies[i].nodes[Bodies[i].borders[j].border_nodes[k]].x_current) ;
                yint.push_back(Bodies[i].nodes[Bodies[i].borders[j].border_nodes[k]].y_current) ;
                indicestot[nc].push_back(n) ;
                n++ ;
                xc += Bodies[i].nodes[Bodies[i].borders[j].border_nodes[k]].x_current ;
                c++ ;
            }
        }
        xc /= c ;
        for (int j=0 ; j<(int)xint.size() ; j++)
        {
            xtot.push_back(xint[j] - period * floor( ( xc - Xmin_period ) * invperiod )) ;
            ytot.push_back(yint[j]) ;
        }
        nc++ ;
    }
    Borders_file << "POINTS " << n + 1 << " float" << endl ;
	for (int j(0) ; j<n ; j++)
	{
		Borders_file << xtot[j] << ' ' << ytot[j] << ' ' << '0' << endl ;
	}
	Borders_file << endl ;

	Borders_file << "POLYGONS " << nc << ' ' << nc + n << endl ;
	for (int j(0) ; j<nc ; j++)
	{
		Borders_file << (int)indicestot[j].size() << ' ' ;
		for (int k(0) ; k<(int)indicestot[j].size() ; k++)
		{
		    Borders_file << indicestot[j][k] << ' ' ;
		}
		Borders_file << endl ;
	}
	Borders_file << endl ;
	Borders_file.close () ;
}



#endif
