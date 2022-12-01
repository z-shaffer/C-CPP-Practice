

class Line
{
   public:
      int64_t tag;           //tag from the address
      Line * next;           //pointer to the next most recently used Line
      Line(int64_t tag);     //constructor
};
