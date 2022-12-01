
class Ngrams 
{
   struct Ngram
   {
      std::string ngram;
      int count;
      struct Ngram * next;
   };

   typedef struct Ngram Ngram_t;

   private:
      Ngram_t * first;
      int ngramSz;
      void insertNgram(std::string s);
   public:
      Ngrams(int ngramSz, WordList & wl);
      ~Ngrams();
      void sortByCount(bool reverse);
      void addIncreasing(NgramList *);
      void addDecreasing(NgramList *);
};

