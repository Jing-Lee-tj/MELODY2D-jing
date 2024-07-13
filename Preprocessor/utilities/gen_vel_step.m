function vel = gen_vel_step(totaltime,pertime,pretime,vel1,vel2)
    vel_list=[vel1,vel2];
    iter = 1;

    switch_time = 0.00001;

    % initializaton
    vel=[0 0;pretime 0;pretime+switch_time vel1];
    currenttime=pretime;
    while(currenttime<totaltime)
        vel = [vel;currenttime+pertime,vel_list(1);currenttime+pertime+switch_time,vel_list(2)];
        vel_list = fliplr(vel_list);
        currenttime = currenttime+pertime;
    end
end