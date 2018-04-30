/******************************************************************************
 * FILE NAME:	StringArray.c
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 344
 * ASSIGNMENT:	Program 3 - smallsh
 * START DATE:	2/18/2018
 * DESCRIPTION: A dynamically allocated array of char pointers. Includes functions
 * 		for creating a StringArray, adding data to the array which will
 * 		reallocate memory if the capacity is met, and for freeing memory
 *****************************************************************************/
#include "StringArray.h"
#include <stdlib.h>
#include <assert.h>

// Allocate memory and initialize array
StringArray* newStringArray(int capacity) {
	StringArray* stringArray = malloc(sizeof(struct StringArray));
	assert(stringArray != NULL);
	stringArray->data = malloc((capacity + 1) * sizeof(char*));
	assert(stringArray->data != NULL);
	stringArray->capacity = capacity;
	stringArray->size = 0;
	return stringArray;
}

// Free all memory used by array
void freeStringArray(StringArray* stringArray) {
	free(stringArray->data);
	free(stringArray);
}

// Add new string to array and resize array if necessary
void addStringArray(StringArray* stringArray, char* string) {
	if (stringArray->size >= stringArray->capacity) {
		stringArray->capacity = 2 * stringArray->capacity;
		stringArray->data = realloc(stringArray->data, (stringArray->capacity + 1) * sizeof(char*));
		assert(stringArray->data != NULL);
	}

	stringArray->data[stringArray->size] = string;
	stringArray->size++;
	stringArray->data[stringArray->size] = NULL;
}
