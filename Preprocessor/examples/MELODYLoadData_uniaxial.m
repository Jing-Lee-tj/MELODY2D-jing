% 1. Simulation name
Simulation_Name='uniaxial';

%=================================TO MODIFY================================
%==========================================================================
%dimension of the sample
gouge_width= 0.004;%m
gouge_length= 0.008;%m
plate_length = 0.008;%m
plate_width = 0.001;%m
first_bodies=6;

mat_file=load('D:\Packing2D1.1\unic_F_26.mat');
ContoursGrains=mat_file.Contours;
SampleProperties = mat_file.SampleProperties;
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
To_Plot=zeros(44,1);


% 3. Bodies
Contours{1,1}={'Closed',[0,-plate_width;plate_length,-plate_width;plate_length,0;0,0],'Linear'};
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
Imposed_Pressures{1,1}={[],'None',[],'None'};
Imposed_Velocities{1,1}={[0,0;1e6,0],'Driven',[],[0,0;1e6,0],'Driven',[]};
Initial_Velocities{1,1}=[0,0];
Mesh_Ratios(1,1:2)=[10,1];
Status{1,1}='active';
Alid{1,1}=[];
Alid{1,2}=[];
Alid{1,3}=[];

xmin_temp = plate_length/2-gouge_width/2;
xmax_temp = plate_length/2+gouge_width/2;
ymin_temp = gouge_length;
ymax_temp = gouge_length+plate_width;
Contours{2,1}={'Closed',[xmin_temp,ymin_temp;xmax_temp,ymin_temp;xmax_temp, ...
    ymax_temp;xmin_temp,ymax_temp],'Linear'};
Distributions{2,1}={'Rigid'};
Distributions{2,2}=4E-5;
Distributions{2,3}=1;
Interpolations{2,1}='MLS';
Interpolations{2,2}=10;
Integrations{2,1}='Gauss';
Integrations{2,2}=3;
Detections(2,1)=1E-5;
Detections(2,2)=1E-5;
Bodies_Materials{2,1}='Rock';
Imposed_Pressures{2,1}={[],'None',[0,-1e6;1e6,-1e6],'Oriented'};
Imposed_Velocities{2,1}={[0,0;1e6,0],'Driven',[],[],'None',[]};
Initial_Velocities{2,1}=[0,0];
Mesh_Ratios(2,1:2)=[10,1];
Status{2,1}='active';
Alid{2,1}=[];
Alid{2,2}=[];
Alid{2,3}=[];

xmin_temp = plate_length/2-gouge_width/2-plate_width;
xmax_temp = xmin_temp+plate_width;
ymin_temp = 0;
ymax_temp = gouge_length+plate_width;
Contours{3,1}={'Closed',[xmin_temp,ymin_temp;xmax_temp,ymin_temp;xmax_temp, ...
    ymax_temp;xmin_temp,ymax_temp],'Linear'};
Distributions{3,1}={'Rigid'};
Distributions{3,2}=4E-5;
Distributions{3,3}=1;
Interpolations{3,1}='MLS';
Interpolations{3,2}=10;
Integrations{3,1}='Gauss';
Integrations{3,2}=3;
Detections(3,1)=1E-5;
Detections(3,2)=1E-5;
Bodies_Materials{3,1}='Rock';
Imposed_Pressures{3,1}={[],'None',[],'None'};
Imposed_Velocities{3,1}={[0,0;1e6,0],'Driven',[],[0,0;1e6,0],'Driven',[]};
Initial_Velocities{3,1}=[0,0];
Mesh_Ratios(3,1:2)=[10,1];
Status{3,1}='active';
Alid{3,1}=[];
Alid{3,2}=[];
Alid{3,3}=[];


xmin_temp = plate_length/2+gouge_width/2;
xmax_temp = xmin_temp+plate_width;
ymin_temp = 0;
ymax_temp = gouge_length+plate_width;
Contours{4,1}={'Closed',[xmin_temp,ymin_temp;xmax_temp,ymin_temp;xmax_temp, ...
    ymax_temp;xmin_temp,ymax_temp],'Linear'};
