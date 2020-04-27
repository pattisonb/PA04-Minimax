#include <iostream>
#include <sstream>

#define BOARD 3

int main() {

    std::string board[BOARD][BOARD];
    int boardSpaces = 0;

    std::cout << "  ";
    for (int i = 0; i <= ((BOARD * 7)); i++) {
        std::cout << "-";
    }
    std::cout << std::endl;

    for (int i = 0; i < BOARD; i++) {
        for (int j = 0; j < BOARD; j++) {
            std::ostringstream ss;
            ss << boardSpaces;
            board[i][j] = ss.str();
            if (board[i][j].length() == 1) {
                std::cout << "  |   " << board[i][j];
            }
            else {
                std::cout << "  |  " << board[i][j];
            }
            boardSpaces++;
        }
        if (i != BOARD - 1) {
            std::cout << "  |  \n" << "  |";
            for (int x = 0; x < BOARD; x++) {
                std::cout << " ---- |";
            }
            std::cout << std::endl;
        }
        else {
            std::cout << "  | \n  ";
            for (int k = 0; k <= (BOARD * 7); k++) {
                std::cout << "-";
            }
            std::cout << std::endl;
        }
    }
    return 0;
}