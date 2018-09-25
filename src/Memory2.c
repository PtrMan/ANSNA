// Memory2.c

// TODO< static array checks and dynamic memory allocation for LookupEntry and Bucket >

#include "SDR.h"


typedef struct {
	Concept *conceptsStatic[30];
	int conceptsCount;
} LookupEntry;

typedef struct {
	int count; // count of elements
	Concept *arr[128];
} Bucket;

typedef struct {
	LookupEntry entries[SDR_SIZE][SDR_SIZE];

	Bucket buckets[SDR_SIZE];
} Memory2;


void memory__bucket_clear(Memory2 *mem) {
	for (int i=0;i<SDR_SIZE;i++) {
		mem->buckets[i].count = 0;

		// actually not necessary but just to make sure that we don't hit a stupid bug
		memset(mem->buckets[i].arr, 0, sizeof(void)*128);
	}
}

void memory__bucket_addConcept(Bucket *bucket, Concept *addedConcept) {
	// insert sort

	int len = bucket->count;

	int min = 0;
	int max = len;

	int idx;

	for(;;) {
		if (min == max) {
			idx = min;
			break;
		}

		int middle = min + (max - min) / 2;

		if ( (long)bucket->arr[middle] < (long)addedConcept ) {
			// we don't do anything because we handle it as a set
			return;
		}
		else if ( (long)bucket->arr[middle] < (long)addedConcept ) {
			max = middle;
		}
		else {
			min = middle;
		}
	}

	// shift
	memcpy(&(bucket->arr[idx+1]), &(bucket->arr[idx]), sizeof(void*) * (bucket->count - idx));

	// insert
	bucket->arr[idx] = addedConcept;

	bucket->count++;
}

// resets all informations in all concepts of the entry
void memory__resetRelatedConceptMetainfo(Memory2 *mem2, LookupEntry *entry, int iBitIdx, int jBitIdx)
{
	for (int i=0;i<entry->conceptsCount;i++)
	{
		Concept *iConcept = entry->conceptsStatic[i];
		iConcept->metaVoteCounter = 0;
	}
}

// contains all the logic for updating or replacing the potential matching list by a entry
void memory__addToPotentials(Memory2 *mem2, LookupEntry *entry, int iBitIdx, int jBitIdx)
{
	for (int i=0;i<entry->conceptsCount;i++)
	{
		Concept *iConcept = entry->conceptsStatic[i];
		// add two because we have two active bits
		iConcept->metaVoteCounter += 2;
	}
}

void memory__storePotentialsByVoteCounter(Memory2 *mem2, LookupEntry *entry, int iBitIdx, int jBitIdx) {
	for (int i=0;i<entry->conceptsCount;i++)
	{
		Concept *iConcept = entry->conceptsStatic[i];
		int metaVoteCounter = iConcept->metaVoteCounter;

		memory__bucket_addConcept(mem2->buckets[metaVoteCounter], iConcept);
	}
}

// adds all possible Concept entries to the potential matching list for a SDR
void memory__addPotentialsOfSdrInternal(Memory2 *mem2, SDR *sdr, void (*fnPtr)(Memory2*,LookupEntry*,int,int))
{
	// used to keep track of read bits with a SDR
	SDR alreadyUsedBits;
	// TODO< reset alreadyUsedBits >
	TODO TODO TODO

	for(int iBitIdx=0; iBitIdx<SDR_SIZE; iBitIdx++)
	{
		if (!SDR_ReadBit(sdr, iBitIdx))
		{
			continue;
		}

		// keep track of already used positions
		//optimized away because this will never be used  SDR_WriteBit(&alreadyUsedBits, iBitIdx, 1);

		for(int jBitIdx=iBitIdx+1; jBitIdx<SDR_SIZE; jBitIdx++)
		{
			// we need to skip already used bits
			if (SDR_ReadBit(&alreadyUsedBits, jBitIdx))
			{
				continue;
			}

			// we skip here because we don't need to consider non-bit pairs
			if (!SDR_ReadBit(sdr, jBitIdx))
			{
				continue;
			}

			// keep track of already used positions
			SDR_WriteBit(&alreadyUsedBits, jBitIdx, 1);

			// else we add the potential entries to the table of potentials

			fnPtr(mem2, &(mem2->entries[iBitIdx][jBitIdx]), iBitIdx, jBitIdx);
		}
	}
}

Concept *memory__queryBestFitConceptBySdr(Memory2 *mem2, SDR *sdr) {
	// we need to clear all buckets which we are using for fast "sorting"
	memory__bucket_clear(mem2);


	memory__addPotentialsOfSdrInternal(mem2, sdr, &memory__resetRelatedConceptMetainfo);
	memory__addPotentialsOfSdrInternal(mem2, sdr, &memory__addToPotentials);

	// store all potential concepts by vote-counter
	memory__addPotentialsOfSdrInternal(mem2, sdr, &memory__storePotentialsByVoteCounter);

	// iterate by bucket and search best vote just for best bucket
	{
		// TODO TODO TODO TODO
		TODO
	}
}
