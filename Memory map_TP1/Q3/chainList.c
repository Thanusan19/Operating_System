#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct ChainElement ChainElement;
struct ChainElement{
	int nbr;
	struct chainElement* next;
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

ChainList createChainList(int length)
{
	ChainList *chainList = initialization();
	for(int i=1;i<=length;i++)
	{
		insertEnd(chainList, i);
	}
}

/*int listLength(struct chainList){
	int length = 0;
	for(chainList* chainList = head; chainList->next!=NULL;chainList=chainList->next){
		length += length;
	}
	return length;
}*/

int main()
{
	ChainList chainList = createChainList(10);
}
