area = SampleProperties(:,6)/1000;
fractal_calculate(area)

% [X,N] = Fractal(min(area),max(area),1.2,2000);
% plot(N)
% hold on;
% [X,N] = Fractal(min(area),max(area),2.6,2000);
% plot(N)
% legend("1.2","2.6")