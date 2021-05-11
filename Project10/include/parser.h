#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <map>
#include <vector>
#include <fstream>

#include "tokenizer.h"
#include "codeGenerator.h"

using namespace std;

class Parser
{
public:  

    /**
     * @brief Constructor
     * 
     * @param tokenFile 
     */
    Parser(string tokenFile, Scanner &scanner, ofstream &stream);

    map<string, tuple<string, string, int>> subScope = 
    {
    };

    map<string, tuple<string, string, int>> staticScope = 
    {
    };

    map<string, tuple<string, string, int>> fieldScope = 
    {
    };

    map<string, int> counts = 
    {
        {"STATIC", 0},
        {"FIELD", 0},
        {"ARG", 0},
        {"VAR", 0}
    };

    map<string, string> kindMap = 
    {
        {"ARG", "ARG"},
        {"STATIC", "STATIC"},
        {"VAR", "LOCAL"},
        {"FIELD", "THIS"}
    };

    map<string, string> arithmeticMap = 
    {
        {"+", "ADD"},
        {"-", "SUB"},
        {"=", "EQ"},
        {">", "GT"},
        {"<", "LT"},
        {"&", "AND"},
        {"|", "OR"}
    };

    map<string, string> unaryMap = 
    {
        {"-", "NEG"},
        {"~", "NOT"}
    };

    string className;

    // class className { classVarDec* subroutineDec* }
    void parseClass();

    // Grammar: (static|field) type varName1 (, varName2)* ;
    void parseClassVarDec();

    void parseSubroutine();

    // ((type varName) (, type varName)*)?
    void parseParameterList();

    // var type varName (, varName)* ;
    void parseVarDec();

    // statement*
    // statement: 
    //  letStatement
    //  | ifStatement 
    //  | whileStatement 
    //  | doStatement 
    //  | returnStatement
    void parseStatements();

    // let varName ([ expression ])? = expression ;
    void parseLetStatement();

    // if ( expression ) { statements } ( else { statements } )?
    void parseIfStatement();

    // while ( expression ) { statements }
    void parseWhileStatement();

    // return expression? ;
    void parseReturnStatement();

    // do subroutineCall ;
    void parseDoStatement();

    // subroutineName ( expressionList ) 
    //  | (className|varName) . subroutineName ( expressionList )
    void parseSubroutineCall();

    // (expression (, expression)*)?
    void parseExpressionList();

    // term (op term)*
    void parseExpression();

    // term: 
    //  integerConstant 
    //  | stringConstant 
    //  | keyword 
    //  | varName 
    //  | varName [ expression ] 
    //  | subroutineCall 
    //  | ( expression ) 
    //  | unaryOp term
    void parseTerm();

    Token getNextToken();

    void defineSymbolTable(string name, string type, string kind);

    void startSubroutine();

private:

    Scanner &scanner;
    ofstream &stream;
};

#endif