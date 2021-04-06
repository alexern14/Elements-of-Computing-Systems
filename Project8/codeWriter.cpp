#include "include/codeWriter.h"

string functionName;
string processedFilename;

int start_ = 0;

void writeInit(ofstream &stream)
{
    stream << "@256" << endl;
    stream << "D=A" << endl;
    stream << "@SP" << endl;
    stream << "M=D" << endl;

    stream << "@START" << start_ << endl;
    stream << "0;JMP" << endl;
    stream << "(MAKETRUE)" << endl;

    stream << "@SP" << endl;
    stream << "AM=M-1" << endl;

    stream << "M=-1" << endl;

    stream << "@SP" << endl;
    stream << "AM=M+1" << endl;

    stream << "@R15" << endl;
    stream << "A=M" << endl;
    stream << "0;JMP" << endl;

    stream << "(START" << start_ << ")" << endl;

    Command command;
    command.arg1 = "Sys.init";
    command.arg2 = 0;
    writeCall(command, stream, 0);

    start_++;
}

void writeArithmetic(Command command, ofstream &stream, string lineNumber)
{
    stream << "@SP" << endl;
    stream << "A=M-1" << endl;

    if (command.arg1 == "neg")
    {
        stream << "M=-M" << endl;
        return;
    }
    else if (command.arg1 == "not")
    {
        stream << "M=!M" << endl;
        return;
    }

    stream << "D=M" << endl;
    stream << "A=A-1" << endl;

    if ((command.arg1 == "add") || (command.arg1 == "sub") || (command.arg1 == "and") || (command.arg1 == "or"))
    {
        if (command.arg1 == "add")
        {
            stream << "M=M+D" << endl;
        }
        else if (command.arg1 == "sub")
        {
            stream << "M=M-D" << endl;
        }
        else if (command.arg1 == "and")
        {
            stream << "M=D&M" << endl;
        }
        else if (command.arg1 == "or")
        {
            stream << "M=D|M" << endl;
        }
    }
    else if ((command.arg1 == "eq") || (command.arg1 == "gt") || (command.arg1 == "lt"))
    {
        stream << "D=M-D" << endl;

        if (command.arg1 == "eq")
        {
            stream << "@EQ_" << lineNumber << endl;
            stream << "D;JEQ" << endl;

            stream << "D=0" << endl;
            stream << "@END" << lineNumber << endl;
            stream << "0;JMP" << endl;

            stream << "(EQ_" << lineNumber << ")" << endl;
        }
        else if (command.arg1 == "gt")
        {
            stream << "@GT_" << lineNumber << endl;
            stream << "D;JGT" << endl;

            stream << "D=0" << endl;
            stream << "@END" << lineNumber << endl;
            stream << "0;JMP" << endl;

            stream << "(GT_" << lineNumber << ")" << endl;
        }
        else if (command.arg1 == "lt")
        {
            stream << "@LT_" << lineNumber << endl;
            stream << "D;JLT" << endl;

            stream << "D=0" << endl;
            stream << "@END" << lineNumber << endl;
            stream << "0;JMP" << endl;
            stream << "(LT_" << lineNumber << ")" << endl;
        }

        stream << "D=-1" << endl;
        stream << "(END" << lineNumber << ")" << endl;
        stream << "@SP" << endl;
        stream << "A=M-1" << endl;
        stream << "A=A-1" << endl;
        stream << "M=D" << endl;
    }
    stream << "@SP" << endl;
    stream << "M=M-1" << endl;
}

void writePush(Command command, ofstream &stream, string fileName)
{
    if (command.arg1 == "constant")
    {
        stream << "@" << command.arg2 << endl;
        stream << "D=A" << endl;
    }
    else if (command.arg1 == "local")
    {
        stream << "@LCL" << endl;
        stream << "D=M" << endl;
        stream << "@" << command.arg2 << endl;
        stream << "A=D+A" << endl;
        stream << "D=M" << endl;
    }
    else if (command.arg1 == "argument")
    {
        stream << "@ARG" << endl;
        stream << "D=M" << endl;
        stream << "@" << command.arg2 << endl;
        stream << "A=D+A" << endl;
        stream << "D=M" << endl;
    }
    else if (command.arg1 == "this")
    {
        stream << "@THIS" << endl;
        stream << "D=M" << endl;
        stream << "@" << command.arg2 << endl;
        stream << "A=D+A" << endl;
        stream << "D=M" << endl;
    }
    else if (command.arg1 == "that")
    {
        stream << "@THAT" << endl;
        stream << "D=M" << endl;
        stream << "@" << command.arg2 << endl;
        stream << "A=D+A" << endl;
        stream << "D=M" << endl;
    }
    else if (command.arg1 == "pointer")
    {
        if (command.arg2 == 0)
        {
            stream << "@THIS" << endl;
        }
        else
        {
            stream << "@THAT" << endl;
        }
        stream << "D=M" << endl;
    }
    else if (command.arg1 == "temp")
    {
        stream << "@R5" << endl;
        stream << "D=A" << endl;
        stream << "@" << command.arg2 << endl;
        stream << "A=D+A" << endl;
        stream << "D=M" << endl;
    }
    else if (command.arg1 == "static")
    {
        stream << "@" << fileName << "." << command.arg2 << endl;
        stream << "D=M" << endl;
    }

    stream << "@SP" << endl;
    stream << "A=M" << endl;
    stream << "M=D" << endl;
    stream << "@SP" << endl;
    stream << "M=M+1" << endl;
}

