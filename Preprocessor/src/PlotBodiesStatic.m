function PlotBodiesStatic
global NUMBER_BODIES NUMBER_RIGIDS BODIES_STATIC GRAPHIC_BOUNDARIES
load('Random_Colors.mat')
% load('F:\Code-MELODY_3.92\Examples\uniaxial\uniaxial_Simulation_0.mat')
% load('F:\Code-MELODY_3.92\Examples\uniaxial\STATIC.mat')


figure
for Body=1:NUMBER_RIGIDS
% for Body = [954 4504]
    NUMBER_BORDERS=BODIES_STATIC(Body).NUMBER_BORDERS;
    x=BODIES_STATIC(Body).INITIAL_POSITIONS(:,1);
    y=BODIES_STATIC(Body).INITIAL_POSITIONS(:,2);
    plot(x,y,'.b')
    hold on
    for i=1:NUMBER_BORDERS
        b=BODIES_STATIC(Body).BORDERS{i,3};
        plot(x(b),y(b),'.-','color',Random_Colors(i,:),'linewidth',2,'markersize',16)
    end
end
axis equal
axis(GRAPHIC_BOUNDARIES)
drawnow