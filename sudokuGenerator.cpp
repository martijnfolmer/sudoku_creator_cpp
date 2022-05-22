// sudokuGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <stack>
#include <iterator>
#include <chrono>
#include <fstream>


/* DisplaySudoku : this takes the array Sudoku and prints it in the window*/
void DisplaySudoku(int** Sudoku, int& widthAndHeight) {

    int boxSize = (int)sqrt(widthAndHeight);        // the size of a box inside of the sudoku

    for (int i = 0; i < widthAndHeight; i++) {
        for (int j = 0; j < widthAndHeight; j++) {
            std::cout << Sudoku[i][j] << " ";
            if ((j + 1) % boxSize == 0 and j > 0) {
                std::cout << "  ";
            }
        }

        std::cout << std::endl;
        if ((i + 1) % boxSize == 0 and i > 0) {
            std::cout << std::endl;
        }
    }
}




// Creating and filling the initial sudoku

// InsertInitialValue : initializes and allocates memory to our sudoku
void InsertInitialValue(int** sudoku, int widthAndheight, int initialValue) {

    int i, j;

    //dynamically allocating column space in heap
    for (i = 0; i < widthAndheight; i++) {
        sudoku[i] = new int[widthAndheight];
    }
    // allocate intial values
    for (i = 0; i < widthAndheight; i++) {
        for (j = 0; j < widthAndheight; j++) {
            sudoku[i][j] = initialValue;
        }
    }
}



// Get the box coordinates (top left = (x1,y1), bottom right = (x2,y2)) we for any set of coordinates inside of the grid
void getBoxCoor(int xloc, int yloc, int boxSize, int& x1, int& y1, int& x2, int& y2) {
    x1 = (int)(std::floor(xloc / boxSize)) * boxSize;
    y1 = (int)(std::floor(yloc / boxSize)) * boxSize;
    x2 = (int)(std::floor(xloc / boxSize) + 1) * boxSize;
    y2 = (int)(std::floor(yloc / boxSize) + 1) * boxSize;
}


// Checks whether a value (val) is unused in a certain row
bool unusedinRow(int** a, int rowToCheck, int rowSize, int val) {
    int i;
    for (i = 0; i < rowSize; i += 1) {
        if (a[rowToCheck][i] == val) {
            return false;
        }
    }
    return true;
}


//checks whether a value (val) is unused in a certain column
bool unusedinColumn(int** sudoku, int columnToCheck, int columnSize, int val) {
    int i;
    for (i = 0; i < columnSize; i += 1) {
        if (sudoku[i][columnToCheck] == val) {
            return false;
        }
    }
    return true;
}


//Checks whether a value (val) is unused in a certain box
bool unusedinBox(int** sudoku, int columnStart, int columnEnd, int rowStart, int rowEnd, int val) {
    int i, j;
    for (i = columnStart; i < columnEnd; i++) {
        for (j = rowStart; j < rowEnd; j++) {
            if (sudoku[j][i] == val) {
                return false;
            }
        }
    }
    return true;
}

// Checks whether a certain value (val) is available for spot (xloc, yloc) in sudoku, because it hasn't been used in its row, column or box
bool isUnused(int** sudoku, int xloc, int yloc, int widthAndHeight, int val) {
    int x1, y1, x2, y2;
    int boxSize = (int)sqrt(widthAndHeight);
    if (unusedinColumn(sudoku, xloc, widthAndHeight, val) == false) {
        return false;
    }
    else if (unusedinRow(sudoku, yloc, widthAndHeight, val) == false) {
        return false;
    }
    else {
        getBoxCoor(xloc, yloc, boxSize, x1, y1, x2, y2);
        if (unusedinBox(sudoku, x1, x2, y1, y2, val) == false) {
            return false;
        }
    }
    return true;
}

// Fill the first row of the sudoku with random/unique numbers
void fillFirstRow(int** sudoku, int widthAndHeight) {
    int i, valfill;
    for (i = 0; i < widthAndHeight; i++) {
        while (true) {
            valfill = rand() % widthAndHeight + 1;
            if (isUnused(sudoku, i, 0, widthAndHeight, valfill)) {
                sudoku[0][i] = valfill;
                break;
            }
        }
    }

}

