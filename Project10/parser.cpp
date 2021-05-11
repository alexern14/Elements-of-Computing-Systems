#include "include/parser.h"

Parser::Parser(string tokenFile, Scanner &scanner, ofstream &stream)
    : scanner(scanner), stream(stream)
{
}

// class className { classVarDec* subroutineDec* }
void Parser::parseClass()
{
    Token token;
    //  <keyword> class </keyword>
    token = getNextToken();
    //  <identifier> classname </identifier>
    className = getNextToken().value;
    //  <symbol> { </symbol>
    token = getNextToken();

    string next;
    while (((next = scanner.peek().value) == "static") || (next == "field"))
    {
        parseClassVarDec();
    }
    while (((next = scanner.peek().value) == "constructor") || (next == "method") || (next == "function"))
    {
        parseSubroutine();
    }
}

// (static|field) type varName1 (, varName2)* ;
void Parser::parseClassVarDec()
{
    Token token;
    // <keyword> static | field </keyword>
    // <identifier> varName1 </identifier>
    Token kind = getNextToken();
    Token type = getNextToken();
    Token name = getNextToken();

    defineSymbolTable(name.value, type.value, kind.value);

    string next;
    while ((next = scanner.peek().value) != ";")
    {
        token = getNextToken();
        // <identifier> varName </identifier>
        name = getNextToken();

        defineSymbolTable(name.value, type.value, kind.value);
    }
    token = getNextToken();
}

void Parser::parseSubroutine()
{
    Token token;

    Token kind = getNextToken();
    token = getNextToken();
    Token name = getNextToken();

    startSubroutine();

    if (kind.value == "method")
    {
        defineSymbolTable("instance", className, "ARG");
    }

    token = getNextToken();
    parseParameterList();
    token = getNextToken();
    token = getNextToken();

    string next;
    while ((next = scanner.peek().value) == "var")
    {
        parseVarDec();
    }

    string functionName;
    int numLocals;

    writeFunction(functionName, numLocals, stream);

    if (kind.value == "constructor")
    {
        int numFields;
        writePush("CONST", numFields, stream);
        writeCall("Memory.alloc", 1, stream);
        writePop("POINTER", 0, stream);
    }
    else if (kind.value == "method")
    {  
        writePush("ARG", 0, stream);
        writePop("POINTER", 0, stream);
    }

    parseStatements();
    token = getNextToken();
}

// ((type varName) (, type varName)*)?
void Parser::parseParameterList()
{
    Token token;
    
    string next;
    while ((next = scanner.peek().value) != ")")
    {

    }
}

// var type varName (, varName)* ;
void Parser::parseVarDec()
{
    Token token;

    token = getNextToken();
    Token type = getNextToken();
    Token name = getNextToken();

    defineSymbolTable(name.value, type.value, "VAR");

    string next;
    while ((next = scanner.peek().value) != ";")
    {
        token = getNextToken();
        name = getNextToken();
        
        defineSymbolTable(name.value, type.value, "VAR");
    }
    token = getNextToken();
}

void Parser::parseStatements()
{
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
}

// let varName ([ expression ])? = expression ;
void Parser::parseLetStatement()
{
    Token token;

    Token varName = getNextToken();
    // var kind
    // var index
    
    string next;
    next = scanner.peek().value;
    if (next == "[")
    {
        token = getNextToken();
        parseExpression();
        token = getNextToken();

        writePush();
        writeArithmetic("ADD", stream);
        writePop("TEMP", 0, stream);

        token = getNextToken();
        parseExpression();
        token = getNextToken();

        writePush("TEMP", 0, stream);
        writePop("POINTER", 1, stream);
        writePop("THAT", 0, stream);
    }
    else
    {
        token = getNextToken();
        parseExpression();
        token = getNextToken();

        writePop();
    }
}

// if ( expression ) { statements } ( else { statements } )?
void Parser::parseIfStatement()
{
    Token token;

    // if index

    token = getNextToken();
    parseExpression();
    token = getNextToken();
    token = getNextToken();

    writeIf();
    writeGoto();
    writeLabel();
    parseStatements();
    writeGoto();
    token = getNextToken();

    writeLabel();

    string next;
    next = scanner.peek().value;
    if (next == "else")
    {
        token = getNextToken();
        token = getNextToken();
        parseStatements();
        token = getNextToken();
    }
    
    writeLabel();
}

// while ( expression ) { statements }
void Parser::parseWhileStatement()
{
    Token token;

    // while index
    writeLabel();

    token = getNextToken();
    parseExpression();

    writeArithmetic();

    token = getNextToken();
    token = getNextToken();

    writeIf();
    parseStatements();
    writeGoto();
    writeLabel();

    token = getNextToken();
}

// return expression? ;
void Parser::parseReturnStatement()
{
    Token token;

    string next;
    next = scanner.peek().value;
    if (next == ";")
    {
        parseExpression();
    }
    else
    {
        writePush("CONST", 0, stream);
    }
    writeReturn();
    token = getNextToken();
}

// do subroutineCall ;
void Parser::parseDoStatement()
{
    parseSubroutineCall();

    writePop();

    Token token = getNextToken();
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
    int numargs = 0;

    string next;
    next = scanner.peek().value;
    if (next == ";")
    {
        numargs++;
        parseExpression();
    }
    
    while ((next = scanner.peek().value) != ")")
    {
        numargs++;
        Token token = getNextToken();
        parseExpression();
    }
}

// term (op term)*
void Parser::parseExpression()
{
    parseTerm();
    
    // while ((next = scanner.peek().value) == binary op)
    // {
        Token op = getNextToken();
        parseTerm();

        // if ()
        // {
            writeArithmetic();
        // }
        // else if ()
        // {
            writeCall();
        // }
        // else if ()
        // {
            writeCall();
        // }
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

Token Parser::getNextToken()
{
    Token token = scanner.next();

    return token;
}

void Parser::defineSymbolTable(string name, string type, string kind)
{
    int num;

    if (kind == "ARG")
    {
        num = counts["ARG"];
        counts["ARG"] = counts["ARG"] + 1;

        subScope[name] = make_tuple(type, kind, num);
    }
    else if (kind == "VAR")
    {
        num = counts["VAR"];
        counts["VAR"] = counts["VAR"] + 1;

        subScope[name] =make_tuple(type, kind, num);
    }
    else if (kind == "STATIC")
    {
        num = counts["STATIC"];
        counts["STATIC"] = counts["STATIC"] + 1;

        staticScope[name] = make_tuple(type, kind, num);
    }
    else if (kind == "FIELD")
    {
        num = counts["FIELD"];
        counts["FIELD"] = counts["FIELD"] + 1;

        fieldScope[name] = make_tuple(type, kind, num);
    }
}

void Parser::startSubroutine()
{
    subScope = {};
    counts["ARG"] = 0;
    counts["VAR"] = 0;
}
