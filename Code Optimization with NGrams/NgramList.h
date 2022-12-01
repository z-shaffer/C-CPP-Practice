#include <vector>
#include <string>
typedef struct
{
   std::string ngram;
   int count;
} ngramT;
class NgramList
{
   private:
      std::vector<ngramT> ngrmLst;
   public:
      NgramList();
      void addToNgramList(std::string str, int count);
      bool isIncreasing();
      bool isDecreasing();
      bool compare(NgramList * correctSol);
      void print();
};


