#include "Map.h"

using namespace std;

Map::Map()
{
    this->root = nullptr;
}
Map::~Map()
{
    // Recursively delete all nodes in the AVL tree
    while (root != nullptr)
        root = remove(root, root->isle);
}

void Map::initializeMap(std::vector<Isle *> isles)
{
    // TODO: Insert innitial isles to the tree
    // Then populate with Goldium and Einstainium items

    for (auto *isle : isles)
    {
        insert(isle);
    }
    populateWithItems();
}

MapNode *Map::rotateRight(MapNode *current)
{
    // TODO: Perform right rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;

    if (!current || !current->left)
    {
        std::cerr << "[Right Rotation] Called on invalid node!" << std::endl;
        return current;
    }

    MapNode *newRoot = current->left;
    current->left = newRoot->right;
    newRoot->right = current;

    // Update heights
    current->height = 1 + std::max(height(current->left), height(current->right));
    newRoot->height = 1 + std::max(height(newRoot->left), height(newRoot->right));

    return newRoot;
}

MapNode *Map::rotateLeft(MapNode *current)
{
    // TODO: Perform left rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
    
    if (!current || !current->right)
    {
        std::cerr << "[Left Rotation] Called on invalid node!" << std::endl;
        return current;
    }

    MapNode *newRoot = current->right;
    current->right = newRoot->left;
    newRoot->left = current;

    // Update heights
    current->height = 1 + std::max(height(current->left), height(current->right));
    newRoot->height = 1 + std::max(height(newRoot->left), height(newRoot->right));

    return newRoot;
}

int Map::calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
    if (totalShaperTreeHeight == 0) return 0; // Avoid division by zero
    return static_cast<int>(totalMapDepth * (static_cast<double>(playerDepth) / totalShaperTreeHeight));
}

int Map::height(MapNode *node)
{
    return (node == nullptr) ? 0 : node->height;
}

MapNode *Map::insert(MapNode *node, Isle *isle)
{
    // Base case: If the current node is null, create a new node
    if (node == nullptr)
        return new MapNode(isle);

    // Recursively insert into the left or right subtree
    if (isle->getName() < node->isle->getName())
        node->left = insert(node->left, isle);
    else if (isle->getName() > node->isle->getName())
        node->right = insert(node->right, isle);
    else
        return node; // Duplicate Isles are not allowed, so return the existing node

    // Update the height of the current node
    node->height = 1 + std::max(height(node->left), height(node->right));

    // Balance the node if it becomes unbalanced
    int balance = balanceFactor(node);

    // Left Left Case
    if (balance > 1 && isle->getName() < node->left->isle->getName())
        return rotateRight(node);

    // Right Right Case
    if (balance < -1 && isle->getName() > node->right->isle->getName())
        return rotateLeft(node);

    // Left Right Case
    if (balance > 1 && isle->getName() > node->left->isle->getName())
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Left Case
    if (balance < -1 && isle->getName() < node->right->isle->getName())
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    // Return the (potentially updated) node pointer
    return node;
}

void Map::insert(Isle *isle)
{
    root = insert(root, isle);
    root = balance(root); // Ensure the tree is balanced
}

MapNode *Map::remove(MapNode *node, Isle *isle)
{
    // TODO: Recursively delete isle from the tree
    // Will be called if there is overcrowding
    // returns node
    // Use std::cout << "[Remove] " << "Tree is Empty" << std::endl;

    if (!node)
    {
        std::cout << "[Remove] Tree is Empty" << std::endl;
        return nullptr;
    }

    // Perform standard BST delete
    if (isle->getName() < node->isle->getName())
    {
        node->left = remove(node->left, isle);
    }
    else if (isle->getName() > node->isle->getName())
    {
        node->right = remove(node->right, isle);
    }
    else
    {
        // Node with one or no child
        if (!node->left || !node->right)
        {
            MapNode *temp = node->left ? node->left : node->right;
            delete node;
            return temp;
        }

        // Node with two children: Get the inorder successor
        MapNode *temp = node->right;
        while (temp->left)
        {
            temp = temp->left;
        }
        node->isle = temp->isle;
        node->right = remove(node->right, temp->isle);
    }

    // Update height and balance
    node->height = 1 + std::max(height(node->left), height(node->right));
    int balance = balanceFactor(node);

    // Balance the node
    if (balance > 1 && balanceFactor(node->left) >= 0)
        return rotateRight(node);

    if (balance > 1 && balanceFactor(node->left) < 0)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && balanceFactor(node->right) <= 0)
        return rotateLeft(node);

    if (balance < -1 && balanceFactor(node->right) > 0)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void Map::remove(Isle *isle)
{
    root = remove(root, isle);
    root = balance(root); // Rebalance after deletion
}

