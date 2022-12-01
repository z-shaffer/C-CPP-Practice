#include <string>
#include <vector>

class WordList 
{
   private:
      std::vector<std::string> wl;
      std::vector<std::string>::iterator lstIter;
   public:
      WordList();
      void addWord(std::string s);
      void beginIter();
      bool endIter();
      void incrementIter();
      std::string getWord();
      std::string getNextNgram(int ngramSz);
};

std::ostream& operator<<(std::ostream& os, WordList & wl);

