/******************************************************************************
 * FILE NAME:	IntArray.c
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 344
 * ASSIGNMENT:	Program 3 - smallsh
 * START DATE:	2/18/2018
 * DESCRIPTION: A dynamically allocated array of integer pointers. Includes functions
 * 		for creating a IntArray, adding data to the array which will
 * 		reallocate memory if the capacity is met, and for freeing memory
 *****************************************************************************/
#include "IntArray.h"
#include <stdlib.h>
#include <assert.h>

// Allocate memory and initialize array
IntArray* newIntArray(int capacity) {
	IntArray* intArray = malloc(sizeof(struct IntArray));
	assert(intArray != NULL);
	intArray->data = malloc((capacity) * sizeof(int));
	assert(intArray->data != NULL);
	intArray->capacity = capacity;
	intArray->size = 0;
	return intArray;
}

// Free all memory used by array
void freeIntArray(IntArray* intArray) {
	free(intArray->data);
	free(intArray);
}

// Add new integer to array and resize array if necessary
void addIntArray(IntArray* intArray, int value) {
	if (intArray->size >= intArray->capacity) {
		intArray->capacity = 2 * intArray->capacity;
		intArray->data = realloc(intArray->data, intArray->capacity * sizeof(int));
		assert(intArray->data !=NULL);
	}
	
	intArray->data[intArray->size] = value;
	intArray->size++;
}
