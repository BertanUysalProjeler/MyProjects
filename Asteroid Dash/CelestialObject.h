#ifndef CELESTIALOBJECT_H
#define CELESTIALOBJECT_H

#include <vector>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// Enum to represent the type of the object (asteroid, life-up, or ammo)
enum ObjectType {
    ASTEROID = 0,
    LIFE_UP = 1,
    AMMO = 2
};

class CelestialObject {
public:

    // Constructor to initialize CelestialObject with essential properties
    CelestialObject(const vector<vector<bool>> &shape, ObjectType type, int start_row, int time_of_appearance);

    // Copy constructor for CelestialObject
    CelestialObject(const CelestialObject *other);

    // Destructor
    virtual ~CelestialObject();

    // Shape of the object represented as a 2D boolean vector
    vector<vector<bool>> shape;

    // Pointer to the object's clockwise neighbor (its right rotation)
    CelestialObject* right_rotation = nullptr;

    // Pointer to the object's clockwise neighbor (its left rotation)
    CelestialObject* left_rotation = nullptr;

    // Pointer to the next celestial object in the list
    CelestialObject* next_celestial_object = nullptr;

    // Type of the object (asteroid, life-up, or ammo)
    ObjectType object_type;

    // Row in the space grid where the object's top will appear
    int starting_row;

    // The step in the game after which the object will appear on the grid
    int time_of_appearance;

    int position_row;

    int position_column;

    // Function to delete rotations of a given celestial object. It should free the dynamically allocated
    // space for each rotation.
    static void delete_rotations(CelestialObject* target);

    static void makeRotationsCircular(CelestialObject* head);

    // Static functions to rotate shapes
    static vector<vector<bool>> rotateRight(const vector<vector<bool>>& shape);
    static vector<vector<bool>> rotateLeft(const vector<vector<bool>>& shape);
};

#endif // CELESTIALOBJECT_H
