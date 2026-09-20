% FILE: bnbExportWeights.m
% AUTH: Taeuk Sun
% DESC: Exporting SAC actor weights to C header file
% DATE: 09/010/26

load("exp_RL/exp12_curriculum_sigma030.mat", "agent");
actor  = getActor(agent);
params = getLearnableParameters(actor);

W1 = extractdata(params{1});   % [64 2]
b1 = extractdata(params{2});   % [64 1]
W2 = extractdata(params{3});   % [64 64]
b2 = extractdata(params{4});   % [64 1]
W3 = extractdata(params{5});   % [1 64]
b3 = extractdata(params{6});   % [1 1]

fid = fopen("bnbPolicy.h", "w");
fprintf(fid, "// FILE: bnbPolicy.h\n");
fprintf(fid, "// DESC: SAC actor weights exported from MATLAB (E1)\n");
fprintf(fid, "// NOTE: action = ALPHA_MAX * tanh(mu)\n\n");
fprintf(fid, "#define N_IN   2\n"); 
fprintf(fid, "#define N_H    64\n");
fprintf(fid, "#define ALPHA_MAX 30.0f\n\n");

writeMat(fid, "W1", W1);
writeVec(fid, "b1", b1);
writeMat(fid, "W2", W2);
writeVec(fid, "b2", b2);
writeMat(fid, "W3", W3);
writeVec(fid, "b3", b3);

fclose(fid);
fprintf("bnbPolicy.h written\n");


function writeMat(fid, name, M)
    M = double(M);                      % single -> double
    [r, c] = size(M);
    fprintf(fid, "const float %s[%d][%d] = {\n", name, r, c);
    for i = 1:r
        fprintf(fid, "  {");
        for j = 1:c
            fprintf(fid, "%.8ef", M(i,j));
            if j < c, fprintf(fid, ", "); end
        end
        fprintf(fid, "}");
        if i < r, fprintf(fid, ","); end
        fprintf(fid, "\n");
    end
    fprintf(fid, "};\n\n");
end

function writeVec(fid, name, v)
    v = double(v);
    n = numel(v);
    fprintf(fid, "const float %s[%d] = {\n  ", name, n);
    for k = 1:n
        fprintf(fid, "%.8ef", v(k));
        if k < n, fprintf(fid, ", "); end
    end
    fprintf(fid, "\n};\n\n");
end