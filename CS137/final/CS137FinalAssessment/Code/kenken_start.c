/*  
   *****************************
   Do not include more libraries
   *****************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// ASSUMPTION:  INPUT is always valid, no wrong characters or out of range numbers.

// The sections of this file:
//     1. Header Portion (provided functions and structs to use but not modify)
//     2. Functions to solve by students (write your code here!)
//     3. Visual debugging tool (GUI for debugging)
//     4. Testing functions (you are encouraged to add more test cases here)

/*
    *******************************************************************************
    1. Header Portion / Structures definitions and Functions Students Do NOT Change
    *******************************************************************************
 */

typedef struct
{
    /*
    guess struct, containing an unsigned int number and 
    char letter. represents a guess on the board
     */
    char letter;         // char
    unsigned int number; // nat
} guess;

typedef struct
{
    /*
    a box on the puzzle board
     */
    char letter;         // char
    unsigned int number; // nat
    bool guess;          // bool, if true then a guess g exists
    guess g;             // guess struct
} entry;

typedef struct
{
    /*
    a constraint on the puzzle board, with char letter here
    corresponding to the char letter in struct entry
     */
    char letter;         // char
    unsigned int number; // nat
    char symbol;         // any of: + - = * /
} constraint;

typedef struct
{
    /*
    the puzzle itself, tracking the board and constraints, as well
    as the size/length of both
     */
    unsigned int boardSize;      // nat
    unsigned int constraintsLen; // nat
    entry **board;               // array of (array of (entry))
    constraint *constraints;     // array of (constraint)
} puzzle;

typedef struct
{
    /*
    position struct, with an int x and an int y as
    coordinates on a puzzle board
     */
    int x; // int
    int y; // int
} posn;

typedef struct
{
    /*
    custom array of ints that also tracks length of array
     */
    int *arr;         // array of (int)
    unsigned int len; // nat
} numArr;

typedef struct
{
    /*
    custom array of puzzles that also tracks length of array
     */
    puzzle *arr;      // array of (puzzle)
    unsigned int len; // nat
} puzArr;

// *********************************
// Provided functions. DO not modify
// *********************************

constraint constraint_create(char letter, int result, char symbol)
{
    /*
    function to create a constraint
     */
    constraint c;
    c.letter = letter;
    c.number = result;
    c.symbol = symbol;
    return c;
}

entry entry_create(char letter, int number, bool guess, char guessLetter, int guessNumber)
{
    /*
    function to create an entry
     */
    entry e;
    e.letter = letter;
    e.number = number;
    e.guess = guess;
    e.g.letter = guessLetter;
    e.g.number = guessNumber;
    return e;
}

puzzle puzzle_init(puzzle *puz, int boardSize, int constraintsLen)
{
    /*
    function to initialize a puzzle
     */
    puz->boardSize = boardSize;
    puz->constraintsLen = constraintsLen;
    if (puz->boardSize < 1)
    {
        puz->board = NULL;
    }
    else
    {
        puz->board = calloc(boardSize, sizeof(entry *));
        for (int i = 0; i < boardSize; ++i)
        {
            puz->board[i] = calloc(boardSize, sizeof(entry));
        }
    }
    if (puz->constraintsLen < 1)
    {
        puz->constraints = NULL;
    }
    else
    {
        puz->constraints = calloc(constraintsLen, sizeof(constraint));
    }
    return *puz;
}

void puzzle_set(puzzle *puz, int size, entry entries[], int len, constraint cons[])
{
    /*
    function called by read_puzzle, which takes the entries and constraints 
    read in as arrays, and puts them into the puzzle object puz
     */
    puz->boardSize = size;
    puz->constraintsLen = len;
    int num = 0;
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            puz->board[y][x].guess = entries[num].guess;
            puz->board[y][x].letter = entries[num].letter;
            puz->board[y][x].number = entries[num].number;
            puz->board[y][x].g.letter = entries[num].g.letter;
            puz->board[y][x].g.number = entries[num].g.number;
            num++;
        }
    }
    for (int i = 0; i < len; ++i)
    {
        puz->constraints[i].letter = cons[i].letter;
        puz->constraints[i].number = cons[i].number;
        puz->constraints[i].symbol = cons[i].symbol;
    }
}

void puzzle_print(const puzzle *puz)
{
    /*
    prints the puzzle puz to stdout
     */
    printf("board size: %d\n", puz->boardSize);
    printf("constraints length: %d\n", puz->constraintsLen);
    for (unsigned int y = 0; y < puz->boardSize; ++y)
    {
        for (unsigned int x = 0; x < puz->boardSize; ++x)
        {
            printf("entry {%d, %d}: ", x, y);
            if (puz->board[y][x].number || puz->board[y][x].letter == '0')
            {
                printf("letter N/A, num %d, ", puz->board[y][x].number);
            }
            else
            {
                printf("letter %c, num %d, ", puz->board[y][x].letter, puz->board[y][x].number);
            }
            if (puz->board[y][x].guess)
            {
                printf("guess true, guess letter %c, guess num %d\n", puz->board[y][x].g.letter, puz->board[y][x].g.number);
            }
            else
            {
                printf("guess false\n");
            }
        }
    }
    for (unsigned int i = 0; i < puz->constraintsLen; ++i)
    {
        printf("con %d: %c %d %c\n", i + 1, puz->constraints[i].letter, puz->constraints[i].number, puz->constraints[i].symbol);
    }
}

