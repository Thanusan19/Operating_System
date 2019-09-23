/*
 * main.c
 * 
 * Copyright 2019 ensea <ensea@StudentLab>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

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
    
    //Création d'un processus fils pour permettre de voir l'exécution du processsus père dans la mémoire
    int pid, status;
    pid=fork();
    
    if(pid!=0){ /*Code du père*/
		printf("Je suis le père %d\n",getpid());
		wait(&status);
	}else{ /*Code du fils*/
		printf("je suis le fils %d\n",getpid());
		int ppid=getppid();
		char buffer [50];

		sprintf(buffer, "%d", ppid);
		execlp("pmap", "pmap","-X",buffer,NULL);
		printf("ceci ne sera jamais affiché");
    }

    fflush(stdout); //S'il y a des problèmes d'affichage
    sleep(1000);



    //printf("Hello, World!\n");
    return 0;
}