// Shift a row of numbers an shiftsize number of spaces, and copies it to the next row
void shiftRowAndFill(int** sudoku, int widthAndHeight, int row_or, int row_new, int shiftsize) {
    int i, newi;
    for (i = 0; i < widthAndHeight; i++) {
        newi = i + shiftsize;
        if (newi >= widthAndHeight) {
            newi -= widthAndHeight;
        }
        sudoku[row_new][i] = sudoku[row_or][newi];
    }
}


//Fill an entire sudoku with an initial solution
void fillSudoku(int** sudoku, int widthAndHeight) {
    int i, j;
    int smallSize = (int)sqrt(widthAndHeight);
    for (i = 0; i < smallSize; i += 1) {
        for (j = 0; j < smallSize; j += 1) {
            if (i == 0 and j == 0) {
                fillFirstRow(sudoku, widthAndHeight);   
            }
            else if (j == 0) {
                shiftRowAndFill(sudoku, widthAndHeight, 0, i * smallSize, i);
            }
            else {
                shiftRowAndFill(sudoku, widthAndHeight, i * smallSize, i * smallSize + j, smallSize * j);
            }
        }
    }
}


// Get Random Coordinates (that aren't already empty)
void RandomCoor(int size, int& xloc, int& yloc) {
    xloc = rand() % size;
    yloc = rand() % size;
}

// Swap 2 rows
void swapRows(int** sudoku, int row1, int row2, int widthAndHeight) {

    std::vector<int> rowToSwap;
    
    for (int i = 0; i < widthAndHeight; i++) {
        rowToSwap.push_back(sudoku[row1][i]);
    }
    for (int i = 0; i < widthAndHeight; i++) {
        sudoku[row1][i] = sudoku[row2][i];
        sudoku[row2][i] = rowToSwap[i];
    }
}

// Swap 2 columns
void swapColumns(int** sudoku, int column1, int column2, int widthAndHeight) {

    std::vector<int> rowToSwap;

    for (int i = 0; i < widthAndHeight; i++) {
        rowToSwap.push_back(sudoku[i][column1]);
    }
    for (int i = 0; i < widthAndHeight; i++) {
        sudoku[i][column1] = sudoku[i][column2];
        sudoku[i][column2] = rowToSwap[i];
    }
}

// Shuffle columns and rows
void SudokuShuffle(int** sudoku, int widthAndHeight) {

    int boxSize = (int)sqrt(widthAndHeight);        // the size of a box inside of the sudoku
    
    //shuffle rows and columns internally

    int x1, y1;             // the individual element
    int x2, y2, x3, y3;     // the box coordinates
    RandomCoor(widthAndHeight, x1, y1);
    getBoxCoor(x1, y1, boxSize, x2, y2, x3, y3);

    int range = x3 - 1 - x2;
    int column1 = rand() % range + x2;

    range = x3 - 1 - x2;
    int column2 = rand() % range + x2;

    swapColumns(sudoku, column1, column2, widthAndHeight);

    range = y3 - 1 - y2;
    int row1 = rand() % range + y2;

    range = y3 - 1 - y2;
    int row2 = rand() % range + y2;

    swapRows(sudoku, row1, row2, widthAndHeight);
}


//Get the number of empty spaces in the sudoku
int getNumEmptySpaces(int** sudoku, int widthAndHeight) {
    
    int i, j;
    int numEmptySpaces = 0;
    for (i = 0; i < widthAndHeight; i++) {
        for (j = 0; j < widthAndHeight; j++) {
            if (sudoku[i][j] <= 0) {
                numEmptySpaces++;
            }
        }
    }
    return numEmptySpaces;
}


// Get all the values from within a column range
std::vector<int> getAllValues(int** sudoku, int columnStart, int columnEnd,
    int rowStart, int rowEnd) {
    int i, j;

    std::vector<int> allValues = {};
    for (i = columnStart; i < columnEnd; i++) {
        for (j = rowStart; j < rowEnd; j++) {
            allValues.push_back(sudoku[j][i]);
        }
    }
    return allValues;
}

// Obtain the values from specific rows, columns and boxes
std::vector<int> getRowValues(int** sudoku, int rowIndex, int widthAndHeight) {
    return getAllValues(sudoku, 0, widthAndHeight, rowIndex, rowIndex + 1);
}

