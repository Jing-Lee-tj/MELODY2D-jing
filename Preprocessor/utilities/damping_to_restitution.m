function restitution = damping_to_restitution(damping,m,kn)
%This function calculates the restitution coefficient from the damping coefficient
%other inputs: m------mass
               % kn-----elastic_stiffness parameters
% note the normalised damping coefficient is defined as
% damping_coefficient/(2 * sqrt (m *kn))
         restitution1 = exp(pi./sqrt( damping*damping *m*kn/4 - 1 ));
         restitution2 = exp(-pi./sqrt(  4*m*kn/ (damping*damping) - 1 ));
         if restitution1 <1
             restitution = restitution1;
         else
             restitution = restitution2;
         end
end