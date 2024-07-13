% Particle small dimension S
mat_file=load('grainfiles\shear_U_1.2_D_20_2.mat');
SampleProperties = mat_file.SampleProperties;
S_small = SampleProperties(:,4);
max_S_small = max(S_small)
min_S_small = min(S_small)
max_min_ratio_S_small = max_S_small/min_S_small
Area = SampleProperties(:,6);
max_Area = max(Area)
min_Area = min(Area)
max_min_ratio_Area = max_Area/min_Area

radius = SampleProperties(:,11);
max_radius = max(radius)
min_radius = min(radius)
max_min_ratio_radius = max_radius/min_radius