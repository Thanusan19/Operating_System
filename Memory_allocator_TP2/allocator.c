#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define MAGIC 0x0123456789ABCDEF

typedef struct HEADER_TAG {
    struct HEADER_TAG * ptr_next; /* pointe sur le prochain bloc libre -->8 octets */
    size_t bloc_size; /* taille du memory bloc en octets --> 8 octets car "long"*/
    long magic_number; /* 0x0123456789ABCDEFL --> 8 octets "long"*/
} HEADER;

/*Structure définissant le début de la liste Chaînée*/
typedef struct freeBlocList{
    HEADER *firstBloc;
}freeBlocList;




/*Allocation d'un bloc mémoire*/
void *malloc_3is(int nbrByte){
    int nbrByteTotal=sizeof(HEADER)+nbrByte+sizeof(MAGIC);
    
    HEADER *newSpace=sbrk(nbrByteTotal);
    if(newSpace==(void*) -1){
        exit(EXIT_FAILURE);
    }

    newSpace->ptr_next=NULL;
    //Espace alloué pour l'utilisateur
    newSpace->bloc_size=nbrByte;
    newSpace->magic_number=MAGIC;
    printf("Total Bloc start Adress: %p \n",newSpace);

    void *dataAddr=newSpace;
    dataAddr=dataAddr + sizeof(HEADER);

    long *wdEndAddr=dataAddr+nbrByte;
    *wdEndAddr=MAGIC;
    printf("%#010lx \n",*wdEndAddr);
    return dataAddr;
}

void check_mem(void *dataAddr){
    HEADER *header=dataAddr-sizeof(HEADER);
    if(header->magic_number==MAGIC){
        printf("Success WD start: %#010lx \n",header->magic_number);
        long *wdEndAddr=dataAddr+ header->bloc_size;
        if(*wdEndAddr==MAGIC){
            printf("Success WD end: %#010lx \n",*wdEndAddr);
        }
    }else{
        exit(EXIT_FAILURE);
    }
}

void free_3is(freeBlocList *freeBlocList,void *dataAddr){
    /*//HEADER *header=dataAddr-sizeof(HEADER);
    HEADER *newFreeBloc=dataAddr-sizeof(HEADER);

    newFreeBloc->ptr_next=freeBlocList->firstBloc;
    freeBlocList->firstBloc=newFreeBloc;*/
    HEADER *newFreeBloc=dataAddr-sizeof(HEADER);
    newFreeBloc->ptr_next=freeBlocList->firstBloc;
    freeBlocList->firstBloc=newFreeBloc;
}

/*Afficher la liste Chaînée*/
void afficherListe(freeBlocList *liste){
	if(liste==NULL){
		exit(EXIT_FAILURE);
	}
	
	//Pointeur de type Element
    HEADER *newFreeBloc=liste->firstBloc;
	//Tant que le pointeur suivant de la liste chaîné n'est pas "NULL" on continue à lire les valeurs
	while(newFreeBloc!=NULL){
		printf(" %ld ",newFreeBloc->bloc_size);
		newFreeBloc= newFreeBloc-> ptr_next;
	}
	printf("NULL\n");
}

int main(){

    printf("\n/******************************/");
    printf("Première allocation");
    printf("/******************************/\n");

    void *dataAddr=malloc_3is(10);
    printf("Memory Bloc Adress: %p \n",dataAddr);
    check_mem(dataAddr);

    freeBlocList *freeBlocList=dataAddr-sizeof(HEADER);
    freeBlocList->firstBloc=dataAddr-sizeof(HEADER);

    HEADER *newFreeBloc=dataAddr-sizeof(HEADER);
    newFreeBloc->ptr_next=NULL;


    /*HEADER *newFreeBloc2=dataAddr2-sizeof(HEADER);
    newFreeBloc2->ptr_next=freeBlocList->firstBloc;
    freeBlocList->firstBloc=newFreeBloc2;*/
    //void *dataAddr2=malloc_3is(100);
    //free_3is(freeBlocList,dataAddr2);
    

    //afficherListe(freeBlocList);



    

    //free_3is(freeBlocList,dataAddr);

    printf("\n/******************************/");
    printf("Deuxième allocation");
    printf("/******************************/\n");

    void *dataAddr2=malloc_3is(100);
    free_3is(freeBlocList,dataAddr2);

    printf("\n/******************************/");
    printf("Deuxième allocation");
    printf("/******************************/\n");

    void *dataAddr3=malloc_3is(200);
    free_3is(freeBlocList,dataAddr3);

    printf("\n/******************************/");
    printf("Afficher la liste Chaînée");
    printf("/******************************/\n");

    //Afficher la liste Chaînée
	while(newFreeBloc!=NULL){
		printf(" %ld ",newFreeBloc->bloc_size);
		newFreeBloc= newFreeBloc-> ptr_next;
	}
	printf("NULL\n");

    /*void *dataAddr2=malloc_3is(100);
    printf("Memory Bloc Adress: %p \n",dataAddr2);
    check_mem(dataAddr2);
    free_3is(freeBlocList,dataAddr2);*/

    //afficherListe(freeBlocList);
    //Tant que le pointeur suivant de la liste chaîné n'est pas "NULL" on continue à lire les valeurs
    /*HEADER *actuel=dataAddr-sizeof(HEADER);
	while(actuel!=NULL){
		printf(" %ld ",actuel->magic_number);
		actuel= actuel-> ptr_next;
	}
	printf("NULL\n");*/

    return 0;
}