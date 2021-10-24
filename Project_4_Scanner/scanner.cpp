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
#include <cctype>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>

#include "scanner.h"
#include "utility.h"

using namespace std;

// Token class methods

Token::Token(string type, string value) {
    for(int i = 0; i < TOKEN_NUM; i++) {
        if(type == TokenString[i]) {
            _type_enum = (TokenType)i;
            i = TOKEN_NUM;
        }
    }

    _type = type;
    _value = value;
}

Token::Token(TokenType type, string value) {
    _type_enum = type;
    _type = TokenString[type];
    _value = value;
}

Token::~Token() { }


string Token::getType() {
    return _type;
}

TokenType Token::getTypeEnum() {
    return _type_enum;
}


string Token::getValue() {
    return _value;
}


string Token::toString() {
    return _type + ": " + _value;
}





// Scanner class methods

/*
    Constructor for the Scanner class

    Takes in a file name and opens the file
    Throws error if file cannot be opened
    Stores and counts every line in the file
        for future processing

    input: string fileName
*/
Scanner::Scanner(string fileName) {
    ifstream file(fileName);

    if(file.is_open()) {
        string line;
        while(getline(file, line)) {
            lines.push_back(line);
        }

        line_count = lines.size();
    } else {
        cout << "Error: File not found" << endl;
        throw fileName + " not found";
    }

    file.close();
}


Scanner::~Scanner() { }

/*
    Method to process all stored lines

    Iterates through all lines in the file
    and assigns a token to each recognized symbol
    Throws error if a symbol is not recognized
*/
void Scanner::scan() {
    vector<Token> tokens;
    for(int i = 0; i < (int)lines.size(); i++) {
        string line = lines[i];
        vector<Token> string_tokens;

        int line_number = i + 1;

        string_tokens = scanLine(line);

        // print out tokens
        for(int k = 0; k < (int)string_tokens.size(); k++) {
            cout << line_number << "-" << k << ": " << string_tokens[k].toString() << endl;
        }

        // test token vector for errors, report line if error detected
        for(int j = 0; j < (int)string_tokens.size(); j++) {
            if(string_tokens[j].getTypeEnum() == ERROR) {
                cout << "Error: Invalid token on line " << line_number << endl;
                cout << "\tError: " << string_tokens[j].getValue() << endl;
                throw "Invalid token on line " + to_string(line_number);
            } else if(string_tokens[j].getTypeEnum() == Stmt) {
                cout << "Error: Stmt escaped recursive scan on line " << line_number << endl;
                cout << "\tStmt: " << string_tokens[j].getValue() << endl;
                throw "Stmt escaped recursive scan on line " + to_string(line_number);
            } else if(string_tokens[j].getTypeEnum() == Expr) {
                cout << "Error: Expr escaped recursive scan on line " << line_number << endl;
                cout << "\tExpr: " << string_tokens[j].getValue() << endl;
                throw "Expr escaped recursive scan on line " + to_string(line_number);
            } else if(string_tokens[j].getTypeEnum() == OP) {
                cout << "Error: OP escaped recursive scan on line " << line_number << endl;
                cout << "\tOP: " << string_tokens[j].getValue() << endl;
                throw "OP escaped recursive scan on line " + to_string(line_number);
            } else if(string_tokens[j].getTypeEnum() == NOT_RESERVED) {
                cout << "Error: NOT_RESERVED escaped recursive scan on line " << line_number << endl;
                cout << "\tNOT_RESERVED: " << string_tokens[j].getValue() << endl;
                throw "NOT_RESERVED escaped recursive scan on line " + to_string(line_number);
            }
        }

        // Now that we know the tokens are valid, add them to the master token vector
        tokens.insert(tokens.end(), string_tokens.begin(), string_tokens.end());
    }

    _tokens = tokens;    
}

/*
    Method prints out all tokens along with their values separated by new lines
*/
void Scanner::printTokens() {
    for (int i = 0; i < (int)_tokens.size(); i++) {
        cout << _tokens[i].toString() << endl;
    }
}

/*
    Getter for the token vector
*/
vector<Token> Scanner::getTokens() {
    return _tokens;
}


