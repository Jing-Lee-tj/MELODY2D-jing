v% 1. Simulation name
Simulation_Name='dense_C_2500_95%_lijing';

%=================================TO MODIFY================================
%==========================================================================
%dimension of the sample
gouge_length = 0.02;%m
gouge_width = 0.002;%m
first_bodies=2;
ratio=0.2;

mat_file=load('D:\Packing2D1.1\grain_5000_F_2.6.mat');
% [ContoursGrains,Ngrains,deltaY]=delete_small_grains(mat_file,gouge_width,ratio);

deltaY=0;
ContoursGrains=mat_file.Contours;
Ngrains=size(ContoursGrains,1);
%--------------------------------------------------------------------------

 %-------------------------------------------------------------------------

% % 2. Initilization
Contours=cell(1,1);
Distributions=cell(1,1);
Interpolations=cell(1,1);
Integrations=cell(1,1);
Bodies_Materials=cell(1,1);
Imposed_Pressures=cell(1,1);
Imposed_Velocities=cell(1,1);
Alid=cell(1,1);
Deactivations=cell(1,1);
To_Plot=zeros(32,1);


% 3. Bodies
Contours{1,1}={'Periodic',[0,-0.003;gouge_length,-0.003],'Linear';...
               'Periodic',[[gouge_length:-0.00001:0]',-0.00205+cos([gouge_length:-0.00001:0]'/gouge_length*2*pi*101)*0.00005],'Linear'};
Distributions{1,1}={'Rigid'};
Distributions{1,2}=1e-5;
Distributions{1,3}=1;
Interpolations{1,1}='MLS';
Interpolations{1,2}=10;
Integrations{1,1}='Gauss';
Integrations{1,2}=3;
Detections(1,1)=1e-5;
Detections(1,2)=1e-5;
Bodies_Materials{1,1}='Rock';
Imposed_Pressures{1,1}={[],'None',[],'None';...
    [],'None',[],'None';...
    };
Imposed_Velocities{1,1}={[0,0;1e6,0],'Driven',[],[0,0;1e6,0],'Driven',[];...
    [],'None',[],[],'None',[];...
    };
Initial_Velocities{1,1}=[0,0];
Mesh_Ratios(1,1:2)=[3,1];
Status{1,1}='active';
Alid{1,1}=[];
Alid{1,2}=[];
Alid{1,3}=[];

Contours{2,1}={'Periodic',[(0:0.00001:0.02)',0.00005+deltaY+cos([0:0.00001:0.02]'/0.02*2*pi*101)*0.00005],'Linear';...
               'Periodic',[0.02,0.001+deltaY;0,0.001+deltaY],'Linear'};
Distributions{2,1}={'Rigid'};
Distributions{2,2}=1E-5;
Distributions{2,3}=1;
Interpolations{2,1}='MLS';
Interpolations{2,2}=10;
Integrations{2,1}='Gauss';
Integrations{2,2}=3;
Detections(2,1)=1E-5;
Detections(2,2)=1E-5;
Bodies_Materials{2,1}='Rock';
Imposed_Pressures{2,1}={[],'None',[],'None';...
    [],'None',[0,-40e6;1e6,-40e6],'Oriented';...
    };
Imposed_Velocities{2,1}={[],'None',[],[],'None',[];...
    [0,0;0.001,0;0.002,1;40e6,1],'Driven',[],[],'None',[];...
    };
Initial_Velocities{2,1}=[0,0];
Mesh_Ratios(2,1:2)=[1,3];
Status{2,1}='active';
Alid{2,1}=[];
Alid{2,2}=[];
Alid{2,3}=[];
% 
for num=1:Ngrains
    Contours{first_bodies+num,1}={'Closed',ContoursGrains{num,1}/1000,'Linear'};   %on divise par 1000 car echantillon en mm
    Distributions{first_bodies+num,1}={'Rigid'};
%     if(ContoursGrains{num,6}<0.001)
%         Distributions{first_bodies+num,2}=5e-6;
%     else
    xydata = ContoursGrains{num,1}/1000;
    maxx = max(xydata(:,1));
    minx = min(xydata(:,1));
    maxy = max(xydata(:,2));
    miny = min(xydata(:,2));
    dx = maxx - minx;
    dy = maxy - miny;
    if dx > dy
        Distributions{first_bodies+num,2}=min([dx/10,dy/3]);
    else 
        Distributions{first_bodies+num,2}=min([dy/10,dx/3]);
    end
