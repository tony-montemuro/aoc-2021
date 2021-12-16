/*
    Programmer: Tony Montemuro
    Date: 12/15/2021
    About: Advent of Code: Day 15 Solution
    To run file:
        g++ day15.cpp -o day15.exe
        ./day15
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <climits>
#include <unordered_set>
#include <utility>

#define log(x) std::cout << x;
#define logLine(x) std::cout << x << std::endl;

class Node {
public:
    //Public variables
    int vertexNumber;
    std::vector<std::pair<int, int>> children;
    
    //Public Methods
    Node(int num) {
        this->vertexNumber = num;
    }

    void addChild(int vNum, int length) {
        std::pair<int, int> p;
        p.first = vNum;
        p.second = length;
        children.push_back(p);
    }
};

int nodeNumber(int x, int y);
std::vector<int> dijkstraDist(std::vector<Node*> g, int s, std::vector<int>& path);

int width = 0, height = 0;

int main() {
    //Define variables
    std::ifstream stream;
    std::vector<std::vector<int>> riskMap;
    std::vector<Node*> graph;
    std::vector<int> distances;
    int minRisk = 0;

    //Open file
    stream.open("input.txt");
    if (!stream) {
        logLine("Error opening file"); //If input could not be read, print information to user
        std::terminate();
    }

    //Parse input
    std::string line;
    while (std::getline(stream, line, '\n')) {
        width = line.size();
        std::vector<int> v;
        for (int i = 0; i < width; i++) {
            int n = line.at(i) - '0';
            v.push_back(n);
        }
        riskMap.push_back(v);
        height++;
    }

    //Close file
    stream.close();

    //Now, construct the graph
    //First, initialize graph
    for (int i = 0; i < (width*height); i++) {
        Node* n = new Node(i);
        graph.push_back(n);
    }
    //Now, add the children
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int nodeNum = nodeNumber(x, y);
            if (x > 0) graph.at(nodeNum)->addChild(nodeNumber(x-1, y), riskMap.at(y).at(x-1));
            if (y > 0) graph.at(nodeNum)->addChild(nodeNumber(x, y-1), riskMap.at(y-1).at(x));
            if (x < width-1) graph.at(nodeNum)->addChild(nodeNumber(x+1, y), riskMap.at(y).at(x+1));
            if (y < height-1) graph.at(nodeNum)->addChild(nodeNumber(x, y+1), riskMap.at(y+1).at(x));
        }
    }

    //Now, run the dijkstra algorithm on the graph. This will return a vector of distances from the starting node
    std::vector<int> path(graph.size());
    distances = dijkstraDist(graph, 0, path);

    //Min risk is the shortest path to final node (at position width-1 and height-1)
    minRisk = distances.at(nodeNumber(width-1, height-1));
    logLine(minRisk);

    //Part 2: Create a graph that is 25 times greater
    //First, initialize graph
    graph.clear();
    for (int i = 0; i < (25*width*height); i++) {
        Node* n = new Node(i);
        graph.push_back(n);
    }

    //Now, create the fullRiskMap
    //First, initialize it
    width = 5*width, height = 5*height;
    std::vector<std::vector<int>> fullRiskMap(height);
    for (int i = 0; i < height; i++) {
        std::vector<int> v(width);
        fullRiskMap.at(i) = v;
    }

    //Now, define it -- first, the hoirzontal row
    for (int y = 0; y < height/5; y++) {
        for (int x = 0; x < width/5; x++) {
            int val = riskMap.at(y).at(x);
            for (int i = 0; i < 5; i++) {
                if (val == 10) val = 1;
                fullRiskMap.at(y).at(x + (i * width / 5)) = val;
                val++;
            }
        }
    }

    //Next, vertically
    for (int y = 0; y < height/5; y++) {
        for (int x = 0; x < height; x++) {
            int val = fullRiskMap.at(y).at(x);
            for (int i = 1; i < 5; i++) {
                val++;
                if (val == 10) val = 1;
                fullRiskMap.at(y + (i * height / 5)).at(x) = val;
            }
        }
    }

    //Now, add the children
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int nodeNum = nodeNumber(x, y);
            if (x > 0) graph.at(nodeNum)->addChild(nodeNumber(x-1, y), fullRiskMap.at(y).at(x-1));
            if (y > 0) graph.at(nodeNum)->addChild(nodeNumber(x, y-1), fullRiskMap.at(y-1).at(x));
            if (x < width-1) graph.at(nodeNum)->addChild(nodeNumber(x+1, y), fullRiskMap.at(y).at(x+1));
            if (y < height-1) graph.at(nodeNum)->addChild(nodeNumber(x, y+1), fullRiskMap.at(y+1).at(x));
        }
    }

    //Now, run the dijkstra algorithm on the graph. This will return a vector of distances from the starting node
    path.clear();
    path.resize(graph.size());
    distances.clear();
    distances = dijkstraDist(graph, 0, path);

    //Min risk is the shortest path to final node (at position width-1 and height-1)
    minRisk = distances.at(nodeNumber(width-1, height-1));
    logLine(minRisk); //2888 [too high]

    return 0;
}

int nodeNumber(int x, int y) {
    return y * height + x;
}

std::vector<int> dijkstraDist(std::vector<Node*> g, int s, std::vector<int>& path) {
    //Create list of distances and visited status for each node
    std::vector<int> dist(g.size());
    std::vector<bool> visited(g.size());

    //Initialize lists
    for (int i = 0; i < g.size(); i++) {
        visited[i] = false;
        path[i] = -1;
        dist[i] = INT_MAX;
    }

    //Initalize the starting index values
    dist[s] = 0;
    path[s] = -1;
    int current = s;

    // Set of vertices that has a parent (one or more) marked as visited
    std::unordered_set<int> sett;

    //Algorithm
    while (true) {
        //Mark the current as visited
        visited[current] = true;

        //Run through the children of the current node
        for (int i = 0; i < g[current]->children.size(); i++) {
            int v = g[current]->children[i].first;

            if (visited[v]) continue;

            //Insert any previosuly unvisited nodes into visited vertex
            sett.insert(v);
            int alt = dist[current] + g[current]->children[i].second;

            // Condition to check the distance is correct and update it if it is minimum from the previous computed distance
            if (alt < dist[v]) {
                dist[v] = alt;
                path[v] = current;
            }
        }
        //Remove current node from set. If the set is empty, the algorithm has completed.
        sett.erase(current);
        if (sett.empty()) break;

        //Find new current node
        int minDist = INT_MAX;
        int index = 0;

        // Loop to update the distance of the vertices of the graph
        for (int i : sett) {
            if (dist[i] < minDist) {
                minDist = dist[i];
                index = i;
            }
        }
        current = index;
    }
    return dist;
}