/*
    Method checks a string to see if it is a reserved word

    input: string word
    output: TokenType
*/
TokenType Scanner::check_reserved(string input) {
    TokenType out;
    string value = input;
    value = strip(value);

    if(value == "BEGIN") {
        out = BeginSym;
    } else if(value == "END") {
        out = EndSym;
    } else if(value == "READ") {
        out = ReadSym;
    } else if(value == "WRITE") {
        out = WriteSym;
    } else {
        out = NOT_RESERVED;
    }

    return out;
}


/*
    Method to scan a single line of code

    Handles the internal recursive descent tokenization

    input: string line
    output: vector<Token>
*/
vector<Token> Scanner::scanLine(string input) {
    vector<Token> tokens;
    string line;

    tokens.push_back(Token(Stmt, input));
    tokens = tokenize(tokens);

    return tokens;
}

/*
    Method tokenizes a line of code

    Internal use only: called by scanLine 
    
    Initially called on a vector of length 1 containing a
        token of type Stmt with the entire line
    Recursively descends through the line and
        tokenizes each symbol
    Base case is a vector of tokens with
        no stmt, expr, or op tokens
    Every time a Stmt, Expr, or OP is found,
        it is tokenized and added to the vector
        a flag to descend is set to true
    If descend is true, begin new recursive call

    input: vector<Token> tokens
    output: vector<Token>
*/
vector<Token> Scanner::tokenize(vector<Token> tokens) {
    vector<Token> out;
    bool descend = false;
    for(int i = 0; i < (int)tokens.size(); i++) {
        string token = tokens[i].getValue();
        TokenType type = tokens[i].getTypeEnum();

        switch(type) {
            case Stmt:
                out = scanStmt(tokens[i], out);
                descend = true;
                break;
            case Expr:
                // out = scanExpr(tokens[i], out);
                out.push_back(Token(Id, "Unprocessed Expression: " + token));
                descend = true;
                break;
            case OP:
                // out = scanOp(tokens[i], out);
                if(token == "+") {
                    out.push_back(Token(PlusOp, token));
                } else if (token == "-") {
                    out.push_back(Token(MinusOp, token));
                } else {
                    out.push_back(Token(ERROR, token + " %% Unrecognized Operator"));
                }
                break;
            default:
                out.push_back(tokens[i]);
                break;
        }
    }

    if(descend) {
        out = tokenize(out);
    }

    return out;
}


