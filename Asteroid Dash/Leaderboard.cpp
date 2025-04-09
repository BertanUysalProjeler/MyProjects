#include "Leaderboard.h"

// Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
// variable will point to the highest all-times score, and all other scores will be reachable from it
// via the "next_leaderboard_entry" member variable pointer.
void Leaderboard::read_from_file(const std::string &filename) {

    std::ifstream file(filename);

    int score;
    long timestamp;
    std::string name;
    LeaderboardEntry* head = nullptr;

    while (file >> score >> timestamp >> name) {
        LeaderboardEntry* newEntry = new LeaderboardEntry(score, timestamp, name);
        insert(newEntry);
    }

    file.close();
}

// Write the latest leaderboard status to the given file in the format specified in the PA instructions
void Leaderboard::write_to_file(const string& filename) {
    // Open the file in write mode
    ofstream outFile(filename);

    // Start from the head of the linked list
    LeaderboardEntry* current = head_leaderboard_entry;

    // Loop through the linked list
    while (current != nullptr) {
        // Write the current entry's data to the file in the specified format
        outFile << current->score << " "
            << current->last_played << " "
            << current->player_name << endl;

        // Move to the next node
        current = current->next;
    }

    // Close the file
    outFile.close();
}

// Print the current leaderboard status to the standard output in the format specified in the PA instructions
void Leaderboard::print_leaderboard() {
    cout << "Leaderboard" << endl;
    cout << "-----------" << endl;
    LeaderboardEntry* current = head_leaderboard_entry;
    int rank = 1;
    while (current != nullptr && rank <= MAX_LEADERBOARD_SIZE) {
        cout << rank++ << ". " << current->player_name << " " << current->score << " " << FormattedTimestamp(current->last_played) << endl;
        current = current->next;
    }
}

//  Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
//  is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
//  top 10 all-time high-scores should be kept in descending order by the score).
void Leaderboard::insert(LeaderboardEntry* newEntry) {

    LeaderboardEntry* head = head_leaderboard_entry;

    // Insert new entry at the correct position in descending order
    if (head == nullptr || head->score <= newEntry->score) {
        newEntry->next = head;
        head = newEntry;
    }
    else {
        LeaderboardEntry* current = head;
        while (current->next != nullptr && current->next->score > newEntry->score) {
            current = current->next;
        }
        newEntry->next = current->next;
        current->next = newEntry;
    }

    // Now ensure there are no more than 10 entries
    LeaderboardEntry* temp = head;
    int count = 1;

    while (temp->next != nullptr) {
        count++;
        if (count == 10 && temp->next->next != nullptr) {
            // We've reached the 10th element, and there's more beyond it
            LeaderboardEntry* toDelete = temp->next;
            temp->next = nullptr;  // Remove the extra entry
            delete toDelete;        // Free memory
            break;
        }
        temp = temp->next;
    }
}

string Leaderboard::FormattedTimestamp(time_t timestamp) {
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%H:%M:%S/%d.%m.%Y", localtime(&timestamp));
    return std::string(buffer);
}

// Free dynamically allocated memory used for storing leaderboard entries
Leaderboard::~Leaderboard() {
    LeaderboardEntry* current = head_leaderboard_entry;
    while (current != nullptr) {
        LeaderboardEntry* next = current->next;  // Save the next pointer
        delete current;  // Delete the current entry
        current = next;  // Move to the next entry
    }
    head_leaderboard_entry = nullptr;  // Optional: set head to nullptr after cleanup
}
