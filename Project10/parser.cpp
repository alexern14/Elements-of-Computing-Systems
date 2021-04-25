#include "include/parser.h"

Parser::Parser(string tokenFile)
{

}

// class className { classVarDec* subroutineDec* }
void Parser::parseClass()
{
    parseClassVarDec();
    parseSubroutineDec();
}

// Grammar: (static|field) type varName1 (, varName2)* ;
void Parser::parseClassVarDec()
{
    parseType();
}

// int|char|boolean|className1
void Parser::parseType()
{

}

// (constructor|function|method) (void|type) subroutineName ( parameterList ) subroutineBody
void Parser::parseSubroutineDec()
{
    parseParameterList();
    parseSubroutineBody();
}

// ((type varName) (, type varName)*)?
void Parser::parseParameterList()
{

}

// { varDec* statements }
void Parser::parseSubroutineBody()
{
    parseVarDec();
    parseStatements();
}

// var type varName (, varName)* ;
void Parser::parseVarDec()
{

}

// statement*
// statement: 
//  letStatement
//  | ifStatement 
//  | whileStatement 
//  | doStatement 
//  | returnStatement

void Parser::parseStatements()
{

}

// let varName ([ expression ])? = expression ;
void Parser::parseLetStatement()
{
    parseExpression();
}

// if ( expression ) { statements } ( else { statements } )?
void Parser::parseIfStatement()
{
    parseExpression();
    parseStatements();
    parseStatements();
}

// while ( expression ) { statements }
void Parser::parseWhileStatement()
{
    parseExpression();
    parseStatements();
}

// return expression? ;
void Parser::parseReturnStatement()
{
    parseExpression();
}

// do subroutineCall ;
void Parser::parseDoStatement()
{
    parseSubroutineCall();
}

// subroutineName ( expressionList ) 
//  | (className|varName) . subroutineName ( expressionList )
void Parser::parseSubroutineCall()
{
    parseExpressionList();
}

// (expression (, expression)*)?
void Parser::parseExpressionList()
{
    parseExpression();
}

// term (op term)*
void Parser::parseExpression()
{
    parseTerm();
}

// term: 
//  integerConstant 
//  | stringConstant 
//  | keyword 
//  | varName 
//  | varName [ expression ] 
//  | subroutineCall 
//  | ( expression ) 
//  | unaryOp term
void Parser::parseTerm()
{

}

