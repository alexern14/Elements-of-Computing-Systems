#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <iostream>
#include <fstream>

using namespace std;

void writePush(string seg, int index, ofstream &stream);
void writePop(string seg, int index, ofstream &stream);
void writeArithmetic(string command, ofstream &stream);
void writeLabel(string label, ofstream &stream);
void writeGoto(string label, ofstream &stream);
void writeIf(string label, ofstream &stream);
void writeCall(string name, int num, ofstream &stream);
void writeFunction(string name, int num, ofstream &stream);
void writeReturn(ofstream &stream);

#endif