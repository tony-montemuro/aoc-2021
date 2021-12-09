/*
    Programmer: Tony Montemuro
    Date: 12/09/2021
    About: Advent of Code: Day 9 Solution
    To run file:
        g++ day9.cpp -o day9.exe
        ./day9
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <algorithm>

#define log(x) std::cout << x;
#define logLine(x) std::cout << x << std::endl;

int charToInt(char c);
bool isHigher(int currentPos, int targetPos);
std::vector<std::vector<bool>> generateMap(std::vector<std::string> lavaTubes);
int calcBasinSize(int x, int y, std::vector<std::string> lavaTubes, std::vector<std::vector<bool>>& boolMap);
int threeLargestBasinProduct(std::vector<int> basinSizes);

const int DOT_VAL = std::numeric_limits<int>::max();

int main() {
    //Define variables
    std::ifstream stream;
    std::vector<std::string> lavaTubes;
    std::vector<int> basinSizes;
    char dot = '.';
    int riskLevel = 0;

    //Open file
    stream.open("input.txt");
    if (!stream) {
        logLine("Error opening file"); //If input could not be read, print information to user
        std::terminate();
    }

    //Read input
    std::string line;
    while (std::getline(stream, line)) {
        lavaTubes.push_back(line);
    }

    //Close file
    stream.close();

    //Pad the edges with dots
    line = lavaTubes.at(0);
    std::string dots;
    for (int i = 0; i < line.size(); ++i) {
        dots.push_back(dot);
    }
    lavaTubes.insert(lavaTubes.begin(), dots);
    lavaTubes.insert(lavaTubes.end(), dots);
    for (std::string& line : lavaTubes) {
        line = dot + line;
        line = line + dot;
    }

    //Now, determine all low points, and use to compute risk level
    for (int i = 1; i < lavaTubes.size() - 1; i++) {
        std::string lavaRow = lavaTubes.at(i);
        for (int j = 1; j < lavaRow.size() - 1; j++) {
            int currentPos = charToInt(lavaTubes.at(i).at(j));
            int up = charToInt(lavaTubes.at(i-1).at(j)), down = charToInt(lavaTubes.at(i+1).at(j));
            int left = charToInt(lavaTubes.at(i).at(j-1)), right = charToInt(lavaTubes.at(i).at(j+1));
            if (isHigher(currentPos, up) && isHigher(currentPos, down) && isHigher(currentPos, left) && isHigher(currentPos, right)) {
                riskLevel += (currentPos + 1);
                //For part 2, we need to create a boolean map of the lavaTubes
                std::vector<std::vector<bool>> boolMap = generateMap(lavaTubes);
                basinSizes.push_back(calcBasinSize(j, i, lavaTubes, boolMap));
            }
        }
    }

    //Print solution 1 and 2
    logLine(riskLevel);
    logLine(threeLargestBasinProduct(basinSizes));

    return 0;
}

//Converts a character to an int
int charToInt(char c) {
    if (c == '.') return DOT_VAL;
    return c - '0';
}

//Determines whether a target position is greater than the current position
bool isHigher(int currentPos, int targetPos) {
    if (targetPos > currentPos) return true;
    return false;
}

//Generates a boolean map of the lava tubes in order to calculate basin sizes
std::vector<std::vector<bool>> generateMap(std::vector<std::string> lavaTubes) {
    std::vector<std::vector<bool>> boolMap;
    for (int i = 0; i < lavaTubes.size(); i++) {
        std::vector<bool> boolRow;
        for (int j = 0; j < lavaTubes.at(i).size(); j++) {
            if (lavaTubes.at(i).at(j) == '.') {
                boolRow.push_back(true);
            } else {
                boolRow.push_back(false);
            }
        }
        boolMap.push_back(boolRow);
    }
    return boolMap;
}

//A recursive function that is used to calculate the size of a particular basin
int calcBasinSize(int x, int y, std::vector<std::string> lavaTubes, std::vector<std::vector<bool>>& boolMap) {
    int size = 1;
    int currentVal = charToInt(lavaTubes.at(y).at(x));
    boolMap.at(y).at(x) = true;
    int up = charToInt(lavaTubes.at(y-1).at(x)), down = charToInt(lavaTubes.at(y+1).at(x));
    int left = charToInt(lavaTubes.at(y).at(x-1)), right = charToInt(lavaTubes.at(y).at(x+1));
    bool upStatus = boolMap.at(y-1).at(x), downStatus = boolMap.at(y+1).at(x);
    bool leftStatus = boolMap.at(y).at(x-1), rightStatus = boolMap.at(y).at(x+1);
    bool cdUp = (up > currentVal && up != 9) && !upStatus;
    bool cdDown = (down > currentVal && down != 9) && !downStatus;
    bool cdLeft = (left > currentVal && left != 9) && !leftStatus;
    bool cdRight = (right > currentVal && right != 9) && !rightStatus;
    if (!cdUp && !cdDown && !cdLeft && !cdRight) {
        return 1;
    }
    if (cdUp) size += calcBasinSize(x, y-1, lavaTubes, boolMap);
    downStatus = boolMap.at(y+1).at(x);
    cdDown = (down > currentVal && down != 9) && !downStatus;
    if (cdDown) size += calcBasinSize(x, y+1, lavaTubes, boolMap);
    leftStatus = boolMap.at(y).at(x-1);
    cdLeft = (left > currentVal && left != 9) && !leftStatus;
    if (cdLeft) size += calcBasinSize(x-1, y, lavaTubes, boolMap);
    rightStatus = boolMap.at(y).at(x+1);
    cdRight = (right > currentVal && right != 9) && !rightStatus;
    if (cdRight) size += calcBasinSize(x+1, y, lavaTubes, boolMap);
    return size;
}

//Computes the product of the size of the three largest basin sizes
int threeLargestBasinProduct(std::vector<int> basinSizes) {
    std::sort(basinSizes.begin(), basinSizes.end());
    int size = basinSizes.size()-1;
    return basinSizes.at(size) * basinSizes.at(size-1) * basinSizes.at(size-2);
}