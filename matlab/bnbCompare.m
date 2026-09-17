% FILE: bnbCompare.m
% DESC: Compare real-hardware logs from two policies (exp01 vs exp07)
% NOTE: logs must be recorded under the same calibration and start position
d1  = readmatrix("bnb_real_exp1.txt");
d6  = readmatrix("bnb_real_exp7.txt");

N = min(size(d1,1), size(d6,1));   % 두 데이터 중 더 짧은 길이에 맞춤
x1  = d1(1:N,2);   a1  = d1(1:N,4);
x6  = d6(1:N,2);   a6  = d6(1:N,4);
t   = (0:N-1) * 0.02;

% --- 정량 비교 (정상상태 구간만) ---
s = 200;                      % 4초 이후. 위치 그래프 보고 조정
fprintf('\n           평균|da|    a std     x std     최종 x [mm]\n');
fprintf('exp01     %8.3f  %8.3f  %9.5f  %9.2f\n', ...
    mean(abs(diff(a1(s:N)))),  std(a1(s:N)),  std(x1(s:N)),  x1(N)*1000);
fprintf('exp07     %8.3f  %8.3f  %9.5f  %9.2f\n', ...
    mean(abs(diff(a6(s:N)))),  std(a6(s:N)),  std(x6(s:N)),  x6(N)*1000);

% --- 그래프 ---
figure;
c1 = [0.0000 0.4470 0.7410];  % blue
c2 = [0.9500 0.5500 0.1000];  % orange

subplot(2,1,1);
plot(t, x1,  'Color', c1, 'LineWidth', 1.2); hold on;
plot(t, x6,  'Color', c2, 'LineWidth', 1.2);
yline(0, 'k:', 'HandleVisibility','off');
grid on;
legend("exp01 baseline", "exp07 rate penalty", 'Location','best');
ylabel('x [m]'); title('Ball position');

subplot(2,1,2);
plot(t, a1,  'Color', c1, 'LineWidth', 1.2); hold on;
plot(t, a6,  'Color', c2, 'LineWidth', 1.2);
grid on;
legend("exp01", "exp07", 'Location','best');
ylabel('\alpha [deg]'); xlabel('Time [s]');
title('Servo angle');