/*
Project 4: Scanner
James Halladay

Here we are building the driver to use scanner class for a simple language

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

    once the line is composed completely of tokens with terminal symbols, the next line is tokenized.
    if an error symbol appears in the line, the scan is stopped and the error is reported.
*/


#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <fstream>

#include "scanner.h"

using namespace std;


int main() {
    string file = "test1.in";
    cout << "Scanning file: " << file << endl;
    Scanner scanner(file);

    scanner.scan();
    cout << endl;
    scanner.printTokens();

    cout << "Finished Scanning" << endl;
    return 0;
}
