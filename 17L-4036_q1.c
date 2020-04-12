#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>

sem_t potentialCPatients;
sem_t fluPatient;
sem_t coronaPatient;

void* CovidManagementSystem(void* var)
{
	int i = 1;
	while (i == 1) {
		sem_wait(&potentialCPatients);
		int a = rand() % 2;

		if (a == 0) {
			sem_post(&fluPatient);
			printf("Patient Has a flu! \n");
		}
		else {
			sem_post(&coronaPatient);
			printf("Patient is Coronavirus Positive! \n");
			int k = 0;
			while (i == 1)
			{
				a = rand() % 2;
				if (a == 0)
					k++;
				if (k == 2) {
					sem_wait(&coronaPatient);
					printf("Patient has recovered! \n");
					break;
				}
			}
		}
		return NULL;
	}
}

int main()
{
	int N;
	printf("Enter Number of Potential Corona Patients that entered the Hospital:\n");
	scanf("%d", &N);
	pthread_t id;
	sem_init(&potentialCPatients, 0, N);
	sem_init(&fluPatient, 0, 0);
	sem_init(&coronaPatient, 0, 0);

	for (int i = 0; i < N; i++)
		pthread_create(&id, NULL, &CovidManagementSystem, (void*)&i);

	pthread_exit(NULL);
	return 0;
}
