/*
    Programmer: Tony Montemuro
    Date: 12/03/2021
    About: Advent of Code: Day 3 Solution
    To run file:
        g++ day3.cpp -o day3.exe
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
int binStrToDecInt(std::string binNum);

int main() {
    //Define variables
    std::ifstream input;
    std::vector<std::string> binNums;
    int gammaInt, epsilonInt, o2, co2, sol1, sol2;
    //Open file
    input.open("input.txt");
    if (!input) logLine("Error opening file"); //If input could not be read, print information to user
    while (!input.eof()) {
        std::string s;
        input >> s;
        binNums.push_back(s);
    }
    input.close();
    std::string gamma = "", epsilon = "";
    //Part 1
    int on = 0, off = 0;
    for (int i = 0; i < binNums.at(0).size(); i++) {
        for (int j = 0; j < binNums.size(); j++) {
            char num = binNums.at(j).at(i);
            if (num == '0') {
                off++;
            } else {
                on++;
            }
        }
        if (on > off) {
            gamma += "1";
            epsilon += "0";
        } else {
            gamma += "0";
            epsilon += "1";
        }
        on = 0, off = 0;
    }
    gammaInt = binStrToDecInt(gamma);
    epsilonInt = binStrToDecInt(epsilon);
    sol1 = gammaInt * epsilonInt;
    log("SOLUTION 1: ");
    logLine(sol1);
    //Part 2
    std::vector<std::string> oldo2vals = binNums;
    std::vector<std::string> oldco2vals = binNums;
    std::vector<std::string> o2vals, co2vals;
    std::string o2Val, co2Val;
    bool valFound = false;
    //for the length of each number
    for (int i = 0; i < binNums.at(0).size(); i++) {
        char target;
        //for the length of the entire input
        for (int j = 0; j < oldo2vals.size(); j++) {
            //if the size of vector is 1, the only element present is o2Val. You can move on to next part
            if (oldo2vals.size() == 1) {
                o2Val = o2vals.at(0);
                valFound = true;
                break;
            }
            //Compute number of 0 (off) and 1 (on) in column i
            char num = oldo2vals.at(j).at(i);
            if (num == '0') {
                off++;
            } else {
                on++;
            }
        }
        //If num 1 > num 2 or num 1 == num 2, target is 1
        if (on >= off) {
            target = '1';
        } else { //Otherwise, target is 0
            target = '0';
        }
        on = 0, off = 0;
        //If the size of oldo2vals is 1, break out of loop and move to next part of code
        if (valFound) {
            break;
        }
        //If the number in column i is equal to the target, push it into the new o2vals vector
        for (int j = 0; j < oldo2vals.size(); j++) {
            std::string currentVal = oldo2vals.at(j);
            char num = oldo2vals.at(j).at(i);
            if (num == target) {
                o2vals.push_back(currentVal);
            }
        }
        //Reset vectors
        oldo2vals.clear();
        oldo2vals = o2vals;
        o2vals.clear();
    }
    //If the value was not found earlier, now you can set o2Val
    if (!valFound) {
        o2Val = oldo2vals.at(0);
    }
    //reset flag
    valFound = false;

    //for the length of each number
    for (int i = 0; i < binNums.at(i).size(); i++) {
        char target;
        //for the length of the entire input
        for (int j = 0; j < oldco2vals.size(); j++) {
            //if the size of vector is 1, the only element present is o2Val. You can move to end of code.
            if (oldco2vals.size() == 1) {
                co2Val = oldco2vals.at(0);
                valFound = true;
                break;
            }
            char num = oldco2vals.at(j).at(i);
            //Compute number of 0 (off) and 1 (on) in column i
            if (num == '0') {
                off++;
            } else {
                on++;
            }
            //If num 1 > num 2 or num 1 == num 2, target is 0. Otherwise, target is 1
            if (on >= off) {
                target = '0';
            } else {
                target = '1';
            }
        }
        //Reset on and off
        on = 0, off = 0;
        //If the size of oldo2vals is 1, break out of loop and move to end of program
        if (valFound) {
            break;
        }
        //If the number in column i is equal to the target, push it into the new co2vals vector
        for (int j = 0; j < oldco2vals.size(); j++) {
            std::string currentVal = oldco2vals.at(j);
            char num = oldco2vals.at(j).at(i);
            if (num == target) {
                co2vals.push_back(currentVal);
            }
        }
        //Reset vectors for next loop
        oldco2vals.clear();
        oldco2vals = co2vals;
        co2vals.clear();
    }
    //If the value was not found earlier, now you can set co2Val
    if (!valFound) {
        co2Val = oldco2vals.at(0);
    }

    //Compute solution 2
    o2 = binStrToDecInt(o2Val);
    co2 = binStrToDecInt(co2Val);
    sol2 = o2 * co2;
    log("SOLUTION 2: ");
    logLine(sol2);
    return 0;
}

int binStrToDecInt(std::string binNum) {
    int decInt = 0, j = 0;
    for (int i = binNum.size()-1; i >= 0; i--) {
        char c = binNum.at(i);
        if (c == '1') {
            decInt += pow(2, j);
        }
        j++;
    }
    return decInt;
}