void puzzle_destroy(puzzle *puz)
{
    /*
    frees all memory allocated by puzzle puz
     */
    // frees the board
    for (unsigned int i = 0; i < puz->boardSize; ++i)
    {
        free(puz->board[i]);
        puz->board[i] = NULL;
    }
    free(puz->board);
    puz->board = NULL;

    // frees the constraints
    free(puz->constraints);
    puz->constraints = NULL;
}

bool puzzle_equal(const puzzle p1, const puzzle p2)
{
    /*
    checks equality between two puzzles p1 and p2
     */
    if (p1.boardSize != p2.boardSize)
    {
        return false;
    }
    else if (p1.constraintsLen != p2.constraintsLen)
    {
        return false;
    }
    for (unsigned int y = 0; y < p1.boardSize; ++y)
    {
        for (unsigned int x = 0; x < p1.boardSize; ++x)
        {
            if (p1.board[y][x].letter != p2.board[y][x].letter)
            {
                return false;
            }
            else if (p1.board[y][x].guess != p2.board[y][x].guess)
            {
                return false;
            }
            else if (p1.board[y][x].number != p2.board[y][x].number)
            {
                return false;
            }
            else if (p1.board[y][x].guess == p2.board[y][x].guess)
            {
                if (p1.board[y][x].g.letter != p2.board[y][x].g.letter)
                {
                    return false;
                }
                else if (p1.board[y][x].g.number != p2.board[y][x].g.number)
                {
                    return false;
                }
            }
        }
    }
    for (unsigned int i = 0; i < p1.constraintsLen; ++i)
    {
        if (p1.constraints[i].number != p2.constraints[i].number)
        {
            return false;
        }
        else if (p1.constraints[i].letter != p2.constraints[i].letter)
        {
            return false;
        }
        else if (p1.constraints[i].symbol != p2.constraints[i].symbol)
        {
            return false;
        }
    }
    return true;
}

puzzle puzzle_deep_copy(puzzle *puz)
{
    /*
    creates a deep copy of puzzle puz called new
     */
    puzzle new;
    new = puzzle_init(&new, puz->boardSize, puz->constraintsLen);
    new.boardSize = puz->boardSize;
    new.constraintsLen = puz->constraintsLen;
    for (unsigned int y = 0; y < new.boardSize; ++y)
    {
        for (unsigned int x = 0; x < new.boardSize; ++x)
        {
            new.board[y][x].number = puz->board[y][x].number;
            new.board[y][x].letter = puz->board[y][x].letter;
            new.board[y][x].guess = puz->board[y][x].guess;
            new.board[y][x].g.letter = puz->board[y][x].g.letter;
            new.board[y][x].g.number = puz->board[y][x].g.number;
        }
    }
    for (unsigned int cons = 0; cons < new.constraintsLen; ++cons)
    {
        new.constraints[cons].letter = puz->constraints[cons].letter;
        new.constraints[cons].number = puz->constraints[cons].number;
        new.constraints[cons].symbol = puz->constraints[cons].symbol;
    }
    return new;
}

entry **board_deep_copy(puzzle *puz)
{
    /*
    creates a deep copy of the board of puzzle puz called board
    and returns it
     */
    entry **board;
    board = calloc(puz->boardSize, sizeof(entry *));
    for (unsigned int i = 0; i < puz->boardSize; ++i)
    {
        board[i] = calloc(puz->boardSize, sizeof(entry));
    }
    for (unsigned int y = 0; y < puz->boardSize; ++y)
    {
        for (unsigned int x = 0; x < puz->boardSize; ++x)
        {
            board[y][x].number = puz->board[y][x].number;
            board[y][x].letter = puz->board[y][x].letter;
            board[y][x].guess = puz->board[y][x].guess;
            board[y][x].g.letter = puz->board[y][x].g.letter;
            board[y][x].g.number = puz->board[y][x].g.number;
        }
    }
    return board;
}

void puzArr_destroy(puzArr *puzzles)
{
    for (unsigned int i = 0; i < puzzles->len; ++i)
    {
        puzzle_destroy(&puzzles->arr[i]);
    }
    if (puzzles->arr != NULL)
    {
        free(puzzles->arr);
        puzzles->arr = NULL;
    }
    puzzles->len = 0;
}

bool puzzle_in(const puzzle p, const puzzle *puzzles, const int len)
{
    /*
    function to check if a puzzle p is in an array of puzzles puzzles
     */
    for (int i = 0; i < len; ++i)
    {
        if (puzzle_equal(p, puzzles[i]))
        {
            return true;
        }
    }
    return false;
}

