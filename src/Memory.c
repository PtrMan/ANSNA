#include <assert.h>

#include "Memory.h"

void Memory_reset(Memory2 *memory2)
{
    for (int i=0;i<SDR_SIZE;i++) {
        memory2->bitToConceptAmount[i] = 0;
    }


    // initialize stuff necessary for memory management

    memory2->conceptsFreeArrayLength = CONCEPTS_MAX;
    
    for (int i=0; i<CONCEPTS_MAX; i++)
    {
        memory2->conceptsFreeArray[i] = &(memory2->preallocatedConcepts[i]);
    }
}

void Memory_addConcept(Memory2 *memory2, Concept *concept)
{
    //try to add it, and if successful add to voting structure
    PriorityQueue<Concept*, CONCEPTS_MAX>::Item pushedItem;
    pushedItem.value = concept;
    // TODO< priority >

    PriorityQueue<Concept*, CONCEPTS_MAX>::Item evicted;
    PriorityQueue_Push_Feedback feedback = memory.push(pushedItem, &evicted);
    
    if(feedback.added)
    {
        for(int j=0; j<SDR_SIZE; j++)
        {
            if(SDR_ReadBit(&(concept->name), j))
            {
                int i = memory2->bitToConceptAmount[j]; //insert on top
                memory2->bitToConcept[j][i] = concept->name_hash;
                memory2->bitToConceptAmount[j]++;
             }
         }    
    }
    if(feedback.evicted)
    {
        //if a concept was evicted, delete from voting structure
        for(int j=0; j<SDR_SIZE; j++)
        {
            if(SDR_ReadBit(&(concept->name), j))
            {
                for(int i=0; i<memory2->bitToConceptAmount[j]; i++)
                {
                    if(memory2->bitToConcept[j][i] == evicted.value->name_hash)
                    {
                        memory2->bitToConcept[j][i] = 0;
                        //Now move the above ones down to remove the gap
                        for(int k=i; k<memory2->bitToConceptAmount[j]-1; k++)
                        {
                            memory2->bitToConcept[j][k] = memory2->bitToConcept[j][k+1];
                        }
                        //and decrement the counter
                        memory2->bitToConceptAmount[j]--;
                        break; //already deleted  
                    }
                }
             }
         }
    }
}

typedef struct
{
    SDR_HASH_TYPE concept;
    int count;
} Vote;

Concept* Memory_getClosestConceptByName(Memory2 *memory, SDR *taskSDR)
{
    SDR_HASH_TYPE taskhash = SDR_Hash(taskSDR);
    Vote voting[CONCEPTS_MAX];
    int votes = 0;
    Vote best = {0};

    for(int j=0; j<SDR_SIZE; j++)
    {
        if(SDR_ReadBit(taskSDR, j))
        {
            for(int i=0; i<memory->bitToConceptAmount[j]; i++)
            {
                int use_index = votes;
                bool existed = false;
                //check first if the SDR already got a vote
                //and if yes, increment that one instead creating a new one
                SDR_HASH_TYPE voted_concept = memory->bitToConcept[j][i];
                for(int h=0; h<votes; h++)
                {
                    if(voting[h].concept == voted_concept)
                    {
                        use_index = h;
                        existed = true;
                        break;
                    }
                }
                voting[use_index].concept = voted_concept;
                voting[use_index].count = existed ? 1 : voting[use_index].count+1;
                if(voting[use_index].count > best.count)
                {
                    best = voting[use_index];
                }
                if(!existed)
                {
                    votes++;
                }
            }
        }
    }
    if(votes == 0)
    {
        return 0;
    }
    //And now retrieve a concept with the same hash:
    for(int i=0; i<memory->concepts_amount; i++)
    {
            if(memory->concepts[i]->name_hash == best.concept)
            {
                //TODO make sure that each block is equal
                return memory->concepts[i];
            }
    }
    return NULL; //closestConceptByName;
}

Concept* Memory_allocateConcept(Memory2 *memory) {
    assert(memory->conceptsFreeArrayLength > 0);

    // take from freelist
    Concept *result = memory->conceptsFreeArray[memory->conceptsFreeArrayLength-1];
    memory->conceptsFreeArrayLength--;

    return result;
}
