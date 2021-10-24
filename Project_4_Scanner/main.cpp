/*
Project 4: Scanner
James Halladay

Here we are building a scanner class
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
#include <vector>
#include <fstream>

#include "scanner.h"

using namespace std;


int main() {
    string file = "test1.in";

    cout << "Scanning file: " << file << endl;

    // Create a scanner object
    Scanner scanner(file);
    // scanner.check_reserved_test(); 
    // scanner.test2(); 
    
    scanner.scan();
    cout << endl;
    scanner.printTokens();

    cout << "Finished Scanning" << endl;
    return 0;
}






