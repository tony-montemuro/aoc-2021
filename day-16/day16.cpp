/*
    Programmer: Tony Montemuro
    Date: 12/16/2021
    About: Advent of Code: Day 16 Solution
    To run file:
        g++ day16.cpp -o day16.exe
        ./day16
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

struct PacketInfo {
    int current = 0;
    long long int val;
};

std::string hexToBinary(std::string hexStr);
int binStrToIntDec(std::string binStr);
PacketInfo process(std::string bits, int& versionSum);
long long int binStrToLongLongIntDec(std::string binStr);

int main() {
    //Define variables
    std::ifstream stream;
    std::string packet, binPacket;
    int versionSum = 0;
    long long int packetVal;

    //Open file
    stream.open("input.txt");
    if (!stream) {
        logLine("Error opening file"); //If input could not be read, print information to user
        std::terminate();
    }

    //Read input, and convert to binary
    std::getline(stream, packet, '\n');
    binPacket = hexToBinary(packet);

    //Close file
    stream.close();


    //Compute the sum of the list of versions, and print to user
    PacketInfo info = process(binPacket, versionSum);
    packetVal = info.val;
    logLine(versionSum);
    logLine(packetVal);

    return 0;
}

PacketInfo process(std::string bits, int& versionSum) {
    //First, create a new PacketInfo object
    PacketInfo info = PacketInfo();
    int version, typeID;

    //Next, parse the version and typeID number
    version = binStrToIntDec(bits.substr(info.current, 3));
    info.current += 3;
    versionSum += version;
    typeID = binStrToIntDec(bits.substr(info.current, 3));
    info.current += 3;

    //If the typeID is 4, this means this packet stores a literal value. Compute this literal value, and return packetinfo.
    if (typeID == 4) {
        std::string literal = "";
        while (true) {
            literal += bits.substr(info.current+1, 4);
            info.current += 5;
            if (bits.at((info.current)-5) == '0') break;
        }
        info.val = binStrToLongLongIntDec(literal);
        return info;
    }

    //Otherwise, packet is some operational packet.

    //Next, define a vals vector: this will store the values returned by each subpacket
    std::vector<long long int> vals;

    //Length type is the 7th-bit, it will determine how the subpackets are parsed.
    char lengthType = bits.at(info.current);
    info.current++;

    //If lengthType is 0, this means that the bits stored in length-bits determine how many bits after the length-bits store subpackets.
    if (lengthType == '0') {
        int subLength = binStrToIntDec(bits.substr(info.current, 15));
        info.current += 15;
        int totalLength = subLength + info.current;
        while (info.current < totalLength) {
            PacketInfo newInfo = process(bits.substr(info.current, bits.size()-info.current), versionSum);
            info.current += newInfo.current;
            vals.push_back(newInfo.val);
        }

    //If lengthType is 1, this means that the bits stored in length-bits determine how many subpackets are stored in the packet.
    } else {
        int numPacks = binStrToIntDec(bits.substr(info.current, 11));
        info.current += 11;
        for (int i = 0; i < numPacks; i++) {
            PacketInfo newInfo = process(bits.substr(info.current, bits.size()-info.current), versionSum);
            info.current += newInfo.current;
            vals.push_back(newInfo.val);
        }
    }

    //Finally, for each operational packet, determine which operations to perform depending on the typeID
    long long int sum = 0, product = 1, minVal = LLONG_MAX, maxVal = LLONG_MIN;
    switch (typeID) {
        //Addition
        case 0:
            for (long long int val : vals) sum += val;
            info.val = sum;
            break;
        //Multiplication
        case 1: 
            for (long long int val : vals) {
                product *= val;
            }
            info.val = product;
            break;
        //Min value
        case 2: 
            for (long long int val : vals) if (val < minVal) minVal = val;
            info.val = minVal;
            break;
        //Max value
        case 3: 
            for (long long int val : vals) if (val > maxVal) maxVal = val;
            info.val = maxVal;
            break;
        //Greater than
        case 5: 
            if (vals.at(0) > vals.at(1)) info.val = 1;
            else info.val = 0;
            break;
        //Less than
        case 6: 
            if (vals.at(0) < vals.at(1)) info.val = 1;
            else info.val = 0;
            break;
        case 7: 
        //Equal to
            if (vals.at(0) == vals.at(1)) info.val = 1;
            else info.val = 0;
            break;
        default: break;
    }

    //Return the packet info
    return info;
}

//This function simply takes any hex string and creates it's corresponding binary string
std::string hexToBinary(std::string hexStr) {
    std::string binStr;
    for (char hexChar : hexStr) {
        switch(hexChar) {
            case '0':
                binStr += "0000";
                break;
            case '1':
                binStr += "0001";
                break;
            case '2':
                binStr += "0010";
                break;
            case '3':
                binStr += "0011";
                break;
            case '4':
                binStr += "0100";
                break;
            case '5':
                binStr += "0101";
                break;
            case '6':
                binStr += "0110";
                break;
            case '7':
                binStr += "0111";
                break;
            case '8':
                binStr += "1000";
                break;
            case '9':
                binStr += "1001";
                break;
            case 'A':
                binStr += "1010";
                break;
            case 'B':
                binStr += "1011";
                break;
            case 'C':
                binStr += "1100";
                break;
            case 'D':
                binStr += "1101";
                break;
            case 'E':
                binStr += "1110";
                break;
            case 'F':
                binStr += "1111";
                break;
            default:
                break; 
        }        
    }
    return binStr;
}

//This function takes any binary string and returns a decimal integer
int binStrToIntDec(std::string binStr) {
    int num = 0;
    double power = 0;
    for (int i = binStr.size()-1; i >= 0; i--) {
        int bit = binStr.at(i) - '0';
        num += (int)(bit * pow(2, power));
        power++;
    }
    return num;
}

//This function takes any binary string and returns a decinal long long integer
long long int binStrToLongLongIntDec(std::string binStr) {
    long long int num = 0;
    double power = 0;
    for (int i = binStr.size()-1; i >= 0; i--) {
        int bit = binStr.at(i) - '0';
        num += (long long int)(bit * pow(2, power));
        power++;
    }
    return num;
}