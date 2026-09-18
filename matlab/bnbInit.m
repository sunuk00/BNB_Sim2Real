% FILE: bnbInit.m
% AUTH: Taeuk Sun
% DESC: Initializing BNB parameters
% DATE: 09/04/26
% REF: bnbInit.h (by Professor P.Oh)

%% --- Physical parameters ---
% Brick BNB dimensions:
% The SoftPot sensor spans 0.3 m, but the ball's diameter limits travel
% to 0.288 m (36 studs). Thus the beam's midpoint is at 0.144 m (18th stud).
% However, for the BNB's dynamics, L is defined as the lever-to-pivot
% length, which is 0.264 m (confirmed by ruler), or 33 studs.

d = 0.016;       % [m] Motor arm (crank) length, i.e. 2 studs = 1.6 cm (previously r_m)
L = 0.264;       % [m] Lever-to-pivot length on the beam, 33 studs = 26.4 cm
R = 0.015875;    % [m] Ball radius, ~1.58 cm (previously r_b)
m = 0.13039;     % [kg] Ball mass, ~130 g (previously m_b)
g = 9.81;        % [m/s^2] Gravitational acceleration (points in -Y direction)
J = 0.000013144; % [kg*m^2] Ball's moment of inertia

%% --- Brick BNB transfer function ---
K_BB    = (R^2 * m * g * d) / (L * (m * R^2 + J)); % [kg*m/s^2]
numG_BB = [K_BB];
denG_BB = [1 0 0];
G_BB    = tf(numG_BB, denG_BB); % BNB open-loop transfer function

%% --- Initial conditions ---
x0    = 0; % [m]   Ball's initial position
xdot0 = 0; % [m/s] Ball's initial velocity

%% --- Design parameters ---
x_max     = 0.144; % [m]   One-sided travel limit from center
xdot_max  = 0.31;  % [m/s] sqrt(2*K_BB*sin(30deg)*0.228)
alpha_max = 30;    % [deg] Action (servo angle) range
Ts        = 0.02;  % [s]   Agent decision (control) period

fprintf('BNB variables ready for Simulink simulation \n')