puzzle read_puzzle_from_file(char *fileName)
{
    /*
    reads information from fname file and returns the info as Puzzle struct;
    */
    FILE *file;
    file = fopen(fileName, "r");
    assert(file != NULL);

    int size = 0;
    fscanf(file, "%d", &size);
    if (size > 0)
    {
        entry *entries = calloc(size * size, sizeof(entry));
        constraint *constraints = calloc(size * size, sizeof(constraint)); // fits size*size #constraints
        int constraintLen = 0;
        char c = fgetc(file);
        while (c != '\n')
        {
            c = fgetc(file);
        }

        for (int i = 0; i < size * size; ++i) // gets us the entries
        {
            c = fgetc(file);
            if (c == 'G')
            { // option 1: the entry is a guess in the form Guess(letter,number)
                for (int l = 0; l < 6; ++l)
                {
                    c = fgetc(file);
                }

                int number = 0;
                fscanf(file, ",%d)", &number);
                entries[i] = entry_create(c, 0, true, c, number);
                c = fgetc(file); // extra read to account for space
            }
            else if (c >= '0' && c <= '9') // option 2: the entry is a given number
            {
                int number = c - '0';
                c = fgetc(file);
                while (c >= '0' && c <= '9')
                {
                    number = (number * 10) + (c - '0');
                    c = fgetc(file);
                }
                entries[i] = entry_create('0', number, false, '0', 0);
            }
            else // option 3: the entry is a char letter
            {
                entries[i] = entry_create(c, 0, false, '0', 0);
                c = fgetc(file); // extra read to account for space
            }

            while (c == '\r') // accounting for different new lines on different machines
            {
                c = fgetc(file);
            }
        }
        int num = 0;
        char operator;
        while (fscanf(file, "%c %d %c\n", &c, &num, &operator) != EOF) // gets us the constraints
        {
            constraints[constraintLen] = constraint_create(c, num, operator);
            ++constraintLen;
        }
        fclose(file);

        puzzle puz = puzzle_init(&puz, size, constraintLen);
        puzzle_set(&puz, size, entries, constraintLen, constraints);

        free(entries);
        entries = NULL;
        free(constraints);
        constraints = NULL;
        return puz;
    }
    else
    {
        // returning puzzle of size 0
        fclose(file);
        puzzle p;
        p.boardSize = 0;
        p.constraintsLen = 0;
        p.board = NULL;
        p.constraints = NULL;
        return p;
    }
}

void print_sol_to_file(puzzle *puz, char fname[])
{
    /*
    prints the solution to Puzzle puz in fname file
    */
    FILE *file;
    file = fopen(fname, "w");
    assert(file != NULL);
    if (puz->boardSize == 0)
    {
        fprintf(file, "No Solution...");
    }
    for (unsigned int x = 0; x < puz->boardSize; ++x)
    {
        for (unsigned int y = 0; y < puz->boardSize; ++y)
        {
            fprintf(file, "%d", puz->board[x][y].number);
            if (y < puz->boardSize - 1)
            {
                fprintf(file, "  ");
            }
            else if (y >= puz->boardSize - 1 && x < puz->boardSize - 1)
            {
                fprintf(file, "\n");
            }
        }
    }
    fclose(file);
}

/*
    *********************************
    2. Functions To Solve by students

    Don’t forget to free any allocated 
    memory once you are done with it.
    *********************************
 */

// a)
posn find_blank(puzzle *puz)
{
    /*
    returns the position of the first blank space in puz.
    If no cells are blank, return {-2, -2}
    If the first constraint has only guesses on the board,
    find_blank returns {-1, -1}.
    */
}

// b)
numArr used_in_row(puzzle *puz, posn pos)
{
    /*
    returns an array of numbers and the array length 
    (through a struct called numArr),
    used in the same row as (x,y) position, pos,
    in the given puz. 
     */
}

numArr used_in_col(puzzle *puz, posn pos)
{
    /*
    returns an array of numbers and the array length 
    (through a struct called numArr),
    used in the same column as (x,y) position, pos,
    in the given puz.
     */
}

// c)
numArr available_vals(puzzle *puz, posn pos)
{
    /*
    returns an array of valid entries, as well as that array length
    (through a struct called numArr) for the (x,y) position, pos, 
    of the consumed puzzle, puz.
     */
}

// d)
entry **place_guess(puzzle *puz, posn pos, int val)
{
    /*
    fills in the (x,y) position, pos, of the puzzle, puz, with a guess with
    value, val
     */
    // should return a clean new copy without aliasing
    entry **board = board_deep_copy(puz);

    // Complete your code here....

    return board;
}

// Provided. Do NOT modify

puzzle fill_in_guess(puzzle *puz, posn pos, int val)
{
    /*
    fills in the pos Position of puz's board with a guess with value val
     */
    puzzle res = puzzle_deep_copy(puz);
    res.board = place_guess(&res, pos, val);

    puzzle_destroy(puz); // freeing the old puzzle now that we made a deep copy res
    return res;
}

// e)
bool guess_valid(puzzle *puz)
{
    /*
    determines if the guesses in puz satisfy their constrain
     */

    // get a list of all the guessed numbers
}

// f)
/*  Hint
Create a function to pop/remove the first constrain from the array of constraints. 
done using malloc by reallocating a new array and
copying the elements after the first over.
then use it to solve apply_guess
*/

puzzle apply_guess(puzzle *puz)
{
    /*
    converts all guesses in puz into their corresponding numbers and removes 
    the first contraint from puz's list of contraints
     */
    puzzle new = puzzle_deep_copy(puz);

    // Complete your code here....

    return new;
}

// g)
/*  Hint:
Consider using some of the functions defined in a-f
*/

puzArr neighbors(puzzle *puz)
{
    /*
    returns a list of next puzzles called struct puzArr after puz in the 
    implicit graph
     */
    puzzle temp = puzzle_deep_copy(puz);
    puzArr nbrs;

    // Complete your code here....

    puzzle_destroy(&temp);
    return nbrs;
}

// ***************************************
// SOLVE KENKEN. Don't Modify this portion
// You don't need to understand how it works
// to complete your a-g functions, however,
// if your functions are correct then solve_kenken
// will solve the puzzle correctly
// ***************************************

