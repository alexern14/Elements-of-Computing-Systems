#include "include/parser.h"

Command parser(ifstream &stream)
{
    Command command;
    string line;
    string commandOnly;

    if (stream.peek() == ifstream::traits_type::eof())
    {
        command.command = "EOF";
        return command;
    }
loop:
    getline(stream, line);
    for (char c : line)
    {
        if ((c == '/') || (c == '\r'))
            goto end;
        commandOnly += c;
    }
end:
    if (commandOnly.empty())
    {
        goto loop;
    }

    istringstream ss(commandOnly);

    vector<string> commandArgs;
    string s;
    while (getline(ss, s, ' '))
    {
        commandArgs.push_back(s);
    }

    command.type = commandTypeTable.at(commandArgs.at(0));
    if (command.type == arithmetic)
    {
        command.arg1 = commandArgs.at(0);
    }
    if ((command.type == push) || (command.type == pop))
    {
        command.arg1 = commandArgs.at(1);
        command.arg2 = stoi(commandArgs.at(2));
    }

    command.command = commandOnly;

    return command;
}