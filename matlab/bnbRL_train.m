% FILE: bnbRL_train.m
% AUTH: Taeuk Sun
% DATE: 09/05/26 13:40
% DESC: Training BNB with RL

% 1. Initialize environment (This creates a fresh 'env' and a new 'agent')
bnbRL_env; 

% =========================================================================
% [TRAINING MODE SELECTION]
% How to use:
% - If you want to train from scratch: Comment out the "MODE B" section.
% - If you want to continue training: Uncomment the 2 lines in "MODE B".
% =========================================================================

% --- MODE A: Train a NEW agent from scratch ---
% You do not need any extra code for this mode. 
% It just uses the new agent from 'bnbRL_env' above.


% --- MODE B: Continue training from a saved agent (Curriculum Learning) ---
% Uncomment the 2 lines below to overwrite the new agent with your saved agent.
% Setting 'ResetExperienceBufferBeforeTraining' to false keeps past memories.

% load("exp_RL/exp11_curriculum_sigma020.mat", "agent"); 
% agent.AgentOptions.ResetExperienceBufferBeforeTraining = false; 

% =========================================================================

% 2. Training options
trainOpts = rlTrainingOptions( ...
    MaxEpisodes = 500, ...         % e.g., 1000 for new, 500 for continue
    MaxStepsPerEpisode = 250, ...
    StopTrainingCriteria = "None", ...
    Verbose = false, ...
    Plots = "training-progress");

% 3. Train the agent
trainStats = train(agent, env, trainOpts);

% 4. Save the results
% Make sure to change the file name before running!
expName = "exp12_curriculum_sigma030"; 
save("exp_RL/" + expName + ".mat", "agent", "trainStats");

%{
    <Experiment list>
    1) exp01_base
    2) exp02_obvNorm
    3) exp03_obvNorm_aWeight
    4) exp04_500step
    5) exp05_500step_terminalPenalty
    6) exp06_base_ratePenalty
    7) exp07_base_obsNoise
    8) exp08_base_obsNoise_filtered
    9) exp09_base_filtered
   10) exp10_curriculum_sigma010        : Continue from exp01_base with small noise // [x + 0.00022, xdot + 0.010]
   11) exp11_curriculum_sigma020        : Continue from exp10 with more noise       // [x + 0.00044, xdot + 0.020]
   12) exp12_curriculum_sigma030        : Continue from exp11 with even more noise  // [x + 0.00066, xdot + 0.030]
%}