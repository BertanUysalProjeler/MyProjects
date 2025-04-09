#include "RealmShapers.h"
#include <cmath>
#include <algorithm>

using namespace std;

ShaperTree::ShaperTree()
{
}

ShaperTree::~ShaperTree()
{
    // Free any dynamically allocated memory
    for (RealmShaper* shaper : realmShapers) {
        delete shaper;
    }
    realmShapers.clear();
}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    // TODO: Insert innitial shapers to the tree
    for (RealmShaper * shaper : shapers)
    {
        insert(shaper);
    }
}

int ShaperTree::getSize()
{
    // TODO: Return number of shapers in the tree
    return sizeof(realmShapers);
}

std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{
    bool isValid = false;
    // TODO: Check if the index is valin in the tree
    isValid = index >= 0 && index < static_cast<int>(realmShapers.size());
    return isValid;
}

void ShaperTree::insert(RealmShaper *shaper)
{
    // TODO: Insert shaper to the tree
    shaper->setDepth(static_cast<int>(std::floor(std::log2(realmShapers.size() + 1)))); // Calculate depth based on position in a complete binary tree
    realmShapers.push_back(shaper); // Add the shaper to the end of the vector
}

int ShaperTree::remove(RealmShaper *shaper)
{
    // TODO: Remove the player from tree if it exists
    // Make sure tree protects its form (complate binary tree) after deletion of a node
    // return index if found and removed
    // else
    // Find the index of the shaper in the tree
    int indexToRemove = findIndex(shaper);

    if (indexToRemove == -1)
        return -1; // Shaper not found

    // Replace the node to be removed with the last node in the tree
    realmShapers[indexToRemove] = realmShapers.back();
    realmShapers[indexToRemove]->setDepth(static_cast<int>(std::floor(std::log2(indexToRemove + 1))));

    // Remove the last node
    realmShapers.pop_back();

    // Maintain the complete binary tree property by reordering the subtree
    reorderTreeAfterRemoval(indexToRemove);

    return indexToRemove; // Return the index of the removed node
}

void ShaperTree::reorderTreeAfterRemoval(int index)
{
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;
    int largest = index;

    // Find the largest node among parent, left child, and right child
    if (isValidIndex(leftChild) && *realmShapers[leftChild] > *realmShapers[largest])
        largest = leftChild;
    if (isValidIndex(rightChild) && *realmShapers[rightChild] > *realmShapers[largest])
        largest = rightChild;

    // If the largest node is not the parent, swap and continue heapifying
    if (largest != index)
    {
        std::swap(realmShapers[index], realmShapers[largest]);
        reorderTreeAfterRemoval(largest);
    }
}

int ShaperTree::findIndex(RealmShaper *shaper)
{
    // return index in the tree if found
    // else
    int indexCounter = 0;
    for (RealmShaper * shaper : realmShapers)
    {
        if (realmShapers[indexCounter] == shaper)
        {
            return indexCounter;
        }
        
    }
    return -1;
}

int ShaperTree::getDepth(RealmShaper *shaper)
{
    // return depth of the node in the tree if found
    // else
    int indexOfShaper = findIndex(shaper);
    return floor(log2(indexOfShaper));
}

int ShaperTree::getDepth()
{
    // return total|max depth|height of the tree
    return floor(log2(sizeof(realmShapers)));
}

RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{
    // TODO: Implement duel logic, return the victor
    string victorName;
    string opponentName;
    RealmShaper *winner = nullptr;   // Declare winner in a scope visible to all
    RealmShaper *opponent = nullptr; // Declare opponent in a scope visible to all
    bool isWinnerChallenger = false;
    if (result)
    {
        winner = challenger;
        opponent = getParent(challenger);
        isWinnerChallenger = true;
    }
    else // Simplify the condition since it's the opposite of 'if (result)'
    {
        winner = getParent(challenger);
        opponent = challenger;
    }

    victorName = winner->getName(); // Now 'winner' is accessible here
    opponentName = opponent->getName();
    winner->gainHonour();
    opponent->loseHonour();
    std::cout << "[Duel] " << victorName << " won the duel" << std::endl;
    std::cout << "[Honour] " << "New honour points: ";
    std::cout << victorName << "-" << winner->getHonour() << " ";
    std::cout << opponentName << "-" << opponent->getHonour() << std::endl;

    if (opponent->getHonour() < 0)
    {
        std::cout << "[Duel] " << opponent << " lost all honour, delete" << std::endl;
        remove(opponent);
    }
    
    if (isWinnerChallenger)
    {
        replace(challenger, opponent);
    }
    
    return *challenger;
}

RealmShaper* ShaperTree::getParent(RealmShaper* shaper)
{
    int index = findIndex(shaper);
    if (index == -1 || index == 0) {
        // Return nullptr if the shaper is not found or if it is the root
        return nullptr;
    }
    int parentIndex = (index - 1) / 2; // Calculate parent index in a binary tree
    return realmShapers[parentIndex];
}

