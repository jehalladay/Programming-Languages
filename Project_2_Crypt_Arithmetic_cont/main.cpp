/*
Project 2 Crypt Arithmatic - C++ Version
James Halladay

We are building a series of functions that will determine a character to integer mapping for the words
SEND, MORE, and MONEY such MORE SEND + MORE = MONEY and the words CROSS, ROADS, DANGER such that
CROSS + ROADS = DANGER.

Since we are adding a 3 digit number to a 4 digit number in order to get a 5 digit number, we can make
some assumptions about the range of valid candidate numbers for each word

SEND - 4 digit number
    1.1. must be large enough to produce a 5 digit number
    1.2. must be less than or equal to 9876
    1.3. must be larger than or equal to 2023
    1.4. no digit can equal another digit
    1.5. no digit can be 1 due to rule 3.3

MORE - 4 digit number
    2.1. must be large enough to produce a 5 digit number
    2.2. must be less than or equal to 1987
    2.3. must be larger than or equal to 1023
    2.4. M must be 1 due to rule 3.3
    2.5. no digit can equal another digit

MONEY - 5 digit number
    3.1. must be larger than 10234
    3.2. must be less than 19876
    3.3. M must be 1 because we are adding two 4 digit numbers together
    3.4. no digit can equal another digit


CROSS - 5 digit number
    4.1. must be larger than or equal to 10233
    4.2. must be less than or equal to 98766
    4.3. The last two digits must be the same
    4.4. The first four digits must be different

ROADS - 5 digit number
    5.1. must be larger than or equal to 10234
    5.2. must be less than or equal to 98765
    5.3. D must be 1 due to rule 6.3
    5.4. no digit can equal another digit

DANGER - 6 digit number
    6.1. must be larger than or equal to 102345
    6.2. must be less than or equal to 198765
    6.3. D must be 1 because we are adding two 5 digit numbers together
    6.4. no digit can equal another digit



We will time the program to compare against the Python and Java versions of the program.
*/

#include <iostream>
#include <string>
#include <chrono>
#include <math.h>


using namespace std;
using namespace std::chrono;


struct range {
    int min;
    int max;
};


range digitize(string word);
int nth_digit(int num, int n);
bool rule_check(string word, int num);
bool consistency_check_1(int num1, int num2, int num3);
bool consistency_check_2(int num1, int num2, int num3);
void print_results_1(int SEND, int MORE, int MONEY);
void print_results_2(int CROSS, int ROADS, int DANGER);







