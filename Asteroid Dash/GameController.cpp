#include "GameController.h"

// Simply instantiates the game
GameController::GameController(
        const string &space_grid_file_name,
        const string &celestial_objects_file_name,
        const string &leaderboard_file_name,
        const string &player_file_name,
        const string &player_name

) {
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name,
                            player_name);
}

// Reads commands from the given input file, executes each command in a game tick
void GameController::play(const string &commands_file) {
    // Opening the file
    ifstream file(commands_file);

    vector<string> linesVector;
    string str;

    // Read the next line from File until it reaches the
    // end.
    while (file >> str) {
        // Now keep reading next line
        // and push it in vector function until end of file
        linesVector.push_back(str);
    }

    for (string line : linesVector) {
        if (line == "PRINT_GRID") {
            game->update_space_grid();
            game->print_space_grid();
        }
        if (line == "SHOOT") {
            game->shoot();
            game->update_space_grid();
        }
        if (line == "MOVE_DOWN") {
            game->player->move_down(game->space_grid[1].size());
            game->update_space_grid();
        }
        if (line == "MOVE_UP") {
            game->player->move_up();
            game->update_space_grid();
        }
        if (line == "MOVE_LEFT") {
            game->player->move_left();
            game->update_space_grid();
        }
        if (line == "MOVE_RIGHT") {
            game->player->move_right(game->space_grid.size());
            game->update_space_grid();
        }
    }
}

// Destructor to delete dynamically allocated member variables here
GameController::~GameController() {
    delete game;  // Delete the dynamically allocated game instance
    game = nullptr;  // Set the pointer to nullptr for safety
}