// Helper
puzArr puzzle_pop(puzArr *puzzles)
{
    /*
    pops the first puzzle from the array of puzzles. 
    done using malloc by reallocating a new array and
    copying the elements after the first over.
    also decrements length by 1
     */
    if (puzzles->len == 1)
    {
        puzzle_destroy(&puzzles->arr[0]);
        puzzles->len = 0;
        return *puzzles;
    }
    else if (puzzles->len == 0)
    {
        puzzles->arr = NULL;
        return *puzzles;
    }
    puzArr new;
    new.arr = malloc(sizeof(puzzle) * (puzzles->len - 1));
    new.len = puzzles->len - 1;
    for (unsigned int i = 0; i < puzzles->len - 1; ++i)
    {
        new.arr[i] = puzzle_deep_copy(&puzzles->arr[i + 1]);
    }
    // freeing the old puzArr struct
    puzArr_destroy(puzzles);
    return new;
}

puzzle solve_kenken(puzzle *orig)
{
    /*
    finds the solution to a KenKen puzzle, orig, or returns NULL 
    if there is no solution.  
     */
    if (orig->boardSize == 0)
    {
        return *orig;
    }

    puzArr to_visit;
    puzArr visited;
    to_visit.arr = malloc(sizeof(puzzle));
    to_visit.arr[0] = puzzle_deep_copy(orig);
    to_visit.len = 1;
    visited.len = 0;
    visited.arr = NULL;
    while (to_visit.len)
    {
        posn blank = find_blank(&to_visit.arr[0]);
        if (blank.x == -2 && blank.y == -2)
        {
            // option 1: solution found
            puzzle soln = puzzle_deep_copy(&to_visit.arr[0]);
            puzArr_destroy(&to_visit);
            puzArr_destroy(&visited);
            puzzle_destroy(orig);
            return soln;
        }
        else if (puzzle_in(to_visit.arr[0], visited.arr, visited.len))
        {
            // option 2: the puzzle has already been visited
            to_visit = puzzle_pop(&to_visit);
        }
        else
        {
            // option 3: finding new neighbors and adding them to to_visit
            puzArr nbrs = neighbors(&to_visit.arr[0]);
            puzArr new;
            new.len = 0;
            new.arr = malloc(sizeof(puzzle) * nbrs.len);
            for (unsigned int i = 0; i < nbrs.len; ++i)
            {
                if (puzzle_in(nbrs.arr[i], visited.arr, visited.len) == false)
                {
                    // only puts in unvisited neighbors
                    new.arr[i] = puzzle_deep_copy(&nbrs.arr[i]);
                    new.len++;
                }
            }

            // add current puzzle to new_visited
            puzArr new_visited;
            new_visited.len = visited.len + 1;
            new_visited.arr = malloc(sizeof(puzzle) * new_visited.len);
            new_visited.arr[0] = puzzle_deep_copy(&to_visit.arr[0]);
            for (unsigned int i = 0; i < visited.len; ++i)
            {
                new_visited.arr[i + 1] = puzzle_deep_copy(&visited.arr[i]);
            }
            to_visit = puzzle_pop(&to_visit); // pop current puzzle from to_visit

            // add new puzzles to new_to_visit
            puzArr new_to_visit;
            new_to_visit.len = new.len + to_visit.len;
            new_to_visit.arr = malloc(sizeof(puzzle) * new_to_visit.len);
            for (unsigned int i = 0; i < new.len; ++i)
            {
                new_to_visit.arr[i] = puzzle_deep_copy(&new.arr[i]);
            }
            int temp = 0;
            for (unsigned int i = new.len; i < new_to_visit.len; ++i)
            {
                new_to_visit.arr[i] = puzzle_deep_copy(&to_visit.arr[temp]);
                temp++;
            }

            // free old to_visit and visited puzArrs
            puzArr_destroy(&to_visit);
            puzArr_destroy(&visited);

            // let new_to_visit become to_visit, and new_visited become visited
            to_visit.arr = malloc(sizeof(puzzle) * new_to_visit.len);
            to_visit.len = new_to_visit.len;
            for (unsigned int i = 0; i < to_visit.len; ++i)
            {
                to_visit.arr[i] = puzzle_deep_copy(&new_to_visit.arr[i]);
            }
            visited.len = new_visited.len;
            visited.arr = malloc(sizeof(puzzle) * new_visited.len);
            for (unsigned int i = 0; i < visited.len; ++i)
            {
                visited.arr[i] = puzzle_deep_copy(&new_visited.arr[i]);
            }

            // free used puzzle arrays
            puzArr_destroy(&new);
            puzArr_destroy(&new_to_visit);
            puzArr_destroy(&new_visited);
            puzArr_destroy(&nbrs);
        }
    }
    // option 4: no solution, return puzzle of size 0
    puzzle p = {0, 0, NULL, NULL};
    puzArr_destroy(&to_visit);
    puzArr_destroy(&visited);
    puzzle_destroy(orig);
    return p;
}

/*
    ****************************************************************
    DO NOT SUBMIT ANYTHING BEYOND THIS LINE!
    SUBMIT ONLY UNTIL THE END OF FUNCTION solve_kenken
    ****************************************************************
 */

/*
    ****************************************************************
    3. Visual tool for debugging, shows the process of the puzzle
    being solved
    You do not need to access/understand any of these functions to complete the
    assignment, do not modify any of them either
    ****************************************************************
 */

constraint constraint_find(puzzle *puz, char c)
{
    /*
    finds the constraint that corresponds to the letter c
    otherwise returns an empty constraint
     */
    for (unsigned int i = 0; i < puz->constraintsLen; ++i)
    {
        if (puz->constraints[i].letter == c)
        {
            return puz->constraints[i];
        }
    }
    constraint con = constraint_create(0, 0, 0);
    return con;
}

