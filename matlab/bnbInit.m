% Brick BNB dimensions
% SoftPot sensor is 0.3 m. But ball's diameter limits travel
% to 0.228 m (36 studs). Thus beam's midpoint is at 0.144 m (18th stud)
% However for the BNB's dynamics, L is defined as the lever-to-pivot
% length which is 0.264 m (confirmed by ruler) or 33 studs

% [m] crank length i.e. 2 studs (before was r_m)
d = 0.016;       % [m] 모터 암(크랭크) 길이: 레고 2칸 = 1.6 cm

% [m] lever-to-pivot length on beam 33 studs
L = 0.264;       % [m] 빔 회전축에서 레버까지의 거리 = 26.4 cm

% [m] ball's radius (before was r_b)
R = 0.015875;    % [m] 쇠구슬의 반지름 = 약 1.58 cm

% [kg] ball's mass (before was m_b)
m = 0.13039;     % [kg] 쇠구슬의 무게 = 약 130 g

% [m/s^2] gravity NB: points in -Y direction
g = 9.81;        % [m/s^2] 중력가속도

% % Ball's Moment of Inertia calculator
J = 0.000013144; % [kg*m^2] 구슬의 회전 관성 모멘트 (Moment of Inertia)

% Brick BNB Transfer Function
K_BB = (R^2*m*g*d) / (L*(m*R^2 + J)); % [kgm/s^2]
numG_BB = [K_BB];
denG_BB = [1 0 0];
G_BB = tf(numG_BB, denG_BB); % BNB OLTF


% --- 초기 조건 ---
x0    = 0;          % [m] 공의 초기 위치
xdot0 = 0;          % [m/s] 공의 초기 속도

% --- 설계 파라미터 ---
x_max = 0.144;      % [m] 중앙 기준 편측 한계
xdot_max = 0.31;    % [m/s] sqrt(2*K_BB*sin(30deg)*0.228)
alpha_max = 30;     % [deg] 행동 범위
Ts = 0.02;          % [s] 에이전트가 행동을 결정하는 주기

fprintf('BNB variables ready for Simulink simulation \n')