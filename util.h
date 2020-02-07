#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <math.h>
#include <errno.h>


struct sh_string{
	char in[80];
	char out[80];
};

struct sh_mem {
	int feed;
	clock_t Tim;
};

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

void up(int semid, int i, int n) {
struct sembuf operation;
operation.sem_flg  = 0;
operation.sem_num  = i;
operation.sem_op   = n;
semop(semid, &operation, 1);
return;
};

void down(int semid, int i, int n) {
struct sembuf operation;
operation.sem_flg  = 0;
operation.sem_num  = i;
operation.sem_op   = -n;
semop(semid, &operation, 1);
return;
};

void seminit(int semid, int i, int value) {
union semun arg;
arg.val = value;
semctl(semid, i, SETVAL, arg);
return;
}
