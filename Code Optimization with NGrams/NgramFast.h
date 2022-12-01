//You will need to add one or more typedefs and
//declarations for whatever structure you use to hold
//your ngrams. You will also need to add prototypes
//for any new methods you add to the class.

#define SIZE 40000

struct Ngram
{
      int count;
      std::string str;
      struct Ngram * next;
};
typedef struct Ngram Ngram_t;	

class Ngrams 
{
   private: 
      int ngramSz;
      int maxCount = 1;
      void insertNgram(std::string s);
      Ngram_t * hashMap[SIZE];
      Ngram_t * sorted[SIZE];
      int hash(std::string &s);
      bool find(std::string s);
   public:
      Ngrams(int ngramSz, WordList & wl);
      ~Ngrams();
      void addIncreasing(NgramList *);
      void addDecreasing(NgramList *);
      void sort();
};

