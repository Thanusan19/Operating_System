#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define MAGIC 0x0123456789ABCDEF

typedef struct HEADER_TAG {
    struct HEADER_TAG * ptr_next; /* pointe sur le prochain bloc libre -->8 octets */
    size_t bloc_size; /* taille du memory bloc en octets --> 8 octets car "long"*/
    long magic_number; /* 0x0123456789ABCDEFL --> 8 octets "long"*/
} HEADER;

/********Variable Globale*********/
//Liste chaînée récupérant les espaces mémoires libérés après un free
HEADER *freeBlocList=NULL;


/****************************************************/
/*Allocation d'un bloc mémoire-> fonction "malloc()"*/
/****************************************************/
void *malloc_3is(int nbrByte){
    /*Taille d'espace nécessaire en plus de l'espace demandé par le process
    HEADER + DATA + WD  (HEADER= ptr_nxt + size + WD)*/
    int nbrByteTotal=sizeof(HEADER)+nbrByte+sizeof(MAGIC);
/*--------------------------------------------------------*/
    /***Première possibilité : teste si un bloc mémoire libéré de la bonne taille a déjà été alloué auparavant***/
    long freeBlocSize=0;
    HEADER *previous, *current;
    if(freeBlocList!=NULL){//Teste pour savoir si la liste chaînée contient au moins un pointeur vers un bloc mémoire libéré
        freeBlocSize=freeBlocList->bloc_size +sizeof(HEADER)+sizeof(MAGIC);
	
	//Vérifie que le dernier bloc stocké dans la liste à le même nombre d'octet que celui demandé par le processus 
        if(freeBlocSize==nbrByteTotal){
            printf("Bloc start Adress (Header+data+WD): %p \n",freeBlocList);
            current=freeBlocList;
            freeBlocList=freeBlocList->ptr_next;
            return (void*) current+sizeof(HEADER);
        }
	/*Parcours sur le reste de la liste chaînée, nécessité du bloc précédent le bloc avec le bon nombre d'octet s'il existe d'où la 
	nécessité du test précédent fait séparément*/
        else{
            current=freeBlocList->ptr_next; 
            previous=freeBlocList; 
            while(current!=NULL){
                freeBlocSize=current->bloc_size +sizeof(HEADER)+sizeof(MAGIC);
                if(freeBlocSize==nbrByteTotal){
                    printf("Bloc start Adress (Header+data+WD): %p \n",current);
                    previous->ptr_next=current->ptr_next;// Permet de supprimer le "current bloc" de la liste chaînée en reliant le pointeur précédent du"current bloc" avec le pointeur suivant 
                    printf("current :%ld \n",current->bloc_size);

                    //"current" sera renvoyé comme adresse de bloc mémoire pour l'espace en octet demandé --> décalé du header pour pointer sur l'espace DATA
                    return (void*) current+sizeof(HEADER);
                }
                previous=current;
                current=current->ptr_next;
            }
        }
    }

    
 /*-------------------------------------------------*/
    /***Deuxième possibilité : demande d'espace à allouer dans la mémoire au dessus du bloc HEAP***/
    HEADER *newSpace=sbrk(nbrByteTotal);
    if(newSpace==(void*) -1){
        exit(EXIT_FAILURE);
    }

    newSpace->ptr_next=NULL;
    //Espace alloué pour l'utilisateur -> DATA (sans tenir compte du header, du WatchDog et du MAGIC number)
    newSpace->bloc_size=nbrByte;
    newSpace->magic_number=MAGIC;
    printf("Bloc start Adress (Header+data+WD): %p \n",newSpace);

    void *dataAddr=newSpace; //Caster une variable "HEADER" en "void" pour effectuer un décalage sur l'adresse correctement
    dataAddr=dataAddr + sizeof(HEADER); //Décalage de l'adresse sur la zone utilisable par l'utilisateur

    long *wdEndAddr=dataAddr+nbrByte; //Lecture du nombre MAGIC sur les 8 derniers octets de l'espace alloué
    *wdEndAddr=MAGIC;
    printf("%#010lx \n",*wdEndAddr);
    return dataAddr; //Retourner l'adresse de l'espace utilisable par l'utilisateur -> DATA
}

/***************************************************/
/*Vérification d'un éventuel débordement de mémoire*/
/***************************************************/
void check_mem(void *dataAddr){
    HEADER *header=dataAddr-sizeof(HEADER);
    if(header->magic_number==MAGIC){ //Vérifier le WatchDog en début de la zone DATA
        printf("Success WD start: %#010lx \n",header->magic_number);
        long *wdEndAddr=dataAddr+ header->bloc_size;
        if(*wdEndAddr==MAGIC){//Vérifier le WatchDog en fin de la zone DATA
            printf("Success WD end: %#010lx \n",*wdEndAddr);
        }
    }else{
        exit(EXIT_FAILURE);
    }
}

/***********************************************************************/
/* Enregistrer la mémoire libérée par le process dans une liste chaînée*/
/***********************************************************************/
void *free_3is(void *dataAddr){
    //"freeBlocList" --> variable globale = pointeur de type HEADER
    //Initialisation dans le cas où aucun élément est dans la liste chaînée
    if(freeBlocList==NULL){
        freeBlocList=dataAddr-sizeof(HEADER);
    }
    else{
        HEADER *newFreeBloc=dataAddr-sizeof(HEADER);
        newFreeBloc->ptr_next=freeBlocList;//Pointeur de la liste actuelle est déplacé vers le "ptr_next" du nouveau bloc à ajouter à la liste chaînée
        freeBlocList=newFreeBloc;//Pointeur de la liste chaînée pointe sur l'adresse de début du nouveau bloc mémoire
    }
    return freeBlocList; //Renvoie l'adresse de début de la liste chaînée
    
}


int main(){
    printf("\n/******************************/");
    printf("Première allocation");
    printf("/******************************/\n");

    void *dataAddr=malloc_3is(10);
    printf("Memory Bloc Adress: %p \n",dataAddr);
    check_mem(dataAddr);
    freeBlocList=free_3is(dataAddr);

    printf("\n/******************************/");
    printf("Deuxième allocation");
    printf("/******************************/\n");

    void *dataAddr2=malloc_3is(100);
    freeBlocList=free_3is(dataAddr2);

    printf("\n/******************************/");
    printf("Troisième allocation");
    printf("/******************************/\n");

    void *dataAddr3=malloc_3is(200);
    freeBlocList=free_3is(dataAddr3);

    printf("\n/******************************/");
    printf("Quatrième allocation");
    printf("/******************************/\n");

    void *dataAddr4=malloc_3is(100);
    freeBlocList=free_3is(dataAddr4);

    printf("\n/******************************/");
    printf("Afficher la liste Chaînée");
    printf("/******************************/\n");

    //Afficher la liste chaînée
	while(freeBlocList!=NULL){
		printf(" %ld ",freeBlocList->bloc_size);
		freeBlocList= freeBlocList-> ptr_next;
	}
	printf("NULL\n");

    return 0;
}