void writePop(Command command, ofstream &stream, string fileName)
{
    if (command.arg1 == "local")
    {
        stream << "@LCL" << endl;
        stream << "D=M" << endl;
        stream << "@" << command.arg2 << endl;
        stream << "D=D+A" << endl;
    }
    else if (command.arg1 == "argument")
    {
        stream << "@ARG" << endl;
        stream << "D=M" << endl;
        stream << "@" << command.arg2 << endl;
        stream << "D=D+A" << endl;
    }
    else if (command.arg1 == "this")
    {
        stream << "@THIS" << endl;
        stream << "D=M" << endl;
        stream << "@" << command.arg2 << endl;
        stream << "D=D+A" << endl;
    }
    else if (command.arg1 == "that")
    {
        stream << "@THAT" << endl;
        stream << "D=M" << endl;
        stream << "@" << command.arg2 << endl;
        stream << "D=D+A" << endl;
    }
    else if (command.arg1 == "pointer")
    {
        if (command.arg2 == 0)
        {
            stream << "@THIS" << endl;
        }
        else
        {
            stream << "@THAT" << endl;
        }
        stream << "D=A" << endl;
    }
    else if (command.arg1 == "temp")
    {
        stream << "@R5" << endl;
        stream << "D=A" << endl;
        stream << "@" << command.arg2 << endl;
        stream << "D=D+A" << endl;
    }
    else if (command.arg1 == "static")
    {
        stream << "@" << fileName << "." << command.arg2 << endl;
        stream << "D=A" << endl;
    }

    stream << "@R13" << endl;
    stream << "M=D" << endl;
    stream << "@SP" << endl;
    stream << "AM=M-1" << endl;
    stream << "D=M" << endl;
    stream << "@R13" << endl;
    stream << "A=M" << endl;
    stream << "M=D" << endl;
}

void writeLabel(Command command, ofstream &stream)
{
    stream << "(" << functionName << "$" << command.arg1 << ")" << endl;
}

void writeGoto(Command command, ofstream &stream)
{
    stream << "@" << functionName << "$" << command.arg1 << endl;
    stream << "0;JMP" << endl;
}

void writeIf(Command command, ofstream &stream)
{
    stream << "@SP" << endl;
    stream << "AM=M-1" << endl;
    stream << "D=M" << endl;

    stream << "@" << functionName << "$" << command.arg1 << endl;
    stream << "D;JNE" << endl;
}

void writeFunction(Command command, ofstream &stream)
{
    functionName = command.arg1;
    string loopLabel = command.arg1 + "_INIT_LOCALS_LOOP";
    string loopEndLabel = command.arg1 + "_INIT_LOCALS_END";

    stream << "(" << command.arg1 << ")" << endl;
    stream << "@" << to_string(command.arg2) << endl;
    stream << "D=A" << endl;
    stream << "@R13" << endl;
    stream << "M=D" << endl;
    stream << "(" << loopLabel << ")" << endl;
    stream << "@" << loopEndLabel << endl;
    stream << "D;JEQ" << endl;
    stream << "@0" << endl;
    stream << "D=A" << endl;
    stream << "@SP" << endl;
    stream << "A=M" << endl;
    stream << "M=D" << endl;
    stream << "@SP" << endl;
    stream << "M=M+1" << endl;
    stream << "@R13" << endl;
    stream << "MD=M-1" << endl;
    stream << "@" << loopLabel << endl;
    stream << "0;JMP" << endl;
    stream << "(" << loopEndLabel << ")" << endl;
}