// function to get the balance factor of a node
int Map::balanceFactor(MapNode* node)
{
    if (node == nullptr)
        return 0;
    return height(node->left) - height(node->right);
}

void Map::preOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop EINSTEINIUM according to rules
    // Use std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;

    if (!current || count <= 0)
        return;

    if (current->isle->getItem() == EMPTY)
    {
        current->isle->setItem(EINSTEINIUM);
        count--;
        std::cout << "[Item Drop] EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }

    preOrderItemDrop(current->left, count);
    preOrderItemDrop(current->right, count);

}

// to Display the values by Post Order Method .. left - right - node
void Map::postOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop GOLDIUM according to rules
    // Use  std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
    if (!current || count <= 0)
        return;

    postOrderItemDrop(current->left, count);
    postOrderItemDrop(current->right, count);

    if (current->isle->getItem() == EMPTY)
    {
        current->isle->setItem(GOLDIUM);
        count--;
        std::cout << "[Item Drop] GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }
}

MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    if (node == nullptr)
        return nullptr;

    // Use the getter to check the item
    if (node->isle->getItem() == EMPTY)
        return node;

    MapNode *leftResult = findFirstEmptyIsle(node->left);
    return leftResult ? leftResult : findFirstEmptyIsle(node->right);
}


void Map::dropItemBFS()
{
    // TODO: Drop AMAZONITE according to rules
    // Use std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
    // Use std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;

    if (!root)
    {
        std::cout << "[BFS Drop] No eligible Isle found for AMAZONITE drop." << std::endl;
        return;
    }

    std::queue<MapNode *> q;
    q.push(root);

    while (!q.empty())
    {
        MapNode *current = q.front();
        q.pop();

        if (current->isle->getItem() == EMPTY)
        {
            current->isle->setItem(AMAZONITE);
            std::cout << "[BFS Drop] AMAZONITE dropped on Isle: " << current->isle->getName() << std::endl;
            return;
        }

        if (current->left)
            q.push(current->left);
        if (current->right)
            q.push(current->right);
    }

    std::cout << "[BFS Drop] No eligible Isle found for AMAZONITE drop." << std::endl;

}

void Map::displayMap()
{
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}

int Map::getDepth(MapNode *node)
{
    if (node == nullptr)
        return 0;
    return 1 + std::max(getDepth(node->left), getDepth(node->right));
}

int Map::getIsleDepth(Isle *isle)
{
    MapNode *current = root;
    int depth = 0;
    while (current != nullptr)
    {
        depth++;
        if (*(current->isle) == *isle)
            return depth;
        current = (*isle < *(current->isle)) ? current->left : current->right;
    }
    return -1; // Isle not found
}

int Map::getDepth()
{
    return getDepth(root);
}

void Map::populateWithItems()
{
    auto distributeItem = [this](Item item) {
        std::queue<MapNode *> queue;
        if (root)
            queue.push(root);
        while (!queue.empty())
        {
            MapNode *current = queue.front();
            queue.pop();

            // Use the getter and setter for item
            if (current->isle->getItem() == EMPTY)
            {
                current->isle->setItem(item);
                return;
            }

            if (current->left)
                queue.push(current->left);
            if (current->right)
                queue.push(current->right);
        }
    };
    distributeItem(GOLDIUM);
    distributeItem(EINSTEINIUM);
}