void print_remaining(int num, int remaining)
{
    /*
    helper function to puzzle_print_solving, which prints the
    right number of blank spaces based on the number of digits of
    int num
     */
    int count = 0;
    while (num != 0)
    {
        num = num / 10;
        ++count;
    }
    for (int i = 0; i < remaining - count; ++i)
    {
        printf(" ");
    }
}

void delay(double seconds)
{
    /*
    delays the program by a number of seconds given by double seconds
    We recommend:
        a delay of 1 for puzzles smaller than 4x4
        a delay of 0.5 for puzzles of size 4x4
        a delay of 0 or 0.1 for puzzles larger than 4x4
     */
    double milli_seconds = CLOCKS_PER_SEC * seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds)
        ;
}

bool posn_in(posn pos, posn *positions, int len)
{
    /*
    checks if a posn pos exists in the array of posn positions
     */
    for (int i = 0; i < len; ++i)
    {
        if (pos.x == positions[i].x && pos.y == positions[i].y)
        {
            return true;
        }
    }
    return false;
}

void puzzle_print_solving(puzzle *puz, posn *positions, int len, puzzle *orig, double delay_time)
{
    /*
    prints the puzzle puz to stdout as it would look on a board, 
    and highlighting the position being changed, given by posn pos.
    helper function to solve_kenken_verbose, so for your individual tests
    use puzzle_print() instead
     */

    if (len == 0 || (positions[0].x == -1 && positions[0].y == -1))
    {
        return; // only print puzzle when changes occur
    }

    delay(delay_time); // delay time between puzzle prints, measured in seconds
    if (positions[0].x == -2 && positions[0].y == -2)
    {
        printf("\033[0;32m"); // solution found, change colour to green
    }
    else if (positions[0].x == -3 && positions[0].y == -3)
    {
        printf("\033[0;31m"); // no solution, change colour to red
    }

    // prints the initial horizontal line
    for (unsigned int x = 0; x < puz->boardSize; ++x)
    {
        printf("|-------");
    }
    printf("|\n");

    // prints the rest of the puzzle
    for (unsigned int y = 0; y < puz->boardSize; ++y)
    {
        // printing constraints
        printf("|");
        for (unsigned int x = 0; x < orig->boardSize; ++x)
        {
            if (orig->board[y][x].number == 0)
            {
                constraint con = constraint_find(orig, orig->board[y][x].letter);
                if (con.number != 0)
                {
                    printf("%d%c", con.number, con.symbol);
                    print_remaining(con.number, 6);
                }
                else
                {
                    print_remaining(0, 7);
                }
            }
            else
            {
                print_remaining(0, 7);
            }
            if (x == orig->boardSize - 1)
            {
                printf("|\n");
            }
            else if (orig->board[y][x].letter != 0 && orig->board[y][x].letter == orig->board[y][x + 1].letter)
            {
                printf(" ");
            }
            else
            {
                printf("|");
            }
        }

        // printing entry
        printf("|");
        for (unsigned int x = 0; x < puz->boardSize; ++x)
        {
            posn current = {x, y};
            if (puz->board[y][x].number)
            {
                if (posn_in(current, positions, len))
                {
                    printf("\033[1;34m"); // print in bold blue
                    printf("   %d", puz->board[y][x].number);
                    printf("\033[0m");
                }
                else
                {
                    printf("   %d", puz->board[y][x].number);
                }
                print_remaining(puz->board[y][x].number, 4);
            }
            else if (puz->board[y][x].guess)
            {
                if (posn_in(current, positions, len))
                {
                    printf("\033[1;34m"); // print in bold blue
                    printf("   %d", puz->board[y][x].g.number);
                    printf("\033[0m"); // resetting colour
                }
                else
                {
                    printf("   %d", puz->board[y][x].g.number);
                }
                print_remaining(puz->board[y][x].g.number, 4);
            }
            else
            {
                printf("       ");
            }
            if (x == puz->boardSize - 1)
            {
                printf("|\n");
            }
            else if (puz->board[y][x].letter != 0 && puz->board[y][x].letter == puz->board[y][x + 1].letter)
            {
                printf(" ");
            }
            else
            {
                printf("|");
            }
        }

        // printing extra line
        printf("|");
        for (unsigned int x = 0; x < puz->boardSize; ++x)
        {
            printf("       ");
            if (x == puz->boardSize - 1)
            {
                printf("|\n");
            }
            else if (puz->board[y][x].letter != 0 && puz->board[y][x].letter == puz->board[y][x + 1].letter)
            {
                printf(" ");
            }
            else
            {
                printf("|");
            }
        }

        // printing horizontal line
        printf("|");
        for (unsigned int x = 0; x < puz->boardSize; ++x)
        {
            if (y == puz->boardSize - 1)
            {
                printf("-------|");
            }
            else if (puz->board[y][x].letter != 0 && puz->board[y][x].letter == puz->board[y + 1][x].letter)
            {
                printf("       |");
            }
            else
            {
                printf("-------|");
            }
            if (x == puz->boardSize - 1)
            {
                printf("\n");
            }
        }
    }

    if (positions[0].x == -2 && positions[0].y == -2) // resetting colour
    {
        printf("\033[0m");
    }
    else if (positions[0].x == -3 && positions[0].y == -3)
    {
        printf("\033[0m");
    }
    printf("\n\n");
}

