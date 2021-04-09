/*
 * Computação Concorrente
 * Laboratório 3, atividade 2
 * Aluno: Gabriel da Fonseca Ottoboni Pinho
 * DRE: 119043838
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>

long long steps;
int threads;

void
die(char *msg)
{
	fprintf(stderr, "%s", msg);
	exit(EXIT_FAILURE);
}

void *
pi_threaded(void *args)
{
	long id = (long) args;
	double sum = 0;

	long long start = id * (steps / threads);
	long long end = (id == threads - 1) ? steps : start + steps / threads;
	for (long long i = start; i < end; i++) {
		if (i % 2 == 0)
			sum += 1.0 / (2 * i + 1);
		else
			sum -= 1.0 / (2 * i + 1);
	}

	double *ret;
	if (!(ret = malloc(sizeof(double))))
		die("Erro ao tentar alocar memória.\n");
	*ret = sum;

	pthread_exit(ret);
}

int
main(int argc, char **argv)
{
	double sum = 0;

	if (argc != 3)
		die("Uso: ./executável ELEMENTOS THREADS\n");

	char *endptr;
	steps = strtol(argv[1], &endptr, 10);
	if (*argv[1] == '\0' || *endptr != '\0' || steps < 1)
		die("Número de elementos inválido.\n");

	threads = strtoll(argv[2], &endptr, 10);
	if (*argv[2] == '\0' || *endptr != '\0' || threads < 1)
		die("Número de threads inválido.\n");

	if (threads > steps)
		die("O número de threads precisa ser menor ou igual ao número de elementos.\n");

	pthread_t *thread_ids;
	if (!(thread_ids = malloc(threads * sizeof(pthread_t))))
		die("Erro ao tentar alocar memória.\n");

	for (long i = 0; i < threads; i++)
		if (pthread_create(thread_ids + i, NULL, pi_threaded, (void *) i))
			die("Erro ao criar threads.\n");

	for (int i = 0; i < threads; i++) {
		double *ret;
		if (pthread_join(thread_ids[i], (void **) &ret))
			die("Erro ao aguardar threads.\n");

		sum += *ret;
		free(ret);
	}
	sum *= 4;

	printf("Calculado: %.15f\n", sum);
	printf("     M_PI: %.15f\n", M_PI);

	free(thread_ids);

	return EXIT_SUCCESS;
}
