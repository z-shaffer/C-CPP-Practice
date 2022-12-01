/*
 * Name:Tyler Taylor
 *
 * Name:Zach Shaffer
 *
 */

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <signal.h>
#include <unistd.h>
#include "WordList.h"
#include "NgramList.h"
#include "NgramSol.h"

#ifdef NGRAMSLOW
#include "NgramSlow.h"
#endif
#ifdef NGRAMFAST
#include "NgramFast.h"
#endif
#ifdef NGRAMSTL
#include "NgramSTL.h"
#endif
/* 
 * If you want to add another ngram version, you'll need to
 * define a macro like above (for example, NGRAMFASTER) and
 * modify the makefile.
 */

/* define handler_t type for signal handlers */
typedef void handler_t(int);

/* functions in this file. static makes them "private" to this file */
static bool hasMemoryErrors();
static void gradeStudentWork(WordList & wl);
static handler_t *Signal(int signum, handler_t *handler);
static void sigalrm_handler(int sig);
static void sigsegv_handler(int sig);
static void runProgram(WordList & wl);
static void checkConfiguration(int argc);
static std::string buildValgrindCommand();
static void usage(int argc, char * argv[]);
static void printUsage();
static int calculateScore(double totalTime, bool correct, bool memErrors);
static void printResults(double totalTime, bool correct, bool memErrors);
static void doSolutionWork(NgramList *& solIncr, NgramList *& solDecr, WordList & wl);
static double doStudentWork(NgramList *& studntIncr, NgramList *& studntDecr, WordList & wl);
static void printList(NgramList * solIncr, NgramList * solDecr, 
            NgramList * studntIncr, NgramList * studntDecr);
static std::string buildValgrindCommand();

/* Configuration parameters */
static std::string GradingInputFile = "shakespeareAllLines";
static std::string BigInputFile = "shakespeare50000Lines";
static std::string executable = "";
static std::string inputFile = "";
static std::string commandArgs = "";
static int ngramSz = 2;                            // -n
static bool doIncrease = false;                    // -i
static bool doDecrease = false;                    // -d
static bool printIncreaseList = false;             // -pi
static bool printDecreaseList = false;             // -pd
static bool printIncreaseSolution = false;         // -pis
static bool printDecreaseSolution = false;         // -pds
static bool doMemCheck = false;                    // -m
static bool doTimeCheck = false;                   // -t
static bool grade = false;                         // -g
static bool done = false;                      
static std::string boldOn = "\e[1m";
static std::string boldOff = "\e[0m";

/*
 * main
 *
 * drives the ngram building process
 *
 * param: int argc - number of command line arguments (either 2 or 4)
 * param: char * argv[] - command line arguments
 *        argv[0] - name of executable
 *        Remainder of arguments array contain the command line arguments.
 *        The last argument is always the input file name 
 */
int main(int argc, char * argv[])
{
   NgramList * solIncr = NULL, * solDecr = NULL, 
             * studntIncr = NULL, * studntDecr = NULL;

   WordList wl;
   //parse the command line arguments and set configuration parameters
   usage(argc, argv);

   if (grade) doIncrease = doDecrease = doMemCheck = doTimeCheck = true;

   //open and read file
   std::ifstream wordfile(inputFile);
   std::string word;
   while (wordfile >> word)
   {
      wl.addWord(word);
   }

   //install signal handlers
   Signal(SIGALRM, sigalrm_handler);
   Signal(SIGSEGV, sigsegv_handler);

   if (GradingInputFile == inputFile || inputFile == BigInputFile)
      std::cout << "This could take a bit. Be patient.\n";
   runProgram(wl);
}

/*
 * doSolutionWork
 * Build the increasing and decreasing ngram lists from the solution code.
 */
static void doSolutionWork(NgramList *& solIncr, NgramList *& solDecr, WordList & wl)
{
   NgramSol ngsollst(ngramSz, wl);
   if (doIncrease)
   {  
      solIncr = new NgramList();
      ngsollst.addIncreasing(solIncr);
   }
   if (doDecrease)
   {  
      solDecr = new NgramList();
      ngsollst.addDecreasing(solDecr);
   } 
}

/*
 * doStudentWork
 * Build the increasing and decreasing ngram lists from the student code.
 */
