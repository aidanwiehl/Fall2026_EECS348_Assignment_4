#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Sudoku {
private:
    vector<vector<char>> grid;

public:
    Sudoku() {
        grid.resize(9, vector<char>(9));
    }

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

    void printBoard() const {
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                cout << grid[row][col] << " ";
            }
            cout << endl;
        }
    }

    bool findEmptyCell(int& row, int& col) const {

        for (row = 0; row < 9; row++) {
            for (col = 0; col < 9; col++) {

                if (grid[row][col] == '_')
                    return true;
            }
        }

        return false;
    }

    bool validInRow(int row, char value) const {

        for (int col = 0; col < 9; col++) {
            if (grid[row][col] == value)
                return false;
        }

        return true;
    }

    bool validInColumn(int col, char value) const {

        for (int row = 0; row < 9; row++) {
            if (grid[row][col] == value)
                return false;
        }

        return true;
    }

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

    bool isValidMove(int row, int col, char value) const {

        return validInRow(row, value) &&
               validInColumn(col, value) &&
               validInBlock(row, col, value);
    }

    void setValue(int row, int col, char value) {
        grid[row][col] = value;
    }

    vector<vector<char>> getGrid() const {
        return grid;
    }
};

class SudokuSolver {
private:
    vector<vector<vector<char>>> solutions;

public:

    void solve(Sudoku& puzzle) {

        int row;
        int col;

        if (!puzzle.findEmptyCell(row, col)) {

            solutions.push_back(puzzle.getGrid());
            return;
        }

        for (char digit = '1'; digit <= '9'; digit++) {

            if (puzzle.isValidMove(row, col, digit)) {

                puzzle.setValue(row, col, digit);

                solve(puzzle);

                // Backtracking step
                puzzle.setValue(row, col, '_');
            }
        }
    }

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

class PuzzleManager {
private:
    vector<string> puzzleFiles;

public:

    PuzzleManager() {

        puzzleFiles.push_back("puzzle1.txt");
        puzzleFiles.push_back("puzzle2.txt");
        puzzleFiles.push_back("puzzle3.txt");
        puzzleFiles.push_back("puzzle4.txt");
        puzzleFiles.push_back("puzzle5.txt");
    }

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
            solver.solve(puzzle);

            cout << "\nSolved Puzzle(s):\n";
            solver.printSolutions();

            cout << "\n";
        }
    }
};

int main() {

    PuzzleManager manager;

    manager.processPuzzles();

    return 0;
}