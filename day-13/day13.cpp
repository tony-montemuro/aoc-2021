/*
    Programmer: Tony Montemuro
    Date: 12/13/2021
    About: Advent of Code: Day 13 Solution
    To run file:
        g++ day13.cpp -o day13.exe
        ./day13
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <climits>

#define log(x) std::cout << x;
#define logLine(x) std::cout << x << std::endl;

struct Point {
    int x;
    int y;
};

void maxXandY(std::vector<Point> points, int& x, int& y);
void performFold(std::vector<std::string>& sheet, bool isVertical);
int numVisibleDots(std::vector<std::string> sheet);

int main() {
    //Define variables
    std::ifstream stream;
    std::vector<std::string> input;
    std::vector<Point> points;
    std::vector<char> instructions;
    std::vector<std::string> sheet;
    int num1 = 0;

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

    //Close file
    stream.close();

    //Parse input
    //First, get all points
    int i = 0;
    line = input.at(i);
    while (line != "") {
        Point p;
        std::stringstream ss(line);
        std::string segment;
        std::getline(ss, segment, ',');
        p.x = std::stoi(segment);
        std::getline(ss, segment, '\n');
        p.y = std::stoi(segment);
        points.push_back(p);
        line = input.at(++i);
    }

    //Second, the instructions (folds will always be in half)
    for (++i; i < input.size(); i++) {
        line = input.at(i);
        std::stringstream ss(line);
        std::string segment;
        std::getline(ss, segment, '=');
        instructions.push_back(segment.at(segment.size()-1));
    }

    //Now, find the max x and y of the coordinate system
    int xMax = INT_MIN, yMax = INT_MIN;
    maxXandY(points, xMax, yMax);
    
    //Now, create the sheet
    //First, place the dots
    for (int y = 0; y <= yMax; y++) {
        std::string line;
        for (int x = 0; x <= xMax; x++) {
            line.push_back('.');
        }
        sheet.push_back(line);
    }

    //Then, the #
    for (Point p : points) {
        sheet.at(p.y).at(p.x) = '#';
    }

    //Now, perform a fold instruction
    for (int i = 0; i < instructions.size(); i++) {
        if (instructions.at(i) == 'y') {
            performFold(sheet, true);
        } else {
            performFold(sheet, false);
        }
        if (i == 0) {
            num1 = numVisibleDots(sheet);
            logLine(num1);
        }
    }

    //Print result for part 2 [ANSWER: EFJKZLBL]
    for (std::string s : sheet) logLine(s);

    return 0;
}

void maxXandY(std::vector<Point> points, int& x, int& y) {
    for (Point p : points) {
        if (p.x > x) x = p.x;
        if (p.y > y) y = p.y;
    }
}

void performFold(std::vector<std::string>& sheet, bool isVertical) {
    if (isVertical) {
        int lineLength = sheet.at(0).size(), sheetSize = sheet.size();
        for (int y = sheetSize/2; y < sheetSize; y++) {
            for (int x = 0; x < lineLength; x++) {
                if (sheet.at(y).at(x) == '#') sheet.at(y - (2 * (y - sheetSize/2))).at(x) = '#';
            }
        }
        for (int y = sheetSize/2; y < sheetSize; y++) {
            sheet.pop_back();
        }
    } else {
        int lineLength = sheet.at(0).size(), sheetSize = sheet.size();
        for (int x = lineLength/2; x < lineLength; x++) {
            for (int y = 0; y < sheetSize; y++) {
                if (sheet.at(y).at(x) == '#') sheet.at(y).at(x - (2 * (x - lineLength/2))) = '#';
            }
        }
        for (int x = lineLength/2; x < lineLength; x++) {
            for (int y = 0; y < sheetSize; y++) {
                sheet.at(y).pop_back();
            }
        }
    }
}

int numVisibleDots(std::vector<std::string> sheet) {
    int n = 0;
    for (int y = 0; y < sheet.size(); y++) {
        for (int x = 0; x < sheet.at(0).size(); x++) {
            if (sheet.at(y).at(x) == '#') n++;
        }
    }
    return n;
}