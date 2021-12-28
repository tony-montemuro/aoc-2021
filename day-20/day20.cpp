/*
    Programmer: Tony Montemuro
    Date: 12/20/2021
    About: Advent of Code: Day 20 Solution
    To run file:
        g++ day20.cpp -o day20.exe
        ./day20
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

#define log(x) std::cout << x;
#define logLine(x) std::cout << x << std::endl;

std::vector<std::string> enhance(std::vector<std::string>& image, std::string enhanceStr);
int calcPointVal(std::string s);
bool isLit(std::string enchanceStr, int val);
int numLit(const std::vector<std::string> image);

int main () {
    //Define variables
    std::ifstream stream;
    std::string enhanceStr;
    std::vector<std::string> image, imageCopy;
    const int ENHANCE_PT_1 = 2, ENHANCE_PT_2 = 50;
    int numLit1, numLit2;
    
    //Open file
    stream.open("input.txt");
    if (!stream) {
        logLine("Error opening file"); //If input could not be read, print information to user
        std::terminate();
    }

    //Read input
    std::getline(stream, enhanceStr, '\n');
    std::string line;
    std::getline(stream, line, '\n');
    while (std::getline(stream, line, '\n')) {
        image.push_back(line);
    }

    //Close file
    stream.close();

    //Pad vertical edges with dots - this is because the image is 'infinite'
    const int PAD_LENGTH = (ENHANCE_PT_2 * 2) + 3;
    for (std::string& line : image) {
        for (int i = 0; i <= PAD_LENGTH; i++) {
            line.push_back('.');
            line.insert(0, 1, '.');
        }
    }

    //Now, pad horizontal edges with dots
    std::string verticalPad;
    for (int i = 0; i < image.at(0).size(); i++) {
        verticalPad.push_back('.');
    }
    for (int i = 0; i <= PAD_LENGTH; i++) {
        image.insert(image.begin(), verticalPad);
        image.push_back(verticalPad);
    }

    //Make a backup copy : will be used in part 2
    imageCopy = image;

    //Now, enhance the image [pt 1]
    for (int i = 0; i < ENHANCE_PT_1; i++) {
        image = enhance(image, enhanceStr);
    }
    numLit1 = numLit(image);
    logLine(numLit1);

    //Same for part 2, just more times
    image.clear();
    image = imageCopy;
    for (int i = 0; i < ENHANCE_PT_2; i++) {
        image = enhance(image, enhanceStr);
    }
    numLit2 = numLit(image);
    logLine(numLit2);

    return 0;
}

std::vector<std::string> enhance(std::vector<std::string>& image, std::string enhanceStr) {
    std::vector<std::string> enhancedImage;

    //For each row in image
    for (int i = 0; i < image.size(); i++) {
        std::string enhancedLine;
        //For each column in row
        for (int j = 0; j < image.at(i).size(); j++) {
            int pointVal;
            bool special = (i == 0 || i == image.size()-1) || (j == 0 || j == image.at(i).size()-1);

            //Case 1: current character is in special position, and the current character is a period
            if (special && image.at(i).at(j) == '.') {
                pointVal = 0;

            //Case 2: current character is in special position, and the current character is a pound sign
            } else if (special && image.at(i).at(j) == '#') {
                pointVal = 255;

            //Case 3: Standard position: calculate the pointVal using the calcPointsVal function
            } else {
                std::string pixels;
                for (int k = -1; k <= 1; k++) {
                    for (int l = -1; l <= 1; l++) {
                        pixels.push_back(image.at(i+k).at(j+l));
                    }
                }
                pointVal = calcPointVal(pixels);

            //If the pointsVal maps to a lit character, push into enhanchedLine. Otherwise, push back a period.
            }
            if (isLit(enhanceStr, pointVal)) enhancedLine.push_back('#');
            else enhancedLine.push_back('.');
        }

        //Push new line into enhanced image
        enhancedImage.push_back(enhancedLine);
    }

    //Clear original image and return
    image.clear();
    return enhancedImage;
}

//This function will calculate the pointVal for a given position and it's 8 surrounding characters
int calcPointVal(std::string s) {
    int j = s.size()-1;
    double val = 0;
    for (char c : s) {
        if (c == '#') {
            val += std::pow(2, j);
        }
        j--;
    }
    return (int) val;
}

//This function will determine whether the enchanced character will be lit or not based on val.
bool isLit(std::string enchanceStr, int val) {
    if (enchanceStr.at(val) == '#') return true;
    return false;
}

int numLit(const std::vector<std::string> image) {
    int val = 0;
    for (std::string s : image) {
        for (char c : s) {
            if (c == '#') val++;
        }
    }
    return val;
}