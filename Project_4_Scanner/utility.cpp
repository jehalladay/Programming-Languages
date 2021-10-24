/*
Project 4: Scanner
James Halladay

Here we are implementing various utility functions
    for a scanner.

*/

#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <vector>
#include <map>

using namespace std;


/*
    Function takes a string and strips it of whitespace
        currently screened chars:
            - space
            - tab
            - newline

    input: string str
    output: string
*/
string strip(string str) {
    string stripped = "";
    for (int i = 0; i < (int)str.length(); i++) {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
            stripped += str[i];
        }
    }
    // cout << "stripped: " << stripped << "%%" <<endl;
    return stripped;
}