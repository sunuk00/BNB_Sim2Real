% FILE: bnbRL_setup.m
% AUTH: Taeuk Sun
% DATE: 09/05/26 13:35
% DESC: Ball and Beam's Reinforcement Learning Environment
% VERS: 1.0 - 

% 환경 변수 세팅
bnbInit;

% observation 생성 (연속 공간): [위치, 속도]
obsInfo = rlNumericSpec([2 1]);
obsInfo.Name = "BNB States";
obsInfo.Description = "x, xdot";

% Action 생성 (연속 공간) : -30 ~ 30 사이의 아무 실수
actInfo = rlNumericSpec([1 1], ...
    LowerLimit = -30, ...
    UpperLimit =  30);
actInfo.Name = "servo angle";

% Simulink 모델 강화학습으로 감싸기
mdl = "bnbRL_Simulink";
agentBlk = mdl + "/RL Agent";
env = rlSimulinkEnv(mdl, agentBlk, obsInfo, actInfo);

% 매 episode마다 x0(위치)를 랜덤으로 배정: -0.08 ~ +0.08
env.ResetFcn = @(in) setVariable(in, "x0", (rand-0.5)*0.16);


% ---------RL Environment 확인용---------
getObservationInfo(env)
getActionInfo(env)
% ---------------------------------------


% 신경망 난수 시드 고정
rng(0, "twister");

% hidden layer의 뉴런 개수 64
initOpts = rlAgentInitializationOptions(NumHiddenUnit = 64);

% RL 하이퍼파라미터 설정
agentOpts = rlSACAgentOptions( ...
    SampleTime      = Ts, ... % 0.02
    DiscountFactor  = 0.99, ...
    MiniBatchSize   = 128);

% SAC(Soft-Actor-Critic) agent 선언
agent = rlSACAgent(obsInfo, actInfo, initOpts, agentOpts);


% ---------RL Agent Action 확인용---------
getAction(agent, {rand(obsInfo.Dimension)})
% ----------------------------------------


% Training 시작
% EXP: A-1 (종료 벌점 없음) — 자살 문제 관찰용
% rng(0), SAC, 2000 ep, 250 steps/ep, gamma=0.99
trainOpts = rlTrainingOptions( ...
    MaxEpisodes         = 2000, ...
    MaxStepsPerEpisode  = 250, ...
    StopTrainingCriteria = "None", ...
    Verbose             = false, ...
    Plots               = "training-progress");

trainStats = train(agent, env, trainOpts);