puzzle solve_kenken_visual(puzzle *orig, double delay_time)
{
    /*
    recommended to resize your terminal window to fit the entire puzzle 
    being printed

    solve_kenken_visual takes a puzzle, and a time delay measured in seconds.
    pass in a different time delay number to change the delay between puzzle prints.
    We recommend:
        a delay of 1 for puzzles smaller than 4x4
        a delay of 0.5 for puzzles of size 4x4
        a delay of 0 or 0.1 for puzzles larger than 4x4
     */
    if (orig->boardSize == 0)
    {
        return *orig;
    }

    puzArr to_visit;
    puzArr visited;
    to_visit.arr = malloc(sizeof(puzzle));
    to_visit.arr[0] = puzzle_deep_copy(orig);
    to_visit.len = 1;
    visited.len = 0;
    visited.arr = NULL;
    while (to_visit.len)
    {
        posn blank = find_blank(&to_visit.arr[0]);
        if (blank.x == -2 && blank.y == -2)
        {
            // option 1: solution found
            puzzle soln = puzzle_deep_copy(&to_visit.arr[0]);
            posn *changed = malloc(sizeof(posn));
            changed[0].x = blank.x;
            changed[0].y = blank.y;
            puzzle_print_solving(&soln, changed, 1, orig, delay_time);
            puzArr_destroy(&to_visit);
            puzArr_destroy(&visited);
            puzzle_destroy(orig);
            free(changed);
            changed = NULL;
            return soln;
        }
        else if (puzzle_in(to_visit.arr[0], visited.arr, visited.len))
        {
            // option 2: the puzzle has already been visited
            to_visit = puzzle_pop(&to_visit);
        }
        else
        {
            // option 3: finding new neighbors and adding them to to_visit
            // puzzle_print_solving(&to_visit.arr[0], blank);
            puzArr nbrs = neighbors(&to_visit.arr[0]);
            puzArr new;
            new.len = 0;
            new.arr = malloc(sizeof(puzzle) * nbrs.len);
            for (unsigned int i = 0; i < nbrs.len; ++i)
            {
                if (puzzle_in(nbrs.arr[i], visited.arr, visited.len) == false)
                {
                    // only puts in unvisited neighbors
                    new.arr[i] = puzzle_deep_copy(&nbrs.arr[i]);
                    new.len++;
                }
            }

            // add current puzzle to new_visited
            puzArr new_visited;
            new_visited.len = visited.len + 1;
            new_visited.arr = malloc(sizeof(puzzle) * new_visited.len);
            new_visited.arr[0] = puzzle_deep_copy(&to_visit.arr[0]);
            for (unsigned int i = 0; i < visited.len; ++i)
            {
                new_visited.arr[i + 1] = puzzle_deep_copy(&visited.arr[i]);
            }
            to_visit = puzzle_pop(&to_visit); // pop current puzzle from to_visit

            // add new puzzles to new_to_visit
            puzArr new_to_visit;
            new_to_visit.len = new.len + to_visit.len;
            new_to_visit.arr = malloc(sizeof(puzzle) * new_to_visit.len);
            for (unsigned int i = 0; i < new.len; ++i)
            {
                new_to_visit.arr[i] = puzzle_deep_copy(&new.arr[i]);
            }
            int temp = 0;
            for (unsigned int i = new.len; i < new_to_visit.len; ++i)
            {
                new_to_visit.arr[i] = puzzle_deep_copy(&to_visit.arr[temp]);
                temp++;
            }

            // free old to_visit and visited puzArrs
            puzArr_destroy(&to_visit);
            puzArr_destroy(&visited);

            // let new_to_visit become to_visit, and new_visited become visited
            to_visit.arr = malloc(sizeof(puzzle) * new_to_visit.len);
            to_visit.len = new_to_visit.len;
            for (unsigned int i = 0; i < to_visit.len; ++i)
            {
                to_visit.arr[i] = puzzle_deep_copy(&new_to_visit.arr[i]);
            }
            visited.len = new_visited.len;
            visited.arr = malloc(sizeof(puzzle) * new_visited.len);
            for (unsigned int i = 0; i < visited.len; ++i)
            {
                visited.arr[i] = puzzle_deep_copy(&new_visited.arr[i]);
            }

            // generate changed values for the print function
            posn *changed = malloc(sizeof(posn) * orig->boardSize * orig->boardSize);
            int count = 0;
            if (to_visit.len > 0)
            {
                for (unsigned int y = 0; y < orig->boardSize; ++y)
                {
                    for (unsigned int x = 0; x < orig->boardSize; ++x)
                    {
                        // if guess changed
                        if (visited.arr[0].board[y][x].g.number != to_visit.arr[0].board[y][x].g.number)
                        {
                            changed[count].x = x;
                            changed[count].y = y;
                            count++;
                        }
                        // else if num changed, not include applied guess
                        else if (visited.arr[0].board[y][x].number != to_visit.arr[0].board[y][x].number && visited.arr[0].board[y][x].guess == to_visit.arr[0].board[y][x].guess)
                        {
                            changed[count].x = x;
                            changed[count].y = y;
                            count++;
                        }
                    }
                }
                puzzle_print_solving(&to_visit.arr[0], changed, count, orig, delay_time);
            }
            else
            {
                changed[0].x = blank.x;
                changed[0].y = blank.y;
                puzzle_print_solving(&visited.arr[0], changed, 1, orig, delay_time);
            }
            free(changed);
            changed = NULL;

            // free used puzzle arrays
            puzArr_destroy(&new);
            puzArr_destroy(&new_to_visit);
            puzArr_destroy(&new_visited);
            puzArr_destroy(&nbrs);
        }
    }
    // option 4: no solution, return puzzle of size 0
    posn *changed = malloc(sizeof(posn));
    changed[0].x = -3;
    changed[0].y = -3;
    puzzle_print_solving(orig, changed, 1, orig, delay_time);
    free(changed);
    changed = NULL;
    puzzle p = {0, 0, NULL, NULL};
    puzArr_destroy(&to_visit);
    puzArr_destroy(&visited);
    puzzle_destroy(orig);
    return p;
}

