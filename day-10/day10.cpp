/*
    Programmer: Tony Montemuro
    Date: 12/10/2021
    About: Advent of Code: Day 10 Solution
    To run file:
        g++ day10.cpp -o day10.exe
        ./day10
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <stack>
#include <algorithm>

#define log(x) std::cout << x;
#define logLine(x) std::cout << x << std::endl;

char returnInvalidSyntax(std::string line);
int calcSyntaxError(int n1, int n2, int n3, int n4);
std::string completeSyntax(std::string line);
long long int calculateAutocompleteScore(std::string line);
long long int getMiddleScore(std::vector<long long int> nums);

int main() {
    //Define variables
    std::ifstream stream;
    std::vector<std::string> syntax;
    int totalSyntaxError = 0;
    long long int autocompleteScore = 0;

    //Open file
    stream.open("input.txt");
    if (!stream) {
        logLine("Error opening file"); //If input could not be read, print information to user
        std::terminate();
    }

    //Read input
    std::string s;
    while (std::getline(stream, s, '\n')) {
        syntax.push_back(s);
    }

    //Close file
    stream.close();

    //Find the number of each invalid syntax characters
    int numParen = 0, numBracket = 0, numBraces = 0, numCompare = 0;
    for (int i = 0; i < syntax.size(); i++) {
        std::string line = syntax.at(i);
        char invalidChar = returnInvalidSyntax(line);
        switch (invalidChar) {
            case ')':
                numParen++;
                break;
            case ']':
                numBracket++;
                break;
            case '}':
                numBraces++;
                break;
            case '>':
                numCompare++;
                break;
            default:
                break;
        }
        //Note: the space character implies that the line was not corrupted; remove all corrupted lines
        if (invalidChar != ' ')  {
            syntax.erase(syntax.begin()+i);
            i--;
        }
    }

    //Calculate the total syntax error
    totalSyntaxError = calcSyntaxError(numParen, numBracket, numBraces, numCompare);

    //Print result to user
    logLine(totalSyntaxError);

    //Now, calculate the autocompleteScore
    std::vector<long long int> scores;
    for (std::string line : syntax) {
        scores.push_back(calculateAutocompleteScore(completeSyntax(line)));
    }
    autocompleteScore = getMiddleScore(scores);

    //Print result to user
    logLine(autocompleteScore); //173651156

    return 0;
}

//Returns the invalid syntax, if any. Otherwise, returns a space character
char returnInvalidSyntax(std::string line) {
    std::stack<char> syntaxStack;
    char syntaxPairs[8] = {'(', '[', '{', '<', ')', ']', '}', '>'};
    for (char symbol : line) {
        for (int i = 0; i < 8; i++) {
            if (symbol == syntaxPairs[i]) {
                if (i < 4) {
                    syntaxStack.push(symbol);
                    break;
                }
                else {
                    char pair = syntaxStack.top(); //Returns one of the first four
                    syntaxStack.pop();
                    if (pair != syntaxPairs[i-4]) {
                        return symbol;
                    }
                    break;
                }
            }
        }
    }
    return ' ';
}

//Returns the total syntax error, which is given by the forumla below
//Param n1: Close Parenthesis, Param n2: Close Brace
//Param n3: Close Bracket, Param n4: Close Compare
int calcSyntaxError(int n1, int n2, int n3, int n4) {
    return (n1 * 3) + (n2 * 57) + (n3 * 1197) + (n4 * 25137);
}

//Returns the string of missing syntax for all lines
std::string completeSyntax(std::string line) {
    std::stack<char> syntaxStack;
    std::string complete;
    char syntaxPairs[8] = {'(', '[', '{', '<', ')', ']', '}', '>'};
    for (char symbol : line) {
        for (int i = 0; i < 8; i++) {
            if (symbol == syntaxPairs[i]) {
                if (i < 4) {
                    syntaxStack.push(symbol);
                    break;
                }
                else {
                    syntaxStack.pop();
                    break;
                }
            }
        }
    }
    while (!syntaxStack.empty()) {
        char c = syntaxStack.top();
        syntaxStack.pop();
        for (int i = 0; i < 4; i++) {
            if (c == syntaxPairs[i]) {
                complete += syntaxPairs[i+4];
                break;
            }
        }
    }
    return complete;
}

//Calculates the autocomplete score for each string of additional syntax
long long int calculateAutocompleteScore(std::string line) {
    long long int score = 0;
    for (char c : line) { 
        score = (5 * score);
        if (c == ')') {
            score++;
        }
        else if (c == ']') {
            score += 2;
        }
        else if (c == '}') {
            score += 3;
        }
        else {
            score += 4;
        }
    }
    return score;
}

//Returns the middle score of all autoComplete scores
long long int getMiddleScore(std::vector<long long int> nums) {
    std::sort(nums.begin(), nums.end());
    return nums.at(nums.size()/2);
}