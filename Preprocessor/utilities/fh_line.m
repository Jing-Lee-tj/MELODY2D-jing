function distance_func = fh_line(line_coords, min_distance, max_distance, transition_distance)
%该函数返回一个指定节点间距的句柄函数，该句柄函数接受点的坐标，计算点到线段的距离，当该距离小于过渡距离时，
% 返回最小节点距离，当距离大于过渡距离时，逐渐过渡到最大节点距离。
    % 确定线段的起点和终点
    p1 = line_coords(1,:);
    p2 = line_coords(2,:);
    
    % 计算线段的方向向量和长度
    line_vec = p2 - p1;
    line_length = vecnorm(line_vec);
    
    % 计算单位方向向量
    unit_dir = line_vec / line_length;
    
    function dist = distance_to_line(point)
        % 计算点到线段起点的向量
        vec_start_to_point = point - p1;
        unit_dirs = repmat(unit_dir, size(vec_start_to_point,1), 1);
        % 计算点到线段的投影长度
        proj_length = dot(vec_start_to_point, unit_dirs, 2);
        
        % 初始化距离数组
        dist = zeros(size(point, 1), 1);
        
        % 计算点到线段的距离
        dist_in_segment = vecnorm(vec_start_to_point - proj_length .* unit_dir, 2, 2);
        
        % 处理投影在线段之外的点
        dist(proj_length < 0) = vecnorm(point(proj_length < 0, :) - p1, 2, 2);
        dist(proj_length > line_length) = vecnorm(point(proj_length > line_length, :) - p2, 2, 2);
        
        % 把在线段内的距离复制到结果数组中
        dist(proj_length >= 0 & proj_length <= line_length) = dist_in_segment(proj_length >= 0 & proj_length <= line_length);
    end

    % 定义外部函数，用于根据距离范围返回距离
    distance_func = @(points) min_distance + (max_distance-min_distance) * ...
        min(max(distance_to_line(points)/transition_distance-1,0), 1);
end
