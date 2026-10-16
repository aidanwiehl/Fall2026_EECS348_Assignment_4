//EECS 348 Assignment 4 
//C++ program that takes in five .txt files containing  
//unfinished Sudoku puzzles. Program uses recursion to 
//find solution(s) for each puzzle, and prints them out.
//Inputs: puzzle1.txt, puzzle2.txt, puzzle3.txt, puzzle4.txt, puzzle5.txt
//Outputs: Terminal output of the solutions to the puzzles
//Collaborators: Source code taken from CoPilot
//Author: Aidan Wiehl
//Creation date: 10/15/2026
//Revision date: 10/15/2026
//Revisions: Added comments to improve maintainability, and 
//          hard coded grid.resize to include '_' to match format



#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

//Sudoku Class stores the Sudoku board and makes functions 
//for loading the puzzles, printing the board, and making validInRow
// moves according to Sudoku rules.

class Sudoku {
private:

    vector<vector<char>> grid; //9x9 board

public:
    Sudoku() {
        grid.resize(9, vector<char>(9, '_')); //Creates empty board
    }

    //Loads puzzle from .txt file, returns false if file cannot be opened
    bool loadPuzzle(const string& filename) {
        ifstream fin(filename);

        if (!fin) {
            cout << "Error opening file: " << filename << endl;
            return false;
        }

        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                fin >> grid[row][col];
            }
        }

        fin.close();
        return true;
    }

    //Prints the current state of the puzzle
    void printBoard() const {
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                cout << grid[row][col] << " ";
            }
            cout << endl;
        }
    }

    //Searches the board for an empty cell 
    //Returns true and updates the row and/or col if found
    //Returns false if the puzzle is full
    bool findEmptyCell(int& row, int& col) const {

        for (row = 0; row < 9; row++) {
            for (col = 0; col < 9; col++) {

                if (grid[row][col] == '_')
                    return true;
            }
        }

        return false;
    }

    //Checks if digit exists in specified row
    bool validInRow(int row, char value) const {

        for (int col = 0; col < 9; col++) {
            if (grid[row][col] == value)
                return false;
        }

        return true;
    }
    
    //Checks if digit exists in specified col
    bool validInColumn(int col, char value) const {

        for (int row = 0; row < 9; row++) {
            if (grid[row][col] == value)
                return false;
        }

        return true;
    }

    //Checks if digit exists in corresponding 3x3 subgrid
    bool validInBlock(int row, int col, char value) const {

        int startRow = (row / 3) * 3;
        int startCol = (col / 3) * 3;

        for (int r = startRow; r < startRow + 3; r++) {
            for (int c = startCol; c < startCol + 3; c++) {

                if (grid[r][c] == value)
                    return false;
            }
        }

        return true;
    }

    //Checks if placing a digit into a specified cell 
    //is valid within the rules of Sudoku
    bool isValidMove(int row, int col, char value) const {

        return validInRow(row, value) &&
               validInColumn(col, value) &&
               validInBlock(row, col, value);
    }

    //Places a digit into a cell on the board
    void setValue(int row, int col, char value) {
        grid[row][col] = value;
    }

    //Returns a copy of the current board
    //Used to store completed solutions
    vector<vector<char>> getGrid() const {
        return grid;
    }
};


//SudokuSolver class uses recursive depth-first search 
//with backtracking to find all valid solutions
class SudokuSolver {
private:

    //Stores every solution found
    vector<vector<vector<char>>> solutions;

public:

    void solve(Sudoku& puzzle) {

        int row;
        int col;
        
        
        //If no empty cells, return complete solution
        if (!puzzle.findEmptyCell(row, col)) {

            solutions.push_back(puzzle.getGrid());
            return;
        }
        
        //Try every possible digit
        for (char digit = '1'; digit <= '9'; digit++) {

            //Continue if move is valid
            if (puzzle.isValidMove(row, col, digit)) {
                
                //Make intial choice
                puzzle.setValue(row, col, digit);
                
                //Recursively solve the puzzle
                solve(puzzle);

                // Backtracking step
                //Undo the previous choice and try 
                //another value
                puzzle.setValue(row, col, '_');
            }
        }
    }

    //Prints every valid solution found
    //Prints a separate message if no solution found
    void printSolutions() const {

        if (solutions.empty()) {

            cout << "\nNo solution found.\n";
            return;
        }

        for (size_t i = 0; i < solutions.size(); i++) {

            cout << "\nSolution " << i + 1 << ":\n\n";

            for (int row = 0; row < 9; row++) {

                for (int col = 0; col < 9; col++) {

                    cout << solutions[i][row][col] << " ";
                }

                cout << endl;
            }
        }

        cout << "\nSolutions found: "
             << solutions.size()
             << endl;
    }
};


//PuzzleManager class manages puzzle file names, loads puzzles 
//runs the solver, and displays the results.
class PuzzleManager {
private:

    //Stores the puzzle file names
    vector<string> puzzleFiles;

public:

    //Constructor initializes the list of puzzle files to process 
    PuzzleManager() {

        puzzleFiles.push_back("puzzle1.txt");
        puzzleFiles.push_back("puzzle2.txt");
        puzzleFiles.push_back("puzzle3.txt");
        puzzleFiles.push_back("puzzle4.txt");
        puzzleFiles.push_back("puzzle5.txt");
    }

    //Processes every puzzle file, solves each puzzle,
    //and prints every valid solution found.
    void processPuzzles() {

        for (string filename : puzzleFiles) {

            Sudoku puzzle;

            if (!puzzle.loadPuzzle(filename))
                continue;

            cout << "\n================================================";
            cout << "\nPuzzle File: " << filename;
            cout << "\n================================================\n";

            cout << "\nOriginal Puzzle:\n\n";
            puzzle.printBoard();

            SudokuSolver solver;
            
            //Starts the recursive search process
            solver.solve(puzzle);

            cout << "\nSolved Puzzle(s):\n";
            solver.printSolutions();

            cout << "\n";
        }
    }
};


//Creates PuzzleManager object and processes the puzzle files.
int main() {

    PuzzleManager manager;

    manager.processPuzzles();

    return 0;
}