function ContoursGrains_voro = load_voro(VoronoiCells, VoronoiVertices)
    % 预分配元胞数组
    ContoursGrains_voro = cell(length(VoronoiCells), 1);
    
    % 遍历每个 Voronoi 单元格
    for i = 1:length(VoronoiCells)
        % 获取当前单元格的顶点索引
        vertex_indices = [VoronoiCells{i} VoronoiCells{i}(1)];
        % 获取当前单元格的顶点坐标
        cell_vertices = VoronoiVertices(vertex_indices, :);
        % 存储当前单元格的顶点坐标到元胞数组
        ContoursGrains_voro{i} = cell_vertices;
    end
end
