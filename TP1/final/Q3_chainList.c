#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

typedef struct ChainElement ChainElement;
struct ChainElement{
	int nbr;
	struct ChainElement* next;
};
	
typedef struct ChainList ChainList;
struct ChainList{
	ChainElement *first;
};

typedef struct DoubleChainList DoubleChainList;
struct DoubleChainList{
	ChainElement *first;
	ChainElement *last;
};

/*Initialise une liste chaînée*/
ChainList *initialization()
{
	ChainList *chainList = malloc(sizeof(*chainList));
	ChainElement *chainElement = malloc(sizeof(*chainElement));
	
	chainElement->nbr = 0;
	chainElement->next = NULL;
	chainList->first = chainElement;
	
	return chainList;
}

/*Insère un élément à la fin d'une liste chaînée*/
void insertEnd(ChainList *chainList, int nbr)
{
	//Parcours de la liste chaînée jusqu'à sa fin
	ChainElement *current = chainList->first;
	while(current->next != NULL){
		current = current->next;
	}
	
	//Création et ajout d'un élément
	ChainElement *new = malloc(sizeof(*new));
	new->nbr = nbr;
	new->next = NULL;
	
	current->next = new;
}

/*Crée une liste chaînée avec les n premiers entiers dans l'ordre croissant*/
ChainList* createChainList(int length)
{
	ChainList *chainList = initialization();
	for(int i=1;i<length;i++)
	{
		insertEnd(chainList, i);
	}
	
	return chainList;
}

/*Affiche la contenu d'une liste chaînée*/
void displayChainList(ChainList *chainList)
{
	ChainElement *current = chainList->first;
	
	while(current != NULL)
	{
		printf("%d -> ", current->nbr);
		current = current->next;
	}
	
	printf("NULL\n");
}

/*Calcule la longueur d'une liste chaînée*/
int listLength(ChainList *chainList)
{
	int length = 0;
	ChainElement *current = chainList->first;
	
	while(current != NULL)
	{
		length +=1;
		current = current->next;
	}
	
	return length;
}

/*Calcule la valeur moyenne des éléments contenus dans la liste chaînée*/
float averageList(ChainList *chainList)
{
	float average = 0.0;
	ChainElement *current = chainList->first;
	
	while(current != NULL)
	{
		average += current->nbr;
		current = current->next;
	}
	return average/listLength(chainList);
}

/*Crée une liste des carrés d'une autre liste chaînée*/
ChainList* squaredList(ChainList *chainList)
{
	ChainElement *current = chainList->first;
	
	//Initialisation de la liste des carrés
	ChainElement *squaredChainElement = malloc(sizeof(*squaredChainElement));
	ChainList *squaredChainList = malloc(sizeof(*squaredChainList));

	squaredChainElement->nbr = current->nbr*current->nbr;
	squaredChainElement->next = NULL;
	squaredChainList->first = squaredChainElement;
	
	//Premier décalage dans l'autre liste du à l'initialisation
	current = current->next;

	while(current != NULL)
	{
		insertEnd(squaredChainList, current->nbr*current->nbr);
		current = current->next;
	}
	
	return squaredChainList;
}

/*Retire le premier élément d'une liste chaînée*/
void deleteStart(ChainList *chainList)
{
	if(chainList->first != NULL)
	{
		ChainElement *toDelete = chainList->first;
		chainList->first = chainList->first->next;
		free(toDelete);
	}
}

/*Retire le dernier élément d'une liste chaînée*/
void deleteEnd(ChainList *chainList)
{
	ChainElement *current = chainList->first;
	//Condition d'arrêt à deux éléments avant la fin pour pouvoir récupérer les données
	while(current->next->next != NULL){
		current = current->next;
	}
	
	ChainElement *toDelete = current->next->next;
	current->next = NULL;
	free(toDelete);
}

/*Insère un élément au début d'une liste chaînée*/
void insertStart(ChainList *chainList, int nbr)
{
	ChainElement *new = malloc(sizeof(*new));
	new->nbr = nbr;
	
	new->next = chainList->first;
	chainList->first = new;
}

/*Concatène deux listes chaînées*/
ChainList* concatenateChainList(ChainList *firstChainList, ChainList *secondChainList)
{
	ChainElement *current = firstChainList->first;
	while(current->next != NULL){
		current = current->next;
	}
	
	//Laison entre le dernier élément de la première liste
	//et le premier élément de la deuxième
	current->next = secondChainList->first;
	
	return firstChainList;
} 

/*Initialise une liste doublement chaînée*/
DoubleChainList *initializationDouble()
{
	DoubleChainList *doubleChainList = malloc(sizeof(*doubleChainList));
	ChainElement *chainElement = malloc(sizeof(*chainElement));
	
	chainElement->nbr = 0;
	chainElement->next = NULL;
	doubleChainList->first = chainElement;
	doubleChainList->last = chainElement;
	
	return doubleChainList;
}

int main()
{
	ChainList *chainList = createChainList(10);
	printf("Voici une liste chaînée : ");
	displayChainList(chainList);
	
	int length = listLength(chainList);
	float average = averageList(chainList);
	printf("Longueur de la liste chaînée : %d\n",length);
	printf("Moyenne des valeurs dans la liste chaînée : %f\n", average);
	
	ChainList *squaredChainList = squaredList(chainList);
	printf("Voici la liste chaînée des carrés : ");
	displayChainList(squaredChainList);
	
	deleteStart(chainList);
	printf("Liste chaînée avec le premier élément retiré : ");
	displayChainList(chainList);
	
	insertStart(chainList,0);
	printf("Liste chaînée avec un élément ajouté au début : ");
	displayChainList(chainList);
	
	deleteEnd(chainList);
	printf("Liste chaînée avec le dernier élément retiré : ");
	displayChainList(chainList);
	
	ChainList *concatenatedChainList = concatenateChainList(chainList, squaredChainList);
	printf("Liste chaînée après concaténation : ");
	displayChainList(concatenatedChainList);
	
	return 0;
}
