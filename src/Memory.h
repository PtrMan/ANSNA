#ifndef MEMORY_H
#define MEMORY_H

///////////////////
//  ANSNA Memory //
///////////////////
//The conce-based memory of ANSNA

//References//
//////////////
#include "Concept.h"
#include "PriorityQueue.h"

//Parameters//
//----------//
#define CONCEPTS_MAX 1000 // 10000
#define BUFFER_TASKS_MAX 64

//Data structure//
//--------------//
////DefinePriorityQueue(Memory, CONCEPTS_MAX, Concept, concepts)
////Memory memory;
//DefinePriorityQueue(AttentionBuffer, BUFFER_TASKS_MAX, Task, tasks)
//AttentionBuffer buffer; 
//TODO do we really need an additional one or can cyclingTasks, inputTasks and derivedTasks compete in the same queue?

typedef struct {
	SDR_HASH_TYPE bitToConcept[SDR_SIZE][CONCEPTS_MAX];
	int bitToConceptAmount[SDR_SIZE];

	int concepts_amount;
    Concept *concepts[CONCEPTS_MAX];


	///DefineMixinPriorityQueue(AttentionBuffer, BUFFER_TASKS_MAX, Task, attentionBuffer)


    //////////////////////
    // memory managment for concepts

	// free list as array for concepts
	Concept *conceptsFreeArray[CONCEPTS_MAX];
	int conceptsFreeArrayLength;

	// all preallocated concepts
	Concept preallocatedConcepts[CONCEPTS_MAX];
} Memory;

//Methods//
//-------//
//TODO do check which methods shouldn't be just PriorityQueue methods
//Init memory
void Memory_reset(Memory *memory);
//Add concept to memory
void Memory_addConcept(Memory *memory, Concept *concept);
//Return closest concept
Concept* Memory_getClosestConceptByName(Memory *memory, SDR *taskSDR);

// allocates a new concept
Concept* Memory_allocateConcept(Memory *memory);

#endif
