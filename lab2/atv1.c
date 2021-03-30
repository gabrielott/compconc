/*
 * Computação Concorrente
 * Laboratório 2, atividade 1
 * Aluno: Gabriel da Fonseca Ottoboni Pinho
 * DRE: 119043838
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define GET(m, i, j) m[i * dim + j]
#define SET(m, i, j, num) m[i * dim + j] = num

int dim;
int *line_ids;
double *in1;
double *in2;
double *out;

void
die(char *msg)
{
	printf("%s", msg);
	exit(EXIT_FAILURE);
}

void
print_matrix(double *matrix)
{
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			printf("%f ", GET(matrix, i, j));
		}
		printf("\n");
	}
}

void *
threaded_multiply(void *args)
{
	int *id = (int *) args;
	for (int i = 0; i < dim; i++) {
		if (line_ids[i] != *id)
			continue;

		for (int j = 0; j < dim; j++) {
			for (int k = 0; k < dim; k++) {
				int value = GET(out, i, j) + GET(in1, i, k) * GET(in2, k, j);
				SET(out, i, j, value);
			}
		}
	}

	return NULL;
}

int
main(int argc, char **argv)
{
	if (argc != 3)
		die("Uso: ./executável DIMENSÃO THREADS\n");

	int thread_num;
	pthread_t *thread_ids;
	char *endptr;

	dim = strtol(argv[1], &endptr, 10);
	if (*argv[1] == '\0' || *endptr != '\0' || dim < 1)
		die("Dimensão inválida.\n");

	thread_num = strtol(argv[2], &endptr, 10);
	if (*argv[2] == '\0' || *endptr != '\0' || thread_num < 1)
		die("Número de threads inválido.\n");

	if (thread_num > dim)
		die("O número de threads precisa ser maior ou igual à dimensão da matriz.\n");

	if (!(line_ids = malloc(dim * sizeof(int))))
		die("Sem memória para alocar vetor de IDs das linhas.\n");

	if (!(thread_ids = malloc(thread_num * sizeof(pthread_t))))
		die("Sem memória para alocar vetor de IDs dos threads.\n");

	if (!(in1 = malloc(dim * dim * sizeof(double))))
		die("Sem memória para alocar matriz de entrada.\n");

	if (!(in2 = malloc(dim * dim * sizeof(double))))
		die("Sem memória para alocar matriz de entrada.\n");

	if (!(out = malloc(dim * dim * sizeof(double))))
		die("Sem memória para alocar matriz de saída.\n");

	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			SET(in1, i, j, i + j);
			SET(in2, i, j, i * j);
		}
	}

	memset(out, 0, dim * dim * sizeof(double));

	/* print_matrix(in1); */
	/* printf("*\n"); */
	/* print_matrix(in2); */
	/* printf("=\n"); */

	for (int i = 0; i < dim; i++)
		line_ids[i] = i % thread_num;

	for (long i = 0; i < thread_num; i++)
		pthread_create(thread_ids + i, NULL, threaded_multiply, line_ids + i);

	for (int i = 0; i < thread_num; i++)
		pthread_join(thread_ids[i], NULL);

	/* print_matrix(out); */

	free(line_ids);
	free(thread_ids);
	free(in1);
	free(in2);
	free(out);

	return EXIT_SUCCESS;
}
