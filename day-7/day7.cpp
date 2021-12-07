/*
    Programmer: Tony Montemuro
    Date: 12/07/2021
    About: Advent of Code: Day 7 Solution
    To run file:
        g++ day7.cpp -o day7.exe
        ./day7
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

#define log(x) std::cout << x;
#define logLine(x) std::cout << x << std::endl;

int fuelCost(int length);

int main() {
    //Define variables
    std::ifstream stream;
    std::string input;
    std::vector<int> horPositions;

    //Open file
    stream.open("input.txt");
    if (!stream) {
        logLine("Error opening file"); //If input could not be read, print information to user
        std::terminate();
    }

    //Read input
    std::getline(stream, input); 

    //Close file
    stream.close();

    //Split the input into a vector
    std::stringstream ss(input);
    std::string num_str;
    int num;
    while (std::getline(ss, num_str, ',')) {
        num = std::stoi(num_str);
        horPositions.push_back(num);
    }

    //Now, find the position that takes the least fuel (part 1)
    int optimalPosition;
    long long int bestFuel = 1000000;
    for (int position : horPositions) {
        int targetPosition = position;
        int fuelCounter = 0;
        for (int i = 0; i < horPositions.size(); i++) {
            fuelCounter += abs(targetPosition - horPositions.at(i));
        }
        if (fuelCounter < bestFuel) {
            optimalPosition = targetPosition;
            bestFuel = fuelCounter;
        }
    }

    //Solution to part 1
    logLine(bestFuel);

    //Now, find the position that takes the least fuel (part 2)
    bestFuel = 1000000000;
    for (int position : horPositions) {
        int targetPosition = position;
        int fuelCounter = 0;
        for (int i = 0; i < horPositions.size(); i++) {
            fuelCounter += fuelCost(abs(targetPosition - horPositions.at(i)));
        }
        if (fuelCounter < bestFuel) {
            optimalPosition = targetPosition;
            bestFuel = fuelCounter;
        }
    }

    //Solution to part 2
    logLine(bestFuel);

    return 0;
}

int fuelCost(int length) {
    return (length * (length + 1) / 2);
}