/*
 * Computação Concorrente
 * Laboratório 5, atividade 1
 * Aluno: Gabriel da Fonseca Ottoboni Pinho
 * DRE: 119043838
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int *arr;
int wait_for;
int active_threads;
pthread_mutex_t wait_mutex;
pthread_cond_t all_done_cond;

void
die(char *msg)
{
	fprintf(stderr, "%s", msg);
	exit(EXIT_FAILURE);
}

void *
threaded_prefix_sum(void *args)
{
	long id = ((long) args + 1) * 2;
	long set_index = id - 1;
	int offset = 1;
	for (;;) {
		id /= 2;
		offset *= 2;
		long get_index = (set_index + 1) - (offset / 2) - 1;
		arr[set_index] += arr[get_index];

		if (active_threads == 1)
			pthread_exit(NULL);

		pthread_mutex_lock(&wait_mutex);
		if (--wait_for > 0) {
			pthread_cond_wait(&all_done_cond, &wait_mutex);
		} else {
			active_threads /= 2;
			wait_for = active_threads;
			pthread_cond_broadcast(&all_done_cond);
		}
		pthread_mutex_unlock(&wait_mutex);

		if (id % 2 != 0)
			pthread_exit(NULL);
	}
}

void
sequential_prefix_sum(int *seq_arr, int size)
{
	for (int i = 1; i < size; i++)
		seq_arr[i] += seq_arr[i - 1];
}

int
main(int argc, char **argv)
{
	if (argc != 2)
		die("Uso: ./executável TAMANHO_ARRAY\n");

	char *endptr;
	int arr_size = strtol(argv[1], &endptr, 0);
	if (*argv[1] == '\0' || *endptr != '\0' || arr_size < 1)
		die("Tamanho do array inválido.\n");

	if (arr_size & (arr_size - 1))
		die("Tamanho deve ser uma potência de 2.\n");

	if (!(arr = malloc(arr_size * sizeof(int))))
		die("Erro ao alocar memória.\n");

	pthread_t *thread_ids;
	if (!(thread_ids = malloc(arr_size / 2 * sizeof(pthread_t))))
		die("Erro ao alocar memória.\n");

	for (int i = 0; i < arr_size; i++)
		arr[i] = i;

	sequential_prefix_sum(arr, arr_size);
	printf("Sequencial:  %d\n", arr[arr_size - 1]);

	for (int i = 0; i < arr_size; i++)
		arr[i] = i;

	pthread_mutex_init(&wait_mutex, NULL);
	pthread_cond_init(&all_done_cond, NULL);
	active_threads = arr_size / 2;
	wait_for = active_threads;

	for (long i = 0; i < arr_size / 2; i++)
		if (pthread_create(thread_ids + i, NULL, threaded_prefix_sum, (void *) i))
			die("Erro ao criar thread.\n");

	for (int i = 0; i < arr_size / 2; i++)
		if (pthread_join(thread_ids[i], NULL))
			die("Erro ao aguardar thread.\n");

	printf("Concorrente: %d\n", arr[arr_size - 1]);

	free(arr);
	free(thread_ids);
	pthread_mutex_destroy(&wait_mutex);
	pthread_cond_destroy(&all_done_cond);

	return EXIT_SUCCESS;
}
