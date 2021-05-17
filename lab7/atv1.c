/*
 * Computação Concorrente
 * Laboratório 7, atividade 1
 * Aluno: Gabriel da Fonseca Ottoboni Pinho
 * DRE: 119043838
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t wait_2;
sem_t wait_3;
sem_t wait_4;

void *
t1(void *args)
{
	sem_wait(&wait_2);
	sem_wait(&wait_3);
	printf("t1: Volte sempre!\n");
	pthread_exit(NULL);
}

void *
t2(void *args)
{
	sem_wait(&wait_4);
	printf("t2: Fique a vontade.\n");
	sem_post(&wait_2);
	sem_post(&wait_4);
	pthread_exit(NULL);
}

void *
t3(void *args)
{
	sem_wait(&wait_4);
	printf("t3: Sente-se por favor.\n");
	sem_post(&wait_3);
	sem_post(&wait_4);
	pthread_exit(NULL);
}

void *
t4(void *args)
{
	printf("t4: Seja bem-vindo!\n");
	sem_post(&wait_4);
	pthread_exit(NULL);
}

int
main(void)
{
	pthread_t threads[4];
	sem_init(&wait_2, 0, 0);
	sem_init(&wait_3, 0, 0);
	sem_init(&wait_4, 0, 0);

	pthread_create(threads + 0, NULL, t1, NULL);
	pthread_create(threads + 1, NULL, t2, NULL);
	pthread_create(threads + 2, NULL, t3, NULL);
	pthread_create(threads + 3, NULL, t4, NULL);

	for (int i = 0; i < 4; i++)
		pthread_join(threads[i], NULL);

	sem_destroy(&wait_2);
	sem_destroy(&wait_3);
	sem_destroy(&wait_4);
	return EXIT_SUCCESS;
}
