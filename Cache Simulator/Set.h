

class Set
{
   private:
      Line * firstLine;           //A set is linked list of lines
      int64_t index;              //index of this set (for printing purposes)
      int64_t associativity;      //Number of lines per set (the way)
      bool verbose;
   public:
      void initialize(int64_t index, int64_t associativity, bool verbose);
      ~Set();
      bool isHit(int64_t tag);
      bool isFull();
      void updateSet(int64_t tag);
      void insertInFront(int64_t tag);
      int64_t getLRUTag();
      void removeLine(int64_t tag);
      void print();
};
