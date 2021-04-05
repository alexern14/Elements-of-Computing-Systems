#ifndef HEADER_CODE_WRITER
#define HEADER_CODE_WRITER

#include "parser.h"

using namespace std;

void writeInit(ofstream &stream);

void writeArithmetic(Command command, ofstream &stream, string lineNumber);
void writePush(Command command, ofstream &stream, string fileName);
void writePop(Command command, ofstream &stream, string fileName);
void writeLabel(Command command, ofstream &stream);
void writeGoto(Command command, ofstream &stream);
void writeIf(Command command, ofstream &stream);
void writeFunction(Command command, ofstream &stream);
void writeCall(Command command, ofstream &stream, int labelsCounter);

void writeReturn(ofstream &stream);

void codeWriter(Command command, ofstream &stream, string fileName, int lineNumber, string processedFileName);

#endif // HEADER_CODE_WRITER