#ifndef BOP_UTIL_H
#define BOP_UTIL_H

#define USAGE \
PROGRAM_DESCRIPTION "\n\
Usage:\t" PROGRAM_NAME " T S0 u d K r\n\
\tT\tExpiration time\n\
\tS0\tInitial stock price\n\
\tu\tUp factor\n\
\td\tDown factor\n\
\tK\tStrike price\n\
\tr\tRisk-free interest rate\n"

#define TREESIZE(T) ((1 << ((T) + 1)) - 1)

#define MAX(x, y)   ((x) > (y) ? (x) : (y))

#define MAIN(func) \
int main(int argc, const char **argv) { \
    long T;         /* Option expiration time */ \
    double S0;      /* Initial stock price */ \
    double u;       /* Up factor */ \
    double d;       /* Down factor */ \
    double K;       /* Option strike price */ \
    double r;       /* Risk-free interest rate */ \
\
    /* Make sure all the arguments are present */ \
    if (--argc != 6) { \
        (void) fprintf(stderr, USAGE); \
        return EXIT_FAILURE; \
    } \
\
    /* Parse the command-line arguments */ \
    T = atol(argv[1]); \
    S0 = atof(argv[2]); \
    u = atof(argv[3]); \
    d = atof(argv[4]); \
    K = atof(argv[5]); \
    r = atof(argv[6]); \
\
    /* Print the arbitrage-free initial price */ \
    (void) printf("%f\n", func(T, S0, u, d, K, r)); \
\
    return EXIT_SUCCESS; \
}

#endif
