% 1. Simulation name
Simulation_Name='shear_N_5000_F_2_D_8x4'; 

%=================================TO MODIFY================================
%==========================================================================
%dimension of the sample
weak_co = 0.8;
gouge_length = 0.004;%m
gouge_width = 0.002;%m
plate_width = 0.001;%m
deltaY = 0.000005;
first_bodies=2;



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
To_Plot=zeros(44,1);


% 3. Bodies
X=(gouge_length:-1E-5:0)';
Y=zeros(size(X));
%-----------Sin_Surface-----------------
% X=(gouge_length:-1E-5:0)';
% Y=-5E-5+cos((gouge_length:-1e-5:0)'/2e-4*2*pi)*5e-5;
%-----------Random_Surface-----------------
% [X1,Y1] = Generate_Random_Surface(0,gouge_length,0,5e-5,1e-5);
% X1 = flipud(X1);
% Y1 = flipud(Y1);
% Y1 = Y1 - max(Y1); 
Contours{1,1}={'Periodic',[0,-plate_width;gouge_length,-plate_width],'Linear';...
               'Periodic',[X,Y],'Linear'};
% Contours{1,1}={'Periodic',[0,-plate_width-gouge_width;gouge_length,-plate_width-gouge_width],'Linear';...
%                'Periodic',[(gouge_length:-1e-5:0)',-gouge_width+cos([gouge_length:-1e-5:0]'/gouge_length*2*pi*101)*5e-5],'Linear'};
Distributions{1,1}={'Rigid'};
Distributions{1,2}=4e-5;
Distributions{1,3}=1;
Interpolations{1,1}='MLS';
Interpolations{1,2}=10;
Integrations{1,1}='Gauss';
Integrations{1,2}=3;
Detections(1,1)=1e-5;
Detections(1,2)=1e-5;
Bodies_Materials{1,1}='Rock';
Imposed_Pressures{1,1}={[],'None',[],'None';...
    [],'None',[0,-10e6;1e6,-10e6],'Oriented';...
    };
Imposed_Velocities{1,1}={[],'None',[],[],'None',[];...
    [0,0;1e6,0],'Driven',[],[],'None',[];...
    };
Initial_Velocities{1,1}=[0,0];
Mesh_Ratios(1,1:2)=[4,1];
Status{1,1}='active';
Alid{1,1}=[];
Alid{1,2}=[];
Alid{1,3}=[];

%-----------plate_Surface-----------------
X=(0:1e-5:gouge_length)';
Y=(gouge_width + deltaY)*ones(size(X));
%-----------Sin_Surface-----------------
% X =(0:1e-5:gouge_length)';
% Y =gouge_width + deltaY + cos((0:1e-5:gouge_length)/1e-4'*2*pi)*5e-5;
%-----------Random_Surface-----------------
% [X,Y] = Generate_Random_Surface(0,gouge_length,gouge_width,5e-5,1e-5);
% Y = Y + (gouge_width - min(Y)) + deltaY; 
% Contours{2,1}={'Periodic',[X,Y],'Linear'; ...
%                'Periodic',[gouge_length,plate_width+gouge_width;0,plate_width+gouge_width],'Linear'};
% Contours{2,1}={'Periodic',[(0:1e-5:gouge_length)',deltaY+cos([0:1e-5:gouge_length]'/gouge_length*2*pi*101)*5e-5],'Linear';...
%                'Periodic',[gouge_length,plate_width+deltaY;0,plate_width+deltaY],'Linear'};

% Distributions{2,1}={'Rigid'};
% Distributions{2,2}=4E-5;
% Distributions{2,3}=1;
% Interpolations{2,1}='MLS';
% Interpolations{2,2}=10;
% Integrations{2,1}='Gauss';
% Integrations{2,2}=3;
% Detections(2,1)=1e-5;
% Detections(2,2)=1e-5;
% Bodies_Materials{2,1}='Rock';
% Imposed_Pressures{2,1}={[],'None',[],'None';...
%     [],'None',[0,-10e6;1e6,-10e6],'Oriented';...
%     };
% Imposed_Velocities{2,1}={[],'None',[],[],'None',[];...
%     [0,0;1e6,0],'Driven',[],[],'None',[];...
%     };
% Initial_Velocities{2,1}=[0,0];
% Mesh_Ratios(2,1:2)=[1,4];
% Status{2,1}='active';
% Alid{2,1}=[];
% Alid{2,2}=[];
% Alid{2,3}=[];

% 4. Materials and contact laws
Materials={'Rock','NeoHookean',[2600,0,0,0,0,0];...
           'Gouge','NeoHookean',[2600,0,0,0,0,0];...
           'Gouge_weak','NeoHookean',[2600,0,0,0,0,0];...
           };

Contact_Laws={'Rock','Gouge','BondedMohrCoulomb','Evolutive',[1e+15 1e+15 1e+15 0 2500e6 2500e6 1 0 0 0.3 1e-5];...
              'Rock','Gouge_weak','BondedMohrCoulomb','Evolutive',[1e+15 1e+15 1e+15 0 2500e6 2500e6 1 0 0 0.3 1e-5];...
              'Gouge','Gouge','BondedMohrCoulomb','Evolutive',[1e+15 1e+15 1e+15 0 2500e6 2500e6 0.5 0 0 0.3 1e-5];...
              'Gouge','Gouge_weak','BondedMohrCoulomb','Evolutive',[1e+15 1e+15 1e+15 0 [2500e6 2500e6]*weak_co 0.5 0 0 0.3 1e-5];...
              'Gouge_weak','Gouge_weak','BondedMohrCoulomb','Evolutive',[1e+15 1e+15 1e+15 0 [2500e6 2500e6]*weak_co 0.5 0 0 0.3 1e-5];...
              };

% 5. General boundary conditions
Periodic_Boundaries=[0,gouge_length+1e-3];
Gravity=[0;0];

% 6. Text outputs
Monitorings=cell(1,1);
Spies={'FORCE_DISP',4,1e-6,{'Displacement X 1 -1';...
                            'Displacement Y 1 -1';...
                            'Force X Contact 1 -1';...
                            'Force Y Contact 1 -1';...
                            };
        % "CONTACT",6,1e-6,{'Contact Gapt 1 0 200';...
        %                    'Contact Gapn 1 0 200';...
        %                    'Contact Px 1 0 200';...
        %                    'Contact Py 1 0 200';...
        %                    'Contact Damage 1 0 200';...
        %                    'Contact Xslave 1 0 200';...

        % }
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
To_Plot(11)=1;  % Dirichlet Force
To_Plot(12)=1;  % Neumann Force
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

Chains_Parameters = [10e6,1];
Fields_Parameters = [0 gouge_length -gouge_width 0 0.00005 0.00001];

% 8. Numerical parameters
Scheme='Adaptive_Euler';
Scheme_Parameters=[0.0001 0.2 10];
Contact_Updating_Period=1e-7;
Time_Stepping_Parameters=[0,1e-9,0.0002];
Save_Periods=[1e-5,1e-5];

% 9. Flags
Activate_Plot=0;
Initialize_CZM=0;                                                                                                                                                                            