% FILE: bnbRL_Animation.m
% AUTH: Taeuk Sun
% DESC: Animating BNB
% DATE: 09/07/26
% NOTE: 규약 — alpha > 0 이면 빔의 +x 끝이 내려가고 공이 +x로 가속

function bnbRL_Animation(t, x, alpha, visGain)

    if nargin < 4, visGain = 8; end   % 각도 과장 배율 (시각화 전용)

    d = 0.016; L = 0.264;      % 기구학 파라미터
    halfBeam = 0.114;          % 빔 반길이
    ballR    = 0.008;          % 화면상 공 반지름

    figure; ax = gca;
    axis equal; grid on;
    xlim([-0.15 0.15]); ylim([-0.06 0.06]);
    xlabel('x [m]'); title('Ball and Beam');

    hBeam = line(ax, [0 0], [0 0], 'LineWidth', 3, 'Color', 'k');
    hBall = rectangle(ax, 'Position', [0 0 2*ballR 2*ballR], ...
                      'Curvature', [1 1], 'FaceColor', 'r');
    hTxt  = text(ax, -0.14, 0.05, '');

    for k = 1:2:length(t)
        % 서보 각도 -> 빔 각도 (화면용으로 과장)
        theta = asin( (d/L) * sin(alpha(k)*pi/180) ) * visGain;
        c = cos(theta);  s = sin(theta);

        % 빔: alpha > 0 이면 +x 끝이 내려가야 하므로 y 부호를 뒤집는다
        set(hBeam, 'XData', [-halfBeam*c, halfBeam*c], ...
                   'YData', [ halfBeam*s, -halfBeam*s]);

        % 공: 빔 위에 얹힘 (빔과 같은 부호 규약)
        cx =  x(k)*c;
        cy = -x(k)*s + ballR;
        set(hBall, 'Position', [cx-ballR, cy-ballR, 2*ballR, 2*ballR]);

        set(hTxt, 'String', sprintf('t = %.2f s   \\alpha = %+.1f°', ...
                                    t(k), alpha(k)));
        drawnow;
        pause(0.02);
    end
end