/*
    Programmer: Tony Montemuro
    Date: 12/17/2021
    About: Advent of Code: Day 17 Solution
    To run file:
        g++ day17.cpp -o day17.exe
        ./day17
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

//Target Object
struct Target {
    int lowerX;
    int lowerY;
    int higherX;
    int higherY;

    Target(int x0, int x1, int y0, int y1) {
        lowerX = x0;
        higherX = x1;
        lowerY = y0;
        higherY = y1;
    }
};


//Probe Object
struct Probe {
    int vx, vxOriginal, vy;
    int x = 0, y = 0;

    Probe(int vx, int vy) {
        this->vx = vx;
        this->vxOriginal = vx;
        this->vy = vy;
    }

    void reset() {
        x = 0;
        y = 0;
        vx = vxOriginal;
    }

    void updateVelocities() {
        if (vx > 0) vx--;
        vy--;
    }

    void updatePosition() {
        x += vx;
        y += vy;
    }
};

//Function Declarations
void parse(std::string data, int& a, int& b);
int findVx(Target t);
int findHighestY(Target t, Probe p, int& highestVy);
int calcHighestY(int y);
int findNumVel(Target t, Probe p, int lowestVx, int highestVx, int lowestVy, int highestVy);

int main() {
    //Define variables
    std::ifstream stream;
    std::string input;
    int lowestVx, highestVx, lowestVy, highestVy;
    int highestY;
    int numVel;

    //Open file
    stream.open("input.txt");
    if (!stream) {
        logLine("Error opening file"); //If input could not be read, print information to user
        std::terminate();
    }

    //Read input
    std::getline(stream, input, '\n');

    //Close file
    stream.close();

    //Parse input
    std::string xData, yData, dummy;
    int lowerX, lowerY, higherX, higherY;
    std::stringstream s(input);
    std::getline(s, dummy, ',');
    xData = dummy.substr(15, dummy.size()-15);
    std::getline(s, dummy, '\n');
    yData = dummy.substr(3, dummy.size()-3);
    parse(xData, lowerX, higherX);
    parse(yData, lowerY, higherY);
    Target target = Target(lowerX, higherX, lowerY, higherY);

    //Now, part 1: find highest y value
    lowestVx = findVx(target);
    Probe probe = Probe(lowestVx, 0);
    highestY = findHighestY(target, probe, highestVy);
    logLine(highestY);

    //Now, part 2: find how many initial velocity setups cause the probe to be within the target area
    //To do this, best way is to calculate minimum and maximums for each velocities [in part 1, we already found lowestVx and highestVy]
    highestVx = target.higherX;
    lowestVy = target.lowerY;
    numVel = findNumVel(target, probe, lowestVx, highestVx, lowestVy, highestVy);
    logLine(numVel);

    return 0;
}

//This function will compute how many possible initial velocities will hit the target
int findNumVel(Target t, Probe p, int lowestVx, int highestVx, int lowestVy, int highestVy) {
    //Counts how many times target is hit
    int n = 0;
    
    //Runs through each initial velocity
    for (int vy = lowestVy; vy <= highestVy; vy++) {
        for (int vx = lowestVx; vx <= highestVx; vx++) {
            p.vx = vx;
            p.vy = vy;
            bool contactedTarget = false;
            
            //Simulate probe until it has passed target
            while (p.y >= t.lowerY && p.x <= t.higherX) {
                p.updatePosition();
                bool inYRange = (p.y >= t.lowerY && p.y <= t.higherY);
                bool inXRange = (p.x >= t.lowerX && p.x <= t.higherX);
                if (inYRange && inXRange) contactedTarget = true;
                p.updateVelocities();
            }

            //If it hit the target, increment counter, and reset
            if (contactedTarget) n++;
            p.reset();
        }
    }
    return n;
}

//Function that will parse string
void parse(std::string data, int& a, int& b) {
    std::string aStr, bStr;
    std::stringstream s(data);
    std::getline(s, aStr, '.');
    a = std::stoi(aStr);
    std::getline(s, bStr, '\n');
    bStr = bStr.substr(1, bStr.size()-1);
    b = std::stoi(bStr);
}

//Function that will find minimum velocity in x direction depending on the target
int findVx(Target t) {
    int n = 0, vx = 0;
    while (n < t.lowerX) {
        vx++;
        n = (vx * (vx + 1)) / 2;
    }
    return vx;
}

//Function that finds the highest y-value a probe can possibly reach and still contact target
int findHighestY(Target t, Probe p, int& highestVy) {
    int highestY = 0;

    //Try all vy up to 1000
    for (int vy = 0; vy < 1000; vy++) {
        p.vy = vy;
        int currentMaxY = calcHighestY(p.vy);
        bool contactedTarget = false;

        //Simulate until probe has gone under the target
        while (p.y >= t.lowerY) {
            p.updatePosition();
            if (p.y >= t.lowerY && p.y <= t.higherY) contactedTarget = true;
            p.updateVelocities();
        }

        //If the probe contacts the target, update both highestY and highest Vy, and reset probe after
        if (contactedTarget) {
            highestY = currentMaxY;
            highestVy = vy;
        } 
        p.reset();
    }
    return highestY;
}

//Function that calculates how high probe will go given its initial vy
int calcHighestY(int vy) {
    return (vy * (vy + 1)) / 2;
}