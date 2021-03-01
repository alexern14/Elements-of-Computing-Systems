#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

using namespace std;

map<string, unsigned int> symbolTable;

struct Comp
{
    string comp;
    string a;
};

map<string, Comp> compTable;
map<string, string> destTable;
map<string, string> jumpTable;
int RAM = 16;

enum commandType
{
    aCommand,
    cCommand,
    label
};

struct Command
{
    string command;
    commandType type;
};

void initialize()
{
    string r = "R";
    for (int i = 0; i <= 15; i++)
    {
        string R = r + to_string(i);
        symbolTable.insert(pair<string, int>(R, i));
    }
    symbolTable.insert(pair<string, unsigned int>("SCREEN", 16384));
    symbolTable.insert(pair<string, unsigned int>("KBD", 24576));
    symbolTable.insert(pair<string, unsigned int>("SP", 0));
    symbolTable.insert(pair<string, unsigned int>("LCL", 1));
    symbolTable.insert(pair<string, unsigned int>("ARG", 2));
    symbolTable.insert(pair<string, unsigned int>("THIS", 3));
    symbolTable.insert(pair<string, unsigned int>("THAT", 4));

    Comp comp;
    comp.comp = "101010";
    comp.a = "0";
    compTable.insert(pair<string, Comp>("0", comp));
    comp.comp = "111111";
    comp.a = "0";
    compTable.insert(pair<string, Comp>("1", comp));
    comp.comp = "111010";
    comp.a = "0";
    compTable.insert(pair<string, Comp>("-1", comp));
    comp.comp = "001100";
    comp.a = "0";
    compTable.insert(pair<string, Comp>("D", comp));
    comp.comp = "110000";
    comp.a = "0";
    compTable.insert(pair<string, Comp>("A", comp));
    comp.a = "1";
    compTable.insert(pair<string, Comp>("M", comp));
    comp.comp = "001101";
    comp.a = "0";
    compTable.insert(pair<string, Comp>("!D", comp));
    comp.comp = "110001";
    comp.a = "0";
    compTable.insert(pair<string, Comp>("!A", comp));
    comp.a = "1";
    compTable.insert(pair<string, Comp>("!M", comp));
    comp.comp = "001111";
    comp.a = "0";
    compTable.insert(pair<string, Comp>("-D", comp));
    comp.comp = "110011";
    comp.a = "0";
    compTable.insert(pair<string, Comp>("-A", comp));
    comp.a = "1";
    compTable.insert(pair<string, Comp>("-M", comp));
    comp.comp = "011111";
    comp.a = "0";
    compTable.insert(pair<string, Comp>("D+1", comp));
    comp.comp = "110111";
    comp.a = "0";
    compTable.insert(pair<string, Comp>("A+1", comp));
    comp.a = "1";
    compTable.insert(pair<string, Comp>("M+1", comp));
    comp.comp = "001110";
    comp.a = "0";
    compTable.insert(pair<string, Comp>("D-1", comp));
    comp.comp = "110010";
    comp.a = "0";
    compTable.insert(pair<string, Comp>("A-1", comp));
    comp.a = "1";
    compTable.insert(pair<string, Comp>("M-1", comp));
    comp.comp = "000010";
    comp.a = "0";
    compTable.insert(pair<string, Comp>("D+A", comp));
    comp.a = "1";
    compTable.insert(pair<string, Comp>("D+M", comp));
    comp.comp = "010011";
    comp.a = "0";
    compTable.insert(pair<string, Comp>("D-A", comp));
    comp.a = "1";
    compTable.insert(pair<string, Comp>("D-M", comp));
    comp.comp = "000111";
    comp.a = "0";
    compTable.insert(pair<string, Comp>("A-D", comp));
    comp.a = "1";
    compTable.insert(pair<string, Comp>("M-D", comp));
    comp.comp = "000000";
    comp.a = "0";
    compTable.insert(pair<string, Comp>("D&A", comp));
    comp.a = "1";
    compTable.insert(pair<string, Comp>("D&M", comp));
    comp.comp = "010101";
    comp.a = "0";
    compTable.insert(pair<string, Comp>("D|A", comp));
    comp.a = "1";
    compTable.insert(pair<string, Comp>("D|M", comp));

    destTable.insert(pair<string, string>("", "000"));
    destTable.insert(pair<string, string>("M", "001"));
    destTable.insert(pair<string, string>("D", "010"));
    destTable.insert(pair<string, string>("MD", "011"));
    destTable.insert(pair<string, string>("A", "100"));
    destTable.insert(pair<string, string>("AM", "101"));
    destTable.insert(pair<string, string>("AD", "110"));
    destTable.insert(pair<string, string>("AMD", "111"));

    jumpTable.insert(pair<string, string>("", "000"));
    jumpTable.insert(pair<string, string>("JGT", "001"));
    jumpTable.insert(pair<string, string>("JEQ", "010"));
    jumpTable.insert(pair<string, string>("JGE", "011"));
    jumpTable.insert(pair<string, string>("JLT", "100"));
    jumpTable.insert(pair<string, string>("JNE", "101"));
    jumpTable.insert(pair<string, string>("JLE", "110"));
    jumpTable.insert(pair<string, string>("JMP", "111"));
}

