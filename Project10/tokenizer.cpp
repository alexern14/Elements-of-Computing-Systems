#include "include/tokenizer.h"

Scanner::Scanner(string inputFile) 
{
    ifstream inFile(inputFile);

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
    // smatch match;

restart:
    
    string::iterator start = wholeFile.begin();
    advance(start, currentIndex);
    string matchString = wholeFile.substr(size_t(currentIndex));

    int regexCount = 0;
    for (regex r : regexes) 
    {
        smatch match;
        regex_search(matchString, match, r, regex_constants::match_continuous);

        if (!match.empty()) 
        {
            if ((regexCount == 0) || (regexCount == 1)) 
            {
                // advance
                int nextIndex = match.position() + match.length() + 1;
                currentIndex = nextIndex;

                goto restart;
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
                    token.value = match.str();
                    break;
                }
                else 
                {
                    regexCount++;
                    continue;
                }
            } 
            else if (regexCount == 3) 
            {
                token.tokenType = SYMBOL;
                token.value = match.str();
                break;
            } 
            else if (regexCount == 4) 
            {
                token.tokenType = INT_CONST;
                token.value = match.str();
                break;
            } 
            else if (regexCount == 5) 
            {
                token.tokenType = STRING_CONST;
                token.value = match.str();
                break;
            } 
            else if (regexCount == 6) 
            {
                token.tokenType = IDENTIFIER;
                token.value = match.str();
                break;
            }
        }
        regexCount++;
    }
    return token;
}

Token Scanner::next() 
{
    Token token = peek();

    // advance
    int nextIndex = currentIndex + token.value.length() + 1;
    currentIndex = nextIndex;

    return token;
}

string Token::toString() 
{

}