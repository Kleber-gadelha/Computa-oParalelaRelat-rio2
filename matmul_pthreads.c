#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

double *A;
double *B;
double *C;

int N;
int num_threads;

typedef struct {
    int inicio;
    int fim;
} ThreadArgs;
void *multiplicar(void *arg) {

    ThreadArgs *dados = (ThreadArgs *)arg;

    for (int i = dados->inicio; i < dados->fim; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i * N + j] +=
                    A[i * N + k] * B[k * N + j];
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

    N = atoi(argv[1]);
    num_threads = atoi(argv[2]);

    if (N <= 0 || num_threads <= 0) {
        printf("N e numero de threads devem ser positivos.\n");
        return 1;
    }

    A = malloc((size_t)N * N * sizeof(double));
    B = malloc((size_t)N * N * sizeof(double));
    C = calloc((size_t)N * N, sizeof(double));
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

    if (threads == NULL || args == NULL) {
        printf("Erro ao alocar estruturas das threads.\n");

        free(A);
        free(B);
        free(C);
        free(threads);
        free(args);

        return 1;
    }
    struct timespec inicio, fim;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int linhas_base = N / num_threads;
    int resto = N % num_threads;
    int linha_atual = 0;

    for (int t = 0; t < num_threads; t++) {

        int quantidade = linhas_base;

        if (t < resto) {
            quantidade++;
        }

        args[t].inicio = linha_atual;
        args[t].fim = linha_atual + quantidade;

        linha_atual = args[t].fim;

        pthread_create(
            &threads[t],
            NULL,
            multiplicar,
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
    printf("Tempo: %.9f segundos\n", tempo);
    printf("C[0][0]: %.2f\n", C[0]);

    free(A);
    free(B);
    free(C);
    free(threads);
    free(args);

    return 0;
}