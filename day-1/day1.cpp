/*
    Programmer: Tony Montemuro
    Date: 12/01/2021
    About: Advent of Code: Day 1 Solution
    To run file:
        g++ day1.cpp -o day1.exe
        ./day1
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#define logLine(x) std::cout << x << std::endl;

int main() {
    //Define variables
    int numIncreases = 0, numIncreases2 = 0;
    std::vector<int> v;
    std::ifstream input;
    //Open file
    input.open("input.txt");
    if (!input) logLine("Error opening file"); //If input could not be read, print information to user
    //Store input in vector, and convert the strings into integers using streamstream
    while (!input.eof()) {
        int i;
        std::string s;
        input >> s;
        std::stringstream stream(s);
        stream >> i;
        v.push_back(i);
    }
    v.pop_back(); //Remove last element: a blank element
    input.close();
    //Part 1: Simply compare previous element to current element, and increase numIncreases if greater than
    for (int i = 1; i < v.size(); i++) {
        if (v.at(i-1) < v.at(i)) numIncreases++;
    }
    //Part 2: Compare the sums of the previous 3 sums to the current 3 sums. Ex:
    // 199  A      
    // 200  A B    
    // 208  A B C  
    // 210    B C D
    for (int i = 3; i < v.size(); i++) {
        int partialSum, sum1, sum2;
        partialSum = v.at(i-2) + v.at(i-1);
        sum1 = v.at(i-3) + partialSum;
        sum2 = partialSum + v.at(i);
        if (sum1 < sum2) numIncreases2++;
    }
    //Print results
    logLine(numIncreases);
    logLine(numIncreases2);
    return 0;
}