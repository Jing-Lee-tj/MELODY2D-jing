function NC = es2nc(surfaceEnergy,R1,R2)
%This script calculates the minimum force required to pull the bonded particles apart
%Suppose jkr theory 
%other inputs: m------mass
               % kn-----elastic_stiffness parameters
         R_equal = 1 / (1/R1+1/R2);
         NC = 3 * pi / 2 *R_equal * surfaceEnergy;
end
%exmaple restitution_to_damping(e,m,kn)