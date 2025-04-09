#include "GameWorld.h"
#include <fstream>
#include <sstream>
#include <iostream>

GameWorld::GameWorld() : mapTree(), shaperTree() {}

void GameWorld::initializeGame(std::vector<Isle *> places, std::vector<RealmShaper *> players)
{
    shaperTree.initializeTree(players);
    mapTree.initializeMap(places);
}

bool GameWorld::hasAccess(RealmShaper *realmShaper, Isle *isle)
{
    int playerDepth = shaperTree.getDepth(realmShaper);
    int totalTreeDepth = shaperTree.getDepth();
    int totalMapDepth = mapTree.getDepth();
    int minAccessDepth = mapTree.calculateMinMapDepthAccess(playerDepth, totalTreeDepth, totalMapDepth);
    int isleDepth = mapTree.getIsleDepth(isle);

    if (isleDepth >= minAccessDepth)
        return true;

    std::cout << "[Access Control] " << realmShaper->getName() << " does not have access to explore " << isle->getName() << std::endl;
    return false;
}

void GameWorld::exploreArea(RealmShaper *realmShaper, Isle *isle)
{
    if (!hasAccess(realmShaper, isle))
        return;

    std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;

    realmShaper->collectItem(isle->getItem());
    isle->setItem(EMPTY);

    std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;

    if (isle->increaseShaperCount())
    {
        std::cout << "[Overcrowding] " << isle->getName() << " self-destructed, it will be removed from the map" << std::endl;
        mapTree.remove(isle);
    }
}

void GameWorld::craft(RealmShaper *shaper, const std::string &isleName)
{
    if (!shaper->hasEnoughEnergy())
    {
        std::cout << "[Energy] " << shaper->getName() << " does not have enough energy points: " << shaper->getEnergyLevel() << std::endl;
        return;
    }

    Isle *newIsle = new Isle(isleName);
    mapTree.insert(newIsle);
    shaper->loseEnergy();

    std::cout << "[Craft] " << shaper->getName() << " crafted new Isle " << isleName << std::endl;
    std::cout << "[Energy] " << shaper->getName() << "'s new energy level is " << shaper->getEnergyLevel() << std::endl;
}

void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs)
{
    std::ifstream accessFile(accessLogs);
    std::ifstream duelFile(duelLogs);
    std::string line;

    int accessCount = 0;

    while (std::getline(accessFile, line))
    {
        std::istringstream ss(line);
        std::string playerName, isleName;
        ss >> playerName >> isleName;

        RealmShaper *player = shaperTree.findPlayer(playerName);
        Isle *isle = mapTree.findIsle(isleName);

        if (!player)
        {
            std::cout << "[Access Log] Player " << playerName << " not found!" << std::endl;
            continue;
        }

        if (!isle)
        {
            craft(player, isleName);
        }
        else
        {
            exploreArea(player, isle);
        }

        accessCount++;

        if (accessCount % 5 == 0 && std::getline(duelFile, line))
        {
            std::istringstream ssDuel(line);
            std::string challengerName;
            int result;
            ssDuel >> challengerName >> result;

            RealmShaper *challenger = shaperTree.findPlayer(challengerName);

            if (challenger)
            {
                shaperTree.duel(challenger, result == 1);
            }
        }
    }

    while (std::getline(duelFile, line))
    {
        std::istringstream ss(line);
        std::string challengerName;
        int result;
        ss >> challengerName >> result;

        RealmShaper *challenger = shaperTree.findPlayer(challengerName);

        if (challenger)
        {
            shaperTree.duel(challenger, result == 1);
        }
    }

    displayGameState();
}

void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);
}

void GameWorld::displayGameState()
{
    //mapTree.displayMap();
    //shaperTree.displayTree();
}

Map &GameWorld::getMapTree()
{
    return mapTree;
}

ShaperTree &GameWorld::getShaperTree()
{
    return shaperTree;
}
