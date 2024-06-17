function extractDataFromImage()
    % 提示用户选择图像文件
    [filename, pathname] = uigetfile({'*.jpg;*.jpeg;*.png;*.bmp', 'Image Files (*.jpg, *.jpeg, *.png, *.bmp)'}, 'Select an Image File');
    if isequal(filename, 0)
        disp('User canceled file selection');
        return;
    end

    % 读取图像
    imgPath = fullfile(pathname, filename);
    img = imread(imgPath);
    
    % 显示图像
    hFig = figure('Name', 'Data Extraction', 'NumberTitle', 'off');
    hAx = axes('Parent', hFig);
    imshow(img, 'Parent', hAx);
    hold(hAx, 'on');
    title(hAx, 'Click to select the origin (0,0)');
    
    % 初始化变量
    xOrigin = [];
    yOrigin = [];
    xScale = [];
    yScale = [];
    xSelected = [];
    ySelected = [];
    dataPoints = table();
    
    % 指定原点
    disp('Select the origin (0,0)');
    [xOrigin, yOrigin] = ginput(1);
    plot(hAx, xOrigin, yOrigin, 'go', 'MarkerSize', 10, 'LineWidth', 2);
    
    % 选择 x 轴刻度
    title(hAx, 'Click to select a point on the x-axis with a known value');
    disp('Select a point on the x-axis with a known value');
    [xXaxis, yXaxis] = ginput(1);
    plot(hAx, xXaxis, yXaxis, 'ro', 'MarkerSize', 10, 'LineWidth', 2);
    xKnown = input('Enter the known x-axis value: ');
    
    % 选择 y 轴刻度
    title(hAx, 'Click to select a point on the y-axis with a known value');
    disp('Select a point on the y-axis with a known value');
    [xYaxis, yYaxis] = ginput(1);
    plot(hAx, xYaxis, yYaxis, 'bo', 'MarkerSize', 10, 'LineWidth', 2);
    yKnown = input('Enter the known y-axis value: ');

    % 计算比例尺
    xScale = xKnown / (xXaxis - xOrigin);
    yScale = yKnown / (yOrigin - yYaxis);
    
    % 设置按钮用于清除数据
    uicontrol('Style', 'pushbutton', 'String', 'Clear Data', 'Position', [20 20 100 30], 'Callback', @clearData);

    while true
        % 提示用户点击图像以选择点
        title(hAx, 'Click on the image to select data points. Press Enter when done.');
        disp('Click on the image to select data points. Press Enter when done.');

        % 选择数据点
        while true
            [x, y, button] = ginput(1);
            if isempty(button) % 按下回车键结束选择
                break;
            end
            if button == 3  % 右键点击清除数据
                clearData();
            else
                xSelected(end+1) = x;
                ySelected(end+1) = y;
                plot(hAx, x, y, 'r*', 'MarkerSize', 10);
            end
        end
        
        % 转换数据点坐标到实际坐标
        xActual = (xSelected - xOrigin) * xScale;
        yActual = (yOrigin - ySelected) * yScale;
    
        % 显示选中的点坐标
        disp('Selected points (in image coordinates):');
        disp(table(xSelected', ySelected'));
        disp('Selected points (in actual coordinates):');
        newData = table(xActual', yActual', 'VariableNames', {'X', 'Y'});
        disp(newData);

        % 合并数据
        dataPoints = [dataPoints; newData];

        % 清除数据点并重新开始
        choice = questdlg('Do you want to select more points?', ...
                          'Continue', ...
                          'Yes', 'No', 'No');
        if strcmp(choice, 'No')
            break;
        else
            clearData();
        end
    end
    
    % 如果需要将选中的点保存到文件
    saveToFile = input('Do you want to save the selected points to a file? (y/n): ', 's');
    if strcmpi(saveToFile, 'y')
        filename = input('Enter the filename (e.g., dataPoints.txt): ', 's');
        writetable(dataPoints, filename, 'Delimiter', '\t');
        disp(['Data points saved to ', filename]);
    end
    
    function clearData(~, ~)
        % 清除选中的点和图形上的标记
        xSelected = [];
        ySelected = [];
        cla(hAx);
        imshow(img, 'Parent', hAx);
        hold(hAx, 'on');
        plot(hAx, xOrigin, yOrigin, 'go', 'MarkerSize', 10, 'LineWidth', 2);
        plot(hAx, xXaxis, yXaxis, 'ro', 'MarkerSize', 10, 'LineWidth', 2);
        plot(hAx, xYaxis, yYaxis, 'bo', 'MarkerSize', 10, 'LineWidth', 2);
        title(hAx, 'Data points cleared. Click on the image to select new data points. Press Enter when done.');
        disp('Data points cleared. Click on the image to select new data points. Press Enter when done.');
    end
end
