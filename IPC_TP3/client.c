#include "segdef.h"
#include <signal.h>


int shmid, semid;
segment *seg;
	
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
	
	init_rand();
}

int main()
{	
	init();
	int forkNumber=2;
	
	for (int i = 0; i < forkNumber; i++)
	{
		int pid;
		pid=fork();
		

		if(pid>0){ //le père
			int cmptRequete=1;
			int nbRequest=1;

			printf("Client pid : %d \n",pid);
			printf("Client Number : %d \n",i);
			
			while(cmptRequete<=nbRequest)
			{
				long moyServeur=0;
				acq_sem(semid, seg_dispo);

				seg->tty=getpid();
				seg->req=cmptRequete;
				long moy=0;
				for(int i=0;i<maxval;i++)
				{
					seg->tab[i]=getrand();
					moy+=seg->tab[i];
				}
				moy/=maxval;//Calcul de la moyenne en local par le client

				acq_sem(semid, seg_init);
						
				wait_sem(semid, res_ok);

				moyServeur=seg->result;//Acquisition du résultat stocké par le serveur

				lib_sem(semid, seg_init);

				acq_sem(semid, res_ok);
				lib_sem(semid, res_ok);

				lib_sem(semid, seg_dispo);
				cmptRequete+=1; //COmpteur du nombre de requêtes faite au serveur par un seul client

				if(moy==moyServeur)
				{
					//printf("On a le même résultat\n");
				}
				else
				{
					//printf("Les résultats sont différents\n");
				}
				//printf("Résultat local : %ld\n", moy);
				//printf("Résultat serveur : %ld\n", seg->result);
				

			}
			
			shmdt(seg);
			//kill(pid, SIGKILL);
			//kill(getpid(), SIGKILL);
		}else{
			//kill(getgid(),SIGKILL);
		}
	}
	return 0;
}