void writeCall(Command command, ofstream &stream, int labelsCounter)
{
    ostringstream oss;

    oss << processedFilename << "." << command.arg2 << "$return" << labelsCounter;

    string returnLabel = oss.str();

    stream << "@" << returnLabel << endl;
    stream << "D=A" << endl;
    stream << "@SP" << endl;
    stream << "A=M" << endl;
    stream << "M=D" << endl;

    stream << "@SP" << endl;
    stream << "AM=M+1" << endl;

    stream << "@LCL" << endl;
    stream << "D=M" << endl;
    stream << "@SP" << endl;
    stream << "A=M" << endl;
    stream << "M=D" << endl;

    stream << "@SP" << endl;
    stream << "AM=M+1" << endl;

    stream << "@ARG" << endl;
    stream << "D=M" << endl;
    stream << "@SP" << endl;
    stream << "A=M" << endl;
    stream << "M=D" << endl;

    stream << "@SP" << endl;
    stream << "AM=M+1" << endl;

    stream << "@THIS" << endl;
    stream << "D=M" << endl;
    stream << "@SP" << endl;
    stream << "A=M" << endl;
    stream << "M=D" << endl;

    stream << "@SP" << endl;
    stream << "AM=M+1" << endl;

    stream << "@THAT" << endl;
    stream << "D=M" << endl;
    stream << "@SP" << endl;
    stream << "A=M" << endl;
    stream << "M=D" << endl;

    stream << "@SP" << endl;
    stream << "AM=M+1" << endl;

    stream << "@" << to_string(command.arg2) << endl;
    stream << "D=A" << endl;
    stream << "@SP" << endl;
    stream << "D=M-D" << endl;
    stream << "@5" << endl;
    stream << "D=D-A" << endl;
    stream << "@ARG" << endl;
    stream << "M=D" << endl;

    stream << "@SP" << endl;
    stream << "D=M" << endl;
    stream << "@LCL" << endl;
    stream << "M=D" << endl;

    stream << "@" << command.arg1 << endl;
    stream << "0;JMP" << endl;
    stream << "(" << returnLabel << ")" << endl;
}

void writeReturn(ofstream &stream)
{
    stream << "@LCL" << endl;
    stream << "D=M" << endl;
    stream << "@FRAME" << endl;
    stream << "M=D" << endl;

    stream << "@5" << endl;
    stream << "D=A" << endl;
    stream << "@FRAME" << endl;
    stream << "A=M-D" << endl;
    stream << "D=M" << endl;
    stream << "@TER" << endl;
    stream << "M=D" << endl;

    stream << "@SP" << endl;
    stream << "AM=M-1" << endl;
    stream << "D=M" << endl;

    stream << "@ARG" << endl;
    stream << "A=M" << endl;
    stream << "M=D" << endl;

    stream << "@ARG" << endl;
    stream << "D=M+1" << endl;
    stream << "@SP" << endl;
    stream << "M=D" << endl;

    stream << "@FRAME" << endl;
    stream << "A=M-1" << endl;
    stream << "D=M" << endl;
    stream << "@THAT" << endl;
    stream << "M=D" << endl;

    stream << "@2" << endl;
    stream << "D=A" << endl;
    stream << "@FRAME" << endl;
    stream << "A=M-D" << endl;
    stream << "D=M" << endl;
    stream << "@THIS" << endl;
    stream << "M=D" << endl;

    stream << "@3" << endl;
    stream << "D=A" << endl;
    stream << "@FRAME" << endl;
    stream << "A=M-D" << endl;
    stream << "D=M" << endl;
    stream << "@ARG" << endl;
    stream << "M=D" << endl;

    stream << "@4" << endl;
    stream << "D=A" << endl;
    stream << "@FRAME" << endl;
    stream << "A=M-D" << endl;
    stream << "D=M" << endl;
    stream << "@LCL" << endl;
    stream << "M=D" << endl;

    stream << "@RET" << endl;
    stream << "A=M" << endl;
    stream << "0; JMP" << endl;
}

void codeWriter(Command command, ofstream &stream, string fileName, int lineNumber, string processedFileName)
{
    processedFilename = processedFileName;

    if (command.type == push)
    {
        writePush(command, stream, fileName);
    }
    else if (command.type == pop)
    {
        writePop(command, stream, fileName);
    }
    else if (command.type == arithmetic)
    {
        writeArithmetic(command, stream, to_string(lineNumber));
    }
    else if (command.type == GoTo)
    {
        writeGoto(command, stream);
    }
    else if (command.type == ifGoto)
    {
        writeIf(command, stream);
    }
    else if (command.type == label)
    {
        writeLabel(command, stream);
    }
    else if (command.type == call)
    {
        writeCall(command, stream, lineNumber);
    }
    else if (command.type == function)
    {
        writeFunction(command, stream);
    }
    else if (command.type == returnFunction)
    {
        writeReturn(stream);
    }
}