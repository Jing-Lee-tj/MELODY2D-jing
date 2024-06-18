figure()
N=size(Contours,1);
plot(0,0)
hold on;
for num=1:N
    xydata = Contours{num,1}(:,2);
    N2 = size(xydata,1);
    for num2 = 1:N2
        grain_x = xydata{num2,1}(:,1); % m units
        grain_y = xydata{num2,1}(:,2); % m units
        plot(grain_x,grain_y,'-')
    end
end
plot(fault_surface(:,1),fault_surface(:,2));
axis equal;
