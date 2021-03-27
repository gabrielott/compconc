/*
 * Computação Concorrente
 * Laboratório 1, atividade 5
 * Aluno: Gabriel da Fonseca Ottoboni Pinho
 * DRE: 119043838
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define ARR_SIZE 10000

int arr[ARR_SIZE];

void *
increment_arr(void *args)
{
	int id = *(int *) args;
	if (id) {
		for (int i = 0; i < ARR_SIZE / 2; i++)
			arr[i]++;
	} else {
		for (int i = ARR_SIZE - 1; i >= ARR_SIZE / 2; i--)
			arr[i]++;
	}

	return NULL;
}

int
main(void)
{
	pthread_t t1, t2;
	int arg1 = 0;
	int arg2 = 1;

	if (pthread_create(&t1, NULL, increment_arr, &arg1) || pthread_create(&t2, NULL, increment_arr, &arg2)) {
		printf("Erro ao criar threads.\n");
		return EXIT_FAILURE;
	}

	if (pthread_join(t1, NULL) || pthread_join(t2, NULL)) {
		printf("Erro ao aguardar threads.\n");
		return EXIT_FAILURE;
	}

	int ok = 1;
	for (int i = 0; i < ARR_SIZE; i++) {
		if (!arr[i]) {
			printf("Resultado incorreto.\n");
			ok = 0;
			break;
		}
	}

	if (ok)
		printf("Resultado correto.\n");

	return EXIT_SUCCESS;
}