/*
    ********************************************************************
    4. TESTING FUNCTIONS, You are encouraged to add more test cases here
    ********************************************************************
 */

bool testing_a(void)
{
    // test case 1
    puzzle p1 = read_puzzle_from_file("puzzle2.txt");
    posn blank1 = find_blank(&p1);
    if (blank1.x != 0 || blank1.y != 0)
    {
        puzzle_destroy(&p1);
        return false;
    }
    puzzle_destroy(&p1);

    // test case 2
    puzzle p2 = read_puzzle_from_file("puzzle3partial1.txt");
    posn blank2 = find_blank(&p2);
    if (blank2.x != -2 || blank2.y != -2)
    {
        puzzle_destroy(&p2);
        return false;
    }
    puzzle_destroy(&p2);

    // test case 3
    puzzle p3 = read_puzzle_from_file("puzzle1partial1.txt");
    posn blank3 = find_blank(&p3);
    if (blank3.x != -1 || blank3.y != -1)
    {
        puzzle_destroy(&p3);
        return false;
    }
    puzzle_destroy(&p3);

    return true;
}

bool testing_b(void)
{
    // test case 1
    puzzle p1 = read_puzzle_from_file("puzzle2partial1.txt");
    posn pos = {0, 1};
    numArr usedRow = used_in_row(&p1, pos);
    if (usedRow.len == 3 && usedRow.arr[0] == 1 && usedRow.arr[1] == 2 && usedRow.arr[2] == 4)
    {
        free(usedRow.arr);
        puzzle_destroy(&p1);
    }
    else
    {
        free(usedRow.arr);
        puzzle_destroy(&p1);
        return false;
    }

    // test case 2
    puzzle p2 = read_puzzle_from_file("puzzle1.txt");
    posn pos2 = {1, 1};
    numArr usedRow2 = used_in_row(&p2, pos2);
    if (usedRow2.len != 0)
    {
        free(usedRow2.arr);
        puzzle_destroy(&p2);
        return false;
    }
    free(usedRow2.arr);
    puzzle_destroy(&p2);

    // test case 3
    puzzle p3 = read_puzzle_from_file("puzzle2partial1.txt");
    posn pos3 = {1, 1};
    numArr usedCol = used_in_col(&p3, pos3);
    if (usedCol.len == 2 && usedCol.arr[0] == 2 && usedCol.arr[1] == 3)
    {
        free(usedCol.arr);
        puzzle_destroy(&p3);
    }
    else
    {
        free(usedCol.arr);
        puzzle_destroy(&p3);
        return false;
    }

    // test case 4
    puzzle p4 = read_puzzle_from_file("puzzle1.txt");
    posn pos4 = {1, 1};
    numArr usedCol2 = used_in_col(&p4, pos4);
    if (usedCol2.len != 0)
    {
        free(usedCol2.arr);
        puzzle_destroy(&p4);
        return false;
    }
    free(usedCol2.arr);
    puzzle_destroy(&p4);

    return true;
}

bool testing_c(void)
{
    // test case 1
    puzzle p1 = read_puzzle_from_file("puzzle2partial1.txt");
    posn pos1 = {0, 1};
    numArr available1 = available_vals(&p1, pos1);
    if (available1.len == 1 && available1.arr[0] == 3)
    {
        free(available1.arr);
        puzzle_destroy(&p1);
    }
    else
    {
        free(available1.arr);
        puzzle_destroy(&p1);
        return false;
    }

    // test case 2
    puzzle p2 = read_puzzle_from_file("puzzle2partial2.txt");
    posn pos2 = {2, 2};
    numArr available2 = available_vals(&p2, pos2);
    if (available2.len == 2 && available2.arr[0] == 2 && available2.arr[1] == 4)
    {
        free(available2.arr);
        puzzle_destroy(&p2);
    }
    else
    {
        free(available2.arr);
        puzzle_destroy(&p2);
        return false;
    }

    return true;
}

bool testing_d(void)
{
    // test case 1
    puzzle p1 = read_puzzle_from_file("puzzle2.txt");
    posn pos = {3, 2};
    p1 = fill_in_guess(&p1, pos, 5);
    // puzzle_print(&p1);
    if (p1.board[2][3].guess && p1.board[2][3].g.number == 5 && p1.board[2][3].g.letter == 'g')
    {
        puzzle_destroy(&p1);
    }
    else
    {
        puzzle_destroy(&p1);
        return false;
    }

    return true;
}

bool testing_e(void)
{
    // test case 1
    puzzle p1 = read_puzzle_from_file("puzzle1partial1.txt");
    if (!guess_valid(&p1))
    {
        puzzle_destroy(&p1);
        return false;
    }
    puzzle_destroy(&p1);

    // test case 2
    puzzle p2 = read_puzzle_from_file("puzzle1partial2.txt");
    if (!guess_valid(&p2))
    {
        puzzle_destroy(&p2);
        return false;
    }
    puzzle_destroy(&p2);

    return true;
}

