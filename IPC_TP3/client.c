#include "segdef.h"
#include <signal.h>


int shmid, semid;
segment *seg;

/*********************************************************/
/*Fonction d'initialisation des segments et des sémaphores*/
/*********************************************************/	
void init()
{	
	/*L'espace mémoire partagé et les sémaphores sont initialisés par le serveur
	d'où les arguments 0 de shmget et semget*/
	
	//Récupération de l'identifiant du segment
	if((shmid = shmget(cle, 0, 0)) == -1) 
	{
		perror("shmget");
		exit(1);
	}
	printf("%s%d\n", "Shared memory id is ", shmid);
	
	//Récupération de l'identifiant du sémaphore
	if((semid = semget(cle, 3, 0)) == -1)
	{
		perror("semid");
		exit(1);
	}
	printf("%s%d\n", "Semaphore set id is ", semid);
	
	//Attachement du segment à une adresse mémoire allouée
	if((seg = (segment *) shmat(shmid, 0, 0)) == (segment *) -1)
	{
		perror("shmat");
		exit(1);
	}
	
	//Initialisaion du générateur
	init_rand();
}

/*************************************************/
/*Acquisition du sémaphore*/
/*************************************************/
void acq_sem_3is(int semid,int SEMO)
{
	struct sembuf sb;
	sb.sem_num=SEMO;
	//Acquisition du sémaphore == -1
	sb.sem_op=-1;
	sb.sem_flg=SEM_UNDO;
	if(semop(semid,&sb,1)==-1)
	{
		perror("semop");
		exit(1);
	}
}

/*************************************************/
/*Libérer le sémaphore*/
/*************************************************/
void lib_sem_3is(int semid,int SEMO){
	struct sembuf sb;
	sb.sem_num=SEMO;
	//Acquisition du sémaphore == -1
	sb.sem_op=1;
	sb.sem_flg=SEM_UNDO;
	if(semop(semid,&sb,1)==-1)
	{
		perror("semop");
		exit(1);
	}
}

/*************************************************/
/*Attente de la libération d'un sémaphore*/
/*************************************************/
void wait_sem_3is(int semid,int SEMO){
	int semVal=semctl(semid,SEMO,GETVAL);
	//Tant que le sémaphore est pris
	/*  1---> sémaphore pris
		0---> sémaphore libre*/
	while(semVal!=0)
	{
		semVal=semctl(semid,SEMO,GETVAL);
		if(semVal==-1)
		{
			perror("semctl");
			exit(1);
		}
	}
}

int main()
{	
	init();
	//Nombre de clients qui vont appeler le serveur
	int forkNumber=1024;
	
	for (int i = 0; i < forkNumber; i++)
	{
		int pid;
		pid=fork();
		
		if(pid!=0)//le père
		{ 
			int cmptRequete=1;
			int nbRequest=10;

			//printf("Client pid : %d \n",pid);
			//printf("Client Number : %d \n",i);
			
			while(cmptRequete<=nbRequest)
			{
				long moyServeur=0;
				long moy=0;

				//Demande d'acquisition du segment
				acq_sem_3is(semid, seg_dispo);

				seg->tty=getpid();
				seg->req=cmptRequete;
				for(int i=0;i<maxval;i++)
				{
					seg->tab[i]=getrand();
					moy+=seg->tab[i];
				}
				moy/=maxval;//Calcul de la moyenne en local par le client

				//Vérification de l'initialisation du segment
				acq_sem_3is(semid, seg_init);
						
				//Attente de la complétion du calcul côté serveur
				wait_sem_3is(semid, res_ok);

				moyServeur=seg->result;//Acquisition du résultat de la moyenne stocké par le serveur

				/*Libération des différents sémaphores*/
				lib_sem_3is(semid, seg_init);

				acq_sem_3is(semid, res_ok);
				lib_sem_3is(semid, res_ok);

				lib_sem_3is(semid, seg_dispo);
				cmptRequete+=1; //Compteur du nombre de requêtes faite au serveur par un seul client

				if(moy==moyServeur)
				{
					printf("On a le même résultat\n");
				}
				else
				{
					printf("Les résultats sont différents\n");
				}				
			}
		}
		else
		{
			//Tue les processus fils pour ne garder que les processus père
			kill(getpid(),SIGKILL);
		}
	}
	
	//Détachement du segment mémoire
	shmdt(seg);
	return 0;
}