static double doStudentWork(NgramList *& studntIncr, NgramList *& studntDecr, WordList & wl)
{
   std::chrono::time_point<std::chrono::system_clock> startTime, endTime;
   //if timed then send an alarm signal after 30 seconds
   //and get the start time
   if (doTimeCheck)
   {
      alarm(30);
      done = false;
      startTime = std::chrono::system_clock::now();
   }

   Ngrams nglst(ngramSz, wl);
   if (doIncrease)  //build list that contains ngram counts in increasing order
   {
      studntIncr = new NgramList();
      nglst.addIncreasing(studntIncr);
   }
   if (doDecrease)  //build list that contains ngram counts in decreasing order
   {
      studntDecr = new NgramList();
      nglst.addDecreasing(studntDecr);
   }
   if (doTimeCheck) //get end time
   {
      endTime = std::chrono::system_clock::now();
      done = true;
   }

   if (doTimeCheck)
   {
      //calculate the duration
      std::chrono::duration<double> totalTime = endTime - startTime;
      return totalTime.count(); //in seconds
   } else
      return 0;
}

/*
 * printResults
 * totalTime - time taken by the student code
 * correct - is the student code correct
 * memErrors - does the student code have memory errors
 */
static void printResults(double totalTime, bool correct, bool memErrors)
{
   //How much time did it take?
   if (doTimeCheck)
   {
      if (totalTime < 1)
      {
         if (grade) std::cout << "\u2713 ";
         std::cout << boldOn << executable << " " << commandArgs << boldOff << " completes in " << totalTime << " seconds \n";
      } else
      {
         if (grade) std::cout << "\u2717 ";
         std::cout << boldOn << executable << " " << commandArgs << boldOff << " completes in " << totalTime << " seconds \n";
      }
   }

   //Are the answers correct?
   if (correct)
   {
      if (grade) std::cout << "\u2713 ";
      std::cout << boldOn << executable << " " << commandArgs << boldOff << " produces correct results\n";
   } else
   {
      if (grade) std::cout << "\u2717 ";
      std::cout << boldOn << executable << " " << commandArgs << boldOff << " does not produce correct results\n";
   } 

   //Did it produce memory errors?
   if (doMemCheck)
   {
      std::string valgrindCmd = buildValgrindCommand();
      if (!memErrors)
      {
         if (grade) std::cout << "\u2713 ";
         std::cout << "Running " << boldOn << valgrindCmd << boldOff << " displays no errors\n";
      }
      else
      {
         if (grade) std::cout << "\u2717 ";
         std::cout << "Running " << boldOn << valgrindCmd << boldOff << " displays errors\n";
      }
   }

   //If grade option selected, display the score
   int score;
   if (grade) 
   {
      score = calculateScore(totalTime, correct, memErrors);
      std::cout << "Score: " << score << "/50\n";
   }
}

/*
 * calculateScore
 * totalTime - time taken by the student code
 * correct - is the student code correct
 * memErrors - does the student code have memory errors
 */
static int calculateScore(double totalTime, bool correct, bool memErrors)
{
   int score = 0;
   if (totalTime < 1.0)
      score += 40;
   else if (totalTime < 2.0)
      score += 30;
   else if (totalTime < 30.0)
      score += 20;

   if (!memErrors) score += 10;
   if (!correct) score = 0;
   return score;
}

/*
 * runProgram
 * Both the solution algorithm and the student algorithm are used to
 * build the ngram lists.
 * The lists are compared to check for correctness.
 */
static void runProgram(WordList & wl)
{
   NgramList * solIncr = NULL, * solDecr = NULL, 
             * studntIncr = NULL, * studntDecr = NULL;

   //build solution
   doSolutionWork(solIncr, solDecr, wl);
   //run student code
   double totalTime = doStudentWork(studntIncr, studntDecr, wl);

   //compare student code to solution
   bool increasing = (doIncrease && studntIncr->isIncreasing());
   bool incrMatch = (doIncrease && increasing && studntIncr->compare(solIncr));
   bool decreasing = (doDecrease && studntDecr->isDecreasing());
   bool decrMatch = (doDecrease && decreasing && studntDecr->compare(solDecr));
   bool correct = ((!doIncrease || (doIncrease && increasing && incrMatch)) &&
                   (!doDecrease || (doDecrease && decreasing && decrMatch)));

   bool memErrors = false;
   if (doMemCheck)
   {
      memErrors = hasMemoryErrors();
   }
   printResults(totalTime, correct, memErrors);
   printList(solIncr, solDecr, studntIncr, studntDecr);

   delete solIncr; 
   delete solDecr; 
   delete studntIncr;
   delete studntDecr;
}

/*
 * printList
 * Print a list if requested.  Only one of these will be printed.
 */
