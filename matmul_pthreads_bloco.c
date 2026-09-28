#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define BLOCO 32

typedef struct {
    int inicio;
    int fim;
    int N;
    double *A;
    double *B;
    double *C;
} ThreadArgs;
void *multiplicar_bloco(void *arg) {
    ThreadArgs *dados = (ThreadArgs *)arg;

    int inicio = dados->inicio;
    int fim = dados->fim;
    int N = dados->N;

    double *A = dados->A;
    double *B = dados->B;
    double *C = dados->C;

    for (int ii = inicio; ii < fim; ii += BLOCO) {
        for (int jj = 0; jj < N; jj += BLOCO) {
            for (int kk = 0; kk < N; kk += BLOCO) {

                int limite_i = ii + BLOCO;
                if (limite_i > fim)
                    limite_i = fim;

                int limite_j = jj + BLOCO;
                if (limite_j > N)
                    limite_j = N;

                int limite_k = kk + BLOCO;
                if (limite_k > N)
                    limite_k = N;

                for (int i = ii; i < limite_i; i++) {
                    for (int j = jj; j < limite_j; j++) {
                        for (int k = kk; k < limite_k; k++) {
                            C[i * N + j] +=
                                A[i * N + k] * B[k * N + j];
                        }
                    }
                }
            }
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Uso: %s <N> <threads>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

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

    pthread_t *threads =
        malloc(num_threads * sizeof(pthread_t));

    ThreadArgs *args =
        malloc(num_threads * sizeof(ThreadArgs));

    int linhas_base = N / num_threads;
    int resto = N % num_threads;

    int linha_atual = 0;

    struct timespec inicio, fim;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int t = 0; t < num_threads; t++) {
        int quantidade = linhas_base;
        if (t < resto) { quantidade++; }
        args[t].inicio = linha_atual;
        args[t].fim = linha_atual + quantidade;
        args[t].N = N;
        args[t].A = A;
        args[t].B = B;
        args[t].C = C;
        linha_atual = args[t].fim;
        pthread_create(
            &threads[t],
            NULL,
            multiplicar_bloco,
            &args[t]
        );
    }
    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo =
        (fim.tv_sec - inicio.tv_sec) +
        (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    printf("N: %d\n", N);
    printf("Threads: %d\n", num_threads);
    printf("Bloco: %d\n", BLOCO);
    printf("Tempo: %.9f segundos\n", tempo);
    printf("C[0][0]: %.2f\n", C[0]);
    free(threads);
    free(args);
    free(A);
    free(B);
    free(C);

    return 0;
}