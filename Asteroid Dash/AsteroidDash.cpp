#include "AsteroidDash.h"

using namespace std;

// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

        : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {

    read_player(player_file_name, player_name);  // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name);  // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name);  // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name);
}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string& input_file) {

    ifstream file(input_file);
    string line;

    // Read remaining lines
    while (getline(file, line)) {

        vector<int> row;
        for (char ch : line) {
            if (ch == ' ') continue;  // Skip spaces
            int value = ch - '0';     // Convert char to int
            row.push_back(value);
        }
        space_grid.push_back(row);
    }

    starting_grid = space_grid;
    file.close();
}

// Function to read the player from a file
void AsteroidDash::read_player(const string& player_file_name, const string& player_name) {

    ifstream file(player_file_name);
    vector<vector<bool>> spacecraft_shape;
    string line;

    int starting_row, starting_col;
    file >> starting_row >> starting_col;

    // Skip newline after reading starting_row and starting_col
    getline(file, line);

    // Read remaining lines
    while (getline(file, line)) {

        vector<bool> row;
        for (char ch : line) {
            // Skip spaces
            if (ch == ' ') continue;
            // Convert '1' to true and '0' to false
            if (ch == '1') {
                row.push_back(true);
            }
            else if (ch == '0') {
                row.push_back(false);
            }
        }
        spacecraft_shape.push_back(row);
    }

    // Close file after reading all lines
    file.close();

    // Initialize player after reading data
    player = new Player(spacecraft_shape, starting_row, starting_col, player_name, 10, 3);
}

// Helper function to trim whitespace from both ends of a string
string AsteroidDash::trim(const string& str) {
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");
    return (first == string::npos || last == string::npos) ? "" : str.substr(first, (last - first + 1));
}

// Function to parse a matrix from a line in the file
vector<vector<bool>> AsteroidDash::parseMatrix(ifstream& file) {
    vector<vector<bool>> matrix;
    string line;
    streampos last_position; // To store the position of the previous line

    while (getline(file, line)) {
        if (line.empty() || line[0] == 's' || line[0] == 't' || line[0] == 'e') break;

        last_position = file.tellg(); // Store the position at the start of each line

        vector<bool> row;
        for (char ch : line) {
            if (ch == '0' || ch == '1') {
                row.push_back(ch == '1');
            }
        }
        matrix.push_back(row);
    }

    // Move the file pointer to the start of the last parsed line in the matrix
    if (last_position != -1) {
        file.clear(); // Clear any potential EOF flag
        file.seekg(last_position);
    }

    return matrix;
}

// Function to parse an object from the file and add it to the linked list
void AsteroidDash::parseAndAddCelestialObject(ifstream& file) {
    string line;
    vector<vector<bool>> shape;
    int start_row = 0, appearance_time = 0;
    ObjectType type = ASTEROID;

    // Read the shape matrix
    shape = parseMatrix(file);

    // Now proceed to read the properties (s, t, e)
    while (getline(file, line) && !line.empty()) {
        line = trim(line);
        if (line.find("s:") == 0) {
            start_row = stoi(line.substr(2));
        }
        else if (line.find("t:") == 0) {
            appearance_time = stoi(line.substr(2));
        }
        else if (line.find("e:") == 0) {
            string type_str = line.substr(2);
            if (type_str == "life") type = LIFE_UP;
            else if (type_str == "ammo") type = AMMO;
        }
    }

    // Create the CelestialObject and add it to the linked list
    CelestialObject* new_object = new CelestialObject(shape, type, start_row, appearance_time);

    createRightRotationsList(*new_object);
    createLeftRotationsList(*new_object);
    CelestialObject::makeRotationsCircular(new_object);

    if (celestial_objects_list_head == nullptr) {
        celestial_objects_list_head = new_object;
    }
    else {
        CelestialObject* temp = celestial_objects_list_head;
        while (temp->next_celestial_object != nullptr) {
            temp = temp->next_celestial_object;
        }
        temp->next_celestial_object = new_object;
    }
}

// Function to read objects from file and populate the linked list
void AsteroidDash::read_celestial_objects(const string& input_file) {
    ifstream file(input_file);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << input_file << endl;
        return;
    }

    while (!file.eof()) {
        parseAndAddCelestialObject(file);
    }

    file.close();
}

// Print the entire space grid
void AsteroidDash::print_space_grid() const {
    for (int i = 0; i < space_grid.size(); i++) {
        for (int l = 0; l < space_grid[i].size(); l++) {
            if (space_grid[i][l] > 0) {
                cout << occupiedCellChar << endl;
            }
            else {
                cout << unoccupiedCellChar << endl;
            }
        }
    }
}

// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid() {
    space_grid = starting_grid; // Reset grid before placing objects

    vector<CelestialObject> objectsOnTheGrid;

    // Place celestial objects that are active based on `game_time`
    CelestialObject* current = celestial_objects_list_head;
    while (current != nullptr) {
        if (current->time_of_appearance <= game_time) {
            objectsOnTheGrid.push_back(current);
            placeCelestialObject(*current);
        }
        current = current->next_celestial_object;
    }

    // Place the player on the grid
    placePlayer();

    // Place active projectiles
    Projectile* projectile = projectileObjectsListHead;
    while (projectile != nullptr) {
        if (projectile->appearanceTime <= game_time) {
            placeProjectile(*projectile, objectsOnTheGrid);
        }
        projectile = projectile->nextProjectile;
    }

    // Increment game tick
    game_time++;
}

// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
void AsteroidDash::shoot() {
    int starting_row = player->spacecraft_shape.size() / 2;
    int starting_column = (player->spacecraft_shape[0].size()) + 1;
    int appearanceTime = game_time;
    
    Projectile* newProjectile = new Projectile(starting_row, starting_column, appearanceTime);

    if (projectileObjectsListHead == nullptr) {
        projectileObjectsListHead = newProjectile;
    }
    else {
        Projectile* currentProjectile = projectileObjectsListHead;
        while (currentProjectile->nextProjectile != nullptr) {
            currentProjectile = currentProjectile->nextProjectile;
        }
        currentProjectile->nextProjectile = newProjectile;
    }
}

// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash() {

    // Delete the player instance if it was allocated
    if (player != nullptr) {
        delete player;
        player = nullptr;
    }

    // Delete the linked list of celestial objects
    while (celestial_objects_list_head != nullptr) {
        CelestialObject* temp = celestial_objects_list_head;
        celestial_objects_list_head = celestial_objects_list_head->next_celestial_object;
        delete temp;
    }

    // Delete the linked list of projectile objects
    while (projectileObjectsListHead != nullptr) {
        Projectile* temp = projectileObjectsListHead;
        projectileObjectsListHead = projectileObjectsListHead->nextProjectile;
        delete temp;
    }

}

void AsteroidDash::placeCelestialObject(CelestialObject& object) {
    vector<vector<bool>> shapeOfObject = object.shape;

    int shapeRows = shapeOfObject.size();
    int shapeCols = shapeOfObject[0].size();
    int x = object.position_row;
    int y = object.position_column;

    // Place shape in matrix starting at (x, y)
    for (int i = 0; i < shapeRows; ++i) {
        for (int j = 0; j < shapeCols; ++j) {
            // Ensure we stay within matrix bounds
            if (x + i < space_grid.size() && y + j < space_grid[0].size()) {
                if (shapeOfObject[i][j]) {
                    space_grid[x + i][y + j] += 1;
                    if (space_grid[x + i][y + j] > 1) {
                        CollisionOfObject(x + i, y + j, object, projectileObjectsListHead, *player);
                    }
                }
            }
        }
    }
}

void AsteroidDash::placeProjectile(Projectile& projectile, vector<CelestialObject>& objectsOnTheGrid) {
    vector<vector<bool>> shape = projectile.shape;

    int shapeRows = shape.size();
    int shapeCols = shape[0].size();
    int x = projectile.position_row;
    int y = projectile.position_column;

    // Place shape in matrix starting at (x, y)
    for (int i = 0; i < shapeRows; ++i) {
        for (int j = 0; j < shapeCols; ++j) {
            // Ensure we stay within matrix bounds
            if (x + i < space_grid.size() && y + j < space_grid[0].size()) {
                if (shape[i][j]) {
                    space_grid[x + i][y + j] += 1;
                    if (space_grid[x + i][y + j] > 1) {
                        CollisionOfProjectile(x + i, y + j, objectsOnTheGrid, &projectile);
                    }
                }
            }
        }
    }
}

void AsteroidDash::placePlayer() {
    vector<vector<bool>> shape = player->spacecraft_shape;

    int shapeRows = shape.size();
    int shapeCols = shape[0].size();
    int x = player->position_row;
    int y = player->position_col;

    // Place shape in matrix starting at (x, y)
    for (int i = 0; i < shapeRows; ++i) {
        for (int j = 0; j < shapeCols; ++j) {
            // Ensure we stay within matrix bounds
            if (x + i < space_grid.size() && y + j < space_grid[0].size()) {
                if (shape[i][j])
                    space_grid[x + i][y + j] += 1;
            }
        }
    }
}

void AsteroidDash::createRightRotationsList(CelestialObject& object) {
    CelestialObject* current = &object;
    int rotationCount = 0;  // Limit to four rotations
    do {
        vector<vector<bool>> rotatedShape = CelestialObject::rotateRight(current->shape);
        CelestialObject* rotatedObject = new CelestialObject(rotatedShape, current->object_type, current->starting_row, current->time_of_appearance);
        current->right_rotation = rotatedObject;
        rotatedObject->left_rotation = current;
        current = rotatedObject;
        rotationCount++;
    } while (rotationCount < 4); // Stop after four rotations

    current->right_rotation = &object; // Close the circular list
    object.left_rotation = current;    // Set the original object's left rotation
}

