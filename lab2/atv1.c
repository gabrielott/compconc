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

/* GET_TIME copiado de timer.h */
#define GET_TIME(now) { \
   struct timespec time; \
   clock_gettime(CLOCK_MONOTONIC, &time); \
   now = time.tv_sec + time.tv_nsec/1000000000.0; \
}

#define GET(m, i, j) m[i * dims + j]
#define SET(m, i, j, num) m[i * dims + j] = num

int dims;
int thread_num;
double *in1;
double *in2;
double *out;

void
die(char *msg)
{
	fprintf(stderr, "%s", msg);
	exit(EXIT_FAILURE);
}

void *
threaded_multiply(void *args)
{
	long id = (long) args;
	for (int i = id; i < dims; i += thread_num) {
		for (int j = 0; j < dims; j++) {
			for (int k = 0; k < dims; k++) {
				SET(out, i, j, GET(out, i, j) + GET(in1, i, k) * GET(in2, k, j));
			}
		}
	}

	return NULL;
}

int
main(int argc, char **argv)
{
	double start, finish;
	double init_time, mult_time, end_time;

	/* Inicialização das estruturas */
	GET_TIME(start);

	if (argc != 3)
		die("Uso: ./executável DIMENSÃO THREADS\n");

	char *endptr;
	dims = strtol(argv[1], &endptr, 10);
	if (*argv[1] == '\0' || *endptr != '\0' || dims < 1)
		die("Dimensão inválida.\n");

	thread_num = strtol(argv[2], &endptr, 10);
	if (*argv[2] == '\0' || *endptr != '\0' || thread_num < 1)
		die("Número de threads inválido.\n");

	if (thread_num > dims)
		die("O número de threads precisa ser menor ou igual à dimensão da matriz.\n");

	pthread_t *thread_ids = NULL;
	int malloc_err = 1;
	if (!(thread_ids = malloc(thread_num * sizeof(pthread_t))))
		goto err;
	if (!(in1 = malloc(dims * dims * sizeof(double))))
		goto err;
	if (!(in2 = malloc(dims * dims * sizeof(double))))
		goto err;
	if (!(out = malloc(dims * dims * sizeof(double))))
		goto err;
	malloc_err = 0;

	for (int i = 0; i < dims; i++) {
		for (int j = 0; j < dims; j++) {
			SET(in1, i, j, 1);
			SET(in2, i, j, 1);
		}
	}

	memset(out, 0, dims * dims * sizeof(double));

	GET_TIME(finish);
	init_time = finish - start;

	/* Threads e multiplicação */
	GET_TIME(start);

	for (long i = 0; i < thread_num; i++)
		pthread_create(thread_ids + i, NULL, threaded_multiply, (void *) i);
	for (int i = 0; i < thread_num; i++)
		pthread_join(thread_ids[i], NULL);

	GET_TIME(finish);
	mult_time = finish - start;

	/* Finalização */
	GET_TIME(start);

	int correct = 1;
	for (int i = 0; i < dims; i++) {
		for (int j = 0; j < dims; j++) {
			if (GET(out, i, j) != dims) {
				correct = 0;
				goto err;
			}
		}
	}

err:
	free(thread_ids);
	free(in1);
	free(in2);
	free(out);

	if (malloc_err)
		die("Erro ao alocar memória.\n");

	printf("Resultado %s.\n", (correct) ? "correto" : "incorreto");

	GET_TIME(finish);
	end_time = finish - start;

	printf("\nTempos:\n");
	printf("Inicialização: %fs\n", init_time);
	printf("Threads e multiplicação: %fs\n", mult_time);
	printf("Finalização: %fs\n", end_time);

	return EXIT_SUCCESS;
}
