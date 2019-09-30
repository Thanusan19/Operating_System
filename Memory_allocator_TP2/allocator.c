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
            printf("Success WD end %#010lx \n",*wdEndAddr);
        }
    }else{
        exit(EXIT_FAILURE);
    }
}

void free_3is(freeBlocList *freeBlocList,void *dataAddr){
    //HEADER *header=dataAddr-sizeof(HEADER);
    HEADER *newFreeBloc=dataAddr-sizeof(HEADER);

    newFreeBloc->ptr_next=freeBlocList->firstBloc;
    freeBlocList->firstBloc=newFreeBloc;
}

int main(){

    void *dataAddr=malloc_3is(10);
    printf("Memory Bloc Adress: %p \n",dataAddr);
    check_mem(dataAddr);

    freeBlocList *freeBlocList=NULL;
    free_3is(freeBlocList,dataAddr);


    return 0;
}