void AsteroidDash::createLeftRotationsList(CelestialObject& object) {
    CelestialObject* current = &object;
    int rotationCount = 0;  // Limit to four rotations
    do {
        vector<vector<bool>> rotatedShape = CelestialObject::rotateLeft(current->shape);
        CelestialObject* rotatedObject = new CelestialObject(rotatedShape, current->object_type, current->starting_row, current->time_of_appearance);
        current->left_rotation = rotatedObject;
        rotatedObject->right_rotation = current;
        current = rotatedObject;
        rotationCount++;
    } while (rotationCount < 4); // Stop after four rotations

    current->left_rotation = &object; // Close the circular list
    object.right_rotation = current;  // Set the original object's right rotation
}

void AsteroidDash::CollisionOfObject(int collisionX, int collisionY, CelestialObject& object, Projectile* projectileObjectsListHead, Player& player) {
    // Check if collision is with the player
    vector<vector<bool>> playerShape = player.spacecraft_shape;
    int playerRows = playerShape.size();
    int playerCols = playerShape[0].size();
    int playerX = player.position_row;
    int playerY = player.position_col;

    // Look for collision with player shape
    for (int i = 0; i < playerRows; ++i) {
        for (int j = 0; j < playerCols; ++j) {
            if (playerX + i == collisionX && playerY + j == collisionY && playerShape[i][j]) {
                // Collision with player
                if (object.object_type == ASTEROID) {
                    player.lives -= 1; // Decrease lives on asteroid collision
                    if (player.lives <= 0) {
                        game_over = true; // End game if player has no lives left
                    }
                }
                else if (object.object_type == LIFE_UP) {
                    player.lives += 1; // Increase lives on life-up collision
                }
                else if (object.object_type == AMMO) {
                    player.current_ammo = player.max_ammo; // Refill ammo on ammo power-up
                }
                // Remove the object since it has been "collected" or collided with the player
                delete& object;
                return; // Exit after handling player collision
            }
        }
    }

    // Check for collision with a projectile
    Projectile* currentProjectile = projectileObjectsListHead;
    while (currentProjectile != nullptr) {
        if (currentProjectile->position_row == collisionX && currentProjectile->position_column == collisionY) {
            break; // Found the colliding projectile
        }
        currentProjectile = currentProjectile->nextProjectile;
    }

    if (currentProjectile != nullptr) {
        // Determine where the collision happened on the asteroid
        int asteroidHeight = object.shape.size();
        int collisionPosition = collisionX - object.position_row;

        // Perform rotation based on collision location
        if (collisionPosition < asteroidHeight / 3) {
            // Collision in the upper third, rotate clockwise
            object = *object.right_rotation;
        }
        else if (collisionPosition > (2 * asteroidHeight) / 3) {
            // Collision in the lower third, rotate counterclockwise
            object = *object.left_rotation;
        }
        // Else: Collision in the middle, no rotation

        // Update the shape by removing the collision point
        object.shape[collisionPosition][collisionY - object.position_column] = false;

        // If the asteroid is entirely destroyed (all false), delete it
        bool isDestroyed = true;
        for (const auto& row : object.shape) {
            for (bool cell : row) {
                if (cell) {
                    isDestroyed = false;
                    break;
                }
            }
            if (!isDestroyed) break;
        }

        if (isDestroyed) {
            delete& object;
        }
        else {
            // Otherwise, we need to recalculate rotations for the new shape
            object.delete_rotations(&object);
            createRightRotationsList(object);
            createLeftRotationsList(object);
            CelestialObject::makeRotationsCircular(&object);
        }

        // Remove the projectile that caused the collision
        delete currentProjectile;
    }
}


void AsteroidDash::CollisionOfProjectile(int collisionX, int collisionY, vector<CelestialObject>& objectsOnTheGrid, Projectile* projectile) {
    // look for every object to find the object that the collision happened with

    CelestialObject* wantedObject;

    for (CelestialObject& object : objectsOnTheGrid) {
        vector<vector<bool>> shape = object.shape;

        int shapeRows = shape.size();
        int shapeCols = shape[0].size();

        bool found = false;
        for (int i = 0; i < shapeRows && !found; ++i) {
            for (int j = 0; j < shapeCols; ++j) {
                if (object.position_row + i == collisionX && object.position_column + j == collisionY) {
                    wantedObject = &object;
                    found = true;  // Set the flag
                    // if the object that collision happenede with is not asteroid then do nothing, let projectile go thorugh it
                    if (wantedObject->object_type != ASTEROID) {
                        return;
                    }
                    break;         // Break out of the inner loop
                }
            }
        }
    }

    vector<vector<bool>> shape = wantedObject->shape;

    int x = wantedObject->position_row;
    int y = wantedObject->position_column;
    int shapeRows = shape.size();
    int shapeCols = shape[0].size();
    int oneCounter = 0;

    for (int i = 0; i < shapeRows; ++i) {
        for (int j = 0; j < shapeCols; ++j) {
            // Ensure we stay within matrix bounds
            if (x + i < space_grid.size() && y + j < space_grid[0].size()) {
                if (shape[x + i][y + j] == 1) {
                    oneCounter += 1;
                }
            }
        }
    }

    if (oneCounter = 1) {
        delete wantedObject;
    }
    else {
        // delete the collision part of the object and do the necessary rotation(not completed)
    }

    delete projectile;
}