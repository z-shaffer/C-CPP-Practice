# core-sdk-cpp-zachary-shaffer

# Dependencies:
- cmake
- curl
- nlohmann/json (included)

# Run the program:
1) Clone the repository
2) Open a terminal and navigate to the local repository
3) Enter the 'cmake .' command from the root of the local repository
4) Now enter 'make' to build the application
5) Insert the pdf file you want to work with into the root of the folder. I have included my resume as an example.
6) Enter './pdfapp' to run the application
7) Enter the process you want to run on the pdf in the folder with the format: <pdf file> <operations>. Current available operations are 'compress' and 'validate'.
8) When you're finished, you may type "exit".

There are a few simple tests added into the program that test the CLI parser. To run these, return to the command line and type "./pdftest"

# Known Bugs:
- Currently, it seems the whole API pipeline is working until the part to check the result. When trying to get the PDF file result, the PDFToolsAPI returns:
{"operationType":"optimize","opearationStatus":"error","error":{"type":"Corrupt","title":"Corrupt","status":422,"detail":"Unable to optimize 'resume.pdf': The file header was not found."}

# Basic documentation:
- Main: Entry point of the program. Main runs the input loop, calls the CLI parser to parse input, passes the parsed input to the API handler to initiate the connection with the PDFTools API server, and then passes this connection with the server to the PDF processor.
- CLI_Parser: Parses the CLI input to format it, build a stack of commands, and ensure a valid input was given, or if 'exit' was entered, the program will terminate.
- PDFTools_API: This class handles all interactions with preparing the PDFTools' API. If a valid command was given, and a valid PDF has been provided, the PDF will now be run through the PDFTools' API according to the given command. An upload URL will be generated, and a file will be sent to the provided upload URL.
- PDF_Processor: After a PDF file has been stored in the API server, the file ID is tretrieved and passed to the PDF processor with the stack of commands given by the user. This class will handle interactions with the API to process the PDF according to the commands given by the user. As well, this class will update the fileId with the fileId of processed files, so commands can be chained together by the user.
