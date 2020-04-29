#ifndef MINIMAX_GAMEBOARD_H
#define MINIMAX_GAMEBOARD_H
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

const std::string person = "X";
const std::string ai = "O";

enum Player { human, AI};

struct Move {
    Move() {};
    Move(int Score) : score(Score) {};
    int row;
    int column;
    int score;
};

class GameBoard {
private:
    int boardSize;
    std::vector<std::vector<std::string>> board;
public:
    GameBoard(int size) {
        boardSize = size;
        for (int i = 0; i < boardSize; i++) {
            std::vector<std::string> row;
            for (int j = 0; j < boardSize; j++) {
                row.push_back("-");
            }
            board.push_back(row);
        }
        printBoard();
        int turn = 0;
        while (checkWin(board) == "null") {
            playerTurn();
            printBoard();
            if (checkWin(board) == "X") {
                std::cout << "You won!" << std::endl;
                break;
            }
            if (checkWin(board) == "Tie") {
                std::cout << "Tie game!" << std::endl;
                break;
            }
            std::cout << "Computer turn" << std::endl;
            Move AIMove = bestMove(AI);
            board[AIMove.row][AIMove.column] = ai;
            printBoard();
            if (checkWin(board) == "O") {
                std::cout << "You lost!" << std::endl;
                break;
            }
            if (checkWin(board) == "Tie") {
                std::cout << "Tie game!" << std::endl;
                break;
            }
        }
    }
    void printBoard() {
        std::cout << "  ";
        for (int i = 0; i <= ((boardSize * 7)); i++) {
            std::cout << "-";
        }
        std::cout << std::endl;

        int boardSpaces = 1;
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if (board[i][j] != "-") {
                    std::cout << "  |   " << board[i][j];
                }
                else if (boardSpaces < 10){
                    std::cout << "  |   " << boardSpaces;
                }
                else {
                    std::cout << "  |  " << boardSpaces;
                }
                boardSpaces++;
            }
            if (i != boardSize - 1) {
                std::cout << "  |  \n" << "  |";
                for (int x = 0; x < boardSize; x++) {
                    std::cout << " ---- |";
                }
                std::cout << std::endl;
            }
            else {
                std::cout << "  | \n  ";
                for (int k = 0; k <= (boardSize * 7); k++) {
                    std::cout << "-";
                }
                std::cout << std::endl;
            }
        }
    }

    void playerTurn() {
        int input = 0;
        bool foundInput = false;
        std::cout << "Where would you like to play?   ";
        int numInputs = 0;
        while (!foundInput) {
            std::cin >> input;
                for (int i = 0; i < boardSize; i++) {
                    for (int j = 0; j < boardSize; j++) {
                        if (board[i][j] == "-") {
                            int row = (input-1) / boardSize;
                            int col = (input-1) % boardSize;
                            board[row][col] = person;
                            foundInput = true;
                        }
                    }
                }
                numInputs++;
                if (!foundInput && numInputs > 1) {
                    std::cout << "Invalid input" << std::endl;
                    std::cout << "Where would you like to play?   ";
                }
        }
    }

    std::string checkWin(std::vector<std::vector<std::string>> newBoard) {
        if (checkPlayerWin(newBoard, human) == "X") {
            return "X";
        }
        if (checkPlayerWin(newBoard, AI) == "O") {
            return "O";
        }
        bool isTie = true;
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if(newBoard[i][j] != person && newBoard[i][j] != ai) {
                    isTie = false;
                }
            }
        }

        if (isTie) {
            return "Tie";
        }

        return "null";
    }

    std::string checkPlayerWin(std::vector<std::vector<std::string>> newBoard, Player player) {
        std::string playerMark;
        std::string returnValue;
        if (player == human) {
            playerMark = person;
            returnValue = "X";
        } else {
            playerMark = ai;
            returnValue = "O";
        }

        std::vector<std::string> winChecker;
        for (int i = 0; i < boardSize; i++) {
            winChecker.push_back(playerMark);
        }
        for (int i = 0; i < boardSize; i++) {
            if (board[i] == winChecker) {
                return returnValue;
            }
        }
        std::vector<std::string> check;
        for (int j = 0; j < boardSize; j++) {
            for (int i = 0; i < boardSize; i++) {
                check.push_back(newBoard[i][j]);
            }
            if (check == winChecker) {
                return returnValue;
            }
            check.clear();
        }

        check.clear();

        int count = 0;
        for (int i = 0; i < boardSize; i++) {
            check.push_back(newBoard[i][count]);
            count++;
        }

        if (check == winChecker) {
            return returnValue;
        }

        check.clear();

        count = boardSize - 1;
        for (int i = 0; i < boardSize; i++) {
            check.push_back(newBoard[i][count]);
            count--;
        }

        if (check == winChecker) {
            return returnValue;
        }

        return "null";
    }

    Move bestMove(Player player) {

        std::string result = checkWin(board);
        if (result == ai) {
            return Move(10);
        }
        else if (result == person) {
            return Move(-10);
        }
        else if (result == "Tie") {
            return Move(0);
        }

        std::vector<Move> moves;

        for (int x = 0; x < boardSize; x++) {
            for (int y = 0; y < boardSize; y++) {
                if (board[x][y] == "-") {
                    Move move;
                    move.row = x;
                    move.column = y;
                    if (player == AI) {
                        board[x][y] = ai;
                        move.score = bestMove(human).score;
                    }
                    else if (player == human) {
                        board[x][y] = person;
                        move.score = bestMove(AI).score;
                    }
                    moves.push_back(move);
                    board[x][y] = "-";
                }
            }
        }

        int bestMove = 0;
        if (player == AI) {
            int bestScore = -100;
            for (int i = 0; i < moves.size(); i++) {
                if (moves[i].score > bestScore) {
                    bestMove = i;
                    bestScore = moves[i].score;
                }
            }
        }
        else if (player == human) {
            int bestScore = 100;
            for (int i = 0; i < moves.size(); i++) {
                if (moves[i].score < bestScore) {
                    bestMove = i;
                    bestScore = moves[i].score;
                }
            }
        }

        return moves[bestMove];
    }
};

#endif //MINIMAX_GAMEBOARD_H
