% FILE: bnbCompare.m
bnbInit;
load("cmp_PID.mat");
load("cmp_RL.mat");

xP_e = 0.1 - xP;    % 목표 기준 오차로 변환 (RL과 부호 맞춤)
aP_e = -aP;

figure;
subplot(2,1,1);
plot(tP, xP_e, 'LineWidth', 1.5); hold on;
plot(tR, xR,   'LineWidth', 1.5);
yline(0, 'k:', 'HandleVisibility','off');
grid on; legend("PID (600, 0.01, 100)", "RL (SAC)");
ylabel('x [m]'); title('Ball position (x_0 = 0.01 m)');

subplot(2,1,2);
plot(tP, aP_e, 'LineWidth', 1.5); hold on;
% plot(tR, aR,   'LineWidth', 1.5);
n = min(length(tR), length(aR));
plot(tR(1:n), aR(1:n), 'LineWidth', 1.5);

yline( 30, 'r--', 'HandleVisibility','off');
yline(-30, 'r--', 'HandleVisibility','off');
grid on; legend("PID", "RL (SAC)");
ylabel('\alpha [deg]'); xlabel('Time [s]');
title('Servo angle');

% --- 정량 비교 ---
fprintf('\n            정상오차   최대각도   제어노력\n');
fprintf('PID         %6.1f mm  %6.1f deg  %8.1f\n', ...
        abs(xP_e(end))*1000, max(abs(aP_e)), trapz(tP, aP_e.^2));
fprintf('RL          %6.1f mm  %6.1f deg  %8.1f\n', ...
        abs(xR(end))*1000, max(abs(aR)), trapz(tR(1:n), aR(1:n).^2));