/*
    Method tokenizes a statement

    Internal use only: called by tokenize

    Takes the input statement token and executes the cfg production rules on it
        The new tokens are added to the vector passed in the second parameter
        That vector is then returned

    Assumes words take the forms below, other code is written to verify 
        that the string is really an Id, an Expr, or a Reserved word.

    Executes the rules of the CFG:
        stmt -> ID := expr ;
        stmt -> BEGIN
        stmt -> END
        stmt -> READ ( ID, ID, ... ) ;
        stmt -> WRITE ( expr, expr, ... ) ;

    input: Token stmt, vector<Token> out
    output: vector<Token>
*/
vector<Token> Scanner::scanStmt(Token token, vector<Token> out) {
    string line = token.getValue(), word, subword, carry = "";
    bool list_flag = false, Assignment_flag = false;
    cout << line << endl;
    stringstream line_stream(line);
    TokenType type, list_type;
    vector<Token> paren_stack;

    subword = line.substr(0, 4);

    while(line_stream >> word) {
        type = check_reserved(word);

        // check for comments, if found, end line
        if(word[0] == '-' && word[1] == '-') {
            cout << "comment found" << endl;
            while(line_stream >> word) { 
                word = "";
            }
            // check last item on output vector, if it isnt a semicolon or empty, push an error
            if(out.size() > 0) {
                TokenType last_type = out[out.size() - 1].getTypeEnum();
                if(last_type != SemiColon || last_type != BeginSym || last_type != EndSym) {
                    out.push_back(Token(ERROR, "Comment terminates line with no terminal SemiColon"));
                }
            }
            break;
        }

        // BEGIN and END are delimiters and cannot be followed by anything except for comments
        if(type == BeginSym || type == EndSym) {
            out.push_back(Token(type, word));
            if(line_stream >> word) {
                if(word[0] != '-' || word[1] != '-') {
                    out.push_back(Token(ERROR, word + " %% Only comments may follow a BEGIN or END statement"));
                }  else {
                    while(line_stream >> word) { 
                        word = "";
                     }
                } 
            }
        } 

        /* 
            READ and WRITE are both followed by parenthesis
                READ has a list of Ids
                WRITE has a list of Expressions
                Both may be followed by a space then a parenthesis or a parenthesis

            READ and WRITE have lists of Ids or Exprs separated by commas, we first
                check for the start paren of the list, then set the flag to true
                and create a Read/Write token with the word and a LParen token then
                push them to the output vector
        */ 
        else if(type == ReadSym || type == WriteSym) {
            if (type == ReadSym) {
                list_type = Id;
            } else {
                list_type = Expr;
            }
            out.push_back(Token(type, word));
            if(line_stream >> word) {
                if(word[0] != '(') {
                    out.push_back(Token(ERROR, word));
                }
                list_flag = true;
                paren_stack.push_back(Token(LParen, "("));
                if((int)word.size() > 1) {
                    carry = word.substr(1);
                }
            } else {
                out.push_back(Token(ERROR, word));
            }
        } else if(check_reserved(word.substr(0, 4)) == ReadSym) {
            if (word[4] == '(') {
                out.push_back(Token(ReadSym, word.substr(0, 4)));
                out.push_back(Token(LParen, "("));
                paren_stack.push_back(Token(LParen, "("));
                list_flag = true;
                type = ReadSym;
                list_type = Id;
                
                // we "carry" the rest of the word down into the list processing block
                carry = word.substr(5);

            }else {
                cout << "found Id: " << word << endl;
                Assignment_flag = true;
            }
        } else if(check_reserved(word.substr(0, 5)) == WriteSym) {
            if(word[5] == '(') {
                out.push_back(Token(WriteSym, word.substr(0, 5)));
                out.push_back(Token(LParen, "("));
                paren_stack.push_back(Token(LParen, "("));
                list_flag = true;
                type = WriteSym;
                list_type = Expr;

                // we "carry" the rest of the word down into the list processing block
                carry = word.substr(6);

            } else if(isdigit(word[5]) || isalpha(word[5]) || word[5] == '_') {
                cout << "found Id: " << word << endl;
                Assignment_flag = true;
            }
        } else if(type == NOT_RESERVED) {
            cout << "not reserved found" << word << endl;
                Assignment_flag = true;
        }


        /* 
            The only other kind of statement allowed is an assignment statement
                Assignments are of the form:        Id := Expr ;
                We assume the Id is the first word in the line
                    it must be followed by :=
                We assume the expression is everything else terminated by a semicolon

            This is done by scanning the words in the line
                every character before a : is the id,
                    if no : is found, the id is the whole word
                    then the next word must start with :=
                every character after a = and before a ; is the expression
        */
       if(Assignment_flag){
           string c = "";
           int i = 0;
           for(; i < (int)word.length(); i++) {
               if(word[i] == ':') {
                   i++;
                   break;
               }
               c += word[i];
            }
            out.push_back(Token(Id, c));

            // now we have out Id, we remove the id from the word
            // and check to find the assignment operator
            if(i >= (int)word.length()) {
                line_stream >> word;
            } else {
                word = word.substr(i - 1);
            }

            if(word[0] != ':' && word[1] != '=') {
                out.push_back(Token(ERROR, word + " %% Assignment must be of the form Id := Expr"));
            } else {
                word = word.substr(2);
            }

            out.push_back(Token(AssignOp, ":="));

            // now everything until a semicolon is detected is an expression
            c = "";
            do {
                for(i = 0; i < (int)word.length(); i++) {
                    if(word[i] == ';') {
                        out.push_back(Token(Expr, c));
                        out.push_back(Token(SemiColon, ";"));
                        i++;
                        while(line_stream >> word) { 
                            word = "";
                         }
                    }
                    c += word[i];
                }
            } while (line_stream >> word);
       }    


        /*  List processing block
            here we will scan through the words in the list character by character
                if we find a left paren, a paren token is pushed to the stack
                if we find a right paren a paren token is popped from the stack
                if the stack is empty after finding a right paren, we have found the end of the list
                After the right paren, the call to read or write must be terminated with a semicolon

                items in the list are separated by commas and assumed to be Ids or Exprs
        */                
        if(list_flag) {
            // cout << "start processing list" << endl;
            while(line_stream >> word && list_flag) {
                if(carry != "") {
                    word.insert(0, carry);
                    carry = "";
                }
                // cout << "list item: " << word << endl;
                string c = "";

                for(int i = 0; i < (int)word.length(); i++) {
                    if(word[i] == '(') {
                        paren_stack.push_back(Token(LParen, "("));
                    } else if(word[i] == ')') {
                        paren_stack.pop_back();
                        if (paren_stack.empty()) {
                            // cout << "hello" << endl;
                            if(c != "") {
                                out.push_back(Token(list_type, c));
                                c = "";
                            } else {
                                out.push_back(Token(ERROR, word));
                            }
                            out.push_back(Token(RParen, ")"));
                            if((int)word.size() > i + 1) {
                                if(word[i + 1] == ';') {
                                    out.push_back(Token(SemiColon, ";"));
                                } else {
                                    out.push_back(Token(ERROR, word));
                                }
                            } else if(line_stream >> word) {
                                if(word[0] != ';') {
                                    out.push_back(Token(ERROR, word));
                                } else {
                                    out.push_back(Token(SemiColon, ";"));
                                }
                            } else {
                                out.push_back(Token(ERROR, word));
                            }
                            list_flag = false;
                            break;
                        }
                    } else if(word[i] == ',') {
                        if(c == "") {
                            out.push_back(Token(ERROR, word));
                        } else {
                            // cout << "found comma " << TokenString[list_type] << endl;
                            out.push_back(Token(list_type, c));
                            out.push_back(Token(Comma, ","));
                            c = "";
                        }
                    } else {
                        c += word[i];
                    }
                }
            }
        }
    }


    

    // out.push_back(token);
    return out;
    
    
}


