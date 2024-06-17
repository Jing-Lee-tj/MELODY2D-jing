function A = dist2A(dist,Ri,Rj)
    A = pi/4 * ((dist-Ri-Rj)*(dist+Ri-Rj)*(dist-Ri+Rj)*(dist+Ri+Rj) )/(dist*dist);
end

%exampel: A = dist2A(100e-6-1e-6,50e-6,50e-6)