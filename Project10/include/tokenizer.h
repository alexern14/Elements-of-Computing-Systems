#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>
#include <vector>

using namespace std;

enum tokentype 
{ 
    KEYWORD, 
    SYMBOL, 
    INT_CONST, 
    STRING_CONST, 
    IDENTIFIER 
};

class Token 
{
public:
    /**
     * @brief
     *
     * @return string
     */
    string toString();

    tokentype tokenType;

    string value;
};

class Scanner 
{
public:
    /**
     * @brief Constructor
     * Reads in the file into a single string
     *
     * @param inputFile
     */
    Scanner(string inputFile);

    /**
     * @brief Gets the next token without advancing.
     *
     * @return Token
     */
    Token peek();

    /**
     * @brief Gets the next token and advances
     *
     * @return Token
     */
    Token next();

    /**
     * @brief regexs
     *
     */
    const regex keyword{"[a-z]*"};
    // const regex keyword{
    //     "(class|constructor|function|method|field|static|var|int|char|boolean|"
    //     "void|true|false|null|this|let|do|if|else|while|return)"};
    const regex symbol{"([{}()[].,;+-*/&|<>=~])"};
    const regex int_const{"(\d+)"};
    const regex string_const{"\"([^\n]*)\""};
    const regex identifier{"([A-Za-z_]\w*)"};

    const regex inline_comment{"//.*\n"};
    const regex multiline_comment{"/\*.*?\*/"};

    vector<regex> regexes{multiline_comment, inline_comment, keyword,   symbol,
                            int_const,         string_const,   identifier};

private:
    int currentIndex;

    string wholeFile;
};

#endif