#ifndef PARSER_H
#define PARSER_H

#include <iostream>

#include "tokenizer.h"

using namespace std;

class Parser
{
public:  

    /**
     * @brief Constructor
     * 
     * @param tokenFile 
     */
    Parser(string tokenFile, Scanner &scanner);

    // class className { classVarDec* subroutineDec* }
    void parseClass();

    // Grammar: (static|field) type varName1 (, varName2)* ;
    void parseClassVarDec();

    // int|char|boolean|className1
    void parseType();

    // (constructor|function|method) (void|type) subroutineName ( parameterList ) subroutineBody
    void parseSubroutineDec();

    // ((type varName) (, type varName)*)?
    void parseParameterList();

    // { varDec* statements }
    void parseSubroutineBody();

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

private:

    Scanner &scanner;
};

#endif