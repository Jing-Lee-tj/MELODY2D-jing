function [Fdim_fit]=fractal_calculate(x)
    [N,edges] = histcounts(x);
    X = (edges(1:end-1)+edges(2:end))/2;
    CumulN = cumsum(N);
    CumulN = CumulN/max(CumulN);
    % modelFunc = @(Fdim, X) 1 - X.^(-Fdim) / max(X.^(-Fdim)); 
    modelFunc = @(Fdim, X) (X >= min(X)) .* (1 - X.^(-Fdim) / max(X.^(-Fdim)));

    options = optimset('Display','off');
    initialFdim = 2;
    Fdim_fit = lsqcurvefit(modelFunc, initialFdim, X, CumulN, [], [], options);
    % 绘制数据点的 CumulN 和拟合的曲线
    figure;
    hold on;
    plot(X, CumulN, 'bo', 'DisplayName', 'Data Points'); % 数据点
    plot(X, modelFunc(Fdim_fit, X), 'r-', 'DisplayName', 'Fitted Curve'); % 拟合曲线
    xlabel('X');
    ylabel('CumulN');
    legend('show');
    title('Cumulative Distribution and Fitted Curve');
    hold off;
end