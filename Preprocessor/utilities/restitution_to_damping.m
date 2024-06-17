function damping = restitution_to_damping(e,m,kn)
%This function calculates the damping coefficient from the recovery coefficient
%other inputs: m------mass
               % kn-----elastic_stiffness parameters
         damping =-2*log(e)*sqrt( m*kn / ( pi*pi+log(e)*log(e) ) );
end
%exmaple restitution_to_damping(e,m,kn)