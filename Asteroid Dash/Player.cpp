#include "Player.h"

// Constructor to initialize the player's spacecraft, position, and ammo
Player::Player(const vector<vector<bool>> &shape, int row, int col, const string &player_name, int max_ammo, int lives)
        : spacecraft_shape(shape), position_row(row), position_col(col), player_name(player_name), max_ammo(max_ammo),
          current_ammo(max_ammo), lives(lives) {
}

// Move player left within the grid boundaries
void Player::move_left() {
    if (position_col > 0) position_col -= 1;
}

// Move player right within the grid boundaries
void Player::move_right(int grid_width) {
    if (position_col < grid_width - spacecraft_shape[0].size()) position_col += 1;
}

// Move player up within the grid boundaries
void Player::move_up() {
    if (position_row > 0) position_row -= 1;
}

// Move player down within the grid boundaries
void Player::move_down(int grid_height) {
    if (position_row < grid_height - spacecraft_shape.size()) position_row += 1;
}