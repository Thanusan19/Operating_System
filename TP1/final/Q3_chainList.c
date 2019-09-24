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

ChainList *initialization()
{
	ChainList *chainList = malloc(sizeof(*chainList));
	ChainElement *chainElement = malloc(sizeof(*chainElement));
	
	chainElement->nbr = 0;
	chainElement->next = NULL;
	chainList->first = chainElement;
	
	return chainList;
}

void insertEnd(ChainList *chainList, int nbr)
{
	ChainElement *current = chainList->first;
	while(current->next != NULL){
		current = current->next;
	}
	
	ChainElement *new = malloc(sizeof(*new));
	new->nbr = nbr;
	new->next = NULL;
	
	current->next = new;
}

ChainList* createChainList(int length)
{
	ChainList *chainList = initialization();
	for(int i=1;i<length;i++)
	{
		insertEnd(chainList, i);
	}
	
	return chainList;
}

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

ChainList* squaredList(ChainList *chainList)
{
	ChainElement *current = chainList->first;
	ChainList *squaredChainList;
	
	while(current != NULL)
	{
		insertEnd(squaredChainList, current->nbr*current->nbr);
		current = current->next;
	}
	
	return squaredChainList;
}

void deleteStart(ChainList *chainList)
{
	if(chainList->first != NULL)
	{
		ChainElement *toDelete = chainList->first;
		chainList->first = chainList->first->next;
		free(toDelete);
	}
}

void deleteEnd(ChainList *chainList)
{
	if(chainList->first != NULL)
	{
		//ChainElement 
	}
}

int main()
{
	ChainList *chainList = createChainList(10);
	displayChainList(chainList);
	
	int length = listLength(chainList);
	float average = averageList(chainList);
	printf("Longueur de la liste chaînée : %d\n",length);
	printf("Moyenne des valeurs dans la liste chaînée : %f\n", average);
	
	ChainList *squaredChainList = squaredList(chainList);
	displayChainList(squaredChainList);
	
	deleteStart(chainList);
	displayChainList(chainList);
	
	return 0;
}
