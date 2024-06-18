function [DP1,DP2] = generate_fault_particles(DP)
    roughness = 5e-3;%mm
    fs1 = (DP(1,:)+DP(2,:))/2;
    fs2 = (DP(3,:)+DP(4,:))/2;
    upwall = generate_rough_curve(fs1, fs2, roughness);
    upwall(:,1) = upwall(:,1)-2*roughness;
    downwall = generate_rough_curve(fs1, fs2, roughness);
    downwall(:,1)= downwall(:,1)+2*roughness;
    DP1 = [DP(1,:);upwall;DP(4,:);DP(1,:)];
    DP2 = [DP(2,:);DP(3,:);flipud(downwall);DP(2,:)];
    figure();
    plot(DP1(:,1),DP1(:,2));
    hold on;
    plot(DP2(:,1),DP2(:,2));
end