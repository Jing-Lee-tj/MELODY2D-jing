
    % 定义曲线
    rms_roughness = 2;
    [x, y] = generate_rough_curve(start_point, end_point, rms_roughness);

    % x = [0,10];
    % y = [0,5];

    fault_surface = [x,y];

    % 创建判断函数
    side_fn = create_side_function(fault_surface);

    % 生成100个随机测试点
    num_points = 100;
    test_points = [10 * rand(1, num_points); 5 * rand(1, num_points)];

    % 判断每个测试点的位置
    sides = arrayfun(@(i) side_fn(test_points(:, i)'), 1:num_points);

    % 绘制生成的曲线和测试点
    figure;
    hold on;

    % 根据测试点的位置用不同颜色标记曲线的左右侧
    for i = 1:length(x)-1
        % 使用函数句柄判断每个点相对于曲线的位置
        segment_side = side_fn([x(i), y(i)]);
        if segment_side == 1
            plot(x(i:i+1), y(i:i+1), 'r-', 'LineWidth', 2); % 左侧标红
        else
            plot(x(i:i+1), y(i:i+1), 'b-', 'LineWidth', 2); % 右侧标蓝
        end
    end

    % 标出起点和终点
    plot(start_point(1), start_point(2), 'ro', 'MarkerSize', 10, 'MarkerFaceColor', 'r');
    plot(end_point(1), end_point(2), 'go', 'MarkerSize', 10, 'MarkerFaceColor', 'g');

    % 标出测试点，并根据左右侧标记不同颜色
    for i = 1:num_points
        if sides(i) == 1
            plot(test_points(1, i), test_points(2, i), 'mo', 'MarkerSize', 5, 'MarkerFaceColor', 'm'); % 左侧标红
        else
            plot(test_points(1, i), test_points(2, i), 'co', 'MarkerSize', 5, 'MarkerFaceColor', 'c'); % 右侧标蓝
        end
    end

    legend('Left Side', 'Right Side', 'Start Point', 'End Point', 'Test Points (Left)', 'Test Points (Right)');
    title('Generated Rough Curve with Test Points');
    xlabel('X');
    ylabel('Y');
    grid on;
    hold off;
