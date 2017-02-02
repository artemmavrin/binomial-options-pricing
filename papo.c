/*
 * File: papo.c (Price American Put Option)
 * Author: Artem Mavrin
 *
 * Find the arbitrage-free initial price of an American put option in the CRR
 * binomial options pricing model.
 */

#include <stdlib.h>
#include <stdio.h>

#define PROGRAM_NAME "papo"
#define PROGRAM_DESCRIPTION "Price American put options"

#define USAGE                                   \
    PROGRAM_DESCRIPTION "\n"                    \
    "Usage:\t" PROGRAM_NAME " T S0 u d K r\n"   \
    "\tT\tExpiration time\n"                    \
    "\tS0\tInitial stock price\n"               \
    "\tu\tUp factor\n"                          \
    "\td\tDown factor\n"                        \
    "\tK\tStrike price\n"                       \
    "\tr\tRisk-free interest rate\n"

#define TREESIZE(T) ((1 << ((T) + 1)) - 1)
#define MAX(x, y)   ((x) > (y) ? (x) : (y))

double papo(
    long T,         /* Option expiration time */
    double S0,      /* Initial stock price */
    double u,       /* Up factor */
    double d,       /* Down factor */
    double K,       /* Option strike price */
    double r)       /* Risk-free interest rate */
{
    double *S;      /* Stock price process */
    double *Y;      /* Option payoff process */
    double *U;      /* Necessary wealth process */
    double U0;      /* Arbitrage-free price */
    double p_star;  /* Risk-neutral probability */
    long t;         /* A time between 0 and T */
    long i;         /* Loop index for arrays */

    /* Allocate tree memory */
    S = malloc(TREESIZE(T) * sizeof(double));
    Y = malloc(TREESIZE(T) * sizeof(double));
    U = malloc(TREESIZE(T) * sizeof(double));

    /* Check if any of the allocations failed */
    if (!S || !Y || !U) {
        U0 = -1;
        goto end;
    }

    /* Build binary trees for the stock price process and the payoff process */
    S[0] = S0;
    Y[0] = MAX(K - S[0], 0);
    for (i = 1; i < TREESIZE(T); i++) {
        if (i % 2 == 1)
            S[i] = d * S[(i - 1) / 2];
        else
            S[i] = u * S[(i - 1) / 2];
        Y[i] = MAX(K - S[i], 0);
    }

    /* Compute p* (risk-neutral probability) */
    p_star = (1 + r - d) / (u - d);

    /* Generate the bottom row of necessary wealth process tree */
    for (i = (1 << T) - 1; i < (1 << (T + 1)) - 1; i++)
        U[i] = Y[i];

    /* Generate the intermediate rows of the necessary wealth process tree */
    for (t = T - 1; t >= 0; t--) {
        for (i = (1 << t) - 1; i < (1 << (t + 1)) - 1; i++) {
            U[i] = (1 - p_star) * U[2 * i + 1] + p_star * U[2 * i + 2];
            U[i] /= 1 + r;
            U[i] = MAX(Y[i], U[i]);
        }
    }

    /* Clean up and return the arbitrage-free initial price */
    U0 = U[0];
end:
    if (S)
        free(S);
    if (Y)
        free(Y);
    if (U)
        free(U);
    return U0;
}

int main(int argc, const char **argv) {
    long T;         /* Option expiration time */
    double S0;      /* Initial stock price */
    double u;       /* Up factor */
    double d;       /* Down factor */
    double K;       /* Option strike price */
    double r;       /* Risk-free interest rate */

    /* Make sure all the arguments are present */
    if (--argc != 6) {
        (void) fprintf(stderr, USAGE);
        return EXIT_FAILURE;
    }

    /* Parse the command-line arguments */
    T = atol(argv[1]);
    S0 = atof(argv[2]);
    u = atof(argv[3]);
    d = atof(argv[4]);
    K = atof(argv[5]);
    r = atof(argv[6]);

    /* Print the arbitrage-free initial price */
    (void) printf("%f\n", papo(T, S0, u, d, K, r));

    return EXIT_SUCCESS;
}
