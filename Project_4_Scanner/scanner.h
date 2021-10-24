/*
Project 4: Scanner
James Halladay

Here we are building a Scanner class
for a simple language

Data types: integer
Identifiers: [a-z|A-Z[a-z|A-Z|0-9]]
Literals: [0-9]
Comments: --
Reserved Words: [begin, end, read, write]
Terminaling character: ;
Statement types:
    Assignment
    Id := Expression
*/

#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

enum TokenType {
    BeginSym ,  EndSym    ,
    ReadSym  ,  WriteSym  ,
    Id       ,  IntLiteral,
    LParen   ,  RParen    ,
    SemiColon,  Comma     ,
    PlusOp   ,  MinusOp   ,
    AssignOp ,  EofSym    ,
    Stmt     ,  Expr      ,
    OP       ,  ERROR     ,
    NOT_RESERVED 
};

const int TOKEN_NUM = 19;
const string TokenString[TOKEN_NUM]  = {
    "BeginSym" ,  "EndSym"    ,
    "ReadSym"  ,  "WriteSym"  ,
    "Id"       ,  "IntLiteral",
    "LParen"   ,  "RParen"    ,
    "SemiColon",  "Comma"     ,
    "PlusOp"   ,  "MinusOp"   ,
    "AssignOp" ,  "EofSym"    ,
    "Stmt"     ,  "Expr"      ,
    "OP"       ,  "ERROR"     ,
    "NOT_RESERVED"
};


const string Variables[3] = {
    "Stmt",     "Expr",    "OP"
};


class Token {
    private:
        string _type;
        string _value;
        TokenType _type_enum;
    public:
        Token(string type, string value);
        Token(TokenType type, string value);
        ~Token();
        string getType();
        TokenType getTypeEnum();
        string getValue();
        string toString();
};


class Scanner {
    private:
        vector<Token> _tokens;
        vector<string> reservedWords;
        vector<string> identifiers;
        vector<string> lines;
        int line_count;

        TokenType check_reserved(string);
        vector<Token> tokenize(vector<Token>);
        vector<Token> scanStmt(Token, vector<Token>);
        vector<Token> scanExpr(Token, vector<Token>);
        vector<Token> scanOp(Token, vector<Token>);

    public:
        Scanner(string filename);
        ~Scanner();

        void scan();
        vector<Token> scanLine(string);

        vector<Token> getTokens();
        void printTokens();
        void check_reserved_test();
        void test2();
};

