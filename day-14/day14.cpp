/*
    Programmer: Tony Montemuro
    Date: 12/14/2021
    About: Advent of Code: Day 14 Solution
    To run file:
        g++ day14.cpp -o day14.exe
        ./day14
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <unordered_map>
#include <climits>

#define log(x) std::cout << x;
#define logLine(x) std::cout << x << std::endl;

long long int getDifference(std::unordered_map<char, long long int> map);
void initializeCharacterCounts(std::unordered_map<char, long long int>& characterCount);
void setCharacterCounts(std::unordered_map<char, long long int>& characterCounts, std::unordered_map<std::string, char> instructions, std::string polymerTemplate, const int steps);

int main() {
    //Define variables
    std::ifstream stream;
    std::vector<std::string> input;
    std::string polymerTemplate;
    std::unordered_map<std::string, char> instructions;
    const int STEPS_PART_1 = 10, STEPS_PART_2 = 40;
    long long int difference;

    //Open file
    stream.open("input.txt");
    if (!stream) {
        logLine("Error opening file"); //If input could not be read, print information to user
        std::terminate();
    }

    //Read file
    std::string line;
    while (std::getline(stream, line, '\n')) {
        input.push_back(line);
    }

    //Close file
    stream.close();

    //Parse the input
    int i = 0;
    polymerTemplate = input.at(i++);
    std::string temp = polymerTemplate;
    for (++i; i < input.size(); i++) {
        std::string key;
        char val;
        std::stringstream ss(input.at(i));
        std::getline(ss, line, '\n');
        key = line.substr(0, 2);
        val = line.at(line.size()-1);
        instructions[key] = val;
    }
    
    //Part 1
    std::unordered_map<char, long long int> characterCounts;
    setCharacterCounts(characterCounts, instructions, polymerTemplate, STEPS_PART_1);
    difference = getDifference(characterCounts);
    logLine(difference);

    //Part 2
    initializeCharacterCounts(characterCounts);
    setCharacterCounts(characterCounts, instructions, polymerTemplate, STEPS_PART_2);
    difference = getDifference(characterCounts);
    logLine(difference); 

    return 0;
}

void setCharacterCounts(std::unordered_map<char, long long int>& characterCounts, std::unordered_map<std::string, char> instructions, std::string polymerTemplate, const int steps) {
    std::unordered_map<std::string, long long int> pairsCount;

    //First, count the pairs in the original polymer template
    for (int i = 0; i < polymerTemplate.size()-1; i++) {
        std::string s = polymerTemplate.substr(i, 2);
        pairsCount[s]++;
    }

    //Next, apply the rules from the template, and count each pair after the rules are applied for steps amount of times
    for (int i = 1; i <= steps; i++) {
        std::unordered_map<std::string, long long int> newPairsCount;
        std::vector<std::string> pairs;
        for (auto itr = pairsCount.begin(); itr != pairsCount.end(); itr++) {
            if (itr->second != 0) pairs.push_back(itr->first);
        }
        for (std::string pair : pairs) {
            char newChar = instructions[pair];
            long long int num = pairsCount[pair];
            std::string pair1, pair2;
            pair1.push_back(pair.at(0));
            pair1.push_back(newChar);
            pair2.push_back(newChar);
            pair2.push_back(pair.at(1));
            newPairsCount[pair1] += num;
            newPairsCount[pair2] += num;
        }
        pairsCount = newPairsCount;
    }

    //Finally, count the number of characters, which is determined by the number of pairs
    for (auto itr = pairsCount.begin(); itr != pairsCount.end(); itr++) {
        char c1, c2;
        c1 = itr->first.at(0), c2 = itr->first.at(1);
        characterCounts[c1] += itr->second;
        characterCounts[c2] += itr->second;
    }
}

void initializeCharacterCounts(std::unordered_map<char, long long int>& characterCount) {
    //Reset all the values of the characterCount map [called before part 2 begins]
    for (auto itr = characterCount.begin(); itr != characterCount.end(); itr++) {
        itr->second = 0;
    }
}

long long int getDifference(std::unordered_map<char, long long int> map) {
    //First, find the max and min character counts
    long long int max = LLONG_MIN, min = LLONG_MAX;
    for (auto itr = map.begin(); itr != map.end(); itr++) {
        if (itr->second > max) {
            max = itr->second;
        }
        if (itr->second < min) {
            min = itr->second;
        }
    }
    
    //Then, return the difference of the max/2 and min/2+1 [Note, this formula will change depending on input]
    return (max/2) - (min/2+1);
}
