#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int globale;
const int constante=12;

int main() {

    int *pointeur = NULL;
    pointeur = malloc(4);
    int nbrEntier = 0;
    char elemChar;
    double nbrDouble;


    printf("test");
    printf("Adresse int --> Stack :%p\n", &nbrEntier);
    printf("Adresse Char --> Stack :%p\n", &elemChar);
    printf("Adresse Double --> Stack:%p\n", &nbrDouble);
    printf("Adresse pointeur --> Heap: %p\n", pointeur);
    printf("Adresse Variable Globale --> BSS:%p\n", &globale);
    printf("Adresse Variable Constante --> Data : %p\n ",&constante);

    fflush(stdout);
    sleep(1000);



    //printf("Hello, World!\n");
    return 0;
}
