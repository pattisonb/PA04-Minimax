#ifndef MINIMAX_GAMEBOARD_H
#define MINIMAX_GAMEBOARD_H
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace std::chrono;

//easier to name the strings here than to deal with X's and O's the whole time
const std::string person = "X";
const std::string ai = "O";

//need to pass a player to bestMove functions to use the right string
enum Player { human, AI};

//move struct to return for an AI move
struct Move {
    Move() {};
    Move(int Score) : score(Score) {};
    int row;
    int column;
    int score;
};

class GameBoard {
private:
    //using a vector so i can easily resize between size of tic tac toe board
    int boardSize;
    std::vector<std::vector<std::string> > board;
public:
    GameBoard(int size) {
        bool alphaBeta = true;
        std::cout << "Would you like to use alpha-beta pruning? (y/n)" << std::endl;
        std::string result;
        std::cin >> result;
        if (result == "n" || result == "N") {
            alphaBeta = false;
        }
        boardSize = size;
        //fill board with - to easily check if spot is taken
        for (int i = 0; i < boardSize; i++) {
            std::vector<std::string> row;
            for (int j = 0; j < boardSize; j++) {
                row.push_back("-");
            }
            board.push_back(row);
        }
        printBoard();
        int turn = 0;
        //run the game as long as nobody has won the game
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
            //set a new move to the best move by the AI
            auto t1 = high_resolution_clock::now();
            Move AIMove;
            if (!alphaBeta) {
                AIMove = bestMove(AI);
            }
            else {
                AIMove = bestMoveAlphaBeta(AI, -100, 100);
            }
            auto t2 = high_resolution_clock::now();
            //make that move
            board[AIMove.row][AIMove.column] = ai;
            printBoard();
            auto elapsedTime = duration_cast <microseconds>( t2 - t1 ).count();
            std::cout << "Time: " << elapsedTime << "\n" << std::endl;
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
        //formatting board properly
        std::cout << "  ";
        for (int i = 0; i <= ((boardSize * 7)); i++) {
            std::cout << "-";
        }
        std::cout << std::endl;

        int boardSpaces = 1;
        //print an X or an O if it's there
        //otherwise print the number according to the input for that space
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
                    std::cout << "------|";
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
                            //using divide and modulo the place the X
                            //only places if empty
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

    std::string checkWin(std::vector<std::vector<std::string> > newBoard) {
        //separate checkWin function to check a win for both players rather than calling checkPlayer win twice everytime
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
        //rerturning the literal value null if it's a tie
        return "null";
    }

    std::string checkPlayerWin(std::vector<std::vector<std::string> > newBoard, Player player) {
        std::string playerMark;
        std::string returnValue;
        if (player == human) {
            playerMark = person;
            returnValue = "X";
        } else {
            playerMark = ai;
            returnValue = "O";
        }

        //fill a vector with either X or O depending on the player passed
        std::vector<std::string> winChecker;
        for (int i = 0; i < boardSize; i++) {
            winChecker.push_back(playerMark);
        }
        //checking horizontal
        for (int i = 0; i < boardSize; i++) {
            if (board[i] == winChecker) {
                return returnValue;
            }
        }
        //checking vertical
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

        //checking right diagonal
        int count = 0;
        for (int i = 0; i < boardSize; i++) {
            check.push_back(newBoard[i][count]);
            count++;
        }

        if (check == winChecker) {
            return returnValue;
        }

        check.clear();

        //checking left diagonal
        count = boardSize - 1;
        for (int i = 0; i < boardSize; i++) {
            check.push_back(newBoard[i][count]);
            count--;
        }

        if (check == winChecker) {
            return returnValue;
        }
        //other wise it's just null
        return "null";
    }

    //minimax function
    Move bestMove(Player player) {
        //base case for the MiniMax recursion
        std::string result = checkWin(board);
        //return a 10 if ai wins, -10 if person wins and a 0 if tie
        if (result == ai) {
            return Move(10);
        }
        else if (result == person) {
            return Move(-10);
        }
        else if (result == "Tie") {
            return Move(0);
        }

        //vector of moves to keep track of all of them to find the one the results in the either min or max score depening on the player
        //AI is trying to max while player is trying to min
        std::vector<Move> moves;

        for (int x = 0; x < boardSize; x++) {
            for (int y = 0; y < boardSize; y++) {
                //find the first available space and make that move
                if (board[x][y] == "-") {
                    Move move;
                    move.row = x;
                    move.column = y;
                    if (player == AI) {
                        board[x][y] = ai;
                        //get the score of that move by running the game to completion once that move is made
                        move.score = bestMove(human).score;
                    }
                    else if (player == human) {
                        board[x][y] = person;
                        move.score = bestMove(AI).score;
                    }
                    //add that move to the vector
                    moves.push_back(move);
                    //set the position back to nothing because we were only checking not placing yet
                    board[x][y] = "-";
                }
            }
        }

        //bestMove is the index of the moves vector to be returned
        int bestMove = 0;
        if (player == AI) {
            //since AI is trying to max, bestScore has to be set really low
            //since it's low any move will be greater than it at first
            int bestScore = -100;
            for (int i = 0; i < moves.size(); i++) {
                //set bestMove to the index of the move with the highest possible score
                if (moves[i].score > bestScore) {
                    bestMove = i;
                    bestScore = moves[i].score;
                }
            }
        }
        else if (player == human) {
            int bestScore = 100;
            //same as above but since it's the human it needs to minimize
            for (int i = 0; i < moves.size(); i++) {
                if (moves[i].score < bestScore) {
                    bestMove = i;
                    bestScore = moves[i].score;
                }
            }
        }

        //return that best move
        return moves[bestMove];
    }


    Move bestMoveAlphaBeta(Player player, int alpha, int beta) {
        //almost the exact same as minimax except for the integers alpha and beta
        //base case for the MiniMax recursion
        std::string result = checkWin(board);
        //return a 10 if ai wins, -10 if person wins and a 0 if tie
        if (result == ai) {
            return Move(10);
        }
        else if (result == person) {
            return Move(-10);
        }
        else if (result == "Tie") {
            return Move(0);
        }

        //vector of moves to keep track of all of them to find the one the results in the either min or max score depening on the player
        //AI is trying to max while player is trying to min
        std::vector<Move> moves;

        for (int x = 0; x < boardSize; x++) {
            for (int y = 0; y < boardSize; y++) {
                //find the first available space and make that move
                if (board[x][y] == "-") {
                    Move move;
                    move.row = x;
                    move.column = y;
                    if (player == AI) {
                        board[x][y] = ai;
                        //get the score of that move by running the game to completion once that move is made
                        move.score = bestMoveAlphaBeta(human, alpha, beta).score;
                        if (alpha < move.score) {
                            alpha = move.score;
                            //set alpha to the highest score we've seen so fair
                        }
                    }
                    else if (player == human) {
                        board[x][y] = person;
                        move.score = bestMoveAlphaBeta(AI, alpha, beta).score;
                        if (beta > move.score) {
                            beta = move.score;
                            //set beta to the lowest score seen so far
                        }
                    }
                    //add that move to the vector
                    moves.push_back(move);
                    //set the position back to nothing because we were only checking not placing yet
                    board[x][y] = "-";
                    //if beta is ever less than alpha than that means the branch will not result in a better result than a perviously
                    //found move so we leave it
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
        }

        //bestMove is the index of the moves vector to be returned
        int bestMove = 0;
        if (player == AI) {
            //since AI is trying to max, bestScore has to be set really low
            //since it's low any move will be greater than it at first
            int bestScore = -100;
            for (int i = 0; i < moves.size(); i++) {
                //set bestMove to the index of the move with the highest possible score
                if (moves[i].score > bestScore) {
                    bestMove = i;
                    bestScore = moves[i].score;
                }
            }
        }
        else if (player == human) {
            int bestScore = 100;
            //same as above but since it's the human it needs to minimize
            for (int i = 0; i < moves.size(); i++) {
                if (moves[i].score < bestScore) {
                    bestMove = i;
                    bestScore = moves[i].score;
                }
            }
        }

        //return that best move
        return moves[bestMove];
    }
};

#endif //MINIMAX_GAMEBOARD_H
