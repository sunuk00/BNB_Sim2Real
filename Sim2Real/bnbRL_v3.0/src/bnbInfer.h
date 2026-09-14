// FILE: bnbInfer.h
// DESC: Forward pass of the exported SAC actor
// NOTE: input  = [x, xdot] in the same units used during training
//       output = servo angle [deg], already squashed to +/- ALPHA_MAX

#include <math.h>
#include "bnbPolicy.h"

float policy_forward(float x, float xdot) {
    float h1[N_H];
    float h2[N_H];
    float in[N_IN] = {x, xdot};

    // Layer 1: fc_1 + ReLU
    for (int i = 0; i < N_H; i++) {
        float s = b1[i];
        for (int j = 0; j < N_IN; j++) s += W1[i][j] * in[j];
        h1[i] = (s > 0.0f) ? s : 0.0f;
    }

    // Layer 2: fc_body + ReLU
    for (int i = 0; i < N_H; i++) {
        float s = b2[i];
        for (int j = 0; j < N_H; j++) s += W2[i][j] * h1[j];
        h2[i] = (s > 0.0f) ? s : 0.0f;
    }

    // Output: fc_mean
    float mu = b3[0];
    for (int j = 0; j < N_H; j++) mu += W3[0][j] * h2[j];

    // Squash to action range
    return ALPHA_MAX * tanhf(mu);
}