std::vector<int> getColumnValues(int** sudoku, int columnIndex, int widthAndHeight) {
    return getAllValues(sudoku, columnIndex, columnIndex+1, 0, widthAndHeight);
}

std::vector<int> getBoxValues(int** sudoku, int xloc, int yloc, int widthAndHeight) {
    int x1, y1, x2, y2;
    int boxSize = (int)sqrt(widthAndHeight);        // the size of a box inside of the sudoku
    getBoxCoor(xloc, yloc, boxSize, x1, y1, x2, y2);
    return getAllValues(sudoku, x1, x2, y1, y2);
}


//check if all values are distinct or not (excepting -1 and 0)
bool areDistinctArray(std::vector<int> arrayWithValues)
{
    int i, j;
    int n = arrayWithValues.size();

    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (arrayWithValues[i] == arrayWithValues[j] and arrayWithValues[i]>0 and arrayWithValues[j]>0) {
                return false;
            }
        }
    }
    return true;
}

//Are all numbers that are specific to a location (xloc,yloc) unique? return true if yes.
bool areDistinctSpecificLocation(int** sudoku, int xloc, int yloc, int widthAndHeight) {

    int x1, y1, x2, y2;
    std::vector<int> values;
    bool valDistinct;
    int boxSize = (int)sqrt(widthAndHeight);        // the size of a box inside of the sudoku
   
    //check row
    values = getRowValues(sudoku, yloc, widthAndHeight);
    valDistinct = areDistinctArray(values);
    if (valDistinct == false) {
        return false;
    }

    //check column
    values = getColumnValues(sudoku, xloc, widthAndHeight);
    valDistinct = areDistinctArray(values);
    if (valDistinct == false) {
        return false;
    }

    //check box
    getBoxCoor(xloc, yloc, boxSize, x1, y1, x2, y2);
    values = getBoxValues(sudoku, xloc, yloc, widthAndHeight);
    valDistinct = areDistinctArray(values);
    if (valDistinct == false) {
        return false;
    }

    return true;
}


// Check all rows on whether they have only unique values in their rows
bool areDistinctAllRows(int** sudoku, int widthAndHeight) {

    bool valDistinct;
    std::vector<int> values;
    for (int i = 0; i < widthAndHeight; i++) {
        values = getRowValues(sudoku, i, widthAndHeight);
        valDistinct = areDistinctArray(values);
        if (valDistinct == false) {
            return false;
        }
    }
    return true;
}

// Check all columns on whether they have only unique values in their columns
bool areDistinctAllColumns(int** sudoku, int widthAndHeight) {

    bool valDistinct;
    std::vector<int> values;
    for (int i = 0; i < widthAndHeight; i++) {
        values = getColumnValues(sudoku, i, widthAndHeight);
        valDistinct = areDistinctArray(values);
        if (valDistinct == false) {
            return false;
        }
    }
    return true;
}

// check all boxes (3x3 boxes for a 9x9 sudoku) on whether they have only unique values in them
bool areDistinctAllBoxes(int** sudoku, int widthAndHeight) {
    int boxsize = (int)sqrt(widthAndHeight);
    bool valDistinct;
    std::vector<int> values;

    for (int i = 0; i < widthAndHeight; i += boxsize) {
        for (int j = 0; j < widthAndHeight; j += boxsize) {
            values = getBoxValues(sudoku, i, j, widthAndHeight);
            valDistinct = areDistinctArray(values);
            if (valDistinct == false) {
                return false;
            }
        }
    }

    return true;
}

// check the whole sudoku for unique values (so if it is correct)
bool areDistinctAll(int** sudoku, int widthAndHeight) {

    bool isDistinct = areDistinctAllColumns(sudoku, widthAndHeight);
    if (isDistinct == false){
        return false;
    }
    isDistinct = areDistinctAllRows(sudoku, widthAndHeight);
    if (isDistinct == false) {
        return false;
    }
    isDistinct = areDistinctAllBoxes(sudoku, widthAndHeight);
    if (isDistinct == false) {
        return false;
    }
    return true;
}



