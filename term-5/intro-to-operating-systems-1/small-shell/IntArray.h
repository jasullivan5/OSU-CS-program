/******************************************************************************
 * FILE NAME:	IntArray.h
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 344
 * ASSIGNMENT:	Program 3 - smallsh
 * START DATE:	2/18/2018
 * DESCRIPTION: A dynamically allocated array of integer pointers. Includes functions
 * 		for creating a IntArray, adding data to the array which will
 * 		reallocate memory if the capacity is met, and for freeing memory
 *****************************************************************************/
#ifndef INT_ARRAY_H
#define INT_ARRYA_H

typedef struct IntArray {
	int* data;	// array of pointers to integers
	int size;	// number of integers in array
	int capacity;	// capacity of array
} IntArray;

IntArray* newIntArray(int capacity);
void freeIntArray(IntArray* intArray);
void addIntArray(IntArray* intArray, int value);

#endif
