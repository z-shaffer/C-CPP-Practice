#include <fstream>
#include <iostream>
#include <string>
#include <assert.h>
#include "Line.h"
#include "Set.h"
#include "Cache.h"
#include "Simulate.h"

//TO DO: implement run and accessCache functions
//Look for TODO 

/* Simulator constructor
 * Dynamically create a Cache object and initialize the Simulator object.
 */

Simulate::Simulate(int32_t associativity, int32_t blockOffsetBits, 
                   int32_t setIndexBits, bool verbose)
{
   hits = misses = evictions = 0;
   cacheP = new Cache(associativity, blockOffsetBits, setIndexBits, verbose);
   this->verbose = verbose;
}

/*
 * Destroy the Simulator object.
 */
Simulate::~Simulate()
{
   delete cacheP;
}

void Simulate::run(std::string filename)
{
   std::fstream fs;
   fs.open(filename, std::fstream::in);
   std::string addressStr;
   std::string type;
   std::string fullLine;
   int64_t address;
   //TODO
   //Loop, reading lines from the file.
   //From each line, get the type and the address.
   //Depending upon the type (L or S), you'll pass it to the
   //accessCache method. (See assignment.)
   //If the verbose flag is true, print out the input line (see csim-ref).
   if (fs){
       while (std::getline(fs, fullLine)){
           if (fullLine[0] == ' ' && fullLine[1] != ' '){
               type = fullLine[1];
               int comma_spot = fullLine.find(",");
               addressStr = fullLine.substr(3,comma_spot);
               address = std::stoul(addressStr,0,16);
               if (type == "L"){
                   if (verbose){ 
                       std::cout << fullLine;
                   }
                   accessCache(address);
               }
               else if (type == "S"){
                   if (verbose){
                       std::cout << fullLine;
                   }
                   accessCache(address);
               }
               else if (type == "M"){
                   if (verbose){
                      std::cout << fullLine;
                   }
                   accessCache(address);
                   accessCache(address);

               }
           }
       }
   }
   fs.close();
}

/* accessCache
 * Takes as input a 64 bit address.
 * Uses the address to access the cache pointed to by cacheP
 * and updates hits, misses, and evictions.
 */
void Simulate::accessCache(int64_t address)
{
   //TODO
   //cacheP contains a pointer to the cache object
   //Call the appropriate methods in the Cache class
   //to determine whether to increment hits, misses, and/or evictions
   //If verbose is true, output the verbose output (see assignment)
   if (cacheP->isHit(address)){
       hits++;
       if (verbose){
           printf("hit\n");
        }
   } else {
        misses++;
        if (verbose){
            printf("miss\n");
        }
   
        if (cacheP->isEvict(address)){
            evictions++;
            if (verbose){
                printf("evict\n");
            }
        }
   }
   cacheP->update(address);
}

/*
 * printSummary
 * Print the number of hits, misses, and evictions at the end of
 * the simulation.
 */
void Simulate::printSummary()
{
   std::fstream fs;
   printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
   fs.open(".csim_results", std::fstream::out);
   assert(fs.is_open());
   fs << hits << " " << misses << " " << evictions <<"\n";;
   fs.close();
}

