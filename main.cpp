#include <iostream>
#include <sstream>
#include "GameBoard.h"

int main() {

    int size;
    std::cout << "Welcome to Tic Tac Toe!" << std::endl;
    std::cout << "What size grid would you like?  ";
    std::cin >> size;
    GameBoard board(size);

    return 0;
}