bool testing_f(void)
{
    // test case 1
    puzzle p1 = read_puzzle_from_file("puzzle3partial2.txt");
    p1 = apply_guess(&p1);
    // puzzle_print(&p1);
    if (p1.board[5][3].guess == false && p1.board[5][4].guess == false && p1.board[5][3].number == 6 &&
        p1.board[5][4].number == 3 && p1.constraintsLen == 0)
    {
        puzzle_destroy(&p1);
    }
    else
    {
        puzzle_destroy(&p1);
        return false;
    }

    return true;
}

bool testing_g(void)
{
    // test case 1
    puzzle p1 = read_puzzle_from_file("puzzle1.txt");
    puzzle temp1 = read_puzzle_from_file("puzzle1partial3.txt");
    puzzle temp2 = read_puzzle_from_file("puzzle1partial4.txt");
    puzArr puzzles = neighbors(&p1);
    // puzzle_print(&puzzles.arr[0]);
    if (puzzles.len == 2 && puzzle_equal(puzzles.arr[0], temp1) && puzzle_equal(puzzles.arr[1], temp2))
    {
        puzzle_destroy(&p1);
        puzzle_destroy(&temp1);
        puzzle_destroy(&temp2);
        puzArr_destroy(&puzzles);
    }
    else
    {
        puzzle_destroy(&p1);
        puzzle_destroy(&temp1);
        puzzle_destroy(&temp2);
        puzArr_destroy(&puzzles);
        return false;
    }

    // test case 2
    puzzle p2 = read_puzzle_from_file("puzzle3partial1.txt");
    puzArr puzzles2 = neighbors(&p2);
    if (puzzles2.len == 0 && puzzles2.arr == NULL)
    {
        puzzle_destroy(&p2);
        puzArr_destroy(&puzzles2);
    }
    else
    {
        puzzle_destroy(&p2);
        puzArr_destroy(&puzzles2);
        return false;
    }

    return true;
}

bool testing_solve_kenken(void)
{
    // check you output files with the ones provided to you. They should be exactly the same.
    // Advice: run one test and comment out the rest, then gradually add tests, to help in debugging

    // testing puzzle1
    puzzle p = read_puzzle_from_file("puzzle1.txt");
    p = solve_kenken(&p);
    print_sol_to_file(&p, "puzzle1soln.txt");
    puzzle_destroy(&p);

    // testing puzzle2
    p = read_puzzle_from_file("puzzle2.txt");
    p = solve_kenken(&p);
    print_sol_to_file(&p, "puzzle2soln.txt");
    puzzle_destroy(&p);

    // testing puzzle3
    p = read_puzzle_from_file("puzzle3.txt");
    p = solve_kenken(&p);
    print_sol_to_file(&p, "puzzle3soln.txt");
    puzzle_destroy(&p);

    // testing puzzle4
    p = read_puzzle_from_file("puzzle4.txt");
    p = solve_kenken(&p);
    print_sol_to_file(&p, "puzzle4soln.txt");
    puzzle_destroy(&p);

    // testing puzzle5
    p = read_puzzle_from_file("puzzle5.txt");
    p = solve_kenken(&p);
    print_sol_to_file(&p, "puzzle5soln.txt");
    puzzle_destroy(&p);

    p = read_puzzle_from_file("puzzle7a.txt");
    p = solve_kenken(&p);
    print_sol_to_file(&p, "puzzle7asoln.txt");
    puzzle_destroy(&p);

    p = read_puzzle_from_file("puzzle7b.txt");
    p = solve_kenken(&p);
    print_sol_to_file(&p, "puzzle7bsoln.txt");
    puzzle_destroy(&p);

    return true;
}

bool testing_solve_kenken_visual(void)
{
    /*
    It is recommended to resize your terminal window to fit
    the printing of an entire puzzle to get the clearest output

    solve_kenken_visual takes a puzzle, and a time delay measured in seconds.
    use a different time delay number to change the delay between puzzle prints.
    We recommend:
        a delay of 1 for puzzles smaller than 4x4
        a delay of 0.5 for puzzles of size 4x4
        a delay of 0 or 0.1 for puzzles larger than 4x4

    make sure to compile this program with gcc, otherwise the colours may not
    work or show up properly
     */
    /*   
    // testing puzzle1
    puzzle p = read_puzzle_from_file("puzzle1.txt");
    p = solve_kenken_visual(&p, 1);
    puzzle_destroy(&p);

    // testing puzzle2
    p = read_puzzle_from_file("puzzle2.txt");
    p = solve_kenken_visual(&p, 0.5);
    puzzle_destroy(&p);

    // testing puzzle3
    p = read_puzzle_from_file("puzzle3.txt");
    p = solve_kenken_visual(&p, 0.1);
    puzzle_destroy(&p);

    // testing puzzle4
    p = read_puzzle_from_file("puzzle4.txt");
    p = solve_kenken_visual(&p, 0.5);
    puzzle_destroy(&p);

    // testing puzzle5
    p = read_puzzle_from_file("puzzle5.txt");
    p = solve_kenken_visual(&p, 1);
    puzzle_destroy(&p);
*/
    return true;
}

int main(void)
{
    // test in steps one function in each execution, starting from testing_a()
    /*
    assert(testing_a());
    assert(testing_b());
    assert(testing_c());
    assert(testing_d());
    assert(testing_e());
    assert(testing_f());
    assert(testing_g());

    assert(testing_solve_kenken());
*/
    // assert(testing_solve_kenken_visual());

    return 0;
}