void ShaperTree::replace(RealmShaper* player_low, RealmShaper* player_high)
{
    int indexLow = findIndex(player_low);
    int indexHigh = findIndex(player_high);

    if (indexLow == -1 || indexHigh == -1) {
        std::cerr << "[Error] Players not found in the tree, cannot replace." << std::endl;
        return;
    }

    // Swap the players in the tree
    std::swap(realmShapers[indexLow], realmShapers[indexHigh]);

    // Update their depths
    player_low->setDepth(static_cast<int>(std::log2(indexHigh + 1)));
    player_high->setDepth(static_cast<int>(std::log2(indexLow + 1)));
}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by object
    // Return the shaper if found
    // Return nullptr if shaper not found

    for (RealmShaper *oneShaper : realmShapers)
    {
        oneShaper->getName() == shaper.getName();
        foundShaper = oneShaper;
        break;
    }

    return foundShaper;
}

// Find shaper by name
RealmShaper *ShaperTree::findPlayer(std::string name)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by name
    // Return the shaper if found
    // Return nullptr if shaper not found

    for (RealmShaper *oneShaper : realmShapers)
    {
        oneShaper->getName() == name;
        foundShaper = oneShaper;
        break;
    }

    return foundShaper;
}

std::vector<std::string> ShaperTree::inOrderTraversal(int index)
{
    std::vector<std::string> result;

    if (!isValidIndex(index)) {
        return result; // Base case: return an empty vector if index is invalid
    }

    // Get indices for left and right children
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    // Recur on the left subtree
    std::vector<std::string> leftSubtree = inOrderTraversal(left);

    // Add the current node's name
    leftSubtree.push_back(realmShapers[index]->getName());

    // Recur on the right subtree
    std::vector<std::string> rightSubtree = inOrderTraversal(right);

    // Combine results: left subtree -> current node -> right subtree
    leftSubtree.insert(leftSubtree.end(), rightSubtree.begin(), rightSubtree.end());

    return leftSubtree;
}

std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> result;

    if (!isValidIndex(index)) {
        return result; // Base case: return an empty vector if index is invalid
    }

    // Add the current node's name
    result.push_back(realmShapers[index]->getName());

    // Recur on the left subtree
    int left = 2 * index + 1;
    std::vector<std::string> leftSubtree = preOrderTraversal(left);

    // Recur on the right subtree
    int right = 2 * index + 2;
    std::vector<std::string> rightSubtree = preOrderTraversal(right);

    // Combine results: current node -> left subtree -> right subtree
    result.insert(result.end(), leftSubtree.begin(), leftSubtree.end());
    result.insert(result.end(), rightSubtree.begin(), rightSubtree.end());

    return result;
}

std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> result;

    if (!isValidIndex(index)) {
        return result; // Base case: return an empty vector if index is invalid
    }

    // Recur on the left subtree
    int left = 2 * index + 1;
    std::vector<std::string> leftSubtree = postOrderTraversal(left);

    // Recur on the right subtree
    int right = 2 * index + 2;
    std::vector<std::string> rightSubtree = postOrderTraversal(right);

    // Add the current node's name
    leftSubtree.insert(leftSubtree.end(), rightSubtree.begin(), rightSubtree.end());
    leftSubtree.push_back(realmShapers[index]->getName());

    return leftSubtree;
}

void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    if (!isValidIndex(index)) {
        return; // Base case: return if index is invalid
    }

    // Write the current node's name
    outFile << realmShapers[index]->getName() << "\n";

    // Recur on the left subtree
    int left = 2 * index + 1;
    preOrderTraversal(left, outFile);

    // Recur on the right subtree
    int right = 2 * index + 2;
    preOrderTraversal(right, outFile);
}

void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{
    if (realmShapers.empty()) {
        return; // If the tree is empty, do nothing
    }

    std::queue<int> nodeQueue; // Queue for level-order traversal
    nodeQueue.push(0);         // Start with the root node (index 0)

    while (!nodeQueue.empty()) {
        int currentIndex = nodeQueue.front();
        nodeQueue.pop();

        // Write the current node's name
        outFile << realmShapers[currentIndex]->getName() << "\n";

        // Enqueue the left and right children if they exist
        int left = 2 * currentIndex + 1;
        int right = 2 * currentIndex + 2;

        if (isValidIndex(left)) {
            nodeQueue.push(left);
        }
        if (isValidIndex(right)) {
            nodeQueue.push(right);
        }
    }
}

void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

// Helper function to print tree with indentation
void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index))
        return;

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;
    int left = 2 * index + 1;  // TODO: Calculate left index
    int right = 2 * index + 2; // TODO: Calculate right index

    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : "")); // ╎
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename)
{
    // TODO: Write the shapers to filename output level by level
    // Use std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;
    // Create an ofstream object
    std::ofstream outFile;

    // Open the file
    outFile.open("example.txt");

    string output;

    for (RealmShaper* shaper:realmShapers)
    {
        outFile << shaper->getName() <<"\n"<< std::endl;
        output += shaper->getName();
    }

    std::cout << output << "Shapers have been written to " << filename << " according to rankings." << std::endl;
}

void ShaperTree::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output pre-order
    // Use std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;

    std::ofstream outFile; // Correct declaration of the output file stream

    outFile.open(filename); // Open the file with the given filename

    if (!outFile.is_open()) {
        std::cerr << "[Error] Failed to open file: " << filename << std::endl;
        return;
    }

    preOrderTraversal(0, outFile); // Corrected function call with a semicolon at the end

    outFile.close(); // Close the file after writing

    std::cout << "[Output] Tree has been written to " << filename << " in pre-order." << std::endl;
}
