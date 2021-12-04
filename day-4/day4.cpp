/*
    Programmer: Tony Montemuro
    Date: 12/04/2021
    About: Advent of Code: Day 4 Solution
    To run file:
        g++ day4.cpp -o day4.exe
        ./day2
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

#define log(x) std::cout << x;
#define logLine(x) std::cout << x << std::endl;
void stringToIntVect(std::string str_nums, std::vector<int>& nums);
void updateBoardState(std::vector<int> numsPlayed, std::vector<std::vector<int>> board, std::vector<std::vector<bool>>& boardState);
bool checkForWin(std::vector<std::vector<bool>> boardState, std::vector<int> numsPlayed);
bool checkRow(std::vector<bool> row);
int computeSol(std::vector<std::vector<int>> board, std::vector<std::vector<bool>> boardState, std::vector<int> playedNums);
void printBoardState(std::vector<std::vector<bool>> boardState);

//Used for debug
int state = 0;

int main() {
    //Define variables
    std::ifstream input;
    std::string str_nums;
    std::vector<int> nums;
    std::vector<std::vector<int>> boards;
    std::vector<std::vector<bool>> boardState;
    std::vector<int> playedNums;
    int numBoards, sol1, sol2, wonBoards = 0;
    const int BOARD_SIZE = 5;
    bool hasWon = false, firstWin = false;
    //Open file
    input.open("input.txt");
    if (!input) logLine("Error opening file"); //If input could not be read, print information to user
    //get the bingo nums
    input >> str_nums;
    std::string str_num = "";
    stringToIntVect(str_nums, nums);
    //get the bingo boards
    while (!input.eof()) {
        std::string line;
        getline(input, line, '\n');
        if (line != "") {
            std::vector<int> nums;
            stringToIntVect(line, nums);
            boards.push_back(nums);
            boardState.push_back({false, false, false, false, false});
        }
    }
    input.close();
    
    //Now, play bingo
    for (int i = 0; i < nums.size(); i++) {
        //Number is played, and the board is updated
        playedNums.push_back(nums.at(i));
        updateBoardState(playedNums, boards, boardState);
        //Check each board. If a row has a 'Bingo', remove it from the list
        for (int j = 0; j < boards.size(); j += BOARD_SIZE) {
            std::vector<std::vector<int>> board;
            std::vector<std::vector<bool>> brdState;
            for (int k = 0; k < BOARD_SIZE; k++) {
                std::vector<bool> rowState = boardState.at(j + k);
                std::vector<int> row = boards.at(j + k);
                brdState.push_back(rowState);
                board.push_back(row);
            }
            //Bingo check. If it is the first bingo, find solution. This is solution 1. Otherwise, just remove from board and keep playing
            bool boardCondition = checkForWin(brdState, playedNums);
            if (boardCondition && !firstWin) {
                sol1 = computeSol(board, brdState, playedNums);
                firstWin = true;
            }
            if (boardCondition) {
                auto it1 = boards.begin()+j;
                auto it2 = it1 + BOARD_SIZE;
                boards.erase(it1, it2);
                auto it3 = boardState.begin()+j;
                auto it4 = it3 + BOARD_SIZE;
                boardState.erase(it3, it4);
            }
            //If all boards have a 'bingo', find solution. This is solution 2. Also, set flag to break out of loop to get to end of program.
            if (boards.size() == 0) {
                sol2 = computeSol(board, brdState, playedNums);
                hasWon = true;
            }
        }
        if (hasWon) {
            break;
        }
    }
    //Print solutions to screen
    logLine(sol1);
    logLine(sol2);

    return 0;
}

//This function will take a string, and convert it into a list of numbers
void stringToIntVect(std::string str_nums, std::vector<int>& nums) {
    int i = 0;
    std::string str_num = "";
    while (i < str_nums.size()) {
        if (isdigit(str_nums.at(i))) {
            str_num += str_nums.at(i);
            i++;
        }
        else if (str_num == "") {
            i++;
        }
        else {
            std::stringstream stream1(str_num);
            int j;
            stream1 >> j;
            nums.push_back(j);
            i++;
            str_num = "";
        }
    }
    std::stringstream stream1(str_num);
    int j;
    stream1 >> j;
    nums.push_back(j);
}

//This function will update the board state after a new number is played
void updateBoardState(std::vector<int> numsPlayed, std::vector<std::vector<int>> boards, std::vector<std::vector<bool>>& boardState) {
    for (int i = 0; i < boards.size(); i++) {
        for (int j = 0; j < boards.at(0).size(); j++) {
            int currentNum = boards.at(i).at(j);
            for (int k = 0; k < numsPlayed.size(); k++) {
                if (currentNum == numsPlayed.at(k)) {
                    boardState.at(i).at(j) = true;
                }
            }
        }
    }
}

//This function will check if the board has a 'bingo'
bool checkForWin(std::vector<std::vector<bool>> boardState, std::vector<int> numsPlayed) {
    //First, check rows
    for (int i = 0; i < boardState.size(); i++) {
        std::vector<bool> row;
        for (int j = 0; j < boardState.size(); j++) {
            row.push_back(boardState.at(i).at(j));
        }
        if (checkRow(row)) {
            return true;
        }
    }
    //Next, check columns
    for (int i = 0; i < boardState.size(); i++) {
        std::vector<bool> row;
        for (int j = 0; j < boardState.size(); j++) {
            row.push_back(boardState.at(j).at(i));
        }
        if (checkRow(row)) {
            return true;
        }
    }
    return false;

}

//This function will check a particular row/column for a bingo
bool checkRow(std::vector<bool> row) {
    int k;
    for (k = 0; k < row.size(); k++) {
        if (!row.at(k)) {
            break;
        }
    }
    if (k == row.size()) {
        return true;
    }
    return false;
}

//This function will compute the solution for a particular board
//This is calulated by taking the product of the sum of all unmarked positions on board plus the last number played in the game
int computeSol(std::vector<std::vector<int>> board, std::vector<std::vector<bool>> boardState, std::vector<int> playedNums) {
    int sum = 0;
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board.size(); j++) {
            if (!boardState.at(i).at(j)) {
                sum += board.at(i).at(j);
            }
        }
    }
    return sum * playedNums.at(playedNums.size()-1);
}

//Function that will print the state of the board. Only used for debugging
void printBoardState(std::vector<std::vector<bool>> boardState) {
    log("\nSTATE ");
    logLine(state);
    for (int i = 0; i < boardState.size(); i++) {
        for (int j = 0; j < boardState.at(i).size(); j++) {
            log(boardState.at(i).at(j));
            log(" ");
        }
        log("\n");
        if (((i + 1) % 5) == 0) {
            log ("\n");
        }
    }
    state++;
}