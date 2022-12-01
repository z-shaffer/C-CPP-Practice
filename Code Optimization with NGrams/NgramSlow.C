
#include <iostream>
#include <string>
#include <vector>
#include "WordList.h"
#include "NgramList.h"
#include "NgramSlow.h"

/*
 * Ngrams
 *
 * Takes as input the size of the ngrams to be built and the list  
 * of words to build the ngrams from and builds a linked list of 
 * ngrams.
 *
 * param: int ngramSz - size of the ngram
 * param: const WordList & wl - list of the words use
 */
Ngrams::Ngrams(int ngramSz, WordList & wl)
{
   this->ngramSz = ngramSz;
   first = NULL;

   wl.beginIter();
   while (!wl.endIter())
   {
      std::string ngram = wl.getNextNgram(ngramSz);
      wl.incrementIter();
      //insert the ngram in the linked list
      if (!ngram.empty()) insertNgram(ngram);
   }
}

/*
 * Ngrams destructor
 *
 * Automatically called when Ngrams object goes out of scope
 * to delete the linked list.
 */
Ngrams::~Ngrams()
{
   /* Warning: this destructor contains a memory error. */
   Ngram_t * nextNgram;
   while (first != NULL)
   {
      nextNgram = first->next;
      free(first);
      first = nextNgram;
   }
}

/*
 * insertNgram
 *
 * looks for the ngram to be inserted. If it is already in
 * the linked list, it increments the count. If not, it 
 * inserts it into the linked list.
 *
 * param: std::string s - ngram to be inserted
 * return: none
 */
void Ngrams::insertNgram(std::string s)
{

   /* Warning: this code contains a memory leak. */
   Ngram_t * ptr = first;
   Ngram_t * newNode = new Ngram_t();
   newNode->ngram = s;
   newNode->count = 1;

   while (ptr != NULL)
   {
      //s already in list
      if (ptr->ngram == s) 
      {
         ptr->count++;
         return;
      }
      ptr = ptr->next;
   }
   //not in list
   //insert in front of list
   newNode = new Ngram_t();
   newNode->ngram = s;
   newNode->count = 1;
   newNode->next = first;
   first = newNode;
}


/*
 * sortByCount
 *
 * performs a bubble sort on the linked list of ngrams, sorting the
 * nodes in the list by the count in increasing or decreasing
 * order of count depending upon the value of reverse
 *
 * param: reverse - true if sort should be decreasing
 * return: none (modfied private linked list)
 */
void Ngrams::sortByCount(bool reverse)
{
   Ngram_t * ptr = first;
   Ngram_t * ptr1;
   Ngram_t * ptr2;
   int tcount;
   bool switchNodes;
   std::string tngram;

   while (ptr != NULL)
   {
      ptr1 = first; 
      ptr2 = ptr1->next;
      while (ptr2 != NULL) 
      {
         switchNodes = (reverse == true && ptr2->count > ptr1->count) ||
                       (reverse == false && ptr2->count < ptr1->count);
         if (switchNodes)
         {
            tcount = ptr1->count;
            tngram = ptr1->ngram;
            ptr1->count = ptr2->count;
            ptr1->ngram = ptr2->ngram; 
            ptr2->count = tcount;
            ptr2->ngram = tngram;
         }
         ptr1 = ptr2; 
         ptr2 = ptr2->next;
      }
      ptr = ptr->next;
   }
}

/*
 * addIncreasing
 *
 * adds to the list of ngrams in increasing order
 *
 */  
void Ngrams::addIncreasing(NgramList * incrLst)
{
   sortByCount(false);
   Ngrams::Ngram_t * ptr = first;
   while (ptr != NULL)
   {
      //add to the increasing list
      incrLst->addToNgramList(ptr->ngram, ptr->count);
      ptr = ptr->next;
   }
}

/*
 * addDecreasing
 *
 * adds to the list of ngrams in decreasing order
 *
 */  
void Ngrams::addDecreasing(NgramList * decrLst)
{
   sortByCount(true);
   Ngrams::Ngram_t * ptr = first;
   while (ptr != NULL)
   {  
      //add to the decreasing list
      decrLst->addToNgramList(ptr->ngram, ptr->count);
      ptr = ptr->next;
   }
}
