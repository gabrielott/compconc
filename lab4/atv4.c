/*
 * Computação Concorrente
 * Laboratório 4, atividade 4
 * Aluno: Gabriel da Fonseca Ottoboni Pinho
 * DRE: 119043838
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int t1_print, t2_print, t3_print;
pthread_mutex_t t1_mutex, t2t3_mutex;
pthread_cond_t t1_cond, t2t3_cond;

void *
t1(void *args)
{
	printf("t1: Volte sempre!\n");

	pthread_mutex_lock(&t1_mutex);
	t1_print = 1;
	pthread_mutex_unlock(&t1_mutex);
	pthread_cond_broadcast(&t1_cond);

	pthread_exit(NULL);
}

void *
t2(void *args)
{
	pthread_mutex_lock(&t1_mutex);
	while (!t1_print)
		pthread_cond_wait(&t1_cond, &t1_mutex);
	pthread_mutex_unlock(&t1_mutex);

	printf("t2: Fique a vontade.\n");

	pthread_mutex_lock(&t2t3_mutex);
	t2_print = 1;
	if (t2_print && t3_print)
		pthread_cond_broadcast(&t2t3_cond);
	pthread_mutex_unlock(&t2t3_mutex);

	pthread_exit(NULL);
}

void *
t3(void *args)
{
	pthread_mutex_lock(&t1_mutex);
	while (!t1_print)
		pthread_cond_wait(&t1_cond, &t1_mutex);
	pthread_mutex_unlock(&t1_mutex);

	printf("t3: Sente-se por favor.\n");

	pthread_mutex_lock(&t2t3_mutex);
	t3_print = 1;
	if (t2_print && t3_print)
		pthread_cond_broadcast(&t2t3_cond);
	pthread_mutex_unlock(&t2t3_mutex);

	pthread_exit(NULL);
}

void *
t4(void *args)
{
	pthread_mutex_lock(&t2t3_mutex);
	while (!(t2_print && t3_print))
		pthread_cond_wait(&t2t3_cond, &t2t3_mutex);
	pthread_mutex_unlock(&t2t3_mutex);

	printf("t4: Seja bem-vindo!\n");

	pthread_exit(NULL);
}

int
main(void)
{
	pthread_mutex_init(&t1_mutex, NULL);
	pthread_mutex_init(&t2t3_mutex, NULL);
	pthread_cond_init(&t1_cond, NULL);
	pthread_cond_init(&t2t3_cond, NULL);

	pthread_t t1_thread, t2_thread, t3_thread, t4_thread;
	pthread_create(&t1_thread, NULL, t1, NULL);
	pthread_create(&t2_thread, NULL, t2, NULL);
	pthread_create(&t3_thread, NULL, t3, NULL);
	pthread_create(&t4_thread, NULL, t4, NULL);
	pthread_join(t1_thread, NULL);
	pthread_join(t2_thread, NULL);
	pthread_join(t3_thread, NULL);
	pthread_join(t4_thread, NULL);

	pthread_mutex_destroy(&t1_mutex);
	pthread_mutex_destroy(&t2t3_mutex);
	pthread_cond_destroy(&t1_cond);
	pthread_cond_destroy(&t2t3_cond);

	return EXIT_SUCCESS;
}
