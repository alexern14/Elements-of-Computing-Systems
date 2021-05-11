#include "include/codeGenerator.h"

void writePush(string seg, int index, ofstream &stream)
{
    stream << "push ";

    if (seg == "CONST")
    {
        stream << "constant ";
    }
    else if (seg == "ARG")
    {
        stream << "argument ";
    }

    stream << index << endl;
}

void writePop(string seg, int index, ofstream &stream)
{
    stream << "pop ";

    if (seg == "CONST")
    {
        stream << "constant ";
    }
    else if (seg == "ARG")
    {
        stream << "argument ";
    }

    stream << index << endl;
}

void writeArithmetic(string command, ofstream &stream)
{
    stream << command << endl;
}

void writeLabel(string label, ofstream &stream)
{
    stream << "label " << label << endl; 
}

void writeGoto(string label, ofstream &stream)
{
    stream << "goto " << label << endl; 
}

void writeIf(string label, ofstream &stream)
{
    stream << "if-goto " << label << endl; 
}

void writeCall(string name, int num, ofstream &stream)
{
    stream << "function " << name << " " << num << endl;
}

void writeFunction(string name, int num, ofstream &stream)
{
    stream << "function " << name << " " << num << endl;
}

void writeReturn(ofstream &stream)
{
    stream << "return" << endl;
}