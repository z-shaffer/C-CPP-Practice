#include "WordList.h"
#include "NgramList.h"
#include "NgramSTL.h"
using namespace std;
/* You may NOT include any additional header files or declare any global variables. */
/* You may add data members to the class by adding them to NgramSTL.h */
/* You may add more methods to the class. Add the prototype to NgramSTL.h */
/* You can also add functions (outside of any class) to this file, but only */
/* if you absolutely are required to by the STL. */

/*
 * Ngrams
 *
 * Takes as input the size of the ngrams to be built and the list  
 * of words to build the ngrams from and calls insertNgram to
 * insert the ngram into some collection object defined using a 
 * C++ template class. You get to choose the template class.
 * You'll need to add a declaration of the template class object
 * to NgramSTL.h.  You may need more than one.
 *
 * param: int ngramSz - size of the ngram
 * param: const WordList & wl - list of the words use
 */
Ngrams::Ngrams(int ngramSz, WordList & wl)
{
   //Add any code you need to initialize whatever structure
   //you use to hold your ngrams.
   //intialize the map
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
//You can make additional calls here if you need to.
	Ngram_t * newNgram = NULL;
	Ngram_t * ptr = NULL;
	//initialize the stored array
	for (int i = 1; i <= maxCount; i++) { sorted[i] = NULL; }
	//traverse through the ngrams
	for (auto it = ngrams.begin(); it != ngrams.end(); it++)
	{
		//create a ngram node for each ngram
		newNgram = new Ngram_t();
		newNgram->str = it->first;
		newNgram->count = it->second;
		// if there is no node with the same count, directly insert it into sorted
		if (sorted[newNgram->count] == NULL)
		{
			sorted[newNgram->count] = newNgram;
		}
		// else link the head of the list to this node and set the head of the list to this node
		else
		{
			ptr = sorted[newNgram->count];
			newNgram->next = ptr;
			sorted[newNgram->count] = newNgram;
		}
	}	
}



/*
 * Ngrams destructor
 *
 * Automatically called when Ngrams object goes out of scope.
 * Delete any space dynamically allocated to hold the ngrams.
 * If you didn't dynamically allocate any date then this 
 * method stays empty.
 */
Ngrams::~Ngrams()
{
    Ngram_t * s_curr = NULL;
    Ngram_t * s_next = NULL;
    // clear the ngram list
    ngrams.clear();
    // clear the sorted array
    for (int i = maxCount; i > 0; i--)
    {
	s_curr = sorted[i];
	while (s_curr != NULL)
	{
	    s_next = s_curr->next;
	    delete (s_curr);
	    s_curr = s_next;
	}
	sorted[i] = NULL;
    }  
}

/*
 * insertNgram
 *
 * Inserts the ngram into whatever collection object you choose to hold
 * your ngrams. If the ngram is already in the collection, it should
 * simply increment the count.
 *
 * param: std::string s - ngram to be inserted
 * return: none
 */
void Ngrams::insertNgram(std::string s)
{
  	// iterate through the ngrams list
	auto it = ngrams.find(s);
	// if the ngram does not exist (reaches end of ngrams) then insert a new ngram with a count of 1
	if (it == ngrams.end())
	{
		ngrams.insert({s,1});
		return;
	}
	// if the ngram does exist, just increase the count
	it->second++;
	// check if there's a new maxcount
	if (ngrams[s] > maxCount) { maxCount = ngrams[s]; }
}


/*
 * addIncreasing
 *
 * Adds the ngrams to the ngram list in increasing order of ngram count.
 *
 */
void Ngrams::addIncreasing(NgramList * incrLst)
{
      //The call to add it to the list looks something like this:
      //incrLst->addToNgramList(ngramStr, ngramCnt);
      //Yu will probably have this call inside of a loop
      //creates a sorted map
      
	Ngram_t * ptr = NULL;
	//traverse the sorted array from front to back
	for (int i = 1; i <= maxCount; i++)
	{
		ptr = sorted[i];
		// traverse each linked list in the array
		while (ptr != NULL)
		{
			incrLst->addToNgramList(ptr->str, ptr->count);
			ptr = ptr->next;
		}
	}
}
/*
 * addDecreasing
 *
 * Adds the ngrams to the ngram list in decreasing order of ngram count.
 *
 */
void Ngrams::addDecreasing(NgramList * decrLst)
{
      //The call to add it to the list looks something like this:
      //decrLst->addToNgramList(ngramStr, ngramCnt);
      //You will probably have this call inside of a loop
	//loop to add to the ngram list in decreasing order
	Ngram_t * ptr = NULL ;
	// traverse the sorted array from back to front
	for (int i = maxCount; i > 0; i--)
	{
		ptr = sorted[i];
		// traverse each linked list in the array
		while (ptr != NULL)
		{
			decrLst->addToNgramList(ptr->str, ptr->count);
			ptr = ptr->next;
		}
	}
}