int main() {

    auto start = high_resolution_clock::now();

    range SEND_candidates = digitize("SEND");
    range MORE_candidates = digitize("MORE");
    int MONEY = -1;

    cout << endl 
         << "Running: Project 2 - SEND + MORE = MONEY" << endl
         << "C++ Version" << endl << endl;
    
    for(int SEND = SEND_candidates.min; SEND <= SEND_candidates.max; SEND++) {
        if(rule_check("SEND", SEND)) {
            for(int MORE = MORE_candidates.min; MORE <= MORE_candidates.max; MORE++) {
                if(rule_check("MORE", MORE)) {
                    MONEY = SEND + MORE;
                    if(rule_check("MONEY", MONEY) && consistency_check_1(SEND, MORE, MONEY)) {
                        print_results_1(SEND, MORE, MONEY);
                    }
                }
            }
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);


    cout << "Total time: " << duration.count() / 1000.0 << " sec" << endl;


    start = high_resolution_clock::now();

    range CROSS_candidates = digitize("CROSS");
    range ROADS_candidates = digitize("ROADS");
    int DANGER = -1;

    cout << endl 
         << "Running: Project 2 - CROSS + ROADS = DANGER" << endl
         << "C++ Version" << endl << endl;
    
    for(int CROSS = CROSS_candidates.min; CROSS <= CROSS_candidates.max; CROSS++) {
        if(rule_check("CROSS", CROSS)) {
            for(int ROADS = ROADS_candidates.min; ROADS <= ROADS_candidates.max; ROADS++) {
                if(rule_check("ROADS", ROADS)) {
                    DANGER = CROSS + ROADS;
                    if(rule_check("DANGER", DANGER) && consistency_check_2(CROSS, ROADS, DANGER)) {
                        print_results_2(CROSS, ROADS, DANGER);
                    }
                }
            }
        }
    }

    stop = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(stop - start);


    cout << "Total time: " << duration.count() / 1000.0 << " sec" << endl;

    return 0;
}



/*
    Function will take a word and create a range of possible values for possible 
    numbers with the same number of digits as characters in the word.
    
    Input: word
    Output: range
*/
range digitize(string word) {

    int length = word.length();
    range candidates;

    candidates.min = pow(10, length - 1);
    candidates.max = pow(10, length) - 1;


    return candidates;
}


/*
    Function will take a number and a digit and return the nth digit
        of the number.
*/
int nth_digit(int num, int n) {
    int den = pow(10, n - 1);
    return  (num / den) % 10;
}


/*
    Function will take a word and a number and see if the number
        is a candidate for mapping to the word using some of the rules
        outlined above. Since the rules are unique to the word, the
        function only works for 6 words: SEND, MORE, MONEY, CROSS, ROADS, 
        and DANGER.

    Input: word, num
    Output: True or False
*/
bool rule_check(string word, int num) {
    bool result = false;

    if(word == "SEND") {
        int S = nth_digit(num, 4), E = nth_digit(num, 3), N = nth_digit(num, 2), D = nth_digit(num, 1);
        if(num >= 2023 && num <= 9876) {
            if(S != 1 && E != 1 && N != 1 && D != 1) {
                if(S != E && S != N && S != D) {
                    if(E != N && E != D) {
                        if(N != D) {
                            result = true;
                        }
                    }
                }
            }
        }
    } else if(word == "MORE") {
        int M = nth_digit(num, 4), O = nth_digit(num, 3), R = nth_digit(num, 2), E = nth_digit(num, 1);
        if(num >= 1023 && num <= 1987) {
            if(M == 1 && O != 1 && R != 1 && E != 1) {
                if(O != R && O != E) {
                    if(R != E) {
                        result = true;
                    }
                }
            }
        }
    } else if(word == "MONEY") {
        int M = nth_digit(num, 5), O = nth_digit(num, 4), N = nth_digit(num, 3), E = nth_digit(num, 2), Y = nth_digit(num, 1);
        if (num >= 10234 && num <= 99876) {
            if(M == 1 && O != 1 && N != 1 && E != 1 && Y != 1) {
                if(O != N && O != E && O != Y) {
                    if(N != E && N != Y) {
                        if(E != Y) {
                            result = true;
                        }
                    }
                }
            }
        }
    } else if(word == "CROSS") {
        int C = nth_digit(num, 5), R = nth_digit(num, 4), O = nth_digit(num, 3), S = nth_digit(num, 2), S2 = nth_digit(num, 1);
        if(num >= 10233 && num <= 98766) {
            if(C != 1 && R != 1 && O != 1 && S != 1) {
                if(S == S2) {
                    if(C != R && C != O && C != S) {
                        if(R != O && R != S) {
                            if(O != S) {
                                result = true;
                            }
                        }
                    }
                }
            }
        }
    } else if(word == "ROADS") {
        int R = nth_digit(num, 5), O = nth_digit(num, 4), A = nth_digit(num, 3), D = nth_digit(num, 2), S = nth_digit(num, 1);
        if(num >= 10234 && num <= 98765) {
            if(R != 1 && O != 1 && A != 1 && D == 1 && S != 1) {
                if(R != O && R != A && R != S) {
                    if(O != A && O != S) {
                        if(A != S) {
                            result = true;
                        }
                    }
                }
            }
        }
    } else if(word == "DANGER") {
        int D = nth_digit(num, 6), A = nth_digit(num, 5), N = nth_digit(num, 4), G = nth_digit(num, 3), E = nth_digit(num, 2), R = nth_digit(num, 1);
        if(num >= 102345 && num <= 198765) {
            if(D == 1 && A != 1 && N != 1 && G != 1 && E != 1 && R != 1) {
                if(A != N && A != G && A != E && A != R) {
                    if(N != G && N != E && N != R) {
                        if(G != E && G != R) {
                            if(E != R) {
                                result = true;
                            }
                        }
                    }
                }
            }
        }
    } else {
        cout << "Error: word not recognized" << endl;
    }

    return result;
}


/*
    Function takes 2 or 3 numbers and makes sure the same
        numbers are mapped to the same characters
        accross words. Assumes num1 maps to SEND,
        num2 maps to MORE and num3 maps to MONEY.

    Input: num1, num2, num3
    Output: True or False
*/
bool consistency_check_1(int num1, int num2, int num3) {
    bool result = false;
    int S = nth_digit(num1, 4), E = nth_digit(num1, 3), N = nth_digit(num1, 2), D = nth_digit(num1, 1);
    int O = nth_digit(num2, 3), R = nth_digit(num2, 2), E2 = nth_digit(num2, 1);
    int O2 = nth_digit(num3, 4), N2 = nth_digit(num3, 3), E3 = nth_digit(num3, 2), Y = nth_digit(num3, 1);

    if(E == E2) {
        if (S != O && S != R) {
            if (N != O && N != R) {
                if (D != O && D != R) {
                    if (E == E3 && O == O2 && N == N2) {
                        if (S != Y && D != Y) {
                            if (R != Y) {
                                result = true;
                            }
                        }
                    }
                }
            }
        }
    }

    return result;
}

/*
    Function takes 2 or 3 numbers and makes sure the same
        numbers are mapped to the same characters
        accross words. Assumes num1 maps to CROSS,
        num2 maps to ROADS and num3 maps to DANGER.

    Input: num1, num2, num3
    Output: True or False
*/
bool consistency_check_2(int num1, int num2, int num3) {
    bool result = false;
    int C = nth_digit(num1, 5), R = nth_digit(num1, 4), O = nth_digit(num1, 3), S = nth_digit(num1, 2);
    int R2 = nth_digit(num2, 5), O2 = nth_digit(num2, 4), A = nth_digit(num2, 3), S2 = nth_digit(num2, 1);
    int A2 = nth_digit(num3, 5), N = nth_digit(num3, 4), G = nth_digit(num3, 3), E = nth_digit(num3, 2), R3 = nth_digit(num3, 1);
    
    if(R == R2 && O == O2 && S == S2) {
        if(C != A) {
            if(A == A2 && R == R3) {
                if(C != N && C != G && C != E) {
                    if(O != N && O != G && O != E) { 
                        if(S != N && S != G && S != E) {
                            result = true;
                        }
                    }
                }
            }
        }
    }    
    
    return result;
}


void print_results_1(int SEND, int MORE, int MONEY) {
    cout << "   SEND :   " << SEND   << endl
         << " + MORE :  "  << MORE  << endl
         << " ______________"       << endl
         << " = MONEY: "   << MONEY << endl << endl;
}



void print_results_2(int CROSS, int ROADS, int DANGER) {
    cout << "   CROSS :   " << CROSS   << endl
         << " + ROADS :  "  << ROADS  << endl
         << " ______________"       << endl
         << " = DANGER: "   << DANGER << endl << endl;
}



