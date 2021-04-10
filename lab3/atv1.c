/*
 * Computação Concorrente
 * Laboratório 3, atividade 1
 * Aluno: Gabriel da Fonseca Ottoboni Pinho
 * DRE: 119043838
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

/* GET_TIME copiado de timer.h */
#define GET_TIME(now) { \
   struct timespec time; \
   clock_gettime(CLOCK_MONOTONIC, &time); \
   now = time.tv_sec + time.tv_nsec/1000000000.0; \
}

int
main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Uso: ./executavel NUMERO_ELEMENTOS.\n");
		return EXIT_FAILURE;
	}

	char *endptr;
	long long n = strtoll(argv[1], &endptr, 0);
	if (*argv[1] == '\0' || *endptr != '\0' || n < 1) {
		fprintf(stderr, "Número de elementos inválido.\n");
		return EXIT_FAILURE;
	}

	double start, finish;
	GET_TIME(start);

	double sum = 0;
	for (long long i = 0; i < n; i++) {
		if (i % 2 == 0)
			sum += 1.0 / (2 * i + 1);
		else
			sum -= 1.0 / (2 * i + 1);
	}
	sum *= 4;

	GET_TIME(finish);

	printf("Calculado: %.15f\n", sum);
	printf("     M_PI: %.15f\n", M_PI);
	printf("    Tempo: %fs\n", finish - start);

	return EXIT_SUCCESS;
}
