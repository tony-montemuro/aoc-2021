/*
    Programmer: Tony Montemuro
    Date: 12/21/2021
    About: Advent of Code: Day 21 Solution
    To run file:
        g++ day21.cpp -o day21.exe
        ./day21
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <climits>
#include <array>

#define log(x) std::cout << x;
#define logLine(x) std::cout << x << std::endl;

class Player {

//Private variables and methods
private:
    int position;
    int score;

    void updatePosition(int dieRolls) {
        position = (position + dieRolls) % 10;
    }

//Public methods
public:
    Player(int pos) {
        resetPlayer(pos);
    }

    int getPosition() {
        return position;
    }

    int getScore() {
        return score;
    }

    void rollDeterministicDie(int& die) {
        int sum = 0;
        for (int i = 1; i <= 3; i++) {
            die = (die+1 <= 100) ? die+1 : 1;
            sum += die;
        }
        updatePosition(sum);
        score += (position + 1);
    }

    void resetPlayer(int pos) {
        position = (pos - 1) % 10;
        score = 0;
    }
    
};

int getStartingPos(std::ifstream& stream);
int playDeterministicDie(Player& p1, Player& p2);
void playDiracDie(Player& p1, Player& p2, long long int numPaths);

int main() {
    //Define variables
    std::ifstream stream;
    int gameScore1, die1Start, die2Start;
    long long int numDie1Wins = 0, numDie2Wins = 0;

    //Open file
    stream.open("input.txt");
    if (!stream) {
        logLine("Error opening file"); //If input could not be read, print information to user
        std::terminate();
    }

    //Read file to create players
    die1Start = getStartingPos(stream), die2Start = getStartingPos(stream);
    Player p1 = Player(die1Start);
    Player p2 = Player(die2Start);

    //Close file
    stream.close();

    //Now, play Deterministic Die, and print result of game
    gameScore1 = playDeterministicDie(p1, p2);
    logLine(gameScore1);

    return 0;
}

//Function that will return the starting position of a particular player
int getStartingPos(std::ifstream& stream) {
    std::string posStr;
    std::getline(stream, posStr, ':');
    std::getline(stream, posStr, '\n');
    posStr = posStr.substr(1, posStr.size()-1);
    return std::stoi(posStr);
}

//Function that will play a round of deterministic die
int playDeterministicDie(Player& p1, Player& p2) {
    int losingScore, numDieRolls = 0, die = 0;
    bool player1Won;
    while (p1.getScore() < 1000 && p2.getScore() < 1000) {
        p1.rollDeterministicDie(die);
        numDieRolls += 3;
        if (p1.getScore() >= 1000) {
            player1Won = true;
            break;
        }
        p2.rollDeterministicDie(die);
        numDieRolls += 3;
    }
    losingScore = player1Won ? p2.getScore() : p1.getScore();
    return losingScore * numDieRolls;
}