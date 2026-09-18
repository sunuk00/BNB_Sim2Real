% FILE: bnbRL_eval.m
% AUTH: Taeuk Sun
% DESC: Evaluate a trained RL agent on the Ball-and-Beam environment
%       and visualize the resulting trajectories.
% DATE: 09/05/26 14:40

clear; clc;

%% --- Configuration ---
% Select the trained agent to evaluate (uncomment one).
% agentFile = "exp_RL/exp01_base.mat";
% agentFile = "exp_RL/exp02_obvNorm.mat";
% agentFile = "exp_RL/exp03_obvNorm_aWeight.mat";
% agentFile = "exp_RL/exp04_500step.mat";
% agentFile = "exp_RL/exp05_500step_terminalPenalty.mat";
% agentFile = "exp_RL/exp06_base_ratePenalty.mat";
% agentFile = "exp_RL/exp07_base_obsNoise.mat";
% agentFile = "exp_RL/exp08_base_obsNoise_filtered.mat";
% agentFile = "exp_RL/exp09_base_filtered.mat";
% agentFile = "exp_RL/exp10_curriculum_sigam010.mat";
% agentFile = "exp_RL/exp11_curriculum_sigma020.mat";
agentFile = "exp_RL/exp12_curriculum_sigma030.mat";

mdl        = "bnbRL_Simulink";
desiredX0  = 0.1;      % Ball's initial position [m]
beamLimit  = 0.114;    % Beam half-length [m]
maxSteps   = 250;      % Simulation horizon [steps]

% Optional post-processing blocks (disabled by default).
enableTrainingCurveComparison = false;
enableSaveForPidComparison    = false;

%% --- Environment and agent setup ---
bnbRL_env;  % Defines the empty RL environment/agent structures

% Randomize the ball's initial position on reset
env.ResetFcn = @(in) setVariable(in, "x0", desiredX0, Workspace = mdl);

% Load the trained agent, overwriting the empty placeholder
load(agentFile, "agent");

%% --- Run simulation ---
simOpts    = rlSimulationOptions(MaxSteps = maxSteps);
experience = sim(env, agent, simOpts);

nSteps = length(experience.Reward.Data);
fprintf('Total reward: %.2f | Steps: %d / %d\n', ...
        sum(experience.Reward.Data), nSteps, maxSteps);

if nSteps < maxSteps
    fprintf('  -> Early termination (ball left the beam)\n');
end

%% --- Extract simulation data ---
t = experience.Observation.BNBStates.Time;

x    = squeeze(experience.Observation.BNBStates.Data(1, 1, :));
xdot = squeeze(experience.Observation.BNBStates.Data(2, 1, :));

% If observations were normalized during training, rescale them here:
% x    = squeeze(experience.Observation.BNBStates.Data(1, 1, :)) * x_max;
% xdot = squeeze(experience.Observation.BNBStates.Data(2, 1, :)) * 0.5;

alpha = squeeze(experience.Action.servoAngle.Data);

%% --- Plot results ---
figure;
tiledlayout(3, 1);

nexttile;
plot(t, x, 'LineWidth', 1.2); hold on;
yline(beamLimit, 'r--'); yline(-beamLimit, 'r--');
yline(0, 'k:'); grid on;
ylabel('x [m]'); title('Ball position (red = beam limits)');

nexttile;
plot(t, xdot, 'LineWidth', 1.2); grid on;
ylabel('xdot [m/s]'); title('Ball velocity');

nexttile;
plot(t(1:length(alpha)), alpha, 'LineWidth', 1.2); grid on;
ylabel('alpha [deg]'); xlabel('Time [s]');
title('Servo angle (agent action)');

%% --- Optional: compare training curves across experiments ---
if enableTrainingCurveComparison
    load("exp_RL/exp01_base.mat", "trainStats");        s1 = trainStats;
    load("exp_RL/exp02_obvNorm.mat", "trainStats");      s2 = trainStats;
    load("exp_RL/exp03_obvNorm_aWeight.mat", "trainStats"); s3 = trainStats;

    figure; hold on;
    plot(movmean(s1.EpisodeReward, 20), 'LineWidth', 1.5);
    plot(movmean(s2.EpisodeReward, 20), 'LineWidth', 1.5);
    plot(movmean(s3.EpisodeReward, 20), 'LineWidth', 1.5);
    legend("E1: no normalization", "E2: observation normalization", ...
           "E3: normalization + higher control penalty");
    xlabel("Episode"); ylabel("Moving-average reward"); grid on;
end

%% --- Animation ---
n = min([length(t), length(x), length(alpha)]);
bnbRL_Animation(t(1:n), x(1:n), alpha(1:n));
