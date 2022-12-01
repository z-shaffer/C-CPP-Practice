#include <iostream>
#include <string>
#include "WordList.h"
#include "NgramList.h"
#include "NgramFast.h"

/* You may NOT declare any global variables. */

/* You may add data members to the NgramFast class by adding them to NgramFast.h */
/* You may add more methods to the NgramFast class.  Put prototypes in NgramFast.h */
/* You may add more classes.  If you do, they need to be cleanly
 * separated into a .h and a .C files. You would add an include
 * for the .h at the top.  You would need to change the makefile
 * so that your new classes are compiled and linked with the
 * executable. */

/*
 * Ngrams
 *
 * Takes as input the size of the ngrams to be built and the list  
 * of words to build the ngrams from and builds a collection
 * of ngrams. You may not use the C++ STL to define a collection object.
 * You need to implement your own. 
 *
 * param: int ngramSz - size of the ngram
 * param: const WordList & wl - list of the words use
 */
Ngrams::Ngrams(int ngramSz, WordList & wl)
{
   //Add any code you need to initialize whatever structure
   //you use to hold your ngrams.
   //intializes hashmap
   for (int i = 0; i < SIZE; i++)
   {
	hashMap[i] = NULL;	
   }
   //This code doesn't change
   this->ngramSz = ngramSz;
   wl.beginIter();
   while (!wl.endIter())
   {
      std::string ngram = wl.getNextNgram(ngramSz);
      wl.incrementIter();
      //Insert the ngram in the collection object
      if (!ngram.empty()) insertNgram(ngram);
   }
// Call method to generate ngram nodes and organize them into an array of linked lists
  sort();
}


/*
 * Ngrams destructor
 *
 * Automatically called when Ngrams object goes out of scope.
 * Delete any space dynamically allocated to hold the ngrams.
 */
Ngrams::~Ngrams()
{
   //pointer to current and next.
   Ngram * t_curr;
   Ngram * t_next;
   Ngram * s_curr;
   Ngram * s_next;

   //loop throught the structure
   for (int i = 0; i < SIZE; i++)
   {
	t_curr = hashMap[i];
	//until the end of the hashmap
    while (t_curr != NULL)
	{
		//adjusts pointer 
        t_next = t_curr->next;
		//deletes node
        delete (t_curr);
		//sets current to what next points at
        t_curr = t_next;
	}
   	hashMap[i] = NULL;
   }
	//repeat for sorted array	
   for (int i = maxCount; i > 0; i--)
   {
	// head of linked list
	s_curr = sorted[i];
	// traverse the linked list
	while (s_curr != NULL)
	{
	    s_next = s_curr->next;
	    delete (s_curr);
	    s_curr = s_next;
	}
	//clear the index
	sorted[i] = NULL;
   }  
}


/*
 * insertNgram
 *
 * Inserts ngram into whatever structure you choose to hold
 * your ngrams. If the ngram is already in the collection, the
 * insert simply increments the count.
 *
 * param: std::string s - ngram to be inserted
 * return: none
 */
void Ngrams::insertNgram(std::string s)
{
	//intializes ptr, a new ngram node, str, and count
	int key = hash(s);
	Ngram_t * curr = hashMap[key];
    //check if space is free
	while (curr != NULL) {
		// if the ngram exists, increase its count and set new maxcount if it exceeds old maxcount
		if (curr->str == s)
		{
			curr->count++;
			if (curr->count > maxCount) maxCount = curr->count;
			return;
		}
		// else, continue to traverse the linked list and search for this ngram
		else
		{
			curr = curr->next;
		}
	}
	// Once free space is found, create new ngram and insert it
	Ngram_t * newNgram = new Ngram_t();
	newNgram->str = s;
	newNgram->count = 1;	 
	newNgram->next = hashMap[key];
	hashMap[key] = newNgram; 
}

void Ngrams::sort()
{
	//initialize sorted array
	for (int j = 1; j <= maxCount; j++)
	{
		sorted[j] = NULL;
	}
	Ngram_t * ptr = NULL;
	// traverse the hash map
	for (int i = 0; i < SIZE; i++)
	{
		ptr = hashMap[i];	
		while (ptr != NULL)
		{
			// ngra found, create a node for it
			Ngram_t * sortedNgram = new Ngram_t;
			sortedNgram->count = ptr->count;
			sortedNgram->str = ptr->str;
			sortedNgram->next = NULL;
			// if there is already a node with its count in sorted, link them together
			if (sorted[ptr->count] != NULL)
			{
				sortedNgram->next = sorted[ptr->count];
			}
			// insert the ngram node into the sorted array
			sorted[ptr->count] = sortedNgram;
			// check for any other nodes with the same hash
			ptr = ptr->next;
		}
	}
}
int Ngrams::hash(std::string &sarray)
{
	//random hash value
    int x = 2917;
	int hash = 0;
    //hash function
	for (int i = 0; i < sarray.length(); i++) {
		hash = ((hash * x) + (int)sarray[i]) % SIZE;
	}
	return hash;	
}

/*bool Ngrams::find(std::string s)
{
	int i = hash(s);
	//loop through the hashmap until you find a match or reach the end of the map
    for (Ngram_t * curr = hashMap[i]; curr != NULL; curr = curr->next)
	{
		if (curr->str.compare(s) == 0)
		{
			return true;
		} 
	}
	return false;
} */

/*
 * addIncreasing
 * 
 * Adds the ngrams to the ngram list in increasing order of ngram count.
 *
 */
void Ngrams::addIncreasing(NgramList * incrLst)
{
 	Ngram_t * ptr = NULL;
	//traverse through the sorted array
	for (int i = 1; i <= maxCount; i++)
	{
		ptr = sorted[i];
		//traverse through the linked list in each index
		while (ptr != NULL)
		{
			incrLst->addToNgramList(ptr->str, ptr->count);
			ptr = ptr->next;
		}
	}
      //The call to add it to the list looks something like this:
       //incrLst->addToNgramList(ngramStr, ngramCnt);
       //You will probably have this call inside of a loop (or two)
}

/*
 * addDecreasing
 *
 * Adds the ngrams to the ngram list in decreasing order of ngram count.
 *
 */
void Ngrams::addDecreasing(NgramList * decrLst)
{
	Ngram_t * ptr = NULL;
	//traverse through sorted in reverse
	for (int i = maxCount; i > 0; i--)
	{
		ptr = sorted[i];
		//traverse through the ;inked list in each index
		while (ptr != NULL)
		{
			decrLst->addToNgramList(ptr->str, ptr->count);
			ptr = ptr->next;
		}
	}
      //The call to add it to the list looks something like this:
      //decrLst->addToNgramList(ngramStr, ngramCnt);
      //You will probably have this call inside of a loop (or two)
}
