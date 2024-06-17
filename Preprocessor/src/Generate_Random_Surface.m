function [X,Y] = Generate_Random_Surface(xmin,xmax,ymean,ydevmax,node_distance)
%input:(xmin,xmax,ymean,ydevmax,node_distance)
%output:(X,Y)
%------for test-------
% xmin = 0;
% xmax = 0.001;
% ymean = 0;
% ydevmax = 5e-5;
% node_distance = 1e-5;
Period= xmax - xmin;%10;
Nnodes= round(Period/node_distance);%200;
D2=0.6;%0;
D3=0.3;%0.5;
Decay1=-2;%-1.3;
D8=0.1;%0.1;
Decay2=-2;%-1.3;

Dx=Period/Nnodes;
X=[0:Dx:Period]';
[Cn]=Spectre(Nnodes,1,10^-10,D2,D3,Decay1,D8,Decay2);
% Cn(1)=0;
%Cn(2:64)=0;
Cn=Cn;

An=Cn.*cos(Cn);
Bn=Cn.*sin(Cn);
Teta=transpose(0:2*3.14159/Nnodes:2*3.14159);
[~,~,Y]=Fourier_Recons(Teta,An,Bn,0,0);
% plot(x,y)

% figure;loglog(Cn.^2);
% Y=zeros(Nnodes+1,1);
% for i=2:Nnodes/2
%     % Y=Y+Cn(i)*cos(2*X/Period*pi*(i-1)+2*pi*rand);
%     Y=Y+Cn(i)*cos(2*X/Period*pi*(i-1));
% end
Y(1) = 0;
Y(end) = Y(1);
Y=smooth(Y,5);
scale = ydevmax/(max(Y) - min(Y))*2;
Y = Y * scale -mean(Y) *scale + ymean;
%------for test-------
% figure;plot(X,Y,'.-b');axis equal
% xlim([0 0.004])
% ylim([-1e-4 1e-4])
% RMS=sqrt(mean(Y(1:Nnodes).^2))
% % Amplitude=[min(Y),max(Y),max(Y)-min(Y)];
% max(Y)-min(Y)

