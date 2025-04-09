#include "RoutePlanner.h"
#include <iostream>
#include <fstream>

using namespace std;

// Array to help you out with name of the cities in order
const std::string cities[81] = { 
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik", 
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne", 
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta", 
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya", 
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya", 
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat", 
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir", 
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce" 
};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {

    loadPriorityProvinces(priority_data);
    loadWeatherRestrictedProvinces(restricted_data);
    map.loadDistanceData(distance_data);
}

void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    int currentIndex = 0;
    std::ifstream inputFile(filename);
    std::string line;

    while (std::getline(inputFile, line) && currentIndex < MAX_PRIORITY_PROVINCES) {
        size_t start = line.find('(');
        size_t end = line.find(')');

        if (start != std::string::npos && end != std::string::npos) {
            std::string numberStr = line.substr(start + 1, end - start - 1);
            priorityProvinces[currentIndex++] = std::stoi(numberStr);
        }
    }
    numPriorityProvinces = currentIndex; // Update the count of loaded priority provinces
    inputFile.close();
}

// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    int currentIndex = 0;
    ifstream inputFile(filename);
    string line;

    while (std::getline(inputFile, line) && currentIndex < MAX_WEATHER_RESTRICTED_PROVINCES) {
        // Find the position of '(' and ')'
        size_t start = line.find('(');
        size_t end = line.find(')');

        if (start != string::npos && end != string::npos) {
            // Extract the number as a substring and convert it to an integer
            std::string numberStr = line.substr(start + 1, end - start - 1);
            weatherRestrictedProvinces[currentIndex++] = std::stoi(numberStr); // Store in the array
        }
    }

    inputFile.close();
}

// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    for (int i = 0; i < MAX_PRIORITY_PROVINCES; ++i) {
        if (priorityProvinces[i] == province) {
            return true; // Index found
        }
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    for (int i = 0; i < MAX_WEATHER_RESTRICTED_PROVINCES; ++i) {
        if (weatherRestrictedProvinces[i] == province) {
            return true; // Index found
        }
    }
    return false;
}

void RoutePlanner::exploreRoute(int startingCity) {
    // Mark the starting city as visited
    map.markAsVisited(startingCity);

    // Push it onto the stack and add to the route
    stack.push(startingCity);
    route.push_back(startingCity);

    // Explore the neighbors
    exploreFromProvince(startingCity);

    // Calculate the total distance using the route
    totalDistanceCovered = calculateTotalDistance();

    // Display results when exploration is complete
    if (isExplorationComplete()) {
        displayResults();
    }
}

// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
    while (!isExplorationComplete()) {
        queue.clear();  // Clear the queue before enqueueing neighbors
        enqueueNeighbors(province);

        if (!queue.isEmpty()) {
            int nextProvince = queue.dequeue();

            if (!map.isVisited(nextProvince)) {
                // Mark as visited, add to the route, and push onto the stack
                map.markAsVisited(nextProvince);
                stack.push(nextProvince);
                route.push_back(nextProvince);

                // Update total distance and explore neighbors from the next province
                totalDistanceCovered += map.getDistance(province, nextProvince);
                province = nextProvince;
            }
        } else {
            // Backtrack if no valid neighbors are found
            backtrack();
            if (!stack.isEmpty()) {
                province = stack.peek();
            } else {
                break;  // No more moves; terminate exploration
            }
        }
    }
}

void RoutePlanner::enqueueNeighbors(int province) {
    for (int i = 0; i < MAX_SIZE; ++i) {
        int distance = map.distanceMatrix[province][i];
        
        // Check constraints: within range, not visited, and not weather-restricted
        if (distance > 0 && distance <= maxDistance && !map.isVisited(i)) {
            if (isWeatherRestricted(i)) {
                std::cout << "Province " << cities[i] << " is weather-restricted. Skipping.\n";
                continue;
            }

            // Enqueue priority neighbors first
            if (isPriorityProvince(i)) {
                queue.enqueuePriority(i);
            } else {
                queue.enqueue(i);
            }
        }
    }
}

void RoutePlanner::backtrack() {
    if (stack.isEmpty()) {
        return;
    }

    // Pop the last visited province from the stack
    int lastProvince = stack.pop();

    // If the stack is not empty, update the current province
    if (!stack.isEmpty()) {
        int previousProvince = stack.peek();
        totalDistanceCovered -= map.getDistance(previousProvince, lastProvince);
    }
}

bool RoutePlanner::isExplorationComplete() const {
    return stack.isEmpty() && queue.isEmpty();
}

void RoutePlanner::displayResults() const {
    printf("----------------------------\n");
    printf("Journey Completed!\n");
    printf("----------------------------\n");

    int provincesVisited = route.size();
    printf("Total Number of Provinces Visited: %d\n", provincesVisited);
    printf("Total Distance Covered: %d km\n", totalDistanceCovered);

    printf("Route Taken:\n");
    for (size_t i = 0; i < route.size(); ++i) {
        printf("%s", cities[route[i]].c_str());
        if (i != route.size() - 1) {
            printf(" -> ");
        }
    }
    printf("\n");

    // Priority Province Summary
    printf("\nPriority Provinces Status:\n");
    int priorityVisitedCount = 0;
    for (int i = 0; i < numPriorityProvinces; ++i) {
        int province = priorityProvinces[i];
        printf("- %s (%s)\n", cities[province].c_str(),
               map.isVisited(province) ? "Visited" : "Not Visited");
        if (map.isVisited(province)) {
            ++priorityVisitedCount;
        }
    }

    printf("\nTotal Priority Provinces Visited: %d out of %d\n",
           priorityVisitedCount, numPriorityProvinces);

    if (priorityVisitedCount == numPriorityProvinces) {
        printf("Success: All priority provinces were visited.\n");
    } else {
        printf("Warning: Not all priority provinces were visited.\n");
    }
}

int RoutePlanner::calculateTotalDistance() const {
    int totalDistance = 0;
    for (size_t i = 0; i < route.size() - 1; ++i) {
        totalDistance += map.getDistance(route[i], route[i + 1]);
    }
    return totalDistance;
}