Distributions{4,1}={'Rigid'};
Distributions{4,2}=4E-5;
Distributions{4,3}=1;
Interpolations{4,1}='MLS';
Interpolations{4,2}=10;
Integrations{4,1}='Gauss';
Integrations{4,2}=3;
Detections(4,1)=1E-5;
Detections(4,2)=1E-5;
Bodies_Materials{4,1}='Rock';
Imposed_Pressures{4,1}={[],'None',[],'None'};
Imposed_Velocities{4,1}={[0,0;1e6,0],'Driven',[],[0,0;1e6,0],'Driven',[]};
Initial_Velocities{4,1}=[0,0];
Mesh_Ratios(4,1:2)=[10,1];
Status{4,1}='active';
Alid{4,1}=[];
Alid{4,2}=[];
Alid{4,3}=[];


xmin_temp = plate_length/2-gouge_width/2-plate_width;
xmax_temp = xmin_temp+plate_width;
ymin_temp = 0;
ymax_temp = gouge_length+plate_width;
Contours{5,1}={'Simple',[xmin_temp,ymin_temp;xmax_temp,ymin_temp],'Linear',4,2;...
    'Simple',[xmax_temp,ymin_temp;xmax_temp,ymax_temp],'Linear',1,3;...
     'Simple',[xmax_temp,ymax_temp;xmin_temp,ymax_temp],'Linear',2,4;...
      'Simple',[xmin_temp,ymax_temp;xmin_temp,ymin_temp],'Linear',3,1};
Distributions{5,1}={'Unstructured'};
Distributions{5,2}=4E-5;
Distributions{5,3}=1;
Interpolations{5,1}='MLS';
Interpolations{5,2}=10;
Integrations{5,1}='Gauss';
Integrations{5,2}=3;
Detections(5,1)=1E-5;
Detections(5,2)=1E-5;
Bodies_Materials{5,1}='ElasticPlate';
Imposed_Pressures{5,1}={[],'None',[],'None';...
    [],'None',[],'None';...
    [],'None',[],'None';...
    [0,1e6;1e6,1e6],'Following',[],'None' ...
    };
Imposed_Velocities{5,1}={[],'None',[],[],'None',[];...%the driven is only applicable for rigids
    [],'None',[],[],'None',[];...
    [],'None',[],[],'None',[];...
    [],'None',[],[],'None',[]};
Initial_Velocities{5,1}=[0,0];
Mesh_Ratios(5,1:4)=[10,1,1,8];
Status{5,1}='inactive';
Alid{5,1}=[];
Alid{5,2}=[];
Alid{5,3}=[];
% 
xmin_temp = plate_length/2+gouge_width/2;
xmax_temp = xmin_temp+plate_width;
ymin_temp = 0;
ymax_temp = gouge_length+plate_width;
Contours{6,1}={'Simple',[xmin_temp,ymin_temp;xmax_temp,ymin_temp],'Linear',4,2;...
    'Simple',[xmax_temp,ymin_temp;xmax_temp,ymax_temp],'Linear',1,3;...
     'Simple',[xmax_temp,ymax_temp;xmin_temp,ymax_temp],'Linear',2,4;...
      'Simple',[xmin_temp,ymax_temp;xmin_temp,ymin_temp],'Linear',3,1};
Distributions{6,1}={'Unstructured'};
Distributions{6,2}=4E-5;
Distributions{6,3}=1;
Interpolations{6,1}='MLS';
Interpolations{6,2}=10;
Integrations{6,1}='Gauss';
Integrations{6,2}=3;
Detections(6,1)=1E-5;
Detections(6,2)=1E-5;
Bodies_Materials{6,1}='ElasticPlate';
Imposed_Pressures{6,1}={[],'None',[],'None';...
    [0,-1e6;1e6,-1e6],'Following',[],'None'; ...
    [],'None',[],'None';...
    [],'None',[],'None'};
Imposed_Velocities{6,1}={[],'None',[],[],'None',[];...%the driven is only applicable for rigids
    [],'None',[],[],'None',[];...
    [],'None',[],[],'None',[];...
    [],'None',[],[],'None',[]};
Initial_Velocities{6,1}=[0,0];
Mesh_Ratios(6,1:4)=[10,1,1,8];
Status{6,1}='inactive';
Alid{6,1}=[];
Alid{6,2}=[];
Alid{6,3}=[];

