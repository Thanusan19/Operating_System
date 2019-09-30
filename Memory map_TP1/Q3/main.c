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
void insertionPremier(Liste *liste, int nvNombre)
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

/*Renvoie la Taille de la liste*/
int tailleStructure(Liste *liste){
	if(liste==NULL){
		exit(EXIT_FAILURE);
	}

	int nbrElement=0;
	Element *actuel=liste->premier;
	while (actuel!=NULL){
		nbrElement++;
		actuel=actuel->suivant;
	}
	
	return nbrElement;
}

/*Renvoie la moyenne de la liste Chaînée*/
float listeMoyenne(Liste *liste){
	if(liste==NULL){
		exit(EXIT_FAILURE);
	}

	float sum=0;
	float moyenne=0;
	int nbrElement=0;
	Element *actuel=liste->premier;
	while(actuel!=NULL){
		sum+=actuel->nombre;
		nbrElement++;
		actuel=actuel->suivant;
	}
	moyenne=sum/nbrElement;
	return moyenne;
}

/*Renvoie le carré de la liste Chaînée donnée en argument*/
Liste *listeAuCarre(Liste *liste){
	if(liste==NULL){
		exit(EXIT_FAILURE);
	}

	Element *actuel=liste->premier;
	while(actuel!=NULL){
		actuel->nombre=(actuel->nombre)*(actuel->nombre);
		actuel=actuel->suivant;
	}
	return liste;
}

/*Supprimer le premier élement de liste chaînée*/
void suppressionPremier(Liste *liste){
	
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

/*Supprimer le premier élement de liste chaînée*/
void suppressionDernier(Liste *liste){
	
	if(liste==NULL){
		exit(EXIT_FAILURE);
	}
	
	if(liste->premier!=NULL){
		Element *actuel=liste->premier;
		while (actuel!=NULL){
			if(actuel->suivant->suivant==NULL){
				actuel->suivant=NULL;
				//Libérer l'espace occuper par la structure supprimer
				//free(actuel->suivant->suivant);
			}
			actuel=actuel->suivant;
		}
	}
}

/*Ajouter un élément en dernier de la liste Chaînée*/
void ajoutDernier(Liste *liste,int nvNombre){
	/* Création du nouvel élément */
    Element *nouveau = malloc(sizeof(*nouveau));
    if (liste == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }
    
    /*Insertion dans la liste ("structure") d'un élément à la fin de la liste Chaînée*/
	nouveau->nombre=nvNombre;
    nouveau->suivant=NULL;
	Element *actuel=liste->premier;
    while (actuel!=NULL)
	{
		actuel=actuel->suivant;
	}
	actuel=nouveau;
	
}


int main()
{
	int n=10;
	//Question 1
	Liste *liste=initialisation(n);
	for(int i=1;i<=n;i++){
		insertionPremier(liste,n-i);
	}
	afficherListe(liste);
	printf("\n");

	//Question 2 --> Taille de la structure
	int nbrElement=tailleStructure(liste);
	printf("Taille de la structure: %d \n\n",nbrElement);

	//Question 3 --> moyenne de la liste
	float moyenne=0;
	moyenne=listeMoyenne(liste);
	printf("Moyenne des éléments de la liste chaînée: %f \n\n",moyenne);

	//Question 4 --> Mettre au carré les éléments de la liste
	Liste *liste2=initialisation(n);
	for(int i=1;i<=n;i++){
		insertionPremier(liste2,n-i);
	}
	Liste *listeCarre=listeAuCarre(liste2);
	afficherListe(listeCarre);
	printf("\n");

	//Question 5 --> Retirer le premier élément de la liste
	suppressionPremier(liste);
	afficherListe(liste);
	printf("\n");

	//Question 6 --> Retirer le dernier élément de la liste
	suppressionDernier(liste);
	afficherListe(liste);
	printf("\n");

	suppressionDernier(liste);
	afficherListe(liste);
	printf("\n");

	//Question 7 --> Ajouter un éléméent à la fin de la liste
	Liste *liste3=initialisation(n);
	for(int i=1;i<=n;i++){
		insertionPremier(liste3,n-i);
	}
	ajoutDernier(liste3,11);
	printf("Insertion d'un élément en dernier: \n");
	afficherListe(liste3);
	printf("\n");
	
	//Question 8 --> Ajouter un élément au début de la liste
	 //insertionPremier(liste,11);
	insertionPremier(liste,10);
	printf("Insertion d'un élément en premier: \n");
	printf("%d\n",liste->premier->nombre);
	afficherListe(liste);
	printf("\n");

	return 0;
}




