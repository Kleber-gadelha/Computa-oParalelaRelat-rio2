#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Uso: %s <N>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);

    double *A = malloc(N * N * sizeof(double));

    if (A == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = (double)(i + j);
        }
    }

    struct timespec inicio, fim;
    //comecar a contar
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    long pares = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {

            if ((int)A[i * N + j] % 2 == 0) {
                pares++;
            }
        }
    }

    //terminar de contar
    clock_gettime(CLOCK_MONOTONIC, &fim);

   //diferenca de tempo
    double tempo = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;


    printf("Tempo: %.9f segundos\n", tempo);
    printf("N: %d\n", N);
    printf("Quantidade de pares: %ld\n", pares);

    free(A);

    return 0;
}