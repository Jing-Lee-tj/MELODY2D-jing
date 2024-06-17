% 设置默认字体名称
% set(0, 'DefaultTextInterpreter', '宋体');
 
% 设置默认字号大小
set(0, 'DefaultAxesFontSize', 18);
set(0, 'DefaultTextFontSize', 18);
 
% 如果需要设置图例、标题等的默认字号，可以继续设置相应的属性
% set(0, 'DefaultLegendFontSize', 18);
% set(0, 'DefaultTitleFontSize', 18);

path = 'C:\Users\jingli\Desktop';
% path = 'F:\Code-MELODY_3.92\Examples\contact_test_CZMLinear';
filename = 'CONTACT.asc';
fopen = fullfile(path,filename);
data = load(fopen);
time = data(:,2)*1000; % in ms units
Gapt = data(:,3) * 1000; % in mm units
Gapn = data(:,4) * 1000; % in mm units
Px = data(:,5);
Py = data(:,6);
Damage = data(:,7);

figure('Name','Gapt') ;plot(time,Gapt);
xlabel('time/ms');ylabel('Gapt/mm')
figure('Name','Gapn') ;plot(time,Gapn);
xlabel('time/ms');ylabel('Gapn/mm')
figure('Name','Px');plot(time,abs(Px));
xlabel('time/ms');ylabel('Px/N')
figure('Name','Py');plot(time,abs(Py));
xlabel('time/ms');ylabel('Py/N')
figure('Name','Py-Gapn');plot(abs(Gapn),abs(Py));
xlabel('Gapn/mm');ylabel('Py/N/m2');
figure('Name','Px-Gapt');plot(abs(Gapt),abs(Px));
xlabel('Gapt/mm');ylabel('Px/N/m2');
% figure('Name','lengthincontact');plot(time,lengthincontact);
% xlabel('time/ms');ylabel('lengthincontact/mm')
% figure('Name','gapt-px');plot(abs(gapt),abs(contactpx));
% xlabel('gapt/mm');ylabel('contactpx(N/m2)')
% figure('Name','gapn-px');plot(abs(gapn),abs(contactpx));
% xlabel('gapn/mm');ylabel('contactpx(N/m2)')
% figure('Name','friction');plot(xdisplacement,friction);
% xlabel('xdisplacement/mm');ylabel('friction')
% figure() ;
% figure() ;plot(xdisplacement,abs(fxcontact));

