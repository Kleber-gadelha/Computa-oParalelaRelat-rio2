CC = gcc
CFLAGS = -Wall -O3
PTHREAD = -pthread

all: varredura_linha varredura_coluna matmul_padrao matmul_bloco matmul_pthreads matmul_pthreads_bloco

varredura_linha: varredura_linha.c
	$(CC) $(CFLAGS) varredura_linha.c -o varredura_linha

varredura_coluna: varredura_coluna.c
	$(CC) $(CFLAGS) varredura_coluna.c -o varredura_coluna

matmul_padrao: matmul_padrao.c
	$(CC) $(CFLAGS) matmul_padrao.c -o matmul_padrao

matmul_bloco: matmul_bloco.c
	$(CC) $(CFLAGS) matmul_bloco.c -o matmul_bloco

matmul_pthreads: matmul_pthreads.c
	$(CC) $(CFLAGS) matmul_pthreads.c -o matmul_pthreads $(PTHREAD)

matmul_pthreads_bloco: matmul_pthreads_bloco.c
	$(CC) $(CFLAGS) matmul_pthreads_bloco.c -o matmul_pthreads_bloco $(PTHREAD)

clean:
	rm -f varredura_linha varredura_coluna matmul_padrao matmul_bloco matmul_pthreads matmul_pthreads_bloco