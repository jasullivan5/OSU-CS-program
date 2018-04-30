/******************************************************************************
 * FILE NAME:	StringArray.h
 * AUTHOR:	Joseph Sullivan
 * CLASS:	CS 344
 * ASSIGNMENT:	Program 3 - smallsh
 * START DATE:	2/18/2018
 * DESCRIPTION: A dynamically allocated array of char pointers. Includes functions
 * 		for creating a StringArray, adding data to the array which will
 * 		reallocate memory if the capacity is met, and for freeing memory
 *****************************************************************************/
#ifndef STRING_ARRAY_H
#define STRING_ARRAY_H

typedef struct StringArray {
	char** data;	// array of pointers to char pointers
	int size;	// number of strings in array
	int capacity;	// capacity of array
} StringArray;


StringArray* newStringArray(int capacity);
void freeStringArray(StringArray* stringArray);
void addStringArray(StringArray* stringArray, char* string);

#endif
