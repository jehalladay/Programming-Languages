'''
Project 2 Crypt Arithmatic - Python Version
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




We will time the program to compare against the C++ and Java versions of the program.
'''

import time




def digitize(word :str) -> range:
    '''
    Function will take a word and create a range of possible values for possible 
        numbers with the same number of digits as characters in the word.
    
    Input: word
    Output: list of possible values
    '''

    length = len(word)
    min = 10**(length-1)
    max = 10**length - 1

    return range(min, max+1)


def nth_digit(num: int, n: int) -> int:
    '''
    Function will take a number and a digit and return the nth digit
        of the number.
    '''
    return (num // 10**(n-1)) % 10


def rule_check(word: str, num: int) -> bool:
    '''
    Function will take a word and a number and see if the number
        is a candidate for mapping to the word using some of the rules
        outlined above. Since the rules are unique to the word, the
        function only works for 6 words: SEND, MORE, MONEY, CROSS, ROADS, 
        and DANGER.

    Input: word, num
    Output: True or False
    '''

    result = False


    if word == 'SEND':
        S, E, N, D = nth_digit(num, 4), nth_digit(num, 3), nth_digit(num, 2), nth_digit(num, 1)
        if num >= 2023 and num <= 9876:
            if S != 1 and E != 1 and N != 1 and D != 1:
                if S != E and S != N and S != D:
                    if E != N and E != D:
                        if N != D:
                            result = True


    elif word == 'MORE':
        M, O, R, E = nth_digit(num, 4), nth_digit(num, 3), nth_digit(num, 2), nth_digit(num, 1)
        if num >= 1023 and num <= 1987:
            if M == 1 and O != 1 and R != 1 and E != 1:
                if O != R and O != E:
                    if R != E:
                        result = True

    elif word == 'MONEY':
        if num >= 10234 and num <= 19876:
            M, O, N, E, Y = nth_digit(num, 5), nth_digit(num, 4), nth_digit(num, 3), nth_digit(num, 2), nth_digit(num, 1)
            if M == 1 and O != 1 and N != 1 and E != 1 and Y != 1:
                if O != N and O != E and O != Y:
                    if N != E and N != Y:
                        if E != Y:
                            result = True

    
    elif word == 'CROSS':
        if num >= 10233 and num <= 98766:
            C, R, O, S, S2 = nth_digit(num, 5), nth_digit(num, 4), nth_digit(num, 3), nth_digit(num, 2), nth_digit(num, 1)
            if S != 1 and C != 1 and R != 1 and O != 1:
                if S == S2: 
                    if C != R and C != O and C != S:
                        if R != O and R != S:
                            if O != S:
                                result = True

    elif word == 'ROADS':
        if num >= 10234 and num <= 98765:
            R, O, A, D, S = nth_digit(num, 5), nth_digit(num, 4), nth_digit(num, 3), nth_digit(num, 2), nth_digit(num, 1)
            if R != 1 and O != 1 and A != 1 and D == 1 and S != 1:
                if R != O and R != A and R != S:
                    if O != A and O != S:
                        if A != S:
                            result = True

    elif word == 'DANGER':
        if num >= 102345 and num <= 198765:
            A, N, G, E, R = nth_digit(num, 5), nth_digit(num, 4), nth_digit(num, 3), nth_digit(num, 2), nth_digit(num, 1)
            if A != 1 and N != 1 and G != 1 and E != 1 and R != 1:
                if A != N and A != G and A != E and A != R:
                    if N != G and N != E and N != R:
                        if G != E and G != R:
                            if E != R:
                                result = True

    else:
        print('Error: word not recognized')
        raise Exception('Error: word not recognized')

    return result


def consistency_check_1(num1: int, num2: int, num3: int = 0) -> bool:
    '''
    Function takes 2 or 3 numbers and makes sure the same
        numbers are mapped to the same characters
        accross words. Assumes num1 maps to SEND,
        num2 maps to MORE and num3 maps to MONEY.

    Input: num1, num2, num3 (optional)
    Output: True or False
    '''
    S  = nth_digit(num1, 4)
    E  = nth_digit(num1, 3)
    N  = nth_digit(num1, 2)
    D  = nth_digit(num1, 1)

    O  = nth_digit(num2, 3)
    R  = nth_digit(num2, 2)
    E2 = nth_digit(num2, 1)
    
    O2 = nth_digit(num3, 4)
    N2  = nth_digit(num3, 3)
    E3 = nth_digit(num3, 2)
    Y  = nth_digit(num3, 1)

    result = False

    # First we check the consistency of SEND and MORE
    if E == E2:
        if S != O and S != R:
            if N != O and N != R:
                if D != O and D != R:
                    if num3 == 0:
                        result = True
                        
                    # Now we check the consistency of SEND, MORE, and MONEY
                    else:
                        if E == E3 and O == O2 and N == N2:
                            if S != Y and D != Y:
                                if R != Y:
                                    result = True
    return result

def consistency_check_2(num1: int, num2: int, num3: int = 0) -> bool:
    '''
    Function takes 2 or 3 numbers and makes sure the same
        numbers are mapped to the same characters
        accross words. Assumes num1 maps to CROSS,
        num2 maps to ROADS and num3 maps to DANGER.

    Input: num1, num2, num3 (optional)
    Output: True or False
    '''
    C  = nth_digit(num1, 5)
    R  = nth_digit(num1, 4)
    O  = nth_digit(num1, 3)
    S  = nth_digit(num1, 2)

    R2 = nth_digit(num2, 5)
    O2 = nth_digit(num2, 4)
    A  = nth_digit(num2, 3)
    S2 = nth_digit(num2, 1)

    A2 = nth_digit(num3, 5)
    N  = nth_digit(num3, 4)
    G  = nth_digit(num3, 3)
    E  = nth_digit(num3, 2)
    R3 = nth_digit(num3, 1)

    result = False


    # First we check the consistency of CROSS and ROADS
    if R == R2 and O == O2 and S == S2:
        if C != A:
            if num3 == 0:
                result = True
                
            # Now we check the consistency of CROSS, ROADS, and DANGER
            else:
                if A == A2 and R == R3:
                    if C != N and C != G and C != E:
                        if O != N and O != G and O != E:
                            if S != N and S != G and S != E:
                                result = True

    return result   

def print_result_1(SEND: int, MORE: int, MONEY: int) -> None:

    print('   SEND : ' + str(SEND))
    print(' + MORE : ' + str(MORE))
    print(' ______________')
    print(' = MONEY: ' + str(MONEY))
    print('\n')


def print_result_2(CROSS: int, ROADS: int, DANGER: int) -> None:

    print('   CROSS : ' + str(CROSS))
    print(' + ROADS : ' + str(ROADS))
    print(' ______________')
    print(' = DANGER: ' + str(DANGER))
    print('\n')


def main()-> None:
    print()
    print('Running: Project 2 - SEND + MORE = MONEY')
    print('Python Version')
    print()

    start = time.time()

    SEND_candidates = digitize('SEND')
    MORE_candidates = digitize('MORE')
    MONEY_candidates = []

    for SEND in SEND_candidates:
        if rule_check('SEND', SEND):
            for MORE in MORE_candidates:
                if rule_check('MORE', MORE):
                    MONEY = SEND + MORE
                    if rule_check('MONEY', MONEY) and consistency_check_1(SEND, MORE, MONEY):
                        print_result_1(SEND, MORE, MONEY)
                        MONEY_candidates.append(MONEY)


    end = time.time()

    print('Total time: ' + str(end - start) + ' sec')
    print()



    print()
    print('Running: Project 2 - CROSS + ROADS = DANGER')
    print('Python Version')
    print()

    start = time.time()


    CROSS_candidates = digitize('CROSS')
    ROADS_candidates = digitize('ROADS')
    DANGER_candidates = []

    # We check the known solution to see if rule check and consistency check are correct
    # known solution is CROSS = 98233, ROADS = 62513, DANGER = 158746

    # CROSS = 96233
    # ROADS = 62513
    # DANGER = 158746

    # print('Checking rule_check: CROSS', rule_check('CROSS', CROSS))
    # print('Checking rule_check: ROADS', rule_check('ROADS', ROADS))
    # print('Checking rule_check: DANGER', rule_check('DANGER', DANGER))
    # print()

    # print('Checking consistency_check_2: CROSS, ROADS', consistency_check_2(CROSS, ROADS))
    # print('Checking consistency_check_2: CROSS, ROADS, and DANGER', consistency_check_2(CROSS, ROADS, DANGER))

    # print('CROSS:', CROSS)
    # print('ROADS:', ROADS)
    # print('DANGER:', DANGER)


    for CROSS in CROSS_candidates:
        if rule_check('CROSS', CROSS):
            for ROADS in ROADS_candidates:
                if rule_check('ROADS', ROADS):
                    DANGER = CROSS + ROADS
                    if rule_check('DANGER', DANGER) and consistency_check_2(CROSS, ROADS, DANGER):
                        print_result_2(CROSS, ROADS, DANGER)
                        DANGER_candidates.append(DANGER)


    end = time.time()
    
    print('Total time: ' + str(end - start) + ' sec')
    print()


if __name__ == '__main__':
    main()