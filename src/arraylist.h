#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_
#pragma once


// ------------------ ARRAYLIST BEHAVIOR FLAGS ------------------
#define ARRAYLIST_SHRINK_MANUAL (1 << 0)
#define ARRAYLIST_SHRINK_AFTER_DELETE (1 << 1)




// ------------------ STRUCTS ------------------

/**
 * ArrayList structure.
 * size - number of elements in ArrayList.
 * maxSize - maximum number of elements in ArrayList.
 * chunkSize - the number of elements by which the table will expand when the maximum capacity is reached.
 * sizeOfType - the size (in bytes) of one data element. about shrinking behaviour.
 * flags - some ArrayList specific flags
 * data - array of integers, size = list->size
 */
typedef struct ArrayList_Int {
  unsigned int size;
  unsigned int maxSize;
  unsigned int chunkSize;
  unsigned int sizeOfType;
  unsigned int flags;
  int* data;
} ArrayList_Int;



typedef union DataTypes {
  int i;
  float f;
  char c;
} DataTypes;


typedef struct ArrayList {
  unsigned int size;
  unsigned int maxSize;
  unsigned int chunkSize;
  unsigned int sizeOfType;
  unsigned int flags;
  DataTypes** data;
} ArrayList;







// Different types of data

ArrayList* createList(unsigned int initialSize, unsigned int chunkSize, unsigned int sizeOfType, unsigned int flags);
int addCharToList(ArrayList* list, char item);
int addIntToList(ArrayList* list, int item);
int clearList(ArrayList** list);
int getIntFromArray(ArrayList* list, unsigned int index);
char getCharFromArray(ArrayList* list, unsigned int index);










// ------------------ PUBLIC FUNCTIONS ------------------



/*
 Create ArrayList of type INT
 Arguments:
 - initialSize: (unsigned int) Initial size of the array.
 - chunkSize (unsigned int) The number of elements by which the table will expand when the maximum capacity is reached.
 - sizeOfType (unsigned int) The size (in bytes) of each element in memory (eg. in windows x86 sizeof(int) = 4 bytes).
 - flags (unsigned int) The flags for maintaining ArrayList.
*/
ArrayList_Int* createArrayList(unsigned int initialSize, unsigned int chunkSize, unsigned int sizeOfType, unsigned int flags);




/*
 Add next value to the ArrayList.
 Arguments:
  - list: (ArrayList_Int) ArrayList.
  - value: (int) a value;
  return: 0 - FALSE, 1 - TRUE (success);
*/
int addIntToArrayList(ArrayList_Int* list, int value);




/*
 Prints information about the array: size (size), max size (maxSize) and chunk size (chunkSize).
 Arguments:
  - list: (ArrayList_Int) ArrayList.
*/
void printValues(ArrayList_Int* list);





/*
 Clearing ArrayList. Removing data.
 Arguments:
  - list: (ArrayList_Int) ArrayList.
  return: 0 - FALSE, 1 - TRUE (success);
*/
int clearArrayList(ArrayList_Int** list);




/*
 Get an item from the ArrayList in the specified index.
 Arguments:
  - list: (ArrayList_Int) ArrayList.
  - index: (unsigned int) an index;
  return: the element at index given.
*/
int getFromArrayList(ArrayList_Int* list, unsigned int index);



/*
 Remove the last item from the ArrayList and returns it.
 Arguments:
  - list: (ArrayList_Int) ArrayList.
  return: the last element from ArrayList.
*/
int popArrayList(ArrayList_Int* list);


/*
 Remove the first element from the ArrayList and returns it. This changes indexes of the ArrayList.
 Arguments:
  - list: (ArrayList_Int) ArrayList.
  return: the first element from ArrayList.
*/
int shiftArrayList(ArrayList_Int* list);


/*
 Remove the first item from the ArrayList and returns it. This changes indexes of the ArrayList after the removed element.
 Arguments:
  - list: (ArrayList_Int) ArrayList.
  - index: (unsigned int) index of the element to remove from ArrayList.
  return: 0 - FALSE, 1 - TRUE (success);
*/
int removeFromArrayList(ArrayList_Int* list, unsigned int index);



/*
 Dupmp from ArrayList.
 Arguments:
  - list: (ArrayList_Int) ArrayList.
*/
void dumpArrayList(ArrayList_Int* list);



/*
 Shrinking ArrayList (maxSize == size)
 Arguments:
  - list: (ArrayList_Int) ArrayList.
  return: 0 - FALSE, 1 - TRUE (success);
*/
int shrinkArrayList(ArrayList_Int* list);


/*
 Change shrinking behaviour flags.
 Arguments:
  - list: (ArrayList_Int) ArrayList.
  - flags (unsigned int) - flags eg. ARRAYLIST_SHRINK_MANUAL, ARRAYLIST_SHIRK_AFTER_DELETE...
  return: 0 - FALSE, 1 - TRUE (success);
*/
int changeFlags(ArrayList_Int* list, unsigned int flags);


#endif
