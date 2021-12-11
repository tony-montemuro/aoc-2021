/*
    Programmer: Tony Montemuro
    Date: 12/11/2021
    About: Advent of Code: Day 11 Solution
    To run file:
        g++ day11.cpp -o day11.exe
        ./day11
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

#define log(x) std::cout << x;
#define logLine(x) std::cout << x << std::endl;

void energyIncrease(std::vector<std::string>& octopii);
int flashCounter(std::vector<std::string>& octopii);
int flash(std::vector<std::string>& octopii, int y, int x, char octopus);
bool haveAllFlashed(std::vector<std::string> octopii);

int main() {
    //Define variables
    std::ifstream stream;
    std::vector<std::string> octopii;
    const int NUM_STEPS = 100;
    int numFlashes = 0, stepsToAllFlash;
    bool first100 = false;

    //Open file
    stream.open("input.txt");
    if (!stream) {
        logLine("Error opening file"); //If input could not be read, print information to user
        std::terminate();
    }

    //Read input
    std::string line;
    while (std::getline(stream, line)) {
        octopii.push_back(line);
    }

    //Close file
    stream.close();

    //Pad the edges with dots
    char dot = '.';
    line = octopii.at(0);
    std::string dots;
    for (int i = 0; i < line.size(); ++i) {
        dots.push_back(dot);
    }
    octopii.insert(octopii.begin(), dots);
    octopii.insert(octopii.end(), dots);
    for (std::string& line : octopii) {
        line = dot + line;
        line = line + dot;
    }

    for (stepsToAllFlash = 1; stepsToAllFlash <= NUM_STEPS; stepsToAllFlash++) {
        energyIncrease(octopii);
        numFlashes += flashCounter(octopii);
        if (haveAllFlashed(octopii)) {
            log("Steps until all flash: ");
            logLine(stepsToAllFlash);
            first100 = true;
        }
    }

    if (!first100) {
        while (!haveAllFlashed(octopii)) {
            energyIncrease(octopii);
            int x = flashCounter(octopii);
            stepsToAllFlash++;
        }
        log("Steps until all flash: ");
        logLine(--stepsToAllFlash);
    }

    //Print results
    log("Number of flashes in first 100 steps: ");
    logLine(numFlashes);

    return 0;
}

void energyIncrease(std::vector<std::string>& octopii) {
    int lineLength = octopii.at(0).size();
    for (int i = 1; i < octopii.size()-1; i++) {
        for (int j = 1; j < lineLength-1; j++) {
            octopii.at(i).at(j)++;
        }
    }
}

int flashCounter(std::vector<std::string>& octopii) {
    int lineLength = octopii.at(0).size(), numFlash = 0;
    for (int i = 1; i < octopii.size()-1; i++) {
        for (int j = 1; j < lineLength-1; j++) {
            char octopus = octopii.at(i).at(j);
            if (octopus > '9') numFlash += flash(octopii, i, j, octopus);
        }
    }
    return numFlash;
}

int flash(std::vector<std::string>& octopii, int y, int x, char octopus) {
    int numFlash = 1;
    //Loop 1, initiate flash
    for (int i = y-1; i <= y+1; i++) {
        for (int j = x-1; j <= x+1; j++) {
            bool validOctopus = (octopii.at(i).at(j) != '.' && octopii.at(i).at(j) != '0');
            if ((i != y || j != x) && validOctopus) {
                octopii.at(i).at(j)++;
            }
            else if (validOctopus) {
                octopii.at(i).at(j) = '0';
            }
        }
    }
    //Loop 2, check for any new flashes
    for (int i = y-1; i <= y+1; i++) {
        for (int j = x-1; j <= x+1; j++) {
            if ((i != y || j != x) && octopii.at(i).at(j) > '9') {
                char c = octopii.at(i).at(j);
                numFlash += flash(octopii, i, j, octopii.at(i).at(j));
            }
        }
    }
    return numFlash;
}

bool haveAllFlashed(std::vector<std::string> octopii) {
    int lineLength = octopii.at(0).size()-1;
    for (int i = 1; i < octopii.size()-1; i++) {
        for (int j = 1; j < lineLength; j++) {
            char octopus = octopii.at(i).at(j);
            if (octopus != '0') return false;
        }
    }
    return true;
}