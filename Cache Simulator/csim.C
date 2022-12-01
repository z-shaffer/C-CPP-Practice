#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <unistd.h>  
#include <assert.h>
#include "Line.h"
#include "Set.h"
#include "Cache.h"
#include "Simulate.h"

//TO DO
//Complete implementation of parseArgs

/* prototypes for functions in this file */
void printUsage();
void parseArgs(int argc, char * argv[], std::string & filename, 
               int64_t & associativity, int64_t & blockOffsetBits, 
               int64_t & setIndexBits, bool & verbose);

int32_t main(int32_t argc, char * argv[])
{
   std::string filename;
   int64_t blockOffsetBits;
   int64_t setIndexBits;
   int64_t associativity;
   bool verbose;
   std::fstream fs;

   //parse command line arguments
   parseArgs(argc, argv, filename, associativity, blockOffsetBits,
             setIndexBits, verbose);

   //make sure the file exists and can be opened
   fs.open(filename, std::fstream::in);
   if (!fs.is_open())
   {
      std::cout << "Cannot open " << filename << "\n";
      printUsage();
   }
   fs.close();

   //create a Simulate object, run the simulation, and print results
   Simulate * simP = new Simulate(associativity, blockOffsetBits, 
                                  setIndexBits, verbose);
   simP->run(filename);
   simP->printSummary();

   delete simP;

   return 0;
}

/*
 * parseArgs
 * Parses the command line arguments.  
 *
 * argv[0] - contains the name of the executable: "csim"
 * The other arguments can be in any order.  To run the simulator, 
 * the "-t", "-s", "-E", and "-b" arguments must be provided and
 * followed by the name of the trace file, the number of set index bits,
 * the associativity, and the number of bits for the block offset,
 * respectively.
 * If "-v" is provided then verbose will be set to true (false otherwise).
 * If "-h" is provided then the usage information is printed and
 * the program is exited. 
 * If the four required command line arguments are not provided,
 * the printUsage function is called.
 * You may choose to include more flags; for example, you might want
 * a "-vv" for very verbose output that you can use for debugging.
 * You would have to modify the code to also have a veryVerbose flag.
 *
 * argc - number of command line arguments
 * argv[] - array of command line arguments
 * filename - set to the name of the file containing the address trace
 *            (-t argument)
 * associativity - set to the cache associativity (-E argument)
 * blockOffsetBits - set to the number of bits for the block offset
 *                   (-b argument)
 * setIndexBits - set to the number of bits for the set index
 *                (-s argument)
 * verbose - set to true if -v provided and false, otherwise 
 *
*/
void parseArgs(int32_t argc, char * argv[], std::string & filename, 
               int64_t & associativity, int64_t & blockOffsetBits, 
               int64_t & setIndexBits, bool & verbose)
{
   char c;
   verbose = false;
   associativity = blockOffsetBits = setIndexBits = -1;
   int opt;
   extern char *optarg;
   //TODO
   //Use getopts here to parse the command line arguments
   //This is a useful resource: https://www.gnu.org/software/libc/manual/html_node/Getopt.html
   while((opt = getopt(argc, argv, ":s:E:b:t:hv")) != -1)
        {
            switch(opt)
            {
               case 'h':
                    printUsage();
                    break;
               case 'v':
                    verbose = true;
                    break;
               case 's':
                    setIndexBits = atoi(optarg);
                    std::cout << setIndexBits << "\n";
                    break;
               case 'E':
                    associativity = atoi(optarg);
                    std::cout << associativity << "\n";
                    break;

               case 'b':
                    blockOffsetBits = atoi(optarg);
                    std::cout << blockOffsetBits << "\n";
                    break;
                    
               case 't':
                    filename = optarg;
                    std::cout << filename << "\n"; 
                    break;
            }
        }   

   //Check to see whether the user provided all of the needed arguments
   //The code below isn't all of the error checking, however.  
   //0 for the block offset bits, the set index bits, or the associativity would also be an error.  
   //
   //(Note: 0 bits are used for the set index in the case of a fully associative cache, but your code
   //doesn't need to support a fully associative cache so flag 0 as an error in input.) 
   //
   if (setIndexBits == -1 || blockOffsetBits == -1 || 
       associativity == -1 || filename == "")
   {
      std::cout << "Required arguments missing\n";
      printUsage();
   }
}

/*
 * printUsage
 * Prints usage information and exits.
*/
void printUsage()
{
   std::cout << "Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n";
   std::cout << "Options: \n";
   std::cout << "  -h         Print this help message.\n";
   std::cout << "  -v         Optional verbose flag.\n";
   std::cout << "  -s <num>   Number of set index bits.\n";
   std::cout << "  -E <num>   Number of lines per set.\n";
   std::cout << "  -b <num>   Number of block offset bits.\n";
   std::cout << "  -t <file>  Trace file.\n\n";
   std::cout << "Examples:\n";
   std::cout << "  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n";
   std::cout << "  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n";
   exit(0);
}
