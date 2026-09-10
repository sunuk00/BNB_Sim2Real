% FILE: bnbRL_train.m
% AUTH: Taeuk Sun
% DATE: 09/05/26 13:40
% DESC: Training BNB with RL

bnbRL_env;  % Initialize environment and agent (calls bnbRL_env.m)

% Training options: episodes, steps per episode, and plot settings
trainOpts = rlTrainingOptions( ...
    MaxEpisodes = 500, ...
    MaxStepsPerEpisode = 250, ...
    StopTrainingCriteria = "None", ...
    Verbose = false, ...
    Plots = "training-progress");

% Train the agent with the specified environment and options
trainStats = train(agent, env, trainOpts);

% Save trained agent and training statistics
% save("exp_RL/exp01_base.mat", "agent", "trainStats");
% save("exp_RL/exp02_obvRegul.mat", "agent", "trainStats");
save("exp_RL/exp03_obvRegul_aWeight.mat", "agent", "trainStats"); % reward function alpha weight 변경
%% save("exp_RL/exp04_obvRegul_penalty.mat", "agent", "trainStats");