for num=1:Ngrains
    Contoursx = ContoursGrains{num,1}(:,1)/1000 + (plate_length-gouge_width)/2;
    Contoursy = ContoursGrains{num,1}(:,2)/1000;
    Contoursx(Contoursx<(plate_length-gouge_width)/2,1) = (plate_length-gouge_width)/2;
    Contoursx(Contoursx>(plate_length+gouge_width)/2,1) = (plate_length+gouge_width)/2;
    Contoursy(Contoursy<0,1) = 0;
    Contoursy(Contoursy>gouge_length,1) = gouge_length;
    Contours{first_bodies+num,1}={'Closed',[Contoursx Contoursy],'Linear'};   %on divise par 1000 car echantillon en mm
    Distributions{first_bodies+num,1}={'Rigid'};
    xydata = ContoursGrains{num,1}/1000;
    perimeter = SampleProperties(num,7)/1000; % m units
    Distributions{first_bodies+num,2} = perimeter/15;
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
Materials={'Rock','NeoHookean',[2600,0,0,0,0,0];...
           'Gouge','NeoHookean',[2600,0,0,0,0,0]; ...
           'ElasticPlate','NeoHookean',[2600,0,0,1e6,0.3,0] ...
           };

Contact_Laws={'Rock','Rock','DampedMohrCoulomb','Evolutive',[1e+15 1e+15 0 0 0 0.3]; ...
    'Rock','Gouge','DampedMohrCoulomb','Evolutive',[1e+15 1e+15 0 0 0 0.3]; ...
    'Rock','ElasticPlate','DampedMohrCoulomb','Evolutive',[1e+15 1e+15 0.1 0 10e6 0.3]; ...
     'Gouge','ElasticPlate','DampedMohrCoulomb','Evolutive',[1e+15 1e+15 0 0 0 0.3]; ...
    'Gouge','Gouge','DampedMohrCoulomb','Evolutive',[1e+15 1e+15 0 0 0 0.3]; ...
     'ElasticPlate','ElasticPlate','DampedMohrCoulomb','Evolutive',[1e+15 1e+15 0 0 0 0.3]; ...
    };
% Contact_Laws={'Rock','Rock','BondedMohrCoulomb','Evolutive',[1e15 1e15 1e15 0 2e9 2e9 1 0 0 0.3 1e-4]
%     'Rock','Gouge','BondedMohrCoulomb','Evolutive',[1e15 1e15 1e15 0 2e9 2e9 0.5 0 0 0.3 1e-4]};


% Contact_Laws={'Rock','Gouge','CZMlinear','Evolutive',[1e15 400e6 2e-6 10e6 1 0.3 1e-5]; ...
%               'Gouge','Gouge','CZMlinear','Evolutive',[1e15 400e6 2e-6 10e6 1 0.3 1e-5]
%               };

% 5. General boundary conditions
Periodic_Boundaries=[-0.1,0.1];
Gravity=[0;-9.8];

% 6. Text outputs
Monitorings=cell(1,1);
% Spies=cell(1,1);
Spies={'FORCE_DISP',4,1e-7,{'Displacement X 1 -1';...
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
To_Plot(38)=1;  % Neumann Work
To_Plot(39)=0;  % Damping Work
To_Plot(40)=0;  % Alid Work
To_Plot(41)=0;  % Temperature
To_Plot(42)=0;  % Scaling Parameter
To_Plot(43)=0;  % Active Contacts
To_Plot(44)=0;  % Contacting Bodies

Chains_Parameters = [10e6,1];
Fields_Parameters = [(plate_length-gouge_width)/2 (plate_length+gouge_width)/2 ...
0 gouge_length 0.0001 0.00001];

% 8. Numerical parameters
Scheme='Adaptive_Euler';
Scheme_Parameters=[1e-4,0.2,10];
Contact_Updating_Period=1e-7;
Time_Stepping_Parameters=[0,1e-9,0.0003];
Save_Periods=[0.000005,0.000005];

% 9. Flags
Activate_Plot=0;
Initialize_CZM=0;                                                                                                                                          