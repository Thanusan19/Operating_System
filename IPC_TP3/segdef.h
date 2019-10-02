#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>

#define cle 		(key_t)3
#define seg_dispo 	0
#define seg_init  	1
#define res_ok    	2
#define maxval   	100

typedef struct shmseg
{
   int tty;
   int req;
   long tab[maxval];
   long result;
} segment;

#define segsize sizeof(segment)

void init_rand();
long getrand();
int ntty();
void wait_sem(int,int);
void acq_sem(int,int);
void lib_sem(int,int);
