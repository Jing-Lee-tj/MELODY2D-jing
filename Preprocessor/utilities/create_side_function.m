function side_fn = create_side_function(fault_surface)
    % 创建函数句柄，用于判断点在曲线的左侧还是右侧
    % fault_surface 是一个 2xN 的矩阵，表示曲线上的点
    
    % 预计算曲线的线段向量
    curve_x = fault_surface(:, 1);
    curve_y = fault_surface(:, 2);
    
    % 创建函数句柄
    side_fn = @(points) calculate_side(points, curve_x, curve_y);
end


function side = calculate_side(points, curve_x, curve_y)
    % Step 1: 计算曲线起点到终点的连线相对于x轴的角度
    angle = atan2(curve_y(end) - curve_y(1), curve_x(end) - curve_x(1));
    
    % Step 2: 顺时针旋转曲线和点
    rotated_curve = rotate_curve(curve_x, curve_y, -angle);
    rotated_points = rotate_points(points, -angle);
    
    % Step 3: 插值计算
    interpolated_y = interp1(rotated_curve(:, 1), rotated_curve(:, 2), rotated_points(:, 1),'pchip', 'extrap');
    
    % 判断方向
    side = sign(rotated_points(:, 2) - interpolated_y);
    side(side<0) = 0;
    % figure()
    % plot(curve_x, curve_y)
    % hold on;
    % plot(points(:,1),points(:,2),'o');
    % figure()
    % plot(rotated_curve(:, 1), rotated_curve(:, 2))
    % hold on;
    % plot(rotated_points(:,1),rotated_points(:,2),'o');
    % side
end

function rotated_curve = rotate_curve(curve_x, curve_y, angle)
    % 构造曲线矩阵
    curve = [curve_x(:), curve_y(:)];
    % 计算旋转矩阵
    R = [cos(angle), -sin(angle); sin(angle), cos(angle)];
    % 对曲线进行旋转
    rotated_curve = (R * curve')';
end

function rotated_points = rotate_points(points, angle)
    % 构造点矩阵
    points = points';
    % 计算旋转矩阵
    R = [cos(angle), -sin(angle); sin(angle), cos(angle)];
    % 对点进行旋转
    rotated_points = (R * points)';
end
