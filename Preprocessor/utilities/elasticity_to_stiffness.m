function stiffness = elasticity_to_stiffness(E1,V1,A1,E2,V2,A2,delta)
%E---Youngs Modulus
%V---possion ratio
%A---radius
    stiffness = 4/3*(((1-V1.^2)/E1+(1-V2.^2)/E2)^(-1))*(((A1+A2)/(A1*A2)./(delta)).^(-1/2));
end
% elasticity_to_stiffness(4e12,0.25,50e-6,4e12,0.25,50e-6,1e-6)