static void printList(NgramList * solIncr, NgramList * solDecr, NgramList * studntIncr, NgramList * studntDecr)
{
   if (printIncreaseList)
   {
       std::cout << "List of ngrams in increasing order of ngram count\n";
       std::cout << "-------------------------------------------------\n";
       studntIncr->print();
   }
   if (printDecreaseList)
   {
       std::cout << "List of ngrams in decreasing order of ngram count\n";
       std::cout << "-------------------------------------------------\n";
       studntDecr->print();
   }
   if (printIncreaseSolution)
   {
       std::cout << "List of ngrams in SOLUTION in increasing order of ngram count\n";
       std::cout << "-------------------------------------------------------------\n";
       solIncr->print();
   }
   if (printDecreaseSolution)
   {
       std::cout << "List of ngrams in SOLUTION in decreasing order of ngram count\n";
       std::cout << "-------------------------------------------------------------\n";
       solDecr->print();
   }
}

/*
 * buildValgrindCommand
 * Build the command that is used to run valgrind.
 */
static std::string buildValgrindCommand()
{
   std::string commandLine = executable + " ";
   if (doIncrease) commandLine = commandLine + "-i ";
   if (doDecrease) commandLine = commandLine + "-d ";
   if (ngramSz != 2) commandLine = commandLine + "-n " + std::to_string(ngramSz) + " ";
   commandLine = commandLine + inputFile;
   std::string valgrindCmd = "valgrind --tool=memcheck --leak-check=full "
                               + commandLine;
   return valgrindCmd;
}

/*
 * hasMemoryErrors
 * Use valgrind to determine if the implementation has memory errors.
 */
static bool hasMemoryErrors()
{
   bool memErrors = false;
   std::string valgrindCmd = buildValgrindCommand() + " 2>&1";
   FILE * stream;
   char buffer[256];
   stream = popen(valgrindCmd.c_str(), "r");
   if (stream)
   {
      while (!feof(stream))
      {
         if (fgets(buffer, 256, stream) != NULL)
         {
            int errorCount, contextCount, parseCnt;
            parseCnt = sscanf(buffer, "==%*d== ERROR SUMMARY: %d errors from %d contexts %*s",
                              &errorCount, &contextCount);
            if (parseCnt == 2) 
            {
               if (errorCount != 0) memErrors = true;
               break;
            }
         }
      }
   }
   return memErrors;
}

/*
 * sigalrm_handler
 * Signal handler for the alarm signal.  It is used to kill program if it is taking to long.
 */
static void sigalrm_handler(int sig)
{
   if (done == false)
   {
      std::cout << executable << " takes longer than 30 seconds. Program aborted.\n";
      std::cout << "Try running gprof to see why your code is so slow.\n";
      std::cout << boldOn << "make clean\n";
      std::cout << "make PFLAG=-pg " << executable << "\n"; 
      std::cout << executable << " -i -d " << inputFile << "\n";
      std::cout << "gprof " << executable << boldOff << "\n";
      std::cout << "You can use a smaller input file and still get useful info.\n";
      exit(0);
   }
}

/*
 * sigsegv_handler
 * Signal handler for the segmentation fault signal.
 */
static void sigsegv_handler(int sig)
{
   std::cout << "You have a segmentation fault. Try running your code in the debugger.\n";
   std::cout << boldOn << "gdb " << executable << "\n";
   std::cout << "(gdb) r " << commandArgs << boldOff << "\n";
   std::cout << "Or use valgrind.\n";
   std::cout << boldOn << buildValgrindCommand() << boldOff << "\n";
   exit(0);
}

/*
 * usage
 *
 * Parses the command line arguments.  Calls printUsage and exits
 * if invalid command line arguments.
 * usage: executable ( -g | ( -i | -d | -i -d ) [ -pi | -pd | -pis | -pds ] [ -n <n> ] [ -m ] [ -t ] <filename> )
 *        -g : grade solution (no other options provided)
 *        BUILD OPTIONS (at least one is required)
 *        -i : build increasing list
 *        -d : build decreasing list
 *        PRINT OPTIONS (at most one is provided):
 *           -pi : print increasing list
 *           -pd : print decreasing list
 *           -pis : print increasing list of solution
 *           -pds : print decreasing list of solution
 *        OPTIONAL:
 *           -n <n> : set the ngram size to n (default 2)
 *           -m     : use valgrind to check for memory errors
 *           -t     : determine the amount of time it takes to build the increasing and/or decreasing list
 *
 * param: int argc - number of command line arguments
 * param: char * argv[] - command line arguments
 *                            used for input
 */
