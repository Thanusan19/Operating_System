#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

const int constInt = 0;
int globalInt;

int main()
{
	int *pointeur = NULL;
	pointeur = malloc(4);
	int nbrEnt;
	char elemChar;
	double nbrDbl;
	
	//Variable globale non modifiable présente dans Data (partie en Read only)
	printf("Adresse variable globale constante : %p\n",&constInt);
	//Variable globale modifiable présente dans BSS (partie en Read and Write)
	printf("Adresse varible globale : %p\n",&globalInt);
	//Variables déclarées localement présentes dans Stack
	printf("Adresse int : %p\n",&nbrEnt);
	printf("Adresse char : %p\n",&elemChar);
	printf("Adresse double : %p\n",&nbrDbl);
	//Variable non modifiable localement déclarée présente dans Heap
	printf("Adresse pointeur : %p\n",pointeur);
	
	//Création d'un processus fils pour permettre de voir l'excécution
	//du processus père dans la mémoire
	int pid, status;
	pid=fork();
	
	if(pid!=0)
	{ /*Code du père*/
		printf("Je suis le père %d\n.",getpid());
		wait(&status);
	}
	else
	{ /*Code du fils*/
		printf("Je suis le fils %d\n.",getpid());
		
		int ppid=getppid();
		char buffer [50];

		sprintf(buffer, "%d", ppid);
		//Execution avec le ppid pour observer le père
		execlp("pmap", "pmap","-X",buffer,NULL);
    }
    
    fflush(stdout); //Pour éviter les problèmes d'affichage
	sleep(20);
		
	return 0;
}