//finds all of the unused numbers for a space (xloc, yloc) (>0, <= size of sudoku)
void findAllUnusedNumbers(int** sudoku, int xloc, int yloc, int size, std::vector<int>& allNumbersUnused) {
    int i;
    allNumbersUnused.clear();   //clear the vector
    for (i = 1; i <= size; i++) {
        if (isUnused(sudoku, xloc, yloc, size, i)) {
            allNumbersUnused.push_back(i);
        }
    }
}

// Get the locations of all empty spaces in a sudoku
void getAllEmptySpaces(int** sudoku, int widthAndHeight, std::vector<int>& allEmptySpaces) {
    int i, j;
    int numEmptySpaces = 0;
    for (i = 0; i < widthAndHeight; i++) {
        for (j = 0; j < widthAndHeight; j++) {
            if (sudoku[i][j] <= 0) {
                allEmptySpaces.push_back(i);
                allEmptySpaces.push_back(j);
            }
        }
    }
}

// Use normal sudoku solving rules to try to solve the sudoku (so checking whether there is only one available answer for a block)
int solveSudokuNormalRules(int** sudoku, int widthAndHeight) {
    std::vector<int> allEmptySpacesCoordinates;
    std::vector<int> allUnusedNumbers;
    int i, xcoor, ycoor;
    int numEmptySpaces = getNumEmptySpaces(sudoku, widthAndHeight);
    if (numEmptySpaces == 0) {
        return numEmptySpaces;      //there are no empty spaces, so the sudoku is solved
    }

    getAllEmptySpaces(sudoku, widthAndHeight, allEmptySpacesCoordinates);
    while (true) {
        for (i = 0; i < allEmptySpacesCoordinates.size(); i += 2) {
            xcoor = allEmptySpacesCoordinates[i];
            ycoor = allEmptySpacesCoordinates[i + 1];
            if (sudoku[xcoor][ycoor] < 1) {
                findAllUnusedNumbers(sudoku, ycoor, xcoor, widthAndHeight, allUnusedNumbers);

                if (allUnusedNumbers.size() == 1) { // there is only one solution
                    sudoku[xcoor][ycoor] = allUnusedNumbers[0];
                    continue;
                }
            }
        }
        //we are at the end, if not everything is found, that means we need backtracing
        break;
    }
    return getNumEmptySpaces(sudoku, widthAndHeight);

}


// the backtracing algorithm. Very slow, so we should always try to solve it a different way
// returns true if we could solve
bool solveSudokuBackTracing(int** sudoku, int widthAndHeight) {

    int icur, xcoor, ycoor;
    bool valid;

    std::vector<int> allEmptySpaces;
    getAllEmptySpaces(sudoku, widthAndHeight, allEmptySpaces);
    if (allEmptySpaces.size() == 0) { //allready solved
        return true;
    }

    //Backtracing
    icur = 0;
    while (true)
    {
        xcoor = allEmptySpaces[icur];
        ycoor = allEmptySpaces[icur + 1];

        if (sudoku[xcoor][ycoor] <= 0) {
            sudoku[xcoor][ycoor] = 1;
            valid = areDistinctSpecificLocation(sudoku, ycoor, xcoor, widthAndHeight);
            if (valid == true) {
                icur += 2;
            }
        }
        else if (sudoku[xcoor][ycoor] > widthAndHeight){
            sudoku[xcoor][ycoor] = -1;
            icur -= 2;
        }
        else {
            sudoku[xcoor][ycoor] += 1;
            valid = areDistinctSpecificLocation(sudoku, ycoor, xcoor, widthAndHeight);
            if (valid == true) {
                icur += 2;
            }
        }

        //break out of it
        if (icur < 0 || icur >=allEmptySpaces.size()) {
            break;
        }
    }

    getAllEmptySpaces(sudoku, widthAndHeight, allEmptySpaces);
    if (allEmptySpaces.size() == 0) { //allready solved
        return true;
    }
    else {
        return false;
    }
}

//Solve the sudoku, by normal rules first, then by backtracing
bool solveSudoku(int** sudoku, int widthAndHeight) {
    bool solved = false;
    int numEmptySpaces = solveSudokuNormalRules(sudoku, widthAndHeight);
    
    if (numEmptySpaces > 0){
        solved = solveSudokuBackTracing(sudoku, widthAndHeight);
        return solved;
    }
    else {
        return true;
    }

}

