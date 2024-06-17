% 1. Simulation name
Simulation_Name='contact_test_DMR'; %Degradable Mohr-Coulomb contact
% Simulation_Name='contact_test_CZMLinear'; %Degradable Mohr-Coulomb contact
%=================================TO MODIFY================================
%==========================================================================
%dimension of the sample
sample_length = 0.004;%m
sample_width = 0.002;%m

xmax = sample_length/2;
xmin = -xmax;
ymax = sample_width/2;
ymin = -ymax;
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
To_Plot=zeros(44,1);


% 3. Bodies
Contours{1,1}={'Simple',[xmin,ymin;xmax,ymin],'Linear',4,2;...
    'Simple',[xmax,ymin;xmax,0],'Linear',1,3;...
    'Simple',[xmax,0;xmin,0],'Linear',2,4;...
    'Simple',[xmin,0;xmin,ymin],'Linear',3,1 ...
    };
Distributions{1,1}={'Rigid'};
Distributions{1,2}=1E-4;
Distributions{1,3}=1;
Interpolations{1,1}='MLS';
Interpolations{1,2}=10;
Integrations{1,1}='Gauss';
Integrations{1,2}=3;
Detections(1,1)=1E-4;
Detections(1,2)=1E-4;
Bodies_Materials{1,1}='Rock';
Imposed_Pressures{1,1}={[],'None',[],'None';...
                        [],'None',[],'None';...
                         [],'None',[],'None';...
                          [],'None',[],'None'...
                        };
Imposed_Velocities{1,1}={[0,0;1e6,0],'Driven',[],[0,0;1e6,0],'Driven',[];...
                         [],'None',[],[],'None',[];...
                          [],'None',[],[],'None',[];...
                           [],'None',[],[],'None',[]...
                         };
Initial_Velocities{1,1}=[0,0];
Mesh_Ratios(1,1:2)=[4,1];
Status{1,1}='active';
Alid{1,1}=[];
Alid{1,2}=[];
Alid{1,3}=[];

Contours{2,1}={'Simple',[xmin,0;xmax,0],'Linear',4,2;...
   'Simple',[xmax,0;xmax,ymax],'Linear',1,3;...
   'Simple',[xmax,ymax;xmin,ymax],'Linear',2,4;...
   'Simple',[xmin,ymax;xmin,0],'Linear',3,1 ...
    };
Distributions{2,1}={'Rigid'};
Distributions{2,2}=1E-4;
Distributions{2,3}=1;
Interpolations{2,1}='MLS';
Interpolations{2,2}=10;
Integrations{2,1}='Gauss';
Integrations{2,2}=3;
Detections(2,1)=1E-4;
Detections(2,2)=1E-4;
Bodies_Materials{2,1}='Rock';
Imposed_Pressures{2,1}={[],'None',[],'None';...
    [],'None',[],'None';...
    [],'None',[0,0;0.001,-10e6;1e6,-10e6],'Oriented';...
    [],'None',[],'None'...
    };
Imposed_Velocities{2,1}={[],'None',[],[],'None',[];...
    [],'None',[],[],'None',[];...
    [0,0;0.001,0;0.003,1;40e6,1],'Driven',[],[],'None',[];...
    [],'None',[],[],'None',[]...
    };
Initial_Velocities{2,1}=[0,0];
Mesh_Ratios(2,1:2)=[4,1];
Status{2,1}='active';
Alid{2,1}=[];
Alid{2,2}=[];
Alid{2,3}=[];

% 4. Materials and contact laws
Materials={'Rock','NeoHookean',[2600,0,0,0,0,0]};
% Contact_Laws={'Rock','Gouge','DampedMohrCoulomb','Evolutive',[1e+15,1e+15,1,100e6,0,0.3];...
%               'Gouge','Gouge','DampedMohrCoulomb','Evolutive',[1e+15,1e+15,0.5,100e6,0,0.3];...
%               };

% Contact_Laws={'Rock','Gouge','DampedMohrCoulomb','Evolutive',[1e+15,1e+15,0,0,0,0.3];...
%               'Gouge','Gouge','CZMlinear','Evolutive',[1e15,50e6,4e-6,10e6,1,1e-5];...
%               };
Contact_Laws={'Rock','Rock','BondedMohrCoulomb','Evolutive',[1e11 1e11 1e11 0 1E8 1E8 0.5 0 0 0.3 1e-4]};
% Contact_Laws={'Rock','Rock','CZMlinear','Evolutive',[1e11,50e6,8e-4,50e5,1,0.3,1e-4]};
% 5. General boundary conditions
Periodic_Boundaries=[2*xmin,2*xmax];
Gravity=[0;0];

% 6. Text outputs
Monitorings=cell(1,1);
Spies={'FORCE_DISP',7,1e-6,{'Displacement X 1 -1';...
                            'Displacement Y 1 -1';...
                            'Force X Contact 1 -1';...
                            'Force Y Contact 1 -1';...
                            'Property LengthInContact 1';...
                            'Contact Gapt 1 0 18'; ...
                            'Contact Px 1 0 18'; ...
                          }
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
To_Plot(9)=1;   % Contact Force
To_Plot(10)=0;  % Body Force
To_Plot(11)=0;  % Dirichlet Force
To_Plot(12)=1;  % Neumann Force
To_Plot(13)=1;  % Damping Force
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

Chains_Parameters = [10e6,1];
Fields_Parameters = [xmin xmax ymin ymax 0.001 0.0005];

% 8. Numerical parameters
Scheme='Adaptive_Euler';
Scheme_Parameters=[1E-6 0.05 100];
Contact_Updating_Period=1e-7;
Time_Stepping_Parameters=[0,2e-8,0.003]; 
Save_Periods=[0.0002,0.0002];

% 9. Flags
Activate_Plot=0;
Initialize_CZM=1;                                                                                                                                                                            