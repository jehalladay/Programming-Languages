
/*
Project 1 Crypt Arithmatic - Java Version
James Halladay

We are building a series of functions that will determine a character to integer mapping for the words
EAT, THAT, and APPLE such that EAT + THAT = APPLE.

Since we are adding a 3 digit number to a 4 digit number in order to get a 5 digit number, we can make
some assumptions about the range of valid candidate numbers for each word

EAT - 3 digit number
1.1. must be large enough to produce a 5 digit number
1.2. must be less than 1000
1.3. must be larger than 100
1.4. E cannot be 0 due to 1.3
1.5. E, A, and T must be different numbers
1.6. None of the digits are 0 due to 1.4, 2.4, and 3.3
1.7. E cannot be 9 due to 2.4
1.8. The largest number is 819 due to 1.2, 1.5, 1.7, and 3.10
1.9. there are only 7 possible candidates for EAT: 219, 319, 419, 519, 619, 719, and 819 due to 1.3, 1.5, 2.4, and 3.10


THAT - 4 digit number
2.1. must be large enough to produce a 5 digit number
2.2. must be less than 10000
2.3. must be larger than 9000 due to 1.1, 1.2, and 2.1
2.4. T must be 9 due to 2.3
2.5. T, H, and A must be different numbers
2.6. The first and last digit must be the same
2.7. H cannot be 0 due to 3.5 and 3.6
2.8. The smallest number is 9219 due to 2.4, 2.5, 2.6, 2.7 and 3.10
2.9. there are only 7 possible candidates for THAT: 9219, 9319, 9419, 9519, 9619, 9719, and 9819 due to 2.4, 2.5, 2.6, 2.7 and 3.10


APPLE - 5 digit number
3.1. must be larger than 10000
3.2. must be less than 100000
3.3. A cannot be 0 due to 3.1
3.4. The largest number produced by a 3 digit + a 4 digit number is 999 + 9999 = 10998
3.5. P must be 0 due to 3.1 and 3.4
3.6. A, P, L, E, and H must be different numbers
3.7. The second and third digit must be the same
3.8. The largest number is 10098 due to 3.5 and 3.7
3.9. the smallest number is 10023 due to 3.1, 3.5, 3.6, and 3.7
3.10. A must be 1 due to 3.8 and 3.9
3.11. L and E cannot be 0 due to 3.5 and 3.6


Rules 1.9 and 2.9 restrict the values for EAT and THAT to 7 possible candidates and the values of APPLE to 49 possible candidates,
but, since the point of this assignment is to compare run-times, we will run the program on all values withing min-max range.

We will time the program to compare against the Python and C++ versions of the program.
*/

import java.lang.Math;
import java.util.concurrent.TimeUnit;

class Range {
    public int min;
    public int max;
};

public class Main {
    /*
        Method will take a word and create a range of possible values for possible 
        numbers with the same number of digits as characters in the word.
        
        Input: word
        Output: range
    */
    static public Range digitize(String word) {
        int length = word.length();
        Range candidates = new Range();
        candidates.min = (int)Math.pow(10, length - 1);
        candidates.max = (int)Math.pow(10, length) - 1;


        return candidates;
    }


    /*
        Method will take a number and a digit and return the nth digit
            of the number.
    */
    static public int nth_digit(int num, int n) {
        int den = (int)Math.pow(10, n - 1);
        return  (num / den) % 10;
    }


    /*
        Method will take a word and a number and see if the number
            is a candidate for mapping to the word using some of the rules
            outlined above. Since the rules are unique to the word, the
            function only works for 3 words: EAT, THAT, and APPLE

        Input: word, num
        Output: True or False
    */
    static public boolean rule_check(String word, int num) {
        boolean result = false;

        if(word == "EAT") {
            if (num >= 100 && num <= 819) {
                if (nth_digit(num, 1) == 9) {
                    if (nth_digit(num, 2) != 0) {
                        if (nth_digit(num, 3) != 0) {
                            if (nth_digit(num, 1) != nth_digit(num, 2)) {
                                if (nth_digit(num, 1) != nth_digit(num, 3)) {
                                    if (nth_digit(num, 2) != nth_digit(num, 3)) {
                                        if (nth_digit(num, 3) != 9) {
                                            result = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } else if(word == "THAT") {
            if (num >= 9000 && num <= 9819) {
                if (nth_digit(num, 1) == 9) {
                    if (nth_digit(num, 1) == nth_digit(num, 4)) {
                        if (nth_digit(num, 1) != nth_digit(num, 2)) {
                            if (nth_digit(num, 1) != nth_digit(num, 3)) {
                                if (nth_digit(num, 2) != nth_digit(num, 3)) {
                                    if (nth_digit(num, 4) != 0) {
                                        result = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } else if(word == "APPLE") {
            if (num >= 10000 && num <= 10998) {
                if (nth_digit(num, 1) != 0) {
                    if (nth_digit(num, 3) == nth_digit(num, 4)) {
                        result = true;
                    }
                }
            }
        } else {
            System.out.println("Error: word not recognized");
        }

        return result;
    }


    /*
        Function takes 2 or 3 numbers and makes sure the same
            numbers are mapped to the same characters
            accross words. Assumes num1 maps to EAT,
            num2 maps to THAT and num3 maps to APPLE.

        Input: num1, num2, num3
        Output: True or False
    */
    static public boolean consistency_check(int num1, int num2, int num3) {
        boolean result = false;
        int E = nth_digit(num1, 3);
        int A = nth_digit(num1, 2);
        int T = nth_digit(num1, 1);
        int H = nth_digit(num2, 3);

        if (A == nth_digit(num2, 2)) {
            if (T == nth_digit(num2, 1)) {
                if (E == nth_digit(num3, 1)) {
                    if (A == nth_digit(num3, 5)) {
                        result = true;
                    }
                }
            }
        }

        return result;
    }


    static public void print_results(int EAT, int THAT, int APPLE) {
        System.out.println("   EAT  :   " +  EAT);
        System.out.println(" + THAT :  " +  THAT);
        System.out.println(" ______________");
        System.out.println(" = APPLE: " +  APPLE);
        System.out.println();
    }
    

    public static void main(String[] args) {
        // We will time the program to compare against the Python and C++ versions of the program.
        long startTime = System.currentTimeMillis();
        Range Eat_candidates = digitize("EAT");
        Range That_candidates = digitize("THAT");
        int APPLE = -1;

        System.out.println();
        System.out.println("Running: Project 1 - EAT + THAT = APPLE");
        System.out.println("Java Version");
        System.out.println();

        for(int EAT = Eat_candidates.min; EAT <= Eat_candidates.max; EAT++) {
            if(rule_check("EAT", EAT)) {
                for(int THAT = That_candidates.min; THAT <= That_candidates.max; THAT++) {
                    if(rule_check("THAT", THAT)) {
                        APPLE = EAT + THAT;
                        if(rule_check("APPLE", APPLE) && consistency_check(EAT, THAT, APPLE)) {
                            print_results(EAT, THAT, APPLE);
                        }
                    }
                }
            }
        }


        long endTime = System.currentTimeMillis();
        long timeElapsed = endTime - startTime;
        System.out.println("Total time: " + (timeElapsed / 1000.0) + " seconds");
    }
}