static void usage(int argc, char * argv[]) 
{
   int i;
   if (argc < 2) printUsage();
   executable = argv[0];
   for (i = 1; i < argc; i++)
   {
      commandArgs = commandArgs + std::string(argv[i]);
      if (i < argc - 1) commandArgs += " ";
      if (strcmp(argv[i], "-n") == 0)
      {
         ngramSz = atoi(argv[i+1]); 
         i++;
         commandArgs = commandArgs + std::string(argv[i]) + " ";
         if (ngramSz == 0) printUsage();
      } else if (strcmp(argv[i], "-i") == 0)
      {
         doIncrease = true;
      } else if (strcmp(argv[i], "-d") == 0)
      {
         doDecrease = true;
      } else if (strcmp(argv[i], "-pi") == 0)
      {
         printIncreaseList = true;
      } else if (strcmp(argv[i], "-pd") == 0)
      {
         printDecreaseList = true;
      } else if (strcmp(argv[i], "-pis") == 0)
      {
         printIncreaseSolution = true;
      } else if (strcmp(argv[i], "-pds") == 0)
      {
         printDecreaseSolution = true;
      } else if (strcmp(argv[i], "-m") == 0)
      {
         doMemCheck = true;
      } else if (strcmp(argv[i], "-t") == 0)
      {
         doTimeCheck = true;
      } else if (strcmp(argv[i], "-g") == 0)
      {
         grade = true;
         inputFile = GradingInputFile;
      } else if (i == argc - 1) 
      {
         inputFile = argv[i];
      } else
         printUsage(); 
   }
   checkConfiguration(argc);
}

/*
 * checkConfiguration
 * Checks to make sure the user entered the proper command line arguments.
 */
static void checkConfiguration(int argc)
{
   if (grade && argc != 2)
   {
      std::cout << "\nWith -g, no other options can be provided\n\n";
      printUsage();
   }

   if (inputFile == "" && !grade)
   {
      std::cout << "\nWithout -g, the name of the input file must be provided\n\n";
      printUsage();
   }

   if (!doIncrease && !doDecrease && !grade)
   {
      std::cout << "\nWithout -g, at least one of these options must be provided: -i -d\n\n";
      printUsage();
   }

   int printOpts = printDecreaseList + printIncreaseList + printIncreaseSolution + printDecreaseSolution;
   if (printOpts > 1)
   {
      std::cout << "\nAt most one of these options can be provided: -pi -pd -pis -pid\n\n";
      printUsage();
   }

   if ((printDecreaseList || printDecreaseSolution) && !doDecrease)
   {
      std::cout << "\nTo print a list in decreasing order, the -d option must be included\n\n";
      printUsage();
   }

   if ((printIncreaseList || printIncreaseSolution) && !doIncrease)
   {  
      std::cout << "\nTo print a list in increasing order, the -i option must be included\n\n";
      printUsage();
   }

   std::ifstream wordfile(inputFile);
   if (!wordfile.is_open()) 
   {
      std::cout << "\nCould not open: " << inputFile << "\n\n";
      printUsage();
   }
   wordfile.close();
}

/*
 * printUsage
 * Prints usage information and then exits
 * 
 * usage: executable ( -g | ( -i | -d | -i -d ) [ -pi | -pd | -pis | -pds ] [ -n <n> ] [ -m ] [ -t ] <filename>
 *        -g : grade solution (no other options provided)
 *        BUILD OPTIONS (at least one is required)
 *        -i : build increasing list
 *        -d : build decreasing list
 *        PRINT OPTIONS (at most one is provided):
 *           -pi : print increasing list
 *           -pd : print decreasing list
 *           -pis : print increasing list of solution
 *           -pds : print decreasing list of solution
 *        OPTIONAL:
 *           -n <n> : set the ngram size to n (default 2)
 *           -m     : use valgrind to check for memory errors
 *           -t     : determine the amount of time it takes to build the increasing and/or decreasing list
 *                  
*/
static void printUsage()
{
   std::cout << "usage: " << executable << " ( -g | "
             << "( -i | -d | -i -d ) [ -pi | -pd | -pis | -pds ] [ -n <n> ] [ -m ] [ -t ] <filename> )\n";
   std::cout << "       -g : grade solution (no other options provided)\n";
   std::cout << "       BUILD OPTIONS (at least one is required)\n";
   std::cout << "       -i : build increasing list\n";
   std::cout << "       -d : build decreasing list\n";
   std::cout << "       PRINT OPTIONS (at most one is provided):\n";
   std::cout << "          -pi : print increasing list\n";
   std::cout << "          -pd : print decreasing list\n";
   std::cout << "          -pis : print increasing list of solution\n";
   std::cout << "          -pds : print decreasing list of solution\n";
   std::cout << "       OPTIONAL:\n";
   std::cout << "          -n <n> : set the ngram size to n (default 2)\n";
   std::cout << "          -m     : use valgrind to check for memory errors\n";
   std::cout << "          -t     : determine the amount of time it takes to build the increasing and/or decreasing list\n";
   exit(1);
}

/*
 * Signal - wrapper for the sigaction function
 */
static handler_t *Signal(int signum, handler_t *handler)
{
    struct sigaction action, old_action;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
       std::cout << "Signal error: " << strerror(errno) << "\n";

    return (old_action.sa_handler);
}

