#include "include/tokenizer.h"

Scanner::Scanner(string inputFIle) 
{
    ifstream inFile(inputFIle);

    if (!inFile.is_open()) 
    {
        exit(1);
    }

    string line;
    while (getline(inFile, line)) 
    {
        wholeFile += line;
        wholeFile += " ";
    }

    currentIndex = 0;
}

Token Scanner::peek() 
{
    Token token;
    smatch match;

    string::const_iterator start = wholeFile.begin() + currentIndex;
    string::const_iterator end = wholeFile.end();

    int regexCount = 0;
    for (regex r : regexes) 
    {
        // regex_search(wholeFile, match, r);
        regex_search(start, end, match, r);

        if (!match.empty()) 
        {
            if ((regexCount == 0) || (regexCount == 1)) 
            {
                // advance
                int nextIndex = match.position() + match.length();
                currentIndex = nextIndex;

                token = peek();
            } 
            else if (regexCount == 2) 
            {
                string compare = match.str();
                if ((compare == "class") || (compare == "constructor") || (compare == "function") || (compare == "method")
                    || (compare == "field") || (compare == "static") || (compare == "var") || (compare == "int")
                    || (compare == "char") || (compare == "boolean") || (compare == "void") || (compare == "true")
                    || (compare == "false") || (compare == "null") || (compare == "this") || (compare == "let")
                    || (compare == "do") || (compare == "if") || (compare == "else") || (compare == "while")
                    || (compare == "return"))
                {
                    token.tokenType = KEYWORD;
                }
            } 
            else if (regexCount == 3) 
            {
                token.tokenType = SYMBOL;
            } 
            else if (regexCount == 4) 
            {
                token.tokenType = INT_CONST;
            } 
            else if (regexCount == 5) 
            {
                token.tokenType = STRING_CONST;
            } 
            else if (regexCount == 6) 
            {
                token.tokenType = IDENTIFIER;
            }
        token.value = match.str();
        }
        regexCount++;
    }
    return token;
}

Token Scanner::next() 
{
    Token token = peek();

    // advance
    int nextIndex = currentIndex + token.value.length();
    currentIndex = nextIndex;

    return token;
}

string Token::toString() {}