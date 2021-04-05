#ifndef HEADER_PARSER
#define HEADER_PARSER

#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <vector>

using namespace std;

enum commandType
{
    arithmetic,
    push,
    pop,
    GoTo,
    ifGoto,
    label,
    call,
    function,
    returnFunction
};

const map<string, commandType> commandTypeTable = 
{
    {"push", push},
    {"pop", pop},
    {"add", arithmetic},
    {"sub", arithmetic},
    {"neg", arithmetic},
    {"eq", arithmetic},
    {"gt", arithmetic},
    {"lt", arithmetic},
    {"and", arithmetic},
    {"or", arithmetic},
    {"not", arithmetic},
    {"goto", GoTo},
    {"if-goto", ifGoto},
    {"label", label},
    {"call", call},
    {"function", function},
    {"return", returnFunction}
};

struct Command
{
    string command;
    commandType type;
    string arg1;
    int arg2;
};

Command parser(ifstream &stream);

#endif // HEADER_PARSER