data = readtable('bnb_real_exp1.txt');
plot(data.t / 1000, data.x); % 시간(s) vs 공 위치(m) 플롯
xlabel('Time [s]'); ylabel('Ball Position [m]');
grid on;