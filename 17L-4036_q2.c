#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

char* buff_1;
char* buff_2;

sem_t* s1;
sem_t* s2;
sem_t* s3;

void proc_C()
{
	sem_wait(s1);
	sem_wait(s2);

	buff_2 = (char*)malloc(25);
	for (int i = 0; buff_1[i] != '\0'; i++)
		buff_2[i] = buff_1[i];

	sem_post(s3);
}
void proc_D()
{
	sem_wait(s3);
	printf("%s", buff_2);
}
void proc_A_read_file1(char* a)
{

	int fd = open(a, O_RDONLY);
	if (fd < 0)
		printf("File Not Opened!\n");

	int s = lseek(fd, 0, SEEK_END);
	close(fd);

	fd = open(a, O_RDONLY);
	buff_1 = (char*)malloc(25);
	read(fd, buff_1, 10);
	close(fd);

	sem_post(s2);
}

void proc_B_read_file2(char* b)
{

	int fd = open(b, O_RDONLY);
	if (fd < 0)
		printf(" File Not Opened!\n");

	close(fd);
	fd = open(b, O_RDONLY);
	char* buff = (char*)malloc(11);
	buff[11] = '\0';
	read(fd, buff, 10);
	close(fd);

	int k = 11;
	for (int i = 0; i < 11; i++, k++)
		buff_1[k] = buff[i];

	sem_post(s1);
}

int main()
{
	sem_init(s1, NULL, NULL);
	sem_init(s2, NULL, NULL);
	sem_init(s3, NULL, NULL);

	pid_t pid_1 = fork();
	pid_t pid_2 = fork();
	pid_t pid_3 = fork();
	pid_t pid_4 = fork();

	if (pid_1 > 0)
		proc_A_read_file1("file1.txt");

	if (pid_2> 0)
		proc_B_read_file2("file2.txt");

	if (pid_3 > 0)
		proc_C();

	if (pid_4> 0)
		proc_D();

	return 0;
}
