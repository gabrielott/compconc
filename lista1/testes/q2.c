#include <stdio.h>
#include <pthread.h>

int x = 0;

void *
t1(void *args)
{
	printf("t1 start\n");
	x++;
	x--;
	if (x == 0)
		printf("t1 ok: %d\n", x);
	else
		printf("t1 fail: %d\n", x);
	printf("t1 end\n");
	return NULL;
}

void *
t2(void *args)
{
	printf("t2 start\n");
	x--;
	x--;
	if (x == -2)
		printf("t2 ok: %d\n", x);
	else
		printf("t2 fail: %d\n", x);
	printf("t2 end\n");
	return NULL;
}

void *
t3(void *args)
{
	printf("t3 start\n");
	x++;
	x++;
	if (x == 2)
		printf("t3 ok: %d\n", x);
	else
		printf("t3 fail: %d\n", x);
	printf("t3 end\n");
	return NULL;
}

int
main(void) {
	pthread_t t;
	pthread_create(&t, NULL, t2, NULL);
	pthread_create(&t, NULL, t1, NULL);
	pthread_create(&t, NULL, t3, NULL);
	pthread_exit(NULL);
}
