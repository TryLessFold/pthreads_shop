#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
pthread_mutex_t m[5]={PTHREAD_MUTEX_INITIALIZER};
int shop[5];
void init()
{
	srand(time(NULL));
	for(int i = 0; i<5; i++)
	{
		shop[i] = rand()%201+900;
	}
}
struct args_shop
{
	int val;
	int pe;
};
void* shopping(void *vali)
{
	struct args_shop *tmp = (struct args_shop*) vali;
	int i = rand()%5;
			if(!pthread_mutex_trylock(&m[i]))
			{
				if(tmp->pe==3)
					printf("Shop%d = %5d + %5d\t Loader: %10d\n", i, shop[i], -(tmp->val), -(tmp->val));
				else
					printf("Shop%d = %5d - %5d\t Buyer%d worth: %10d\n", i, shop[i], shop[i], tmp->pe, tmp->val);
				fflush(stdout);
				if (tmp->val>=shop[i])
				{
					tmp->val -= shop[i];
					shop[i] = 0;
				}
				else
				{
					shop[i] -= tmp->val;
					tmp->val = 0;
				}
				pthread_mutex_unlock(&m[i]);
			}
}
void* buyer(void *numi)
{
	int *num = (int*) numi;
	int grid = 100000;
	struct args_shop tmp;
	tmp.val = grid;
	tmp.pe = *num;
	pthread_t tid;
	while(tmp.val>0)
	{
		//	printf("Buyer = %d", grid);
	//	fflush(stdout);
		pthread_create(&tid, NULL, shopping, &tmp);
		pthread_join(tid, NULL);
		sleep(2);
	}
}
void* loader()
{
	int load;
	int num = 3;
	struct args_shop tmp;
	pthread_t tid;
	while(1)
	{
		load = -500;
	//	printf("Loader = %d", load);
	//	fflush(stdout);
		tmp.val = load;
		tmp.pe = 3;
		pthread_create(&tid, NULL, shopping, &tmp);
		pthread_join(tid, NULL);
		sleep(3);
	}
}
int main()
{
	int tmp[3] = {0};
	pthread_t tid[4];
	for(int i=0; i<3; i++)
	{
		tmp[i] = i;
		pthread_create(&tid[i], NULL, buyer, &tmp[i]);
	}
	pthread_create(&tid[3], NULL, loader, NULL);
	for(int i =0; i<3; i++)
	{
		pthread_join(tid[i], NULL);
	}
	pthread_cancel(tid[3]);
	return 0;
}
