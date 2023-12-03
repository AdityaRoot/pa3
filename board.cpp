#include "board.h"
#include "const.h"

// Implement the Board class here

// Create an empty board with the given score grid.
// All data members are non-dynamic, so default memberwise copy constructor and assignment operator are enough.
Board::Board(const int score[][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            cells[i][j] = EMPTY;
            this->score[i][j] = score[i][j];
        }
    }
    curPlayer = X;
    id = 0;
}

// bool isFull() const: Returns true if all cells are non-empty.
bool Board::isFull() const
{
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (cells[i][j] == EMPTY) {
                return false;
            }
        }
    }
    return true;
}

// bool isFinished() const: Returns true if the game has finished (a match is found (therefore score is WIN_SCORE or -WIN_SCORE) or board is full
bool Board::isFinished() const
{
    return (getBoardScore() == WIN_SCORE || getBoardScore() == -WIN_SCORE || isFull());
}

// int getBoardScore() const: Checks if a player has a match, if so, returns WIN_SCORE or -WIN_SCORE.
// Otherwise, returns the sum of X's score minus O's score, scaled by getCellWeight.

int Board::getBoardScore() const
{
    // Check if all of a row, column, or diagonal is the same.

    // Check for if the sum of a row is board size * X or board size * O
    for (int i = 0; i < BOARD_SIZE; ++i) {
        int rowSum = 0;
        int colSum = 0;
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (cells[i][j] != EMPTY) {
                rowSum += cells[i][j];
            }
            if (cells[j][i] != EMPTY) {
                colSum += cells[j][i];
            }
        }
        if (rowSum == BOARD_SIZE * X || colSum == BOARD_SIZE * X) {
            // std::cout << "rowSum" << std::endl;
            return WIN_SCORE;
        }
        if (rowSum == BOARD_SIZE * O || colSum == BOARD_SIZE * O) {
            // std::cout << "rowSumo" << std::endl;
            return -WIN_SCORE;
        }
    }

    // Check for if the sum of a column is board size * X or board size * O
    // for (int i = 0; i < BOARD_SIZE; ++i) {
    //         int colSum = 0;
    //         for (int j = 0; j < BOARD_SIZE; j++) {
    //             if (cells[j][i] != EMPTY) {
    //                 colSum += cells[j][i];
    //             }
    //         }
    //         if (colSum == BOARD_SIZE * X) {
    //             // std::cout << "colSum" << std::endl;
    //             return WIN_SCORE;
    //         }
    //         if (colSum == BOARD_SIZE * O) {
    //             // std::cout << "colSumo" << std::endl;
    //             return -WIN_SCORE;
    //         }
    //     }

    // Check for if the sum of a diagonal is board size * X or board size * O
    int diagSum1 = 0;
    int diagSum2 = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (cells[i][i] != EMPTY) {
            diagSum1 += cells[i][i];
        }
        if (cells[i][BOARD_SIZE - i - 1] != EMPTY) {
            diagSum2 += cells[i][BOARD_SIZE - i - 1];
        }
    }
    if (diagSum1 == BOARD_SIZE * X || diagSum2 == BOARD_SIZE * X) {
        // std::cout << "diagSum1" << std::endl;
        return WIN_SCORE;
    }
    if (diagSum1 == BOARD_SIZE * O || diagSum2 == BOARD_SIZE * O) {
        // std::cout << "diagSum1o" << std::endl;
        return -WIN_SCORE;
    }

    // If there is no match, return the sum of X's score minus O's score, scaled by getCellWeight.
    int xSum = 0;
    int oSum = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (cells[i][j] == X) {
                xSum += score[i][j];
            }
            if (cells[i][j] == O) {
                oSum += score[i][j];
            }
        }
    }
    // std::cout << "no match" << std::endl;
    return xSum * getCellWeight(Cell::X) - oSum * getCellWeight(Cell::O);
}

// bool play(const BoardCoordinate& coords) : Plays at the given coordinates if the cell is empty and in bounds
// Returns true if the move is valid, false otherwise.
// Change the current player if the move is valid.
// Change the id of the board based on the following formula:
// id = 0;
// for each cell {
//   cellValue = 0 if cell is empty, 1 if cell is X, 2 if cell is O;
//   id += cellValue * (3 ^ (row * BOARD_SIZE + column));
// }
bool Board::play(const BoardCoordinate& coords)
{
    if (coords.row < 0 || coords.row >= BOARD_SIZE || coords.col < 0 || coords.col >= BOARD_SIZE) {
        return false;
    }
    if (cells[coords.row][coords.col] != EMPTY) {
        return false;
    }
    cells[coords.row][coords.col] = curPlayer;
    curPlayer = (curPlayer == X) ? O : X;
    id = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        int cellValue = 0;
        for (int j = 0; j < BOARD_SIZE; j++) {
            cellValue = (cells[i][j] == X) ? 1 : ((cells[i][j] == O) ? 2 : 0);
            id += cellValue * (int)pow(3, i * BOARD_SIZE + j);
        }
    }
    return true;
}
