% FILE: bnbRL_eval.m
% AUTH: Taeuk Sun
% DESC: Evaluate trained agent and visualize results
% DATE: 09/05/26 14:40

% --- Evaluate agents ---
agentFile = "exp_RL/exp01_base.mat";
% agentFile = "exp_RL/exp02_obvNorm.mat";
% agentFile = "exp_RL/exp03_obvNrom_aWeight.mat";

% Define environment (creates empty agent)
bnbRL_env;      

% Set ball's start position
mdl = "bnbRL_Simulink";
desiredX = 0.114;
env.ResetFcn = @(in) setVariable(in, "x0", desiredX, Workspace=mdl);

% Overwrite with trained agent
load(agentFile, "agent");       

% --- Simulation ---
simOpts = rlSimulationOptions(MaxSteps = 250);
experience = sim(env, agent, simOpts);

nSteps = length(experience.Reward.Data);
fprintf('Total reward: %.2f | Steps: %d / 250\n', ...
        sum(experience.Reward.Data), nSteps);

if nSteps < 250
    fprintf('  → Early termination (ball left the beam)\n');
end

% --- Extract data ---
t     = experience.Observation.BNBStates.Time;

x    = squeeze(experience.Observation.BNBStates.Data(1,1,:));
xdot = squeeze(experience.Observation.BNBStates.Data(2,1,:));

% < When observations were normalized, re-scale them here >
% x    = squeeze(experience.Observation.BNBStates.Data(1,1,:)) * x_max;
% xdot = squeeze(experience.Observation.BNBStates.Data(2,1,:)) * 0.5;

alpha = squeeze(experience.Action.servoAngle.Data);
disp(x)

% --- Plots ---
figure;
subplot(3,1,1);
plot(t, x, 'LineWidth', 1.2); hold on;
yline( 0.114, 'r--'); yline(-0.114, 'r--');
yline(0, 'k:'); grid on;
ylabel('x [m]'); title('Ball position (red = beam limits)');

subplot(3,1,2);
plot(t, xdot, 'LineWidth', 1.2); grid on;
ylabel('xdot [m/s]'); title('Ball velocity');

subplot(3,1,3);
plot(t(1:length(alpha)), alpha, 'LineWidth', 1.2); grid on;
ylabel('alpha [deg]'); xlabel('Time [s]');
title('Servo angle (agent action)');


% 실험 3개 학습 곡선 비교
load("exp_RL/exp01_base.mat", "trainStats");         s1 = trainStats;
load("exp_RL/exp02_obvNorm.mat", "trainStats");     s2 = trainStats;
load("exp_RL/exp03_obvNorm_aWeight", "trainStats"); s3 = trainStats;

figure; hold on;
plot(movmean(s1.EpisodeReward, 20), 'LineWidth', 1.5);
plot(movmean(s2.EpisodeReward, 20), 'LineWidth', 1.5);
plot(movmean(s3.EpisodeReward, 20), 'LineWidth', 1.5);
legend("E1: 정규화 없음", "E2: 관측 정규화", "E3: 정규화+제어벌점↑");
xlabel("에피소드"); ylabel("이동평균 보상"); grid on;


% --- Animation ---
n = min([length(t), length(x), length(alpha)]);
bnbRL_Animation(t(1:n), x(1:n), alpha(1:n));
