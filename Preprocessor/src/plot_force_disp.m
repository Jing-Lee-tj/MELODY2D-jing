% 设置默认字体名称
% set(0, 'DefaultTextInterpreter', '宋体');
 
% 设置默认字号大小
set(0, 'DefaultAxesFontSize', 18);
set(0, 'DefaultTextFontSize', 18);
 
% 如果需要设置图例、标题等的默认字号，可以继续设置相应的属性
set(0, 'DefaultLegendFontSize', 18);
% path = 'F:\Code-MELODY_3.92\Examples\direcshear_load_rigid';
% path = 'C:\Users\jingli\Desktop';
% path = 'F:\Code-MELODY_3.92\Examples\uniaxial'
path = 'F:\LIGGGHTS-PUBLIC\examples\inertial_number';
% path = 'F:\Code-MELODY_3.92\Examples\Samples_and_results\Data_from_simulation\dense_C_2500_95%';
% % path = 'F:\Code-MELODY_3.92\Examples\contact_test_CZMLinear';
filename = 'FORCE_DISP.asc';
fopen = fullfile(path,filename);
data = load(fopen);
time = data(:,2)*1000; % in ms units
xdisplacement = data(:,3) * 1000; % in mm units
ydisplacement = data(:,4) * 1000; % in mm units
fxcontact = data(:,5);
fycontact = data(:,6);
% fxcontact = data(:,7);
% fycontact = data(:,8);
% lengthincontact = data(:,7)*1000;
% gapt = data(:,8)*1000;
% contactpx = data(:,9);

friction = abs(fxcontact./fycontact);
% figure('Name','xdisplacement') ;plot(time,xdisplacement);
% xlabel('time/ms');ylabel('xdisplacement/mm')
% figure('Name','ydisplacement') ;plot(time,ydisplacement);
% xlabel('time/ms');ylabel('ydisplacement/mm')
figure('Name','fxcontact');plot(time,abs(fxcontact));
xlabel('time/ms');ylabel('fxcontact/N')
figure('Name','fycontact');plot(time,abs(fycontact));
xlabel('time/ms');ylabel('fycontact/N')
% figure('Name','fycontact-ydisplacement');plot(abs(ydisplacement),abs(fycontact));
% xlabel('ydisplacement/mm');ylabel('fycontact/N');
% figure('Name','lengthincontact');plot(time,lengthincontact);
% xlabel('time/ms');ylabel('lengthincontact/mm')
% figure('Name','gapt-px');plot(abs(gapt),abs(contactpx));
% xlabel('gapt/mm');ylabel('contactpx(N/m2)')
% figure('Name','gapn-px');plot(abs(gapn),abs(contactpx));
% xlabel('gapn/mm');ylabel('contactpx(N/m2)')
figure('Name','friction');plot(xdisplacement,smooth(friction));
xlabel('xdisplacement/mm');ylabel('friction')
% figure() ;
% figure() ;plot(xdisplacement,abs(fxcontact));

