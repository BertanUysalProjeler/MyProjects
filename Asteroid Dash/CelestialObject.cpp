#include "CelestialObject.h"


// Constructor to initialize CelestialObject with essential properties
CelestialObject::CelestialObject(const vector<vector<bool>> &shape, ObjectType type, int start_row,
                                 int time_of_appearance)
        : shape(shape), object_type(type), starting_row(start_row), time_of_appearance(time_of_appearance) {
}


// Copy constructor for CelestialObject
CelestialObject::CelestialObject(const CelestialObject *other)
        : shape(other->shape),  // Copy the 2D vector shape
          object_type(other->object_type),  // Copy the object type
          starting_row(other->starting_row),  // Copy the starting row
          time_of_appearance(other->time_of_appearance)  // Copy the time of appearance
{
}

// Destructor
CelestialObject::~CelestialObject() {
    // Call delete_rotations to safely delete any dynamically allocated rotations
    delete_rotations(this);
}

// Function to generate a clockwise (right) rotation of a shape
vector<vector<bool>> CelestialObject::rotateRight(const vector<vector<bool>>& shape) {
    int rows = shape.size();
    int cols = shape[0].size();
    
    std::cout << "before the rotating" << std::endl;
    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y) {
            std::cout << shape[x][y] << " ";
        }
        std::cout << std::endl;
    }

    vector<vector<bool>> rotated(cols, vector<bool>(rows));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            rotated[j][rows - i - 1] = shape[i][j];
        }
    }

    std::cout << "after the rotating to right" << std::endl;
    for (int x = 0; x < cols; ++x) {
        for (int y = 0; y < rows; ++y) {
            std::cout << rotated[x][y] << " ";
        }
        std::cout << std::endl;
    }

    return rotated;
}

// Function to generate a counterclockwise (left) rotation of a shape
vector<vector<bool>> CelestialObject::rotateLeft(const vector<vector<bool>>& shape) {
    int rows = shape.size();
    int cols = shape[0].size();

    std::cout << "before the rotating" << std::endl;
    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y) {
            std::cout << shape[x][y] << " ";
        }
        std::cout << std::endl;
    }

    vector<vector<bool>> rotated(cols, vector<bool>(rows));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            rotated[cols - j - 1][i] = shape[i][j];
        }
    }

    std::cout << "after the rotating to left" << std::endl;
    for (int x = 0; x < cols; ++x) {
        for (int y = 0; y < rows; ++y) {
            std::cout << rotated[x][y] << " ";
        }
        std::cout << std::endl;
    }

    return rotated;
}

// Function to delete rotations of a given celestial object. It should free the dynamically allocated
// memory for each rotation.
void CelestialObject::delete_rotations(CelestialObject* target) {
    if (target == nullptr || target->right_rotation == nullptr || target->right_rotation == target) return;

    CelestialObject* current = target->right_rotation;
    while (current != target) {
        CelestialObject* next = current->right_rotation;
        current->right_rotation = nullptr;  // Set to nullptr to avoid circular references
        current->left_rotation = nullptr;
        delete current;
        current = next;
    }
    target->right_rotation = target->left_rotation = nullptr; // Reset rotations to nullptr
}

void CelestialObject::makeRotationsCircular(CelestialObject* head) {
    if (!head) return;  // If the head is null, do nothing

    // Check if the object has no distinct rotations
    if (!head->right_rotation || head->right_rotation == head) {
        head->right_rotation = head;
        head->left_rotation = head;
        return;
    }

    CelestialObject* tail = head;

    // Traverse to the last rotation
    while (tail->right_rotation && tail->right_rotation != head) {
        tail = tail->right_rotation;
    }

    // Connect the tail to the head to make the list circular
    tail->right_rotation = head;
    head->left_rotation = tail;
}
