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

    string functionName = className + "." + className;
    int numLocals = varCount("VAR");

    writeFunction(functionName, numLocals, stream);

    if (kind.value == "constructor")
    {
        int numFields = varCount("FIELD");
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
    Token type;
    Token name;
    
    string next;

    if ((next = scanner.peek().value) != ")")
    {
        type = getNextToken();
        name = getNextToken();  

        defineSymbolTable(name.value, type.value, "ARG");
    }
    while ((next = scanner.peek().value) != ")")
    {
        token = getNextToken();
        type = getNextToken();
        name = getNextToken();  

        defineSymbolTable(name.value, type.value, "ARG");
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

void Parser::parseLetStatement()
{
    Token token;

    Token varName = getNextToken();
    string varKind = kind(varName.value);
    int varIndex = index(varName.value);
    
    string next;
    next = scanner.peek().value;
    if (next == "[")
    {
        token = getNextToken();
        parseExpression();
        token = getNextToken();

        writePush(varKind, varIndex, stream);
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

        writePop(varKind, varIndex, stream);
    }
}

void Parser::parseIfStatement()
{
    Token token;

    ifCount++;

    token = getNextToken();
    parseExpression();
    token = getNextToken();
    token = getNextToken();

    string ifString = "IF_TRUE" + to_string(ifCount) + "\n";
    writeIf(ifString, stream);
    ifString = "IF_FALSE" + to_string(ifCount) + "\n";
    writeGoto(ifString, stream);
    ifString = "IF_TRUE" + to_string(ifCount) + "\n";
    writeLabel(ifString, stream);
    parseStatements();
    ifString = "IF_END" + to_string(ifCount) + "\n";
    writeGoto(ifString, stream);
    token = getNextToken();

    ifString = "IF_FALSE" + to_string(ifCount) + "\n";
    writeLabel(ifString, stream);

    string next;
    next = scanner.peek().value;
    if (next == "else")
    {
        token = getNextToken();
        token = getNextToken();
        parseStatements();
        token = getNextToken();
    }
    
    ifString = "IF_END" + to_string(ifCount) + "\n";
    writeLabel(ifString, stream);
}

// while ( expression ) { statements }
void Parser::parseWhileStatement()
{
    Token token;

    whileCount++;
    string whileString = "WHILE" + to_string(whileCount) + "\n";
    writeLabel(whileString, stream);

    token = getNextToken();
    parseExpression();

    writeArithmetic("NOT", stream);

    token = getNextToken();
    token = getNextToken();

    whileString = "WHILE_END" + to_string(whileCount) + "\n";
    writeIf(whileString, stream);
    parseStatements();
    whileString = "WHILE" + to_string(whileCount) + "\n";
    writeGoto(whileString, stream);
    whileString = "WHILE_END" + to_string(whileCount) + "\n";
    writeLabel(whileString, stream);

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
    writeReturn(stream);
    token = getNextToken();
}

// do subroutineCall ;
void Parser::parseDoStatement()
{
    parseSubroutineCall();

    writePop("TEMP", 0, stream);

    Token token = getNextToken();
}

void Parser::parseSubroutineCall()
{
    Token token;

    Token identifier = getNextToken();
    string funcName = identifier.value;
    int numArgs = 0;

    string next;
    if ((next = scanner.peek().value) == ".")
    {
        token = getNextToken();                
        Token subName = getNextToken();

        string type = Parser::type(identifier.value);

        if (type != "NONE")
        {
            string kind =  Parser::kind(identifier.value);
            int index =  Parser::index(identifier.value);

            writePush(kindMap[kind], index, stream);

            funcName = type + "." + subName.value;
            numArgs++;
        }
        else
        {
            Token class_name = identifier;
            funcName = class_name.value + "." + subName.value;
        }
    }
    else if ((next = scanner.peek().value) == "(")
    {
        Token subName = identifier;
        funcName = className + "." + subName.value;
        numArgs++;

        writePush("POINTER", 0, stream);
    }

    token = getNextToken();
    numArgs += parseExpressionList();
    token = getNextToken();

    writeCall(funcName, numArgs, stream);
}

// (expression (, expression)*)?
int Parser::parseExpressionList()
{
    int numArgs = 0;

    string next;
    next = scanner.peek().value;
    if (next == ";")
    {
        numArgs++;
        parseExpression();
    }
    
    while ((next = scanner.peek().value) != ")")
    {
        numArgs++;
        Token token = getNextToken();
        parseExpression();
    }

    return numArgs;
}

void Parser::parseExpression()
{
    parseTerm();
    
    string next;
    vector<string> operations = {"+", "-", "*", "/", "&", "|", "<", ">", "="};
    while (find(operations.begin(), operations.end(), scanner.peek().value) != operations.end())
    {
        Token op = getNextToken();
        parseTerm();

        if (arithmeticMap.find(op.value) != arithmeticMap.end())
        {
            writeArithmetic(arithmeticMap[op.value], stream);
        }
        else if (op.value == "*")
        {
            writeCall("Math.multiply", 2, stream);
        }
        else if (op.value == "/")
        {
            writeCall("Math.divide", 2, stream);
        }
    }
}

void Parser::parseTerm()
{
    Token token;

    string next;

    if (unaryMap.find(scanner.peek().value) != unaryMap.end())
    {
        Token unary_op = getNextToken();
        parseTerm();
        writeArithmetic(unaryMap[unary_op.value], stream);
    }
    else if ((next = scanner.peek().value) == "(")
    {
        token = getNextToken(); 
        parseExpression();
        token = getNextToken(); 
    }
    else if ((next = scanner.peek().tokenType) == "INT_CONST")
    {
        token = getNextToken();
        writePush("CONST", stoi(token.value), stream);
    }
    else if ((next = scanner.peek().tokenType) == "STRING_CONST")
    {
        parseString();
    }
    else if ((next = scanner.peek().tokenType) == "KEYWORD")
    {
        parseKeyword();
    }
    else
    {
        if ((next = scanner.peek().value) == "[")
        {
            Token varName = getNextToken();

            token = getNextToken();
            parseExpression();
            token = getNextToken();

            string kind =  Parser::kind(varName.value);
            int index =  Parser::index(varName.value);
            writePush(kindMap[kind], index, stream);

            writeArithmetic("ADD", stream);
            writePop("POINTER", 1, stream);
            writePush("THAT", 0, stream);
        }
        else if (((next = scanner.peek().value) == ".") || ((next = scanner.peek().value) == "("))
        {
            parseSubroutineCall();
        }
        else
        {
            Token var = getNextToken();
            string varKind = kind(var.value);
            int varIndex = index(var.value);
            writePush(varKind, varIndex, stream);
        }
    }
}

void Parser::parseKeyword()
{
    Token keyword = getNextToken();

    if (keyword.value == "this")
    {
        writePush("POINTER", 0, stream);
    }
    else 
    {
        writePush("CONST", 0, stream);
        if (keyword.value == "true")
        {
            writeArithmetic("NOT", stream);
        }
    }
}

void Parser::parseString()
{
    Token stringTok = getNextToken();

    writePush("CONST", stringTok.value.length(), stream);
    writeCall("String.new", 1, stream);

    for (auto character : stringTok.value)
    {
        writePush("CONST", character, stream);
        writeCall("String.appendChar", 2, stream);
    }
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

int Parser::varCount(string kind)
{
    return counts[kind];
}

string Parser::kind(string name)
{
    if (subScope.find(name) != subScope.end())
    {
        auto t = subScope[name];
        return get<1>(t);
    }
    else if (fieldScope.find(name) != fieldScope.end())
    {
        auto t = fieldScope[name];
        return get<1>(t);
    }
    else if (staticScope.find(name) != staticScope.end())
    {
        auto t = staticScope[name];
        return get<1>(t);
    }
    else
    {
        return "NONE";
    }
}

string Parser::type(string name)
{
    if (subScope.find(name) != subScope.end())
    {
        auto t = subScope[name];
        return get<0>(t);
    }
    else if (fieldScope.find(name) != fieldScope.end())
    {
        auto t = fieldScope[name];
        return get<0>(t);
    }
    else if (staticScope.find(name) != staticScope.end())
    {
        auto t = staticScope[name];
        return get<0>(t);
    }
    else
    {
        return "NONE";
    }
}

int Parser::index(string name)
{
    if (subScope.find(name) != subScope.end())
    {
        auto t = subScope[name];
        return get<2>(t);
    }
    else if (fieldScope.find(name) != fieldScope.end())
    {
        auto t = fieldScope[name];
        return get<2>(t);
    }
    else if (staticScope.find(name) != staticScope.end())
    {
        auto t = staticScope[name];
        return get<2>(t);
    }
    else
    {
        return 0;
    }
}