//Check if a location has already been set to 'empty'
bool IsCoorAlreadyEmpty(int xloc, int yloc, std::vector<int>& allEmptyLocations)
{
    for (int i = 0; i < allEmptyLocations.size(); i += 2) {
        if (allEmptyLocations[i] == xloc and allEmptyLocations[i + 1] == yloc) {
            return true;
        }
    }
    return false;
}

//Write the sudoku to a txt file which we can read in our python file
void writeToFile(int** sudoku, int widthAndHeight, std::string fileName) {
   
    //turn Sudoku to vector
    std::vector<int> v;
    for (int i = 0; i < widthAndHeight; i++) {
        for (int j = 0; j < widthAndHeight; j++) {
            v.push_back(sudoku[j][i]);
        }
    }
    //write to file
    std::ofstream outFile(fileName);
    for (const auto& e : v) outFile << e << "\n";
}


int main()
{
    int** sudokuField;                      //** means that this is a pointer to a pointer, sudokuField is a multidimensional array
    int sizeOfSudoku = 9;                   // the width and height of the sudoku. Must be valid : 4, 9, 16, 25, 36, .. 100, etc. sqrt(size) == full int
    double fracEmpty = 0.4;                 // the total fraction of the sudoku we want to be empty [0, 1.0]
    std::vector<int> allEmptySpaces;        // where we store the coordinates of the empty spaces
    int x1, y1, valor;                      // the random coordinates of where we create an empty spot

    int desiredNumEmptySpaces = (int)(sizeOfSudoku * sizeOfSudoku * fracEmpty);     //how many empty spaces we want in total

    auto start = std::chrono::high_resolution_clock::now();                 // timer, so we can check how fast we create one

    // Initialize the sudoku and create a filled out field
    sudokuField = new int* [sizeOfSudoku];
    InsertInitialValue(sudokuField, sizeOfSudoku, 0);
    fillSudoku(sudokuField, sizeOfSudoku);
    for (int kn = 0; kn < 100; kn++) {
        SudokuShuffle(sudokuField, sizeOfSudoku);
    }


    // Begin removing the fraction of the sudoku
    while (true){
        std::cout << allEmptySpaces.size()/2 << " / " << desiredNumEmptySpaces << std::endl;    // progress bar during creation

        while (true){
            RandomCoor(sizeOfSudoku, x1, y1);
            if (IsCoorAlreadyEmpty(x1, y1, allEmptySpaces) == false) {
                break;
            }
        }

        valor = sudokuField[x1][y1];        // store the original value of the place we are emptying
        sudokuField[x1][y1] = -1;           // set to -1

        for (int j = 0; j < allEmptySpaces.size(); j += 2) {
            sudokuField[allEmptySpaces[j]][allEmptySpaces[j + 1]] = -1;
        }

        //check if this is a valid solution or not. If yes, add it to the list of emptySpaces
        bool check = solveSudoku(sudokuField, sizeOfSudoku);
        if (check == true){
            allEmptySpaces.push_back(x1);
            allEmptySpaces.push_back(y1);
        }
        else {
            sudokuField[x1][y1] = valor;    // reset the original value
        }

        //we succeeded, so we break out of the loop
        if (allEmptySpaces.size() / 2 >= desiredNumEmptySpaces) {
            break;
        }
    }

    // Puzzle:
    for (int j = 0; j < allEmptySpaces.size(); j += 2) {
        sudokuField[allEmptySpaces[j]][allEmptySpaces[j + 1]] = -1;
    }
    std::cout << std::endl << "This is the puzzle : " << std::endl;
    DisplaySudoku(sudokuField, sizeOfSudoku);
    writeToFile(sudokuField, sizeOfSudoku, "sudokuPuzzle_unsolved.txt");

    // Solution : 
    bool solved = solveSudoku(sudokuField, sizeOfSudoku);
    std::cout << "This is the solved puzzle: " << solved << std::endl;
    DisplaySudoku(sudokuField, sizeOfSudoku);
    writeToFile(sudokuField, sizeOfSudoku, "sudokuPuzzle_solved.txt");

    // Check how fast we created the sudoku
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "We created a sudoku in : " << duration.count() << " milliseconds" << std::endl;

	system("pause>0");
}



