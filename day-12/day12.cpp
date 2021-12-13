/*
    Programmer: Tony Montemuro
    Date: 12/12/2021
    About: Advent of Code: Day 12 Solution
    To run file:
        g++ day12.cpp -o day12.exe
        ./day12
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <unordered_map>
#include <set>

#define log(x) std::cout << x;
#define logLine(x) std::cout << x << std::endl;

struct Path {
    std::string caveA;
    std::string caveB;
};

int numPathsPart1(std::vector<Path> paths, std::vector<std::string>& visitedCaves, std::string currentCave);
bool hasValidReturn(std::vector<Path> paths, std::string cave);
bool isStrLower(std::string s);
std::vector<std::string> nextCaves(std::vector<Path> paths, std::vector<std::string> visitedCaves, std::string cave);
std::vector<std::string> getUniqueCaves(std::vector<Path> paths);

int main() {
    //Define variables
    std::ifstream stream;
    std::vector<std::string> input;
    std::vector<Path> paths;
    int pathCount1 = 0;

    //Open file
    stream.open("input.txt");
    if (!stream) {
        logLine("Error opening file"); //If input could not be read, print information to user
        std::terminate();
    }

    //Read input
    std::string line;
    while (std::getline(stream, line)) {
        input.push_back(line);
    }

    //Parse the input into the paths vector
    for (std::string line : input) {
        std::string s;
        Path p = Path();
        std::stringstream ss(line);
        std::getline(ss, s, '-');
        p.caveA = s;
        std::getline(ss, s, '\n');
        p.caveB = s;
        paths.push_back(p);
    }

    //Close file
    stream.close();

    //Now, get the number of paths through the cave system [part 1]
    std::vector<std::string> visitedCaves;
    pathCount1 = numPathsPart1(paths, visitedCaves, "start");

    //Print result to screen
    logLine(pathCount1);

    //Unable to solve part 2

    return 0;
}

int numPathsPart1(std::vector<Path> paths, std::vector<std::string>& visitedCaves, std::string currentCave) {
    int num = 0;
    visitedCaves.push_back(currentCave);
    if (isStrLower(currentCave)) {
        if (!hasValidReturn(paths, currentCave)) {
            visitedCaves.pop_back();
            return 0;
        }
    }
    if (currentCave == "end") {
        visitedCaves.pop_back();
        return 1;
    }
    std::vector<std::string> connectedCaves = nextCaves(paths, visitedCaves, currentCave);
    for (std::string c : connectedCaves) {
        num += numPathsPart1(paths, visitedCaves, c);
    }
    visitedCaves.pop_back();
    return num;
}

bool hasValidReturn(std::vector<Path> paths, std::string cave) {
    for (Path p : paths) {
        if (p.caveA == cave || p.caveB == cave) {
            if (!isStrLower(p.caveA) || !isStrLower(p.caveB)) return true;
        }
    }
    return false;
}

std::vector<std::string> nextCaves(std::vector<Path> paths, std::vector<std::string> visitedCaves, std::string cave) {
    std::vector<std::string> next;
    for (Path p : paths) {
        std::string nextCave = "";
        bool validCave = true;
        if (p.caveA == cave) nextCave = p.caveB;
        else if (p.caveB == cave) nextCave = p.caveA;
        if (nextCave.empty() || nextCave == "start") continue;
        for (std::string visitedCave : visitedCaves) {
            if (nextCave == visitedCave && isStrLower(nextCave)) validCave = false;
        }
        if (validCave) next.push_back(nextCave);
    }
    return next;
}


bool isStrLower(std::string s) {
    if (std::islower(s.at(0)) && s != "end") return true;
    return false;
}

std::vector<std::string> getUniqueCaves(std::vector<Path> paths) {
    std::vector<std::string> uniquePath;
    uniquePath.push_back(paths.at(0).caveA);
    for (Path p : paths) {
        bool uniqueCaveA = true, uniqueCaveB = true;
        std::string caveA = p.caveA, caveB = p.caveB;
        for (std::string cave : uniquePath) {
            if (caveA == cave) uniqueCaveA = false;
            if (caveB == cave) uniqueCaveB = false;
        }
        if (uniqueCaveA) uniquePath.push_back(caveA);
        if (uniqueCaveB) uniquePath.push_back(caveB);
    }
    return uniquePath;
}