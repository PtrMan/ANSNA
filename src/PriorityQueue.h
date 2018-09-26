#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

///////////////////////////
//  ANSNA priority queue //
///////////////////////////
//The priority queue for concepts and tasks
//Idea from https://stackoverflow.com/a/2935995

//References//
//-----------//
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//Defines//
//--------------//

#define PRIORITYQUEUE_ITEMS 1024

typedef struct
{
    bool added;
    bool evicted;
} PriorityQueue_Push_Feedback;

template<typename Type, int n>
struct PriorityQueue {
	typedef struct 
	{
		Type value;
	    double priority;
	} Item;

	int amount;
    Item items[n];



	//Push element into the queue. Note that while data struct surely starts with Prioritized (so has priority), it contains the entire data of the item
	//so might be a Task or Concept, so make sure to pass sizeof(Task) or sizeof(Concept) as datasize
	//returns if the object as rejected
	PriorityQueue_Push_Feedback push(Item& item, Item *evicted_item)
	{
	    PriorityQueue_Push_Feedback feedback = {0};
	    int i = amount + 1;
	    int j = i / 2;

	    //"move the item "up" (exchanging child with parent) till element to insert priority is smaller than the node
	    while(i > 1 && items[j].priority <= item.priority)
	    {
	        if(i < n) //we can't put the parent there if it's outside of the array
	        {
	            //items[i] = items[j];
	            memcpy(&(items[i]), &(items[j]), sizeof(Item));
	        } 
	        else //item_j got evicted as there was no space(i) move it to, it gets replaced with item_j/2 now
	        {
	                //evicted_item = items[j];
	                feedback.evicted = true;
	                memcpy(evicted_item, &(items[j]), sizeof(Item));
	        }
	        i = j;
	        j = j / 2; //parent
	    }
	    //now put element at the "free" parent, which was already copied to the child (which itself was copied to its child..):
	    if(i < n) //we can't put the element there if it's outside of the array
	    {
	        //queue->items[i] = item;
	        feedback.added = true;
	        memcpy(&(items[i]), &item, sizeof(Item));
	    }
	    amount++;

	    return feedback;
	}

/* commented because we don't use pop
void PriorityQueue_Pop(PriorityQueue *queue, Prioritized *returnedItem, int itemsize)
{
    //No items, we can only return null
    if (!queue->items_amount)
    {
        return NULL;
    }
    //the highest priority item is the first in the array
    //returnedItem = queue->items[1];
    memcpy(returnedItem, &(queue->items[1]), itemsize);
    //queue->items[1] = queue->items[queue->items_amount];
    memcpy(&(queue->items[1]), &(queue->items[queue->items_amount]), itemsize);
    //if we remove it the amount gets reduced
    queue->items_amount--;
    //heapify what is left after the removal (decide new parent, propagate this handling iteratively down the tree)
    int current = 1, left, right, largest;
    while(current != queue->items_amount+1)
    {
        int left = 2*current;
        int right = left+1;
        largest = queue->items_amount+1;
        if (left <= queue->items_amount && queue->items[left].priority >= queue->items[largest].priority)
        {
            largest = left; //left is largest
        }
        if (right <= queue->items_amount && queue->items[right].priority >= queue->items[largest].priority)
        {
            largest = right; //move up right child
        }
        //queue->items[current] = queue->items[largest];
        memcpy(&(queue->items[current]), &(queue->items[largest]), itemsize);
        current = largest;
    }
}
 */
};


//Data structure//
//--------------//




// TODO< move into memory because it is a global structure >
//DefinePriorityQueue(PriorityQueue, PRIORITYQUEUE_ITEMS, Prioritized, items)



//Methods//
//-------//




#endif

