#include "include/parser.h"

Parser::Parser(string tokenFile, Scanner &scanner)
    : scanner(scanner)
{

}

// class className { classVarDec* subroutineDec* }
void Parser::parseClass()
{
    // <class>
    //  <keyword> class </keyword>
    //  <identifier> classname </identifier>
    //  <symbol> { </symbol>
    string next;
    while (((next = scanner.peek().value) == "static") || (next == "field"))
    {
        parseClassVarDec();
    }
    while (((next = scanner.peek().value) == "constructor") || (next == "method") || (next == "function"))
    {
        parseSubroutineDec();
    }
    //  <symbol> } </symbol>
    // </class>
}

// (static|field) type varName1 (, varName2)* ;
void Parser::parseClassVarDec()
{
    // <classVarDec>
    // <keyword> static | field </keyword>
    parseType();
    // <identifier> varName1 </identifier>
    string next;
    while ((next = scanner.peek().value) != ";")
    {
        // <symbol> , </symbol>
        // <identifier> varName </identifier>
    }
    // <symbol> ; </symbol>
    // </classVarDec>
}

// int|char|boolean|className1
void Parser::parseType()
{
    // <keyword> int | char | boolean </keyword>
    // | <identifier> className1 </identifier>
}

// (constructor|function|method) (void|type) subroutineName ( parameterList ) subroutineBody
void Parser::parseSubroutineDec()
{
    //  <keyword> constructor | function | method </keyword>
    parseType();
    //  <identifier> subroutineName </identifier>
    //  <symbol> ( </symbol>
    parseParameterList();
    //  <symbol> ) </symbol>
    parseSubroutineBody();
}

// ((type varName) (, type varName)*)?
void Parser::parseParameterList()
{
    // <symbol> ( </symbol>
    // <symbol> ( </symbol>
    parseType();
    // <identifier> varName </identifier>
    // <symbol> ) </symbol>
    string next;
    while ((next = scanner.peek().value) != ")")
    {
        // <symbol> , </symbol>
        // <identifier> varName </identifier>
    }
    // <symbol> ) </symbol>
}

// { varDec* statements }
void Parser::parseSubroutineBody()
{
    //  <symbol> { </symbol>
    string next;
    while ((next = scanner.peek().value) == "var")
    {
        parseVarDec();
    }
    parseStatements();
    //  <symbol> } </symbol>
}

// var type varName (, varName)* ;
void Parser::parseVarDec()
{
    // <keyword> var </keyword>
    parseType();
    // <identifier> varName </identifier>
    string next;
    while ((next = scanner.peek().value) != ";")
    {
        // <symbol> , </symbol>
        // <identifier> varName </identifier>
    }
    // <symbol> ; </symbol>
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
    // <statements>
    // loop over statements and call corresponding parsing function
    string next;
    while ((next = scanner.peek().value) != ";")
    {
        if (next == "let")
        {
            parseLetStatement();
        }
        else if (next == "if")
        {
            parseIfStatement();
        }
        else if (next == "while")
        {
            parseWhileStatement();
        }
        else if (next == "do")
        {
            parseDoStatement();
        }
        else if (next == "return")
        {
            parseReturnStatement();
        }
    }
    // </statements>
}

// let varName ([ expression ])? = expression ;
void Parser::parseLetStatement()
{
    // <letStatement>
    //  <keyword> let </keyword>
    //  <identifier> varName </identifer>
    //  <symbol> ( </symbol>
    //  <symbol> [ </symbol>
    parseExpression();
    //  <symbol> } </symbol>
    //  <symbol> ] </symbol>
    //  <symbol> = </symbol>
    parseExpression();
    //  <symbol> ; </symbol>
    // </letStatement>
}

// if ( expression ) { statements } ( else { statements } )?
void Parser::parseIfStatement()
{
    // <ifStatement>
    //  <keyword> if </keyword>
    //  <symbol> ( </symbol>
    parseExpression();
    //  <symbol> ) </symbol>
    //  <symbol> { </symbol>
    parseStatements();
    //  <symbol> } </symbol>
    //  <symbol> ( </symbol>
    //  <keyword> else </keyword>
    //  <symbol> { </symbol>
    parseStatements();
    //  <symbol> } </symbol>
    //  <symbol> ) </symbol>
    // </ifStatement>
}

// while ( expression ) { statements }
void Parser::parseWhileStatement()
{
    // <whileStatement>
    //  <keyword> while </keyword>
    //  <symbol> ( </symbol>
    parseExpression();
    //  <symbol> ) </symbol>
    //  <symbol> { </symbol>
    parseStatements();
    //  <symbol> } </symbol>
    // </whileStatement>
}

// return expression? ;
void Parser::parseReturnStatement()
{
    // <returnStatement>
    //  <keyword> return </keyword>
    parseExpression();
    //  <symbol> ; </symbol>
    // </returnStatement>
}

// do subroutineCall ;
void Parser::parseDoStatement()
{
    // <doStatement>
    //  <keyword> do </keyword>
    parseSubroutineCall();
    //  <symbol> ; </symbol>
    // </doStatement>
}

// subroutineName ( expressionList ) 
//  | (className|varName) . subroutineName ( expressionList )
void Parser::parseSubroutineCall()
{
    // TODO
    parseExpressionList();
}

// (expression (, expression)*)?
void Parser::parseExpressionList()
{
    // <symbol> ( </symbol>
    parseExpression();
    string next;
    while ((next = scanner.peek().value) != ")")
    {
        // <symbol> , </symbol>
        parseExpression();
    }
    // <symbol> ) </symbol>
}

// term (op term)*
void Parser::parseExpression()
{
    parseTerm();
    // TODO
    // while ((next = scanner.peek().value) == binary op)
    // {
        // binary op
        // parseTerm();
    // }
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
    // TODO
}

