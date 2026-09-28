#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BLOCO 32

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Uso: %s <N>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);

    double *A = malloc((size_t)N * N * sizeof(double));
    double *B = malloc((size_t)N * N * sizeof(double));
    double *C = calloc((size_t)N * N, sizeof(double));

    if (A == NULL || B == NULL || C == NULL) {
        printf("Erro ao alocar memoria.\n");
        free(A);
        free(B);
        free(C);
        return 1;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = (double)(i + j);
            B[i * N + j] = (double)(i * j);
        }
    }

    struct timespec inicio, fim;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int ii = 0; ii < N; ii += BLOCO) {
        for (int jj = 0; jj < N; jj += BLOCO) {
            for (int kk = 0; kk < N; kk += BLOCO) {

                int max_i = (ii + BLOCO < N) ? ii + BLOCO : N;
                int max_j = (jj + BLOCO < N) ? jj + BLOCO : N;
                int max_k = (kk + BLOCO < N) ? kk + BLOCO : N;

                for (int i = ii; i < max_i; i++) {
                    for (int j = jj; j < max_j; j++) {
                        for (int k = kk; k < max_k; k++) {
                            C[i * N + j] +=
                                A[i * N + k] * B[k * N + j];
                        }
                    }
                }
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo =
        (fim.tv_sec - inicio.tv_sec) +
        (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("N: %d\n", N);
    printf("Tempo: %.9f segundos\n", tempo);
    printf("C[0][0]: %.2f\n", C[0]);

    free(A);
    free(B);
    free(C);

    return 0;
}