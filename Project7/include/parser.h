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
    pop
};

const map<string, commandType> commandTypeTable = {
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
    {"not", arithmetic}
};

struct Command
{
    string command;
    commandType type;
    string arg1;
    int arg2;
};

Command parser(ifstream& stream);

#endif // HEADER_PARSER