/*
    Programmer: Tony Montemuro
    Date: 12/08/2021
    About: Advent of Code: Day 8 Solution
    To run file:
        g++ day8.cpp -o day8.exe
        ./day8
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>

#define log(x) std::cout << x;
#define logLine(x) std::cout << x << std::endl;

struct DisplayInformation {
    std::vector<std::string> inputVals;
    std::vector<std::string> outputVals;
    std::string code[10];
};

struct compare_length {
    bool operator()(const std::string &l, const std::string &r) const {
        return l.size()<r.size();
    }
};

void setSignalPatterns(DisplayInformation& entry);
bool str2CointainedInSr1(std::string str1, std::string str2);

int main() {
    //Define variables
    std::ifstream stream;
    std::vector<std::string> input;
    std::vector<DisplayInformation> entries;

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

    //Parse input into two parts: input and output for each line
    for (std::string line : input) {
        std::string s = "";
        DisplayInformation info = DisplayInformation();
        //First, the input values
        std::stringstream ss(line);
        ss >> s;
        while (s != "|") {
            info.inputVals.push_back(s);
            ss >> s;
        }
        //Next, the output values
        while (!ss.eof())
        {
            ss >> s;
            info.outputVals.push_back(s);
        }
        entries.push_back(info);
    }

    //Now, calculate the number of unique digits in the outputs
    int numUnique = 0;
    for (int i = 0; i < entries.size(); i++) {
        for (std::string line : entries.at(i).outputVals) {
            int length = line.size();
            if (length == 2 || length == 3 || length == 4 || length == 7) numUnique++;
        }
    }
    
    //Now, decode each entry, and sum the outputs
    int sum = 0;
    for (DisplayInformation entry : entries) {
        setSignalPatterns(entry);
        int size = entry.outputVals.size();
        for (int j = 0; j < size; j++) {
            std::string line = entry.outputVals.at(j);
            for (int k = 0; k < 10; k++) {
                std::string sortedCode = entry.code[k];
                std::string sortedLine = line;
                std::sort(sortedCode.begin(), sortedCode.end());
                std::sort(sortedLine.begin(), sortedLine.end());
                if (sortedCode == sortedLine) {
                    sum += k * std::pow(10, size - 1 - j);
                    break;
                }
            }
        }
    }

    //Print solutions
    logLine(numUnique);
    logLine(sum);

    //Close file
    stream.close();

    return 0;
}

void setSignalPatterns(DisplayInformation& entry) {
    //Now, create a new vector that will store each input sorted by length
    //param 0: the one string
    //param 1: the seven string
    //param 2: the four string
    //params 3-5: must be two, three, or five string
    //param 6-8: must be 0, 6, or 9 string
    //param 9: the eight string
    std::vector<std::string> sortedInput = entry.inputVals;
    std::sort(sortedInput.begin(), sortedInput.end(), compare_length());

    //Begin establishing our code array: the code in the array corresponds to the code of the index
    entry.code[1] = sortedInput.at(0);
    entry.code[7] = sortedInput.at(1);
    entry.code[4] = sortedInput.at(2);
    entry.code[8] = sortedInput.at(9);

    //Remove the confirmed elements from the vector
    for (auto it = sortedInput.begin(); it != sortedInput.begin()+3; ++it) {
        sortedInput.erase(sortedInput.begin());
    }
    sortedInput.erase(sortedInput.end());

    //Now, using another array we can determine the rest of the map

    //Next, we can determine 9. It must contain all the characters in the code for the number four
    for (int i = 0; i < sortedInput.size(); i++) {
        std::string code = sortedInput.at(i);
        if (code.size() == 6) {
            if (str2CointainedInSr1(code, entry.code[4])) {
                entry.code[9] = code;
                sortedInput.erase(sortedInput.begin()+i);
                break;
            }
        }
    }

    //Next, we can determine 2. It must NOT contain all the elements in the code for 9
    for (int i = 0; i < sortedInput.size(); i++) {
        std::string code = sortedInput.at(i);
        if (code.size() == 5) {
            if (!str2CointainedInSr1(entry.code[9], code)) {
                entry.code[2] = code;
                sortedInput.erase(sortedInput.begin()+i);
                break;
            }
        }
    }

    //Next, we can determine 0. It must contain all the elements in the code for 1
    for (int i = 0; i < sortedInput.size(); i++) {
        std::string code = sortedInput.at(i);
        if (code.size() == 6) {
            if (str2CointainedInSr1(code, entry.code[1])) {
                entry.code[0] = code;
                sortedInput.erase(sortedInput.begin()+i);
                break;
            }
        }
    }

    //Next, we can determine 6. It is the only code with a length of 6 left
    for (int i = 0; i < sortedInput.size(); i++) {
        std::string code = sortedInput.at(i);
        if (code.size() == 6) {
            entry.code[6] = code;
            sortedInput.erase(sortedInput.begin()+i);
            break;
        }
    }

    //Next, we can determine 3. It must contain all the elements in the code for 1
    for (int i = 0; i < sortedInput.size(); i++) {
        std::string code = sortedInput.at(i);
        if (str2CointainedInSr1(code, entry.code[1])) {
            entry.code[3] = code;
            sortedInput.erase(sortedInput.begin()+i);
            break;
        }
    }

    //Finally, the last element in the sorted input vector must be the last code, which is the code for 5
    entry.code[5] = sortedInput.at(0);
    sortedInput.clear();
}

bool str2CointainedInSr1(std::string str1, std::string str2) {
    for (char c2 : str2) {
        bool found = false;
        for (char c1 : str1) {
            if (c1 == c2) found = true;
        }
        if (!found) return false;
    }
    return true;
}