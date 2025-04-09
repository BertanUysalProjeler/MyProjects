#include "Map.h"
#include <fstream>
#include <iostream>
  
using namespace std;

Map::Map() {
    // Initialize all distances
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j){
            distanceMatrix[i][j] = 0;
        }
    }
    // Initialize all provinces as unvisited
    for (int i = 0; i < MAX_SIZE; ++i) {
        visited[i] = false;
    }
}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    // Opening the file
    ifstream file(filename);

    vector<string> linesVector;
    string line;

    // Read the next line from File until it reaches the
    // end.
    int x = 0;
    while (file >> line) {
        stringstream ss(line);
        string token;
        int y = 0;
        while(getline(ss, token, ',')) {
            distanceMatrix[x][y] = stoi(token);
            y++;
        }
        x++;
    }
}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    int distance = distanceMatrix[provinceA][provinceB];

    if (distance == maxDistance || distance < maxDistance)
    {
        return true;
    }
    
    return false; 
}

// Marks a province as visited
void Map::markAsVisited(int province) {
    visited[province] = true;
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    return visited[province];
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    for (int i = 0; i < MAX_SIZE; ++i) {
        visited[i] = false;
    }
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    int counter = 0;
    for (int i = 0; i < MAX_SIZE; ++i) {
       if(visited[i]){
        counter++;
       }
    }
    return counter;
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    return distanceMatrix[provinceA][provinceB];
}