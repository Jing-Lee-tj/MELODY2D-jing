function tc = Get_shock_characteristic_time(kn,gn,m)
%This function returns the characteristic time of interparticle swelling collisions
%other input parameters: kn------stiffness coefficient
%gn------damping coefficient
%m------particle mass
% A time step equal to τc/100 was usually chosen to perform simulations with standard molecular dynamics method
    tc = pi/sqrt(2*kn/m-(gn/m).^2);
end

% example:
% Get_shock_characteristic_time(2e13*6.3e-6,0.3,2000*pi*100e-6*100e-6/4)