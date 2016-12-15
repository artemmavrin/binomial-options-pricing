/*
 * File: peco.c (Price European Call Option)
 * Author: Artem Mavrin
 *
 * Find the arbitrage-free initial price of a European call option in the CRR
 * binomial options pricing model.
 */

#include <stdlib.h>
#include <stdio.h>

#define PROGRAM_NAME "peco"
#define PROGRAM_DESCRIPTION "Price European call options"

#include "bop_util.h"

double peco(
    long T,         /* Option expiration time */
    double S0,      /* Initial stock price */
    double u,       /* Up factor */
    double d,       /* Down factor */
    double K,       /* Option strike price */
    double r)       /* Risk-free interest rate */
{
    double *S;      /* Stock price process */
    double *V;      /* Value process */
    double V0;      /* Arbitrage-free price */
    double p_star;  /* Risk-neutral probability */
    long t;         /* A time between 0 and T */
    long i;         /* Loop index for arrays */

    /* Allocate tree memory */
    S = malloc(TREESIZE(T) * sizeof(double));
    V = malloc(TREESIZE(T) * sizeof(double));

    /* Check if any of the allocations failed */
    if (!S || !V) {
        V0 = -1;
        goto end;
    }

    /* Build the stock price process binary tree */
    S[0] = S0;
    for (i = 1; i < TREESIZE(T); i++) {
        if (i % 2 == 1)
            S[i] = d * S[(i - 1) / 2];
        else
            S[i] = u * S[(i - 1) / 2];
    }

    /* Compute p* (risk-neutral probability) */
    p_star = (1 + r - d) / (u - d);

    /* Generate the bottom row of the value process tree */
    for (i = (1 << T) - 1; i < (1 << (T + 1)) - 1; i++) {
        V[i] = MAX(S[i] - K, 0);
    }

    /* Generate the intermediate rows of the value process tree */
    for (t = T - 1; t >= 0; t--) {
        for (i = (1 << t) - 1; i < (1 << (t + 1)) - 1; i++) {
            V[i] = (1 - p_star) * V[2 * i + 1] + p_star * V[2 * i + 2];
            V[i] /= 1 + r;
        }
    }

    /* Clean up and return the arbitrage-free initial price */
    V0 = V[0];
end:
    if (S)
        free(S);
    if (V)
        free(V);
    return V0;
}

MAIN(peco)
