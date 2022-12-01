class Cache
{
   private:
      Set * sets;                      //The cache is an array of sets
      int64_t numSets;                 //number of sets
      int64_t associativity;           //associativity (way)
      int64_t blockOffsetBits;         //number of bits in the block offset
      int64_t setIndexBits;            //number of bits in the set index
      bool verbose;                    //whether verbose output should be produced
      int64_t getBits(int64_t start, int64_t end, uint64_t source);  
   public:
      Cache(int64_t associativity, int64_t blockOffsetBits, 
            int64_t setIndexBits, bool verbose);
      ~Cache();
      bool isHit(int64_t address);
      bool isEvict(int64_t address);
      void update(int64_t address);
      void print();
};
