#include <stdio.h>
#include <stdlib.h>
#include "SDR.h"
#include "Memory.h"
#include "Encode.h"

int main() 
{
    
    SDR mySDR = Encode_Term(1);
    SDR_PrintWhereTrue(&mySDR);
    //not ready yet:
    SDR sdr2 = SDR_Permute(&mySDR, true);
    SDR_PrintWhereTrue(&sdr2);
    


    // memory
    Memory2 *memory;
    memory = static_cast<Memory2*>(malloc(sizeof(Memory2)));
    if (!memory) {
        printf("Out of memory!\n");
        return 1;
    }

    Memory_reset(memory);

    
    // first test for concept
    SDR conceptAName = Encode_Term(2);
    Concept *conceptA = Memory_allocateConcept(memory);
    Concept_RESET(conceptA, conceptAName);
    Memory_addConcept(memory, conceptA);

    printf("conceptA.ptr=%d\n", conceptA);

    Concept *closest = Memory_getClosestConceptByName(memory, &conceptAName);

    printf("closest.ptr=%d\n", closest);
    


    /*
    //numeric encoder test
    int w = 40;
    SDR sdrForNumber = Encode_Scalar(w, 0, 64, 30);

    printf("SDR for number 30:\n");
    SDR_PrintWhereTrue(&sdrForNumber);
    */

    free(memory);
    memory = NULL;


    return 0;
}

