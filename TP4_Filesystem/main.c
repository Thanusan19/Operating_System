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
#include "tosfs.h"

int main()
{
	//Récupération du descripteur de fichier à mapper
    int fd = open("test_tosfs_files", O_RDWR);
    printf("Le descripteur de fichier fd= %d\n", fd);
    
    struct stat fileStat;
    //Récupération des informations sur le fichier test_tosfs_files (sa taille)
    stat("test_tosfs_files", &fileStat);
    printf("ID du périph : %ld\n",fileStat.st_dev);
    printf("Numéro inoeud : %ld\n",fileStat.st_ino);
    printf("Protection : %d\n",fileStat.st_mode);
    printf("Nb liens matériels : %ld\n",fileStat.st_nlink);
    printf("UID propriétaire : %d\n",fileStat.st_uid);
    printf("GID propriétaire : %d\n",fileStat.st_gid);
    printf("ID périphérique : %ld\n",fileStat.st_rdev);
    long fileSize= fileStat.st_size;
    printf("File size : %ld\n",fileSize);
    printf("Taille de bloc : %ld\n",fileStat.st_blksize);
    printf("Nombre de blocs allouées : %ld\n",fileStat.st_blocks);
    printf("Heure dernier accès : %ld\n",fileStat.st_atime);
    printf("Heure dernière modification : %ld\n",fileStat.st_mtime);
    printf("Heure dernier changement état : %ld\n",fileStat.st_ctime);

    //Création d'un segment en mémoire avec le texte de test_tosfs_files inscrit à cet emplacement
    void *mmappedData = mmap(NULL, fileStat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    printf("Adresse de la mmap: %p\n\n",mmappedData);

    //Récupération des informations sur le système de fichier: SUPERBLOCK
    struct tosfs_superblock* SB;
    SB = (struct tosfs_superblock*) mmappedData;

    printf("Super block infos \n");
    printf("Magic number : 0x%d\n",SB->magic);
    printf("Block bitmap : "PRINTF_BINARY_PATTERN_INT32 "\n", PRINTF_BYTE_TO_BINARY_INT32(SB->block_bitmap));
    printf("Inode bitmap : "PRINTF_BINARY_PATTERN_INT32 "\n", PRINTF_BYTE_TO_BINARY_INT32(SB->inode_bitmap));
    printf("Block size : %d\n",SB->block_size);
    printf("Blocks : %d\n",SB->blocks);
    printf("Inodes : %d\n",SB->inodes);
    printf("Root inode : %d\n\n",SB->root_inode);

    //Récupération des numéros d'inodes et d'autres informations (pointeur vers les fichiers) associés aux différents fichiers
    struct tosfs_inode *inode;
    inode = (struct tosfs_inode*) ((void*) SB + SB->block_size);

    printf("Inode infos \n");
    for(unsigned int i=0;i<SB->inodes;i++){
        printf("Inode number : %d\n",inode->inode);
        printf("Block number for data : %d\n",inode->block_no);
        printf("uid : %d\n",inode->uid);
        printf("gid : %d\n",inode->gid);
        printf("Mode : %o\n",inode->mode);
        printf("Perm : %d\n",inode->perm);
        printf("Size in byte : %d\n",inode->size);
        printf("Number of hardlink : %d\n\n",inode->nlink);
        inode += 1;
    }
    printf("\n");

    //Récupération des noms ou liens vers les fichiers
    struct tosfs_dentry *dentry;
    dentry = (struct tosfs_dentry*) ((void*) SB + SB->block_size*2); 

    printf("Root infos\n");
    while(dentry->inode!=0){
        printf("Inode number : %d\n",dentry->inode);
        printf("Name : %s\n\n",dentry->name);
        dentry += 1;
    }

    //Création d'un processus fils pour permettre de voir la segementation de la mémoire pendant l'exécution du processsus père "mmap"
    int pid, status;
    pid=fork();
    
    if(pid!=0){ 
		printf("\n\nJe suis le père %d\n",getpid());
		wait(&status);
	}else{ 
		printf("Je suis le fils %d\n",getpid());
		int ppid=getppid();
		char buffer [50];

		sprintf(buffer, "%d", ppid);
		execlp("pmap", "pmap","-X",buffer,NULL);
    }

	//Gestion d'erreur lors de la libération de l'espace mémoire
	if(munmap(mmappedData,fileSize)==0)
	{
		printf("\nFree :mmap memory\n");
	}
	else
	{
		printf("Something went wrong with munmap()! %s\n", strerror(errno));
	}   
	
	return 0;
}