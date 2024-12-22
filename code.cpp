#include <bits/stdc++.h>
using namespace std;

// Constants for player identification
#define COMPUTER 1
#define HUMAN 2
#define SIDE 3 // Board size is 3x3
#define COMPUTERMOVE 'O' // Computer's symbol
#define HUMANMOVE 'X' // Human's symbol

// Enum for difficulty levels
enum Difficulty { EASY = 1, MODERATE, HARD };

// Function to ask the user if they want to start first
char getPlayerChoice() {
    char choice;
    cout << "\nDo you want to start first? (y/n): ";
    cin >> choice;
    return choice;
}

// Function to ask the user to select the difficulty level
Difficulty getDifficultyLevel() {
    int choice;
    cout << "\nSelect Difficulty Level:\n";
    cout << "1. Easy\n";
    cout << "2. Moderate\n";
    cout << "3. Hard\n";
    cout << "Enter your choice (1-3): ";
    while (true) {
        cin >> choice;
        if (choice >= 1 && choice <= 3) {
            return static_cast<Difficulty>(choice);
        } else {
            cout << "Invalid choice. Please enter 1, 2, or 3: ";
        }
    }
}

// Function to display the current state of the board
void showBoard(const vector<vector<char>> &board) {
    cout<<endl;
    cout << "\t\t\t " << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << "\n";
    cout << "\t\t\t-----------\n";
    cout << "\t\t\t " << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << "\n";
    cout << "\t\t\t-----------\n";
    cout << "\t\t\t " << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << "\n";
}

// Function to display instructions for playing the game
void showInstructions() {
    cout << "\nChoose a cell numbered from 1 to 9 as below and play\n\n";
    cout << "\t\t\t 1 | 2 | 3 \n";
    cout << "\t\t\t-----------\n";
    cout << "\t\t\t 4 | 5 | 6 \n";
    cout << "\t\t\t-----------\n";
    cout << "\t\t\t 7 | 8 | 9 \n";
}

// Function to initialize the game board with empty cells
void initialise(vector<vector<char>> &board) {
    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            board[i][j] = '*'; // Empty cells represented by '*'
        }
    }
}

// Function to declare the winner of the game
void declareWinner(int whoseTurn) {
    if (whoseTurn == COMPUTER) {
        cout << "COMPUTER has won!\n";
    } else {
        cout << "HUMAN has won!\n";
    }
}

// Check if any row has all the same symbols (win condition)
bool rowCrossed(const vector<vector<char>> &board) {
    for (int i = 0; i < SIDE; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != '*') {
            return true;
        }
    }
    return false;
}

// Check if any column has all the same symbols (win condition)
bool columnCrossed(const vector<vector<char>> &board) {
    for (int i = 0; i < SIDE; i++) {
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != '*') {
            return true;
        }
    }
    return false;
}

// Check if either diagonal has all the same symbols (win condition)
bool diagonalCrossed(const vector<vector<char>> &board) {
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != '*') {
        return true;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != '*') {
        return true;
    }
    return false;
}

// Check if the game is over by evaluating all win conditions
bool gameOver(const vector<vector<char>> &board) {
    return (rowCrossed(board) || columnCrossed(board) || diagonalCrossed(board));
}

// Check if the board is completely filled
bool isBoardFull(const vector<vector<char>> &board) {
    for (const auto &row : board) {
        for (const auto &cell : row) {
            if (cell == '*') return false; // Empty cells remain
        }
    }
    return true;
}

// Minimax algorithm to determine the best move for AI (COMPUTER)
int minimax(vector<vector<char>> &board, int depth, bool isAI) {
    if (gameOver(board)) {
        if (isAI) {
            return -10 + depth; // AI lost
        } else {
            return 10 - depth; // AI won
        }
    }

    if (isBoardFull(board)) {
        return 0; // Draw
    }

    if (isAI) {
        int bestScore = -1000;
        for (int i = 0; i < SIDE; i++) {
            for (int j = 0; j < SIDE; j++) {
                if (board[i][j] == '*') {
                    board[i][j] = COMPUTERMOVE;
                    int score = minimax(board, depth + 1, false);
                    board[i][j] = '*';
                    bestScore = max(bestScore, score);
                }
            }
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (int i = 0; i < SIDE; i++) {
            for (int j = 0; j < SIDE; j++) {
                if (board[i][j] == '*') {
                    board[i][j] = HUMANMOVE;
                    int score = minimax(board, depth + 1, true);
                    board[i][j] = '*';
                    bestScore = min(bestScore, score);
                }
            }
        }
        return bestScore;
    }
}

// Generate a random move for easy difficulty
int randomMove(const vector<vector<char>> &board) {
    vector<int> availableMoves;
    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            if (board[i][j] == '*') {
                availableMoves.push_back(i * SIDE + j);
            }
        }
    }
    if (!availableMoves.empty()) {
        return availableMoves[rand() % availableMoves.size()];
    }
    return -1; // No moves left
}

