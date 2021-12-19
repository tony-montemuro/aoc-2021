/*
    Programmer: Tony Montemuro
    Date: 12/18/2021
    About: Advent of Code: Day 18 Solution
    To run file:
        g++ day18.cpp -o day18.exe
        ./day18
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <climits>
#include <stack>
#include <queue>

#define log(x) std::cout << x;
#define logLine(x) std::cout << x << std::endl;

enum sumState {
    Explodable,
    Splittable,
    Reduced
};

std::string add(std::string s1, std::string s2);
void reduce(std::string& s);
void explode(std::string& s);
void split(std::string& s);
sumState isReduceable(std::string s);
bool isExplodable(std::string s);
bool isSplittable (std::string s);
int magnitude(std::string s);
int findLargestMag(std::vector<std::string> pairsVect);

int main() {
    //Define variables
    std::ifstream stream;
    std::queue<std::string> pairs;
    std::string sum;
    std::vector<std::string> pairsVect;
    int mag, largestMag;

    //Open file
    stream.open("input.txt");
    if (!stream) {
        logLine("Error opening file"); //If input could not be read, print information to user
        std::terminate();
    }

    //Read input
    std::string line;
    while (std::getline(stream, line, '\n')) {
        pairs.push(line);
    }

    //Close file
    stream.close();

    //Set sum to first element, and begin adding
    sum = pairs.front();
    pairsVect.push_back(pairs.front());
    pairs.pop();
    while (!pairs.empty()) {
        sum = add(sum, pairs.front());
        reduce(sum);
        pairsVect.push_back(pairs.front());
        pairs.pop();
    }

    //Then, take the magnitude of the sum
    mag = magnitude(sum);
    logLine(mag);

    //Now, find the largest magnitude of any sum of two different snailfish numbers
    largestMag = findLargestMag(pairsVect);
    logLine(largestMag);

    return 0;
}

std::string add(std::string s1, std::string s2) {
    std::string s = "[";
    s += s1;
    s += ",";
    s += s2;
    s += "]";
    return s;
}

void reduce(std::string& s) {
    sumState state = isReduceable(s);
    while (state != Reduced) {
        while (isExplodable(s)) {
            explode(s);
        }
        if (isSplittable(s)) {
            split(s);
        }
        state = isReduceable(s);
    }
}

//This function will add each pair together in both directions, and will return the largest sum
int findLargestMag(std::vector<std::string> pairsVect) {
    int largestMag = INT_MIN;
    for (int i = 0; i < pairsVect.size(); i++) {
        for (int j = 0; j < pairsVect.size(); j++) {
            if (i != j) {
                std::string num1 = pairsVect.at(i), num2 = pairsVect.at(j);
                std::string sum = add(num1, num2);
                reduce(sum);
                int mag = magnitude(sum);
                if (mag > largestMag) largestMag = mag;
            }
        }
    }
    return largestMag;
}

//This function will 'explode' the string
void explode(std::string& s) {
    std::stack<char> bracketStack;
    int i = 0;
    
    //First, find the position of the first pair nested in 4 pairs. Extract the values in the pair, and replace with a 0
    for (i; i < s.size(); ++i) {
        if (s.at(i) == '[') bracketStack.push(s.at(i));
        if (s.at(i) == ']') bracketStack.pop();
        if (bracketStack.size() == 5) {
            break;
        }
    }
    if (i == s.size()) return;
    std::string leftValStr = "", rightValStr = "";
    int itr = i+1;
    while (std::isdigit(s.at(itr))) {
        leftValStr += s.at(itr);
        ++itr;
    }
    ++itr;
    while (std::isdigit(s.at(itr))) {
        rightValStr += s.at(itr);
        ++itr;
    }
    s.replace(i, 3 + leftValStr.size() + rightValStr.size(), "0");

    //Now, check for number closest to the left value
    std::string firstLeftStr = "";
    int j = i-1, leftSum;
    while (j >= 0) {
        if (std::isdigit(s.at(j))) {
            while (std::isdigit(s.at(j))) {
                firstLeftStr.insert(0, 1, s.at(j));
                --j;
            }
            break;
        }
        --j;
    }

    //If there does exist a number, add it by the left value in the nested pair. Otherwise, do nothing
    std::string leftSumStr = ".";
    if (j >= 0) {
        leftSum = std::stoi(leftValStr) +  std::stoi(firstLeftStr);
        leftSumStr = std::to_string(leftSum);
        s.replace(j+1, firstLeftStr.size(), leftSumStr);
    }

    //Now, check for number closest to the right value
    j = i+leftSumStr.size();
    std::string firstRightStr = "";
    int rightSum, breakPoint;
    while (j < s.size()) {
        if (std::isdigit(s.at(j))) {
            breakPoint = j;
            while (std::isdigit(s.at(j))) {
                firstRightStr += s.at(j);
                ++j;
            }
            break;
        }
        ++j;
    }

    //If there does exist a number, add it by the right value in the nested pair. Otherwise, do nothing
    if (j < s.size()) {
        rightSum = std::stoi(rightValStr) + std::stoi(firstRightStr);
        std::string rightSumStr = std::to_string(rightSum);
        s.replace(breakPoint, firstRightStr.size(), rightSumStr);
    }
    return;
}

//This function will 'split' the string
void split(std::string& s) {
    //First, find the number that needs to be split
    int i = 0;
    for (i; i < s.size()-1; i++) {
        if (std::isdigit(s.at(i)) && std::isdigit(s.at(i+1))) break;
    }
    if (i == s.size()-1) return;
    std::string splitNumStr = s.substr(i, 2);

    //Next, compute the value of each pair
    float splitNum;
    int leftVal, rightVal;
    splitNum = std::stof(splitNumStr);
    leftVal = (int) splitNum / 2;
    rightVal = (int) std::ceil(splitNum / 2);
    
    //Finally, construct the new pair, and insert into s
    std::string newPair = "[";
    newPair += std::to_string(leftVal);
    newPair += ",";
    newPair += std::to_string(rightVal);
    newPair += "]";
    s.replace(i, 2, newPair);
    return;
}

//This function will return the state of the string
//If it can be exploded, it will return Explodable; if it can be split, it will return Splittable;
// if no more operations can be performed, it will return Reduced
//NOTE: IF the function does not return reduced, it will return the operation that needs to be performed next.
sumState isReduceable(std::string s) {
    std::stack<char> bracketStack;
    for (int i = 0; i < s.size()-1; i++) {
        if (s.at(i) == '[') bracketStack.push(s.at(i));
        if (s.at(i) == ']') bracketStack.pop();
        if (bracketStack.size() == 5) return Explodable; //Explosion can occur
        if (std::isdigit(s.at(i)) && std::isdigit(s.at(i+1))) return Splittable; //Split can occur
    }
    return Reduced; //If for-loop completes, that means no reductions can occur
}

//This function will determine if the string is explodeable or not
bool isExplodable(std::string s) {
    std::stack<char> bracketStack;
    for (int i = 0; i < s.size(); i++) {
        if (s.at(i) == '[') bracketStack.push(s.at(i));
        if (s.at(i) == ']') bracketStack.pop();
        if (bracketStack.size() == 5) return true;
    }
    return false;
}

//This function will determine whether the string can be split or not
bool isSplittable (std::string s) {
    for (int i = 0; i < s.size()-1; i++) {
        if (std::isdigit(s.at(i)) && std::isdigit(s.at(i+1))) return true;
    }
    return false;
}

//This function will return the magnitude of the string
int magnitude(std::string s) {
    int mag = 0;
    
    //CASE 1: This is a pair of two numbers. Simply return the values added together and multiplied by their respective values.
    if (s.size() == 5) {
        int leftVal = s.at(1) - '0', rightVal = s.at(3) - '0';
        return (3 * leftVal) + (2 * rightVal);
    }

    //CASE 2: This is a pair that is a single number in the left value, and a pair of pairs in the right value.
    if (std::isdigit(s.at(1))) {
        mag = 3 * (s.at(1) - '0');
        mag += (2 * magnitude(s.substr(3, s.size()-4)));
        return mag;
    }

    //CASE 3: This is a pair that is a pair of pairs in the left value, and a single value in the right value.
    if (std::isdigit(s.at(s.size()-2))) {
        mag = 3 * magnitude(s.substr(1, s.size()-4));
        mag += 2 * (s.at(s.size()-2) - '0');
        return mag;
    }

    //CASE 4: Both values are pairs of pairs.
    std::stack<char> bracketStack;
    int i = 1;
    do {
        if (s.at(i) == '[') bracketStack.push(s.at(i));
        if (s.at(i) == ']') bracketStack.pop();
        ++i;
    } while (!bracketStack.empty());
    std::string left = s.substr(1, i-1);
    mag = (3 * magnitude(left));
    i++;
    std::string right = s.substr(i, s.size()-i-1);
    mag += (2 * magnitude(right));
    return mag;
}