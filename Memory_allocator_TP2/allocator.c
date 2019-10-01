#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define MAGIC 0x0123456789ABCDEF

typedef struct HEADER_TAG {
    struct HEADER_TAG * ptr_next; /* pointe sur le prochain bloc libre -->8 octets */
    size_t bloc_size; /* taille du memory bloc en octets --> 8 octets car "long"*/
    long magic_number; /* 0x0123456789ABCDEFL --> 8 octets "long"*/
} HEADER;




/*Allocation d'un bloc mémoire*/
void *malloc_3is(int nbrByte,HEADER *freeBlocList){
    int nbrByteTotal=sizeof(HEADER)+nbrByte+sizeof(MAGIC);

    long freeBlocSize=0;
    while(freeBlocList!=NULL){
		freeBlocSize=freeBlocList->bloc_size +sizeof(HEADER)+sizeof(MAGIC);
        if(freeBlocSize==nbrByteTotal){
            printf("Bloc start Adress (Header+data+WD): %p \n",freeBlocList);

            return freeBlocList;
        }
		freeBlocList= freeBlocList-> ptr_next;
	}
    
    HEADER *newSpace=sbrk(nbrByteTotal);
    if(newSpace==(void*) -1){
        exit(EXIT_FAILURE);
    }

    newSpace->ptr_next=NULL;
    //Espace alloué pour l'utilisateur
    newSpace->bloc_size=nbrByte;
    newSpace->magic_number=MAGIC;
    printf("Bloc start Adress (Header+data+WD): %p \n",newSpace);

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

void *free_3is(HEADER *freeBlocList,void *dataAddr){
   
    if(freeBlocList==NULL){
        freeBlocList=dataAddr-sizeof(HEADER);
    }else
    {
        HEADER *newFreeBloc=dataAddr-sizeof(HEADER);
        newFreeBloc->ptr_next=freeBlocList;
        freeBlocList=newFreeBloc;
    }
    return freeBlocList;
    
}


int main(){

    printf("\n/******************************/");
    printf("Première allocation");
    printf("/******************************/\n");

    HEADER *freeBlocList=NULL;

    void *dataAddr=malloc_3is(10,freeBlocList);
    printf("Memory Bloc Adress: %p \n",dataAddr);
    check_mem(dataAddr);

    //freeBlocList->firstBloc=dataAddr-sizeof(HEADER);
    //freeBlocList->firstBloc->ptr_next=NULL;
    freeBlocList=free_3is(freeBlocList,dataAddr);

    //HEADER *newFreeBloc=dataAddr-sizeof(HEADER);
    //newFreeBloc->ptr_next=NULL;


    printf("\n/******************************/");
    printf("Deuxième allocation");
    printf("/******************************/\n");

    void *dataAddr2=malloc_3is(100,freeBlocList);
    freeBlocList=free_3is(freeBlocList,dataAddr2);


    printf("\n/******************************/");
    printf("Troisième allocation");
    printf("/******************************/\n");

    void *dataAddr3=malloc_3is(200,freeBlocList);
    freeBlocList=free_3is(freeBlocList,dataAddr3);


    printf("\n/******************************/");
    printf("Quatrième allocation");
    printf("/******************************/\n");

    void *dataAddr4=malloc_3is(400,freeBlocList);
    freeBlocList=free_3is(freeBlocList,dataAddr4);


    printf("\n/******************************/");
    printf("Afficher la liste Chaînée");
    printf("/******************************/\n");

    //Afficher la liste Chaînée
    //HEADER *newFreeBloc=freeBlocList->firstBloc;
	while(freeBlocList!=NULL){
		printf(" %ld ",freeBlocList->bloc_size);
		freeBlocList= freeBlocList-> ptr_next;
	}
	printf("NULL\n");
    //afficherListe(freeBlocList);

    return 0;
}