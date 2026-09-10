% FILE: bnbRL_Animation.m
% AUTH: Taeuk Sun
% DESC: Animating BNB
% DATE: 09/05/26 14:40

function bnbRL_Animation(t, x, alpha)
% t     : 시간 벡터
% x     : 공의 위치 (중앙 기준, m)
% alpha : 서보 각도 (도)

    d = 0.016; L = 0.264;      % 기구학 파라미터
    halfBeam = 0.114;          % 빔 반길이

    figure; ax = gca;
    axis equal; grid on;
    xlim([-0.15 0.15]); ylim([-0.06 0.06]);
    xlabel('x [m]'); title('Ball and Beam');

    hBeam = line(ax, [0 0], [0 0], 'LineWidth', 3, 'Color', 'k');
    hBall = rectangle(ax, 'Position', [0 0 0.02 0.02], ...
                      'Curvature', [1 1], 'FaceColor', 'r');
    hTxt  = text(ax, -0.14, 0.05, '');

    for k = 1:2:length(t)
        % 서보 각도 -> 빔 각도
        theta = asin( (d/L) * sin(alpha(k)*pi/180) );

        % 빔 양 끝점
        bx = halfBeam * cos(theta);
        by = halfBeam * sin(theta);
        set(hBeam, 'XData', [-bx bx], 'YData', [-by by]);

        % 공의 위치 (빔 위에 얹힘)
        cx = x(k) * cos(theta);
        cy = x(k) * sin(theta) + 0.008;
        set(hBall, 'Position', [cx-0.008, cy-0.008, 0.016, 0.016]);

        set(hTxt, 'String', sprintf('t = %.2f s', t(k)));
        %drawnow limitrate;

        drawnow;                 % limitrate 제거
        pause(0.02);             % 프레임마다 대기
    end
end