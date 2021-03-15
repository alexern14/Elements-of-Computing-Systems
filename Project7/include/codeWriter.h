#ifndef HEADER_CODE_WRITER
#define HEADER_CODE_WRITER

#include "parser.h"

using namespace std;

void writeArithmetic(Command command, ofstream &stream, string lineNumber);
void writePush(Command command, ofstream &stream, string fileName);
void writePop(Command command, ofstream &stream, string fileName);

void codeWriter(Command command, ofstream &stream, string fileName, int lineNumber);

#endif // HEADER_CODE_WRITER