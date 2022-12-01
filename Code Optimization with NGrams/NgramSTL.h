
//Lots of STL header files included here.
//You shouldn't have to include any others.
#include <iostream>
#include <set>
#include <string>
#include <array>
#include <iterator>
#include <list>
#include <tuple>
#include <utility>
#include <map>
#include <unordered_map>
#include <vector>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <algorithm>
#include <functional>
#define SIZE 40000

//You will need to add more method headers and data members
//to the class declaration below

class Ngrams 
{

   struct Ngram
   {
      int count;
      std::string str;
      struct Ngram * next;
   };

   typedef struct Ngram Ngram_t;

   private:
      int ngramSz;
      int maxCount = 1;
      Ngram_t * sorted[SIZE];
      std::unordered_map<std::string,int> ngrams;
      void insertNgram(std::string s); 

   public:
      Ngrams(int ngramSz, WordList & wl);
      ~Ngrams();
      void addIncreasing(NgramList * );
      void addDecreasing(NgramList * );
};

