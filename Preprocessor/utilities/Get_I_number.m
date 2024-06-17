function I =Get_I_number(shear_strain_rate,D,rho,pressure)
    I = shear_strain_rate * D * sqrt(rho/pressure);
end

%example:
% Get_I_number(1000,100E-6,2000,20e6)