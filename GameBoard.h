#ifndef MINIMAX_GAMEBOARD_H
#define MINIMAX_GAMEBOARD_H
#include <iostream>
#include <sstream>
#include <vector>

class GameBoard {
private:
    int boardSize;
    std::vector<std::vector<std::string>> board;
public:
    GameBoard(int size) {
        boardSize = size;
        int boardSpaces = 0;
        for (int i = 0; i < boardSize; i++) {
            std::vector<std::string> row;
            for (int j = 0; j < boardSize; j++) {
                std::ostringstream ss;
                ss << boardSpaces;
                row.push_back(ss.str());
                boardSpaces++;
            }
            board.push_back(row);
        }
    }
    void printBoard() {
        std::cout << "  ";
        for (int i = 0; i <= ((boardSize * 7)); i++) {
            std::cout << "-";
        }
        std::cout << std::endl;

        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if (board[j][i].length() == 1) {
                    std::cout << "  |   " << board[j][i];
                }
                else {
                    std::cout << "  |  " << board[j][i];
                }
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
};

#endif //MINIMAX_GAMEBOARD_H
