#include <iostream>
#include <fstream>

#include "include/tokenizer.h"
#include "include/parser.h"
#include "include/codeGenerator.h"

using namespace std;

int main(int argc, char** argv) 
{
    // string inputFile = "../inputs/Main.jack";
    string inputFile = argv[0];

    string outFile = "../inputs/Main.txt";

    ofstream out(outFile);

    Scanner scanner(inputFile);

    Parser parser(inputFile, scanner, out);
    parser.parseClass();
}
