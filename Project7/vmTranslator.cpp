#include <iostream>
#include <fstream>

#include "include/parser.h"
#include "include/codeWriter.h"

using namespace std;

int main(int argc, char **argv)
{

    string inputFile = argv[1];

    string outputFile = "";
    bool start = false;
    for (int i = inputFile.length() - 1; i >= 0; i--)
    {
        char c = inputFile[i];
        if (start)
        {
            if (c == '/')
            {
                break;
            }
            string add{c};
            outputFile.insert(0, add);
        }
        else if (c == '.')
            start = true;
    }
    string fileName = outputFile;
    cout << fileName << endl;
    outputFile += ".asm";

    // string inputFile = "/home/parallels/CSCI-410-Computing-systems/nand2tetris/projects/07/StackArithmetic/StackTest/StackTest.vm";
    // string outputFile = "./StackTest.asm";

    ifstream fin;
    fin.open(inputFile);

    ofstream outFile;
    outFile.open(outputFile);

    int lineNumber = 0;
    Command next = parser(fin);
    while (next.command != "EOF")
    {
        codeWriter(next, outFile, fileName, lineNumber);

        lineNumber++;
        next = parser(fin);
    }
    int x = 0;
}