// Determine the best move based on difficulty level
int bestMove(const vector<vector<char>> &board, Difficulty difficulty, int moveIndex) {
    if (difficulty == EASY) {
        return randomMove(board); // Easy difficulty uses random moves
    } else if (difficulty == MODERATE) {
        if (rand() % 2 == 0) { // 50% chance of random move
            return randomMove(board);
        } else { // 50% chance of calculated move
            vector<vector<char>> tempBoard = board;
            int bestScore = -1000;
            int move = -1;
            for (int i = 0; i < SIDE; i++) {
                for (int j = 0; j < SIDE; j++) {
                    if (tempBoard[i][j] == '*') {
                        tempBoard[i][j] = COMPUTERMOVE;
                        int score = minimax(tempBoard, moveIndex + 1, false);
                        tempBoard[i][j] = '*';
                        if (score > bestScore) {
                            bestScore = score;
                            move = i * SIDE + j;
                        }
                    }
                }
            }
            if (move != -1 && rand() % 100 < 80) { // 80% chance to use calculated move
                return move;
            } else {
                return randomMove(board);
            }
        }
    } else { // Hard difficulty always uses the best calculated move
        vector<vector<char>> tempBoard = board;
        int bestScore = -1000;
        int move = -1;
        for (int i = 0; i < SIDE; i++) {
            for (int j = 0; j < SIDE; j++) {
                if (tempBoard[i][j] == '*') {
                    tempBoard[i][j] = COMPUTERMOVE;
                    int score = minimax(tempBoard, moveIndex + 1, false);
                    tempBoard[i][j] = '*';
                    if (score > bestScore) {
                        bestScore = score;
                        move = i * SIDE + j;
                    }
                }
            }
        }
        return move;
    }
}

// Main game loop to play Tic-Tac-Toe
void playTTT(int whoseTurn, Difficulty difficulty) {
    vector<vector<char>> board(SIDE, vector<char>(SIDE)); // 3x3 Tic-Tac-Toe board
    initialise(board);
    showInstructions();
    int moveIndex = 0;
    while (!gameOver(board) && !isBoardFull(board)) {
        int x, y;
        if (whoseTurn == COMPUTER) {
            int move = bestMove(board, difficulty, moveIndex);
            if (move != -1) {
                x = move / SIDE;
                y = move % SIDE;
                board[x][y] = COMPUTERMOVE;
                moveIndex++;
                showBoard(board);
                whoseTurn = HUMAN;
            }
        } else if (whoseTurn == HUMAN) {
            int n;
            cout << "\nEnter the position = ";
            cin >> n;
            n--;
            x = n / SIDE;
            y = n % SIDE;
            if (board[x][y] == '*' && n >= 0 && n < SIDE * SIDE) {
                board[x][y] = HUMANMOVE;
                moveIndex++;
                showBoard(board);
                whoseTurn = COMPUTER;
            } else {
                cout << "Invalid move. Try again.\n";
            }
        }
    }

    if (!gameOver(board)) {
        cout << "It's a draw!\n";
    } else {
        declareWinner(whoseTurn == COMPUTER ? HUMAN : COMPUTER);
    }
}

// Main function
int main() {
    cout << "\n-----------------------------------";
    cout << "\t\t\t Tic-Tac-Toe\t\t\t";
    cout << "-------------------------------------";
    char cont = 'y';
    do {
        srand(time(0)); // Seed random number generator
        char choice = getPlayerChoice();
        Difficulty difficulty = getDifficultyLevel();
        if (choice == 'n') {
            playTTT(COMPUTER, difficulty);
        } else if (choice == 'y') {
            playTTT(HUMAN, difficulty);
        } else {
            cout << "Invalid choice\n";
        }
        cout << "\nDo you want to quit? (y/n): ";
        cin >> cont;
    } while (cont == 'n');
    cout << "\n\nThank you for playing <3";
    return 0;
}
