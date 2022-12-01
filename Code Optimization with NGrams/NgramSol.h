class NgramSol 
{
 private:
      int ngramSz;
      const int HASHSIZE = 200000;
      struct NgramNode
      {
         std::string ngram;
         int count;
         NgramNode* next;
      };
      typedef struct NgramNode NgramNode_t;

      NgramNode_t ** ngramTable;
      NgramNode_t ** sortedNgramTable;
      int maxCount;
      void sortNgrams();
      void insertNgram(std::string s);
   public:
      NgramSol(int ngramSz, WordList & wl);
      ~NgramSol();
      void addIncreasing(NgramList *);
      void addDecreasing(NgramList *);
};

