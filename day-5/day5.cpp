/*
    Programmer: Tony Montemuro
    Date: 12/05/2021
    About: Advent of Code: Day 5 Solution
    To run file:
        g++ day5.cpp -o day5.exe
        ./day5
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

#define log(x) std::cout << x;
#define logLine(x) std::cout << x << std::endl;

enum orientation
{
    VERTICAL = 0,
    HORIZONTAL = 1,
    DIAG = 2
};

void updateDiagram(int x1, int x2, int y1, int y2, std::vector<std::vector<int>>& diagram, orientation o);

int main() {
    //Define variables
    std::ifstream input;
    std::vector<std::vector<int>> diagram;
    std::vector<std::vector<int>> points1;
    std::vector<std::vector<int>> points2;
    const int SIZE = 1000;
    int sol1 = 0, sol2 = 0;
    //Open file
    input.open("input.txt");
    if (!input) logLine("Error opening file"); //If input could not be read, print information to user
    //Read input
    while (!input.eof()) {
        int x1, x2, y1, y2;
        std::string line, numStr = "";
        getline(input, line, '\n');
        char c = ',';
        size_t found = line.find(c);
        std::string x1Str = line.substr(0, found);
        std::stringstream x1Stream(x1Str);
        x1Stream >> x1;
        c = ' ';
        size_t temp = found+1;
        found = line.find(c);
        std::string y1Str = line.substr(temp, found);
        std::stringstream y1Stream(y1Str);
        y1Stream >> y1;
        c = ',';
        temp = found+4;
        found = line.find(c, temp);
        std::string x2Str = line.substr(temp, found);
        std::stringstream x2Stream(x2Str);
        x2Stream >> x2;
        std::string y2Str = line.substr(found+1, line.size()-1);
        std::stringstream y2Stream(y2Str);
        y2Stream >> y2;
        std::vector<int> p1, p2;
        p1.push_back(x1);
        p1.push_back(y1);
        p2.push_back(x2);
        p2.push_back(y2);
        points1.push_back(p1);
        points2.push_back(p2);
    }
    //Close File
    input.close();
    //Initialize diagram
    for (int i = 0; i < SIZE; i++) {
        std::vector<int> line;
        for (int j = 0; j < SIZE; j++) {
            line.push_back(0);
        } 
        diagram.push_back(line);
    }
    //Find all vertical & horizontal lines
    for (int i = 0; i < points1.size(); i++) {
        int x1 = points1.at(i).at(0), x2 = points2.at(i).at(0);
        int y1 = points1.at(i).at(1), y2 = points2.at(i).at(1);
        if (x1 == x2) {
            updateDiagram(x1, x2, y1, y2, diagram, VERTICAL);
        }
        else if (y1 == y2) {
            updateDiagram(x1, x2, y1, y2, diagram, HORIZONTAL);
        }
    }

    //Now count all the points where 2 or more lines overlap
    for (int i = 0; i < diagram.size(); i++) {
        for (int j = 0; j < diagram.at(0).size(); j++) {
            if (diagram.at(i).at(j) >= 2) {
                sol1++;
            }
        }
    }
    //Find all diagonal lines
    for (int i = 0; i < points1.size(); i++) {
        int x1 = points1.at(i).at(0), x2 = points2.at(i).at(0);
        int y1 = points1.at(i).at(1), y2 = points2.at(i).at(1);
        if (abs(x2-x1) == abs(y2-y1)) {
            updateDiagram(x1, x2, y1, y2, diagram, DIAG);
        }
    }
    //Now, count all the points where 2 or more lines overlap
    for (int i = 0; i < diagram.size(); i++) {
        for (int j = 0; j < diagram.at(0).size(); j++) {
            if (diagram.at(i).at(j) >= 2) {
                sol2++;
            }
        }
    }
    logLine(sol1);
    logLine(sol2);
    return 0;
}

void updateDiagram(int x1, int x2, int y1, int y2, std::vector<std::vector<int>>& diagram, orientation o) {
    if (o == VERTICAL) {
        int size = abs(y2-y1)+1;
        if (y1 < y2) {
            for (int i = y1; i < size+y1; i++) {
                diagram.at(i).at(x1)++;
            }
        } else {
            for (int i = y1; i > y1-size; i--) {
                diagram.at(i).at(x1)++;
            }
        }
    } 
    else if (o == HORIZONTAL) {
        int size = abs(x2-x1)+1;
        if (x1 < x2) {
            for (int i = x1; i < size+x1; i++) {
                diagram.at(y1).at(i)++;
            }
        } else {
            for (int i = x1; i > x1-size; i--) {
                diagram.at(y1).at(i)++;
            }
        }
    } 
    else if (o == DIAG) {
        int size = abs(x2-x1);
        size++;
        int deltaX = x2-x1, deltaY = y2-y1;
        //Case 1: Down Right
        if (deltaX > 0 && deltaY > 0) {
            int j = y1;
            for (int i = x1; i < size+x1; i++) {
                diagram.at(j).at(i)++;
                j++;
            }
        }
        //Case 2: Up Left
        else if (deltaX < 0 && deltaY < 0) {
            int j = y1;
            for (int i = x1; i > x1-size; i--) {
                diagram.at(j).at(i)++;
                j--;
            }
        }
        //Case 3: Up Right
        else if (deltaX > 0 && deltaY < 0) {
            int j = y1;
            for (int i = x1; i < size+x1; i++) {
                diagram.at(j).at(i)++;
                j--;
            }
        }
        //Case 4: Down Left
        else if (deltaX < 0 && deltaY > 0) {
            int j = y1;
            for (int i = x1; i > x1-size; i--) {
                diagram.at(j).at(i)++;
                j++;
            }
        }
    }
}