string convert(int convert)
{
    string str = bitset<15>(convert).to_string();
    return str;
}

string translateC(string command)
{
    bool destination = false;
    bool Jump = false;
    for (char c : command)
    {
        if (c == '=')
            destination = true;
        else if (c == ';')
            Jump = true;
    }

    string comp;
    string dest;
    string jump;

    if (destination && Jump)
    {
        Jump = false;
        for (char c : command)
        {
            if (c == '=')
            {
                destination = false;
                continue;
            }
            else if (c == ';')
            {
                Jump = true;
                continue;
            }
            if (destination)
                dest += c;
            else if (Jump)
                jump += c;
            else
                comp += c;
        }
    }
    else if (destination && !Jump)
    {
        for (char c : command)
        {
            if (c == '=')
            {
                destination = false;
                continue;
            }
            if (destination)
                dest += c;
            else
                comp += c;
        }
    }
    else if (!destination && Jump)
    {
        Jump = false;
        for (char c : command)
        {
            if (c == ';')
            {
                Jump = true;
                continue;
            }
            if (Jump)
                jump += c;
            else
                comp += c;
        }
    }
    else
    {
        for (char c : command)
            comp += c;
    }

    string a = compTable[comp].a;
    string c = compTable[comp].comp;
    string d = destTable[dest];
    string j = jumpTable[jump];

    string binaryExpression = "111";
    binaryExpression += a;
    binaryExpression += c;
    binaryExpression += d;
    binaryExpression += j;

    return binaryExpression;
}

string translateA(string command)
{
    string binaryExpression = "0";
    int digit;
    command.erase(0, 1);

    if (isdigit(command.at(0)))
    {
        digit = stoi(command);
        binaryExpression += convert(digit);
    }
    else
    {
        if (symbolTable.find(command) == symbolTable.end())
        {
            symbolTable.insert(pair<string, unsigned int>(command, RAM));
            RAM++;
        }
        digit = symbolTable[command];

        binaryExpression += convert(digit);
    }

    return binaryExpression;
}

string translate(Command command)
{
    string binaryExpression;
    if (command.type == aCommand)
        binaryExpression = translateA(command.command);
    else if (command.type == cCommand)
        binaryExpression = translateC(command.command);

    return binaryExpression;
}

Command nextCommand(ifstream &stream)
{
    Command command;
    string line;
    string commandOnly;

loop:
    if (stream.peek() == ifstream::traits_type::eof())
    {
        command.command = "EOF";
        return command;
    }

    getline(stream, line);
    for (char c : line)
    {
        if ((c == '/') || (c == '\r'))
            goto end;
        commandOnly += c;
    }
end:
    commandOnly.erase(remove(commandOnly.begin(), commandOnly.end(), ' '), commandOnly.end());
    if (commandOnly.length() == 0)
        goto loop;

    command.command = commandOnly;
    if (commandOnly.at(0) == '@')
        command.type = aCommand;
    else if (commandOnly.at(0) == '(')
        command.type = label;
    else
        command.type = cCommand;

    return command;
}

void secondPass(vector<Command> &commands, ofstream &stream)
{
    for (Command command : commands)
    {
        string binaryExpression;
        if (command.type != label)
        {
            binaryExpression = translate(command);
            stream << binaryExpression << endl;
        }
    }
}

vector<Command> firstPass(ifstream &stream)
{
    vector<Command> commands;
    int pc = 0;

    Command next = nextCommand(stream);
    while (next.command != "EOF")
    {
        if (next.type == label)
        {
            next.command.erase(0, 1);
            next.command.pop_back();
            symbolTable.insert(pair<string, int>(next.command, pc));
        }
        else
        {
            pc++;
            commands.push_back(next);
        }
        next = nextCommand(stream);
    }

    return commands;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "Need to enter an input file." << endl;
        return 1;
    }

    string inputFile = argv[1];

    // string inputFile = "/home/parallels/CSCI-410-Computing-systems/Project6/Pong.asm";
    // string outputFile = "./add.hack";

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
    outputFile += ".hack";

    ifstream file(inputFile);
    if (!file.is_open())
    {
        cout << "Could not find file: " << inputFile << endl;
        exit(1);
    }
    ofstream out(outputFile);
    if (!out.is_open())
    {
        cout << "Could not open file: " << outputFile << endl;
        exit(1);
    }

    initialize();
    vector<Command> commands = firstPass(file);
    secondPass(commands, out);

    file.close();
    out.close();

    return 0;
}
