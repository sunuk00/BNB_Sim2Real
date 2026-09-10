% FILE: bnbRL_env.m
% AUTH: Taeuk Sun
% DATE: 09/05/26 13:00
% DESC: RL Environment

bnbInit;  % Initialize BNB environment (custom init script)

% Observation specification: 2 states (position x and velocity xdot)
obsInfo = rlNumericSpec([2 1]);
obsInfo.Name = "BNB States";
obsInfo.Description = "x, xdot";

% Action specification: single action (servo angle) bounded [-30, 30]
actInfo = rlNumericSpec([1 1], LowerLimit = -30, UpperLimit = 30);
actInfo.Name = "servo angle";

% Create Simulink RL environment using model and agent block path
mdl = "bnbRL_Simulink";
agentBlk = mdl + "/RL Agent";
env = rlSimulinkEnv(mdl, agentBlk, obsInfo, actInfo);

% Reset function: set initial state x0 randomly in ±0.08
env.ResetFcn = @(in) setVariable(in, "x0", (rand-0.5)*0.16);

% RNG for reproducibility and agent options
rng(0, "twister");
initOpts  = rlAgentInitializationOptions(NumHiddenUnit = 64);  % network size
agentOpts = rlSACAgentOptions(SampleTime = Ts, ...             % agent sampling time
    DiscountFactor = 0.99, ...
    MiniBatchSize = 128);

% Create SAC agent with specified observation/action specs and options
agent = rlSACAgent(obsInfo, actInfo, initOpts, agentOpts);
