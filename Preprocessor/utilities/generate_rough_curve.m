function curve = generate_rough_curve(start_point, end_point, rms_roughness, num_points, smoothing_sigma)
    % 输入参数
    if nargin < 4
        num_points = 100; % 默认点数
    end
    if nargin < 5
        smoothing_sigma = 1.0; % 默认平滑标准差
    end
    
    % 计算线段的距离
    distance = norm(end_point - start_point);
    
    % 生成具有给定粗糙度的水平线段
    x = linspace(0, distance, num_points);
    y = rms_roughness * randn(1, num_points);
    
    % 平滑处理
    y = imgaussfilt(y, smoothing_sigma);
    
    % 确保起点和终点的y坐标为0（因为是水平线段）
    y(1) = 0;
    y(end) = 0;
    
    % 计算旋转角度
    angle = atan2(end_point(2) - start_point(2), end_point(1) - start_point(1));
    
    % 旋转矩阵
    rotation_matrix = [cos(angle), -sin(angle); sin(angle), cos(angle)];
    
    % 旋转和平移
    rotated_points = rotation_matrix * [x; y];
    x = rotated_points(1, :) + start_point(1);
    y = rotated_points(2, :) + start_point(2);
    
    % 确保起点和终点的坐标不变
    x(1) = start_point(1);
    y(1) = start_point(2);
    x(end) = end_point(1);
    y(end) = end_point(2);
    curve = [x',y'];
    % % 绘制结果
    % figure;
    % plot(x, y, 'b-', 'LineWidth', 2);
    % hold on;
    % plot(start_point(1), start_point(2), 'ro', 'MarkerSize', 10, 'MarkerFaceColor', 'r');
    % plot(end_point(1), end_point(2), 'go', 'MarkerSize', 10, 'MarkerFaceColor', 'g');
    % legend('Rough Curve', 'Start Point', 'End Point');
    % title('Generated Rough Curve');
    % xlabel('X');
    % ylabel('Y');
    % grid on;
    % hold off;
end
