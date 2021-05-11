#include <iostream>
#include <fstream>

#include "include/tokenizer.h"
#include "include/parser.h"
#include "include/codeGenerator.h"

using namespace std;

int main(int argc, char** argv) 
{
    string inputFile = "../inputs/average.jack";

    string outFile = "";

    ofstream out(outFile);

    Scanner scanner(inputFile);

    Parser parser(inputFile, scanner, out);
    parser.parseClass();
}
