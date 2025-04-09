#include "RealmShaper.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define NECESSARY_ENERGY 2000 // Necessary energy to craft an Isle

RealmShaper::RealmShaper(std::string name, int honour) 
    : name(name), honour(honour), collectedEnergyPoints(0), depth(0) {}

const std::string &RealmShaper::getName() const
{
    return this->name;
}

int RealmShaper::getEnergyLevel()
{
    return this->collectedEnergyPoints;
}

int RealmShaper::getHonour()
{
    return this->honour;
}

void RealmShaper::gainHonour()
{
    this->honour += 150;
}

void RealmShaper::loseHonour()
{
    this->honour -= 150;
}

void RealmShaper::collectItem(Item item)
{
    this->collectedEnergyPoints += item;
}

void RealmShaper::loseEnergy()
{
    this->collectedEnergyPoints -= NECESSARY_ENERGY;
}

bool RealmShaper::hasEnoughEnergy()
{
    return this->collectedEnergyPoints >= NECESSARY_ENERGY;
}

void RealmShaper::setDepth(int newDepth) {
    depth = newDepth;
}

std::vector<RealmShaper *> RealmShaper::readFromFile(const std::string &filename)
{
    std::vector<RealmShaper *> players;
    std::ifstream inputFile(filename);
    std::string line;

    while (std::getline(inputFile, line)) {
        std::istringstream ss(line);
        std::string playerName;
        int honourPoints;
        ss >> playerName >> honourPoints;
        players.push_back(new RealmShaper(playerName, honourPoints));
    }

    inputFile.close();
    return players;
}

bool RealmShaper::operator>(const RealmShaper &other) const
{
    return this->honour > other.honour;
}

std::ostream &operator<<(std::ostream &os, const RealmShaper &p)
{
    return os << p.name << std::endl;
}