void Scanner::check_reserved_test() {
    vector<Token> tokens;
    cout << "Starting tests on check_reserved internal method" << endl << endl;

    cout << "\tExpecting BeginSym:\t" << TokenString[check_reserved("BEGIN")] << endl;
    cout << "\tExpecting BeginSym:\t" << TokenString[check_reserved("BEGIN ")] << endl;
    cout << "\tExpecting BeginSym:\t" << TokenString[check_reserved("BEGIN\t")] << endl;
    cout << "\tExpecting BeginSym:\t" << TokenString[check_reserved("BEGIN \t")] << endl;
    cout << "\tExpecting BeginSym:\t" << TokenString[check_reserved("BEGIN\n")] << endl;
    cout << "\tExpecting BeginSym:\t" << TokenString[check_reserved("BEGIN \n")] << endl;
    
    cout << "\tExpecting NOT_RESERVED:\t" << TokenString[check_reserved("BEGIN_")] << endl;
    cout << "\tExpecting NOT_RESERVED:\t" << TokenString[check_reserved("BEGINA")] << endl;
    cout << "\tExpecting NOT_RESERVED:\t" << TokenString[check_reserved("BEGINa")] << endl;
    cout << "\tExpecting NOT_RESERVED:\t" << TokenString[check_reserved("BEGIN0")] << endl;

    cout << "\tExpecting EndSym:\t" << TokenString[check_reserved("END")] << endl;
    cout << "\tExpecting EndSym:\t" << TokenString[check_reserved("END ")] << endl;
    cout << "\tExpecting EndSym:\t" << TokenString[check_reserved("END\t")] << endl;
    cout << "\tExpecting EndSym:\t" << TokenString[check_reserved("END \t")] << endl;
    cout << "\tExpecting EndSym:\t" << TokenString[check_reserved("END\n")] << endl;
    cout << "\tExpecting EndSym:\t" << TokenString[check_reserved("END \n")] << endl;
    
    cout << "\tExpecting NOT_RESERVED:\t" << TokenString[check_reserved("END_")] << endl;
    cout << "\tExpecting NOT_RESERVED:\t" << TokenString[check_reserved("ENDA")] << endl;
    cout << "\tExpecting NOT_RESERVED:\t" << TokenString[check_reserved("ENDa")] << endl;
    cout << "\tExpecting NOT_RESERVED:\t" << TokenString[check_reserved("END0")] << endl;

    cout << "\tExpecting ReadSym:\t" << TokenString[check_reserved("READ")] << endl;
    cout << "\tExpecting ReadSym:\t" << TokenString[check_reserved("READ ")] << endl;
    cout << "\tExpecting ReadSym:\t" << TokenString[check_reserved("READ\t")] << endl;
    cout << "\tExpecting ReadSym:\t" << TokenString[check_reserved("READ \t")] << endl;
    cout << "\tExpecting ReadSym:\t" << TokenString[check_reserved("READ\n")] << endl;
    cout << "\tExpecting ReadSym:\t" << TokenString[check_reserved("READ \n")] << endl;
    
    cout << "\tExpecting NOT_RESERVED:\t" << TokenString[check_reserved("READ_")] << endl;
    cout << "\tExpecting NOT_RESERVED:\t" << TokenString[check_reserved("READA")] << endl;
    cout << "\tExpecting NOT_RESERVED:\t" << TokenString[check_reserved("READa")] << endl;
    cout << "\tExpecting NOT_RESERVED:\t" << TokenString[check_reserved("READ0")] << endl;

    cout << "\tExpecting WriteSym:\t" << TokenString[check_reserved("WRITE")] << endl;
    cout << "\tExpecting WriteSym:\t" << TokenString[check_reserved("WRITE ")] << endl;
    cout << "\tExpecting WriteSym:\t" << TokenString[check_reserved("WRITE\t")] << endl;
    cout << "\tExpecting WriteSym:\t" << TokenString[check_reserved("WRITE \t")] << endl;
    cout << "\tExpecting WriteSym:\t" << TokenString[check_reserved("WRITE\n")] << endl;
    cout << "\tExpecting WriteSym:\t" << TokenString[check_reserved("WRITE \n")] << endl;
    
    cout << "\tExpecting NOT_RESERVED:\t" << TokenString[check_reserved("WRITE_")] << endl;
    cout << "\tExpecting NOT_RESERVED:\t" << TokenString[check_reserved("WRITEA")] << endl;
    cout << "\tExpecting NOT_RESERVED:\t" << TokenString[check_reserved("WRITEa")] << endl;
    cout << "\tExpecting NOT_RESERVED:\t" << TokenString[check_reserved("WRITE0")] << endl;




    cout << endl << "Finished tests on check_reserved internal method" << endl;
}

void Scanner::test2() {
    vector<Token> tokens;
    cout << "Starting tests enum types" << endl << endl;

    
    cout << "Testing enums mapping for Reserved words" << endl;
    cout << "\tExpecting BeginSym:\t" << TokenString[BeginSym] << endl;
    cout << "\tExpecting EndSym:\t" << TokenString[EndSym] << endl;

    
    cout << "Checking enum: Stmt" << (Stmt < TOKEN_NUM - (TokenType)5) << endl;
    cout << "Checking enum: Expr" << (Expr < TOKEN_NUM - (TokenType)5) << endl;
    cout << "Checking enum: OP" << (OP < TOKEN_NUM - (TokenType)5) << endl;
    cout << "Checking enum: ERROR" << (ERROR < TOKEN_NUM - (TokenType)5) << endl;
    cout << "Checking enum: NOT_RESERVED" << (NOT_RESERVED < TOKEN_NUM - (TokenType)5) << endl;
    cout << "Checking enum: EofSym" << (EofSym < TOKEN_NUM - (TokenType)5) << endl;
    cout << "Checking enum: BeginSym" << (BeginSym < TOKEN_NUM - (TokenType)5) << endl;
    
    
    cout << endl << "Finished tests on enum types" << endl;
}
    