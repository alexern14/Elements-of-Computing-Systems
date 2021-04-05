#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <dirent.h>

#include "include/codeWriter.h"
#include "include/parser.h"

using namespace std;

int main(int argc, char **argv)
{
    bool directory;

    string inputFile = argv[1];

    int dotVM = inputFile.find(".vm");
    // find_last_of(".");
    if (dotVM != -1)
    {
        directory = false;
    }
    else
    {
        directory = true;
    }

    // string inputFile = "/home/parallels/CSCI-410-Computing-systems/nand2tetris/projects/08/FunctionCalls/StaticsTest/";
    // directory = true;

    string outputFile = "";
    bool start = false;
    for (int i = inputFile.length() - 1; i >= 0; i--)
    {
        char c = inputFile[i];
        if (directory)
        {
            if ((c == '/') && (i != inputFile.length() - 1))
            {
                break;
            }
            else if ((c == '/') && (i == inputFile.length() - 1))
            {
                continue;
            }
            string add{c};
            outputFile.insert(0, add);
        }
        else
        {
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
    }
    string fileName = outputFile;
    cout << fileName << endl;
    outputFile += ".asm";

    if (!directory)
    {
        ifstream fin;
        fin.open(inputFile);

        ofstream outFile;
        outFile.open(outputFile);

        int lineNumber = 0;
        Command next = parser(fin);
        while (next.command != "EOF")
        {
            codeWriter(next, outFile, fileName, lineNumber, outputFile);

            lineNumber++;
            next = parser(fin);
        }
    }
    else
    {
        struct dirent *entry;
        DIR *dp;
        string currentFilename;

        dp = opendir(inputFile.c_str());

        if (dp == NULL)
        {
            exit(1);
        }

        ofstream outFile;
        outFile.open(outputFile);

        while ((entry = readdir(dp)))
        {
            currentFilename = inputFile + entry->d_name;
            int dotVM = currentFilename.find(".vm");

            if (dotVM != -1)
            {
                ifstream fin;
                fin.open(currentFilename);

                writeInit(outFile);

                int lineNumber = 0;
                Command next = parser(fin);
                while (next.command != "EOF")
                {
                    codeWriter(next, outFile, fileName, lineNumber, outputFile);

                    lineNumber++;
                    next = parser(fin);
                }
            }
        }
    }
}