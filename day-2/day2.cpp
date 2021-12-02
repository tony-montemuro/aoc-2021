/*
    Programmer: Tony Montemuro
    Date: 12/02/2021
    About: Advent of Code: Day 2 Solution
    To run file:
        g++ day2.cpp -o day2.exe
        ./day2
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#define logLine(x) std::cout << x << std::endl;

int main() {
    //Define variables
    int hor = 0, ver = 0, aim = 0, sol1, sol2;
    std::vector<std::string> directions;
    std::vector<int> vals;
    std::ifstream input;
    //Open file
    input.open("input.txt");
    if (!input) logLine("Error opening file"); //If input could not be read, print information to user
    //Read each line from the file
    while (!input.eof()) {
        int i = 0, num;
        std::string line, dir, strNum;
        getline(input, line, '\n');
        while (isalpha(line[i])) {
            i++;
        }
        //This is the direction of each line
        dir = line.substr(0, i);
        directions.push_back(dir);
        //This is the number of each line
        strNum = line[++i];
        std::stringstream stream1(strNum);
        stream1 >> num;
        vals.push_back(num);
    }
    //3 cases:
    //If direction is forward, increment hor by the value of instruction
    //If direction is up, decrement ver by the value of instruction
    //If direction is down, increment ver by the value of instruction
    for (int i = 0; i < directions.size(); i++) {
        if (directions.at(i) == "forward") {
            hor += vals.at(i);
        }
        else if (directions.at(i) == "up") {
            ver -= vals.at(i);
        }
        else {
            ver += vals.at(i);
        }
    }
    //Solution 1 is the product of hor and ver
    sol1 = hor * ver;
    logLine(sol1);
    hor = 0, ver = 0; //Reset hor and ver
    //3 cases:
    //If direction is forward, increment hor by the value of instruction, and increment ver by the product of aim and the value of instruction
    //If direction is up, decrement aim by the value of instruction
    //If direction is down, increment aim by the value of instruction
    for (int i = 0; i < directions.size(); i++) {
        if (directions.at(i) == "forward") {
            hor += vals.at(i);
            ver += (aim * vals.at(i));
        }
        else if (directions.at(i) == "up") {
            aim -= vals.at(i);
        }
        else {
            aim += vals.at(i);
        }
    }
    //Solution 2 is the product of hor and ver
    sol2 = hor * ver;
    logLine(sol2);
    return 0;
}