Isle *Map::findIsle(Isle isle)
{
    MapNode *node = findNode(isle);
    return node ? node->isle : nullptr;
}

Isle *Map::findIsle(std::string name)
{
    MapNode *node = findNode(name);
    return node ? node->isle : nullptr;
}

MapNode *Map::findNode(Isle isle)
{
    MapNode *current = root;
    while (current != nullptr)
    {
        if (*(current->isle) == isle)
            return current;
        current = (isle < *(current->isle)) ? current->left : current->right;
    }
    return nullptr;
}

MapNode *Map::findNode(std::string name)
{
    MapNode *current = root;
    while (current != nullptr)
    {
        if (current->isle->getName() == name) // Use the getter for the name
            return current;
        current = (name < current->isle->getName()) ? current->left : current->right; // Use the getter for comparison
    }
    return nullptr;
}


void Map::display(MapNode *current, int depth, int state)
{
    // SOURCE:

    if (current->left)
        display(current->left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("   ┌───");
    else if (state == 2) // right
        printf("   └───");

    std::cout << "[" << *current->isle << "] - (" << current->height << ")\n"
              << std::endl;

    if (current->right)
        display(current->right, depth + 1, 2);
}

void Map::writeToFile(const std::string &filename)
{
    std::ofstream outFile(filename);
    if (!outFile.is_open())
        throw std::runtime_error("Cannot open file " + filename);

    std::queue<MapNode *> queue;
    if (root)
        queue.push(root);

    while (!queue.empty())
    {
        MapNode *current = queue.front();
        queue.pop();

        // Use getters to access the Isle's name and item
        outFile << current->isle->getName() << " - " << current->isle->getItem() << std::endl;

        if (current->left)
            queue.push(current->left);
        if (current->right)
            queue.push(current->right);
    }

    outFile.close();
}


void Map::writeIslesToFile(const std::string &filename)
{
    // Open the output file
    std::ofstream outFile(filename);
    if (!outFile.is_open())
        throw std::runtime_error("Cannot open file " + filename);

    // Collect all Isles from the map
    std::vector<Isle *> isles;
    collectIsles(root, isles);

    // Sort the Isles alphabetically by their names
    std::sort(isles.begin(), isles.end(), [](Isle *a, Isle *b) {
        return a->getName() < b->getName();
    });

    // Write the sorted Isles to the file
    for (const auto &isle : isles)
    {
        outFile << isle->getName() << " - ";

        // Get item name using enum-to-string conversion
        switch (isle->getItem())
        {
        case GOLDIUM:
            outFile << "GOLDIUM";
            break;
        case EINSTEINIUM:
            outFile << "EINSTEINIUM";
            break;
        case AMAZONITE:
            outFile << "AMAZONITE";
            break;
        default:
            outFile << "EMPTY";
            break;
        }

        outFile << std::endl;
    }

    outFile.close();

    // Log output message
    std::cout << "[Output] Isles have been written to " << filename << " in alphabetical order." << std::endl;
}


MapNode *Map::balance(MapNode *node)
{
    if (node == nullptr)
        return nullptr;

    // Update the height of the current node
    node->height = 1 + std::max(height(node->left), height(node->right));

    // Get the balance factor
    int bf = balanceFactor(node);

    // Left-Left Case
    if (bf > 1 && balanceFactor(node->left) >= 0)
        return rotateRight(node);

    // Left-Right Case
    if (bf > 1 && balanceFactor(node->left) < 0)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right-Right Case
    if (bf < -1 && balanceFactor(node->right) <= 0)
        return rotateLeft(node);

    // Right-Left Case
    if (bf < -1 && balanceFactor(node->right) > 0)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    // Return the node if no rebalancing is needed
    return node;
}

void Map::collectIsles(MapNode *node, std::vector<Isle *> &isles)
{
    if (node == nullptr)
        return;

    // Add the current node's isle to the vector
    isles.push_back(node->isle);

    // Recursively traverse the left and right subtrees
    collectIsles(node->left, isles);
    collectIsles(node->right, isles);
}
