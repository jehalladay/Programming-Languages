/*
Project 4: Scanner
James Halladay

Here we are building the header of a scanner class for a simple language

Data types: integer
Identifiers: [a-z|A-Z[a-z|A-Z|0-9]]
Literals: [0-9]
Variables: {stmt, expr, op}
Terminal Symbols: {BeginSym, EndSym, ReadSym, WriteSym, Id,  IntLiteral, LParen, RParen, SemiColon, Comma, PlusOp, MinusOp, AssignOp}
Comments: --
Reserved Words: [BEGIN, END, READ, WRITE]
Terminaling character: ;
Statement types:
    stmt -> ID := expr ;
    stmt -> BEGIN
    stmt -> END
    stmt -> READ ( Id, Id, ... ) ;
    stmt -> WRITE ( expr, expr, ... ) ;

Expression types:
    expr -> expr OP expr
    expr -> ( expr )
    expr -> Id
    expr -> IntLiteral

Operator types:
    op -> +
    op -> -

Errors are given as a token type but are caught by the scanner
    When caught, the line they were caught on is reported. This is a feature that allows easy 
    debugging. This feature also made it easier to debug the scanner itself when coding it.

Recursive Descent is implemented in the scanner. 
    Each line is tokenized as a statement. 
    Each statement is tokenized into terminal symbols and expresssions.
    Each expression is tokenized into terminal symbols, expressions, and operators. 

    once the line is composed completely of tokens with terminal symbols, 
    the next line is tokenized.
*/

#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <vector>

using namespace std;


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
        bool is_Id(string);
        vector<Token> tokenize(vector<Token>);
        vector<Token> scanStmt(Token, vector<Token>);
        vector<Token> scanExpr(Token, vector<Token>);

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
