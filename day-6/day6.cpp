/*
    Programmer: Tony Montemuro
    Date: 12/06/2021
    About: Advent of Code: Day 6 Solution
    To run file:
        g++ day6.cpp -o day6.exe
        ./day6
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

#define log(x) std::cout << x;
#define logLine(x) std::cout << x << std::endl;

int main() {
    //Define variables
    int nums[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::string input;
    std::ifstream stream;
    std::vector<int> listOfFish;
    std::vector<long long int> occurences(9);
    const int NUM_DAYS = 80;
    long long int sol2 = 0;

    //Read in the input from file
    stream.open("input.txt");
    if (!stream) {
        logLine("Error opening file.");
        std::terminate();
    }
    std::getline(stream, input);

    //Close file
    stream.close();

    //Split the input into a vector
    std::stringstream ss(input);
    std::string num_str;
    int num;
    while (std::getline(ss, num_str, ',')) {
        num = std::stoi(num_str);
        listOfFish.push_back(num);
        nums[num]++;
    }
 
    for (int i = 0; i < 9; i++) {
        occurences.at(i) = nums[i];
    }

    
    //Simulate 80 days
    for (int i = 0; i < NUM_DAYS; i++) {
        int size = listOfFish.size();
        for (int j = 0; j < size; j++) {
            listOfFish.at(j)--;
            int num = listOfFish.at(j);
            if (num == -1) {
                listOfFish.push_back(8);
                listOfFish.at(j) = 6;
            }
        }
    }

    //Simulate 256 days 
    for (int i = 0; i < 256; i++) {
        std::vector<long long int> copy(9);
        for (int j = 0; j < 9; j++) {
            if (j == 0) {
                copy.at(6) += occurences.at(j);
                copy.at(8) += occurences.at(j);
            } else {
                copy.at(j - 1) += occurences.at(j);
            }
        }
        occurences = copy;
    }

    //Compute solution 2
    for (int i = 0; i < 9; i++) {
        sol2 += occurences.at(i);
    }

    //Print solutions
    logLine(listOfFish.size());  
    logLine(sol2);  

    //Terminate program
    return 0;   
}