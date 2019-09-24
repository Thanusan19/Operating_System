#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main()
{
	int fd = open("test.txt", O_RDWR);
    printf("Le descripteur du fichier test.txt fd : %d\n", fd);
	
	struct stat fileStat;
	//Récupération des informations sur le fichier test.txt
	stat("test.txt", &fileStat);

	printf("Taille du fichier : %ld\n", fileStat.st_size);
	
	//Création d'un segment en mémoire avec le texte de test.txt inscrit à cet emplacement
	void *mmappedData = mmap(NULL, fileStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	printf("Adresse de la mémoire :%p\n",mmappedData);
	
	//Inversion des octets du fichier
	char* mmappedDatap=mmappedData;
	
	//Buffeur pour récupérer le contenu inversé
	char* invBuffer=NULL;
	invBuffer = malloc(fileStat.st_size * sizeof(char));
   
    for(int i=0;i<fileStat.st_size;i++)
    {
		invBuffer[fileStat.st_size-i-1]=*mmappedDatap;
		mmappedDatap+=sizeof(char);
	}
	
	invBuffer[fileStat.st_size]='\0';
	
	for(int i=0;i<fileStat.st_size;i++)
	{
		printf("%c",invBuffer[i]);
	}
	
	//Copie dans la mémoire le fichier inversé à l'emplacement mémoire associé au fichier mappé
	memcpy(mmappedData,invBuffer,fileStat.st_size);

	//Gestion d'erreur lors de la libération de l'espace mémoire
	if(munmap(mmappedData,fileStat.st_size)==0)
	{
		printf("\nFree : mmap memory\n");
	}
	else
	{
		printf("Something went wrong with munmap()! %s\n", strerror(errno));
	}
	
	printf("Vérification de l'inversion d'octets: \n");
	system("cd TP_OS/TP1/Q2_p/;cat test.txt");
	
    //Création d'un processus fils pour permettre de voir l'exécution du processsus père "mmap" dans la mémoire
    int pid, status;
    pid=fork();
    
    if(pid!=0)
    { /*Code du père*/
		printf("\n\nJe suis le père %d\n",getpid());
		wait(&status);
	}
	else
	{ /*Code du fils*/
		printf("je suis le fils %d\n",getpid());
		int ppid=getppid();
		char buffer [50];

		sprintf(buffer, "%d", ppid);
		execlp("pmap", "pmap","-X",buffer,NULL);
		printf("ceci ne sera jamais affiché");
    }
		
	return 0;
}
