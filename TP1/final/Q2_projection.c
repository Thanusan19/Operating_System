#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char **argv)
{
    //Récupération du descripteur de fichier à mapper
    int fd = open("test.txt", O_RDWR);
    printf("Le descripteur de fichier fd= %d\n", fd);
    
    struct stat fileStat;
    //Récupération des informations sur le fichier test.txt (sa taille)
    stat("test.txt", &fileStat);
    long fileSize= fileStat.st_size;
    printf("File size : %ld\n",fileSize);


    //Création d'un segment en mémoire avec le texte de test.txt inscrit à cet emplacement
    void *mmappedData = mmap(NULL, fileStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    printf("Adresse de la mmap: %p\n",mmappedData);
   
 /*----------------------------------------------------*/ 
    //Inversion des octets du fichier en les récupérant dans un tableau de manière décroissante
    char* mmappedDatap=mmappedData;
    char* invBuffer=NULL;
    invBuffer = malloc(fileSize * sizeof(char));
   
    printf("\nAffichage du contenu du fichier stocké en mémoire via mmap :\n");
    for(int i=0;i<fileSize;i++)
    {
	invBuffer[fileSize-i-1]=*mmappedDatap;
	printf("%c",*mmappedDatap);
	mmappedDatap+=sizeof(char);
    }
	
    invBuffer[fileSize]='\0';
    printf("\n");
	
    printf("\nAffichage du tableau invBUffer: \n");
    for(int i=0;i<fileSize;i++)
    {
	printf("%c",invBuffer[i]);
    }
    //Copie dans la mémoire le contenu inversé à l'emplacement mémoire associé au fichier mappé
    memcpy(mmappedData,invBuffer,fileSize);

/*---------------------------------------------------*/  
    //Création d'un processus fils pour permettre de voir la segementation de la mémoire pendant l'exécution du processsus père "mmap"
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
	//Transformer ppid en un tableau "char" pour l'utiliser comme argument de execlp
	sprintf(buffer, "%d", ppid);
	execlp("pmap", "pmap","-X",buffer,NULL);
	printf("ceci ne sera jamais affiché");
    }
/*---------------------------------------------------*/

    //Gestion d'erreur lors de la libération de l'espace mémoire
    if(munmap(mmappedData,fileSize)==0)
    {
 	printf("\nFree :mmap memory\n");
    }
    else
    {
	printf("Something went wrong with munmap()! %s\n", strerror(errno));
    }
	
    //Vérifier l'inversion des octets (il faut cloner TP_OS dans le répertoire racine de votre PC)
    printf("\n\nVérification de l'inversion d'octets dans le fichier via 'cat': \n");
    system("cat test.txt");
	
    return 0;
}
