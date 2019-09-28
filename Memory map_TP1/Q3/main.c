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


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

	/*Element de la liste --> Duplicable autant de fois que l'on veut*/
	typedef struct Element Element; //Au lieu d'écrire "Struct Element" on pourra écrire "Element" 
	struct Element {
		int nombre;
		Element *suivant;// Type Element: pointeur diré vers un élément de type structure
	};
	
	
	/*Structure de contrôle --> un seul exemplaire*/
	typedef struct Liste Liste;
	struct Liste{
		Element *premier;
	};

/*Fonction d'initialisation de la structure*/
Liste *initialisation(int initNumber){ // Sortie : un pointeur sur une structure de type "Liste"
	
	/*Allocation de la méoire pour les deux structure*/
	Liste *liste=malloc(sizeof(*liste));
	Element *element=malloc(sizeof(*element));
	
	if (liste==NULL || element==NULL){ //SI Pointeur NULL --> Allocation de mémoire a échoué
		exit(EXIT_FAILURE);
	}
	/*Initialisation des éléments de la structure*/
	liste->premier=element; // Pour accéder à l'élement d'un pointeur il faut "(*liste).premier" <==> "liste->premier" //"element" est un pointeur sur le premier élément de la structure
	element->nombre=initNumber;
	element->suivant=NULL;
	
	return liste;
}

/*Insérer un élément au début de la liste chaînée*/
void insertion(Liste *liste, int nvNombre)
{
    /* Création du nouvel élément */
    Element *nouveau = malloc(sizeof(*nouveau));
    if (liste == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }
    nouveau->nombre=nvNombre;
    
    /*Insertion dans la liste ("structure") d'un élément au début de la liste Chaînée*/
    nouveau->suivant=liste->premier;
    liste->premier=nouveau;
}

/*Supprimer le premier élement de liste chaînée*/
void suppression(Liste *liste){
	
	if(liste==NULL){
		exit(EXIT_FAILURE);
	}
	
	if(liste->premier!=NULL){
		//Changer le pointeur du premier élement sur l'adresse de l'élément suivant
		//"liste->premier" donne l'aderesse de la première structure (donc un pointeur)
		//"suivant" s'applique sur un (*pointeur) de la strucutre Element.
		liste->premier=liste->premier->suivant;
		
		//Libérer l'espace occuper par la structure supprimer
		Element *aSupprimer=liste->premier;
		free(aSupprimer);
	}
}

/*Afficher la liste Chaînée*/
void afficherListe(Liste *liste){
	if(liste==NULL){
		exit(EXIT_FAILURE);
	}
	
	//Pointeur de type Element
	Element *actuel=liste->premier;
	
	//Tant que le pointeur suivant de la liste chaîné n'est pas "NULL" on continue à lire les valeurs
	while(actuel!=NULL){
		printf(" %d ",actuel->nombre);
		actuel= actuel-> suivant;
	}
	printf("NULL\n");
}




int main(int argc, char **argv)
{

	Liste *liste=initialisation(0);
	int n=1000000;
	for(int i=1;i<=n;i++){
		insertion(liste,i);
	}
	
	afficherListe(liste);
	return 0;
}