%     end
    Distributions{first_bodies+num,3}=1;
    Interpolations{first_bodies+num,1}='MLS';
    Interpolations{first_bodies+num,2}=10;
    Integrations{first_bodies+num,1}='Gauss';
    Integrations{first_bodies+num,2}=3;
    Detections(first_bodies+num,1)=1e-5;
    Detections(first_bodies+num,2)=1e-5;
    Bodies_Materials{first_bodies+num,1}='Gouge';
    Imposed_Pressures{first_bodies+num,1}={[],'None',[],'None';...
                            };
    Imposed_Velocities{first_bodies+num,1}={[],'None',[],[],'None',[];...
                             };
    Initial_Velocities{first_bodies+num,1}=[0,0];
    Mesh_Ratios(first_bodies+num,1:2)=[1,1];
    Status{first_bodies+num,1}='active';
    Alid{first_bodies+num,1}=[];
    Alid{first_bodies+num,2}=[];
    Alid{first_bodies+num,3}=[];
end

% 4. Materials and contact laws
Materials={'Rock','NeoHookean',[2600,0,0,0,0];...
           'Gouge','NeoHookean',[2600,0,0,0,0];...
           };
Contact_Laws={'Rock','Gouge','DampedMohrCoulomb','Evolutive',[1e+15,1e+15,0,0,0,0.3];...
              'Gouge','Gouge','DampedMohrCoulomb','Evolutive',[1e+15,1e+15,0,0,0,0.3];...
              };

% 5. General boundary conditions
Periodic_Boundaries=[0,0.02];
Gravity=[0;0];

% 6. Text outputs
Monitorings=cell(1,1);
Spies=cell(1,1);
Spies={'FORCE_DISP',4,1e-5,{'Displacement X 1 -1';...
                            'Displacement Y 1 -1';...
                            'Force X Contact 1 -1';...
                            'Force Y Contact 1 -1';...
                            };
      };


% 7. Graphic outputs
To_Plot(1)=1;   % Body Index
To_Plot(2)=1;   % Initial Position
To_Plot(3)=0;   % Current Position
To_Plot(4)=1;   % Displacement
To_Plot(5)=1;   % Velocity
To_Plot(6)=0;   % Acceleration
To_Plot(7)=1;   % Force
To_Plot(8)=0;   % Internal Force
To_Plot(9)=0;   % Contact Force
To_Plot(10)=0;  % Body Force
To_Plot(11)=0;  % Dirichlet Force
To_Plot(12)=0;  % Neumann Force
To_Plot(13)=0;  % Damping Force
To_Plot(14)=0;  % Alid Force
To_Plot(15)=0;  % Jacobian
To_Plot(16)=0;  % Cauchy XX Stress
To_Plot(17)=0;  % Cauchy YY Stress
To_Plot(18)=0;  % Cauchy XY Stress
To_Plot(19)=0;  % Cauchy ZZ Stress
To_Plot(20)=0;  % Tresca Stress
To_Plot(21)=0;  % Von Mises Stress
To_Plot(22)=0;  % Major Principal Stress
To_Plot(23)=0;  % Intermediate Principal Stress
To_Plot(24)=0;  % Minor Principal Stress
To_Plot(25)=0;  % Spherical Stress
To_Plot(26)=0;  % Green-Lagrange XX strain
To_Plot(27)=0;  % Green-Lagrange YY strain
To_Plot(28)=0;  % Green-Lagrange XY strain
To_Plot(29)=0;  % Norm of the Green-Lagrange strain tensor
To_Plot(30)=1;  % Body Damage
To_Plot(31)=1;  % Body Relative Damage
To_Plot(32)=0;  % Normalized Displacement Error
To_Plot(33)=0;  % Displacement Error
To_Plot(34)=0;  % Internal Work
To_Plot(35)=0;  % Contact Work
To_Plot(36)=0;  % Body Work
To_Plot(37)=0;  % Dirichlet Work
To_Plot(38)=0;  % Neumann Work
To_Plot(39)=0;  % Damping Work
To_Plot(40)=0;  % Alid Work
To_Plot(41)=0;  % Temperature
To_Plot(42)=0;  % Scaling Parameter
To_Plot(43)=0;  % Active Contacts
To_Plot(44)=0;  % Contacting Bodies

Chains_Parameters = [40e6,1];
Fields_Parameters = [0 0.02 -0.00205 -0.00021 0.00001 0.0001];

% 8. Numerical parameters
Scheme='Adaptive_Euler';
Scheme_Parameters=[1e-4,0.2,20];
Contact_Updating_Period=1e-7;
Time_Stepping_Parameters=[0,1e-9,0.002];
Save_Periods=[0.00001,0.0001];

% 9. Flags
Activate_Plot=1;
Initialize_CZM=1;                                                                                                                                                                            