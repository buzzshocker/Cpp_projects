#include <iostream>
#include "treemap.h"
#include <fstream>
#include <cstring>
#include <iomanip>


int main(int argc, char* argv[]) {
    // Handling command line errors
    if (argc == 3) {
        // Check for all acceptable commands
        if ((strcmp(argv[2], "cheap") != 0) & (strcmp(argv[2], "rich") != 0)
        && (strcmp(argv[2], "all") != 0) && (strcmp(argv[2], "who") != 0)) {
            // If wrong, throw error statement
            std::cerr << "Command '" << argv[2] << "' is invalid\n";
            // With possible commands
            std::cerr << "Possible commands are: all|cheap|rich|who\n";
            return 1;
        } else if (strcmp(argv[2], "who") == 0) {  // If command is who
            // Throw error with right usage
            std::cerr << "Command 'who' expects another ";
            std::cerr << "argument: [+/-]amount\n";
            return 1;
        }
    } else if (argc < 3) {
        // Throw usage error if wrong number of args
        std::cerr << "Usage: " << argv[0];
        std::cerr << " <donations_file.dat> <command> [<args>]\n";
        return 1;
    }
    // Create treemap
    Treemap<int, std::string> donations_list;
    // Create strings to read from the file
    std::string p1, p2;
    // Open file
    std::ifstream donations_file(argv[1]);

    if (!donations_file.is_open()) {  // If file doesn't open
        // Throw error
        std::cerr << "Error: cannot open file " << argv[1] << "\n";
    } else {  // If file opens
        // Read line by line, split name and amount
        while (getline(donations_file, p1, ',')
        && getline(donations_file, p2)) {
            // Insert into the treemap
            donations_list.Insert(std::stoi(p2), p1);
        }
    }
    // Variables for printing data
    int key;
    std::string value;

    // If command is all
    if (strcmp(argv[2], "all") == 0) {
        while (!donations_list.Empty()) {  // While tree not empty
            // Current key becomes the minimum key
            key = donations_list.MinKey();
            // Get value for that key
            value = donations_list.Get(key);
            // Print out the key-value pair
            std::cout << value << " (" << key << ")\n";
            // Remove that key from the treemap
            donations_list.Remove(key);
            // The next key that will be printed will be the next minimum key
            // causing the printing out of the treemap in ascending order
        }
        return 0;
    }

    // If command is all
    if (strcmp(argv[2], "all") == 0) {
        // Take the initial key as the minimum key
        key = donations_list.MinKey();
        value = donations_list.Get(key);
        // Loop while key is less than max key
        while (key < donations_list.MaxKey()) {
            // Print out the key value pair
            std::cout << value << " (" << key << ")\n";
            // Next key will be the key that is the least key
            // greater than the current one
            key = donations_list.CeilKey(key + 1);
            // Get the value of the next key
            value = donations_list.Get(key);
        }
        // Exit loop and done
    }

    // If command is cheap
    if (strcmp(argv[2], "cheap") == 0) {
        // Key is the minimum key
        key = donations_list.MinKey();
        // Value is the corresponding key
        value = donations_list.Get(key);
    } else if (strcmp(argv[2], "rich") == 0) {  // If command is rich
        // Key is the maximum key
        key = donations_list.MaxKey();
        // Value is the corresponding key
        value = donations_list.Get(key);
    } else if (strcmp(argv[2], "who") == 0) {  // If command is who
        // Temporary string
        std::string temp = argv[3];
        if (temp[0] == '+') {  // If precursor to who is '+'
            // If the key is greater than the maximum key
            if (std::stoi(argv[3]) >= donations_list.MaxKey()) {
                // Put out no match since there can be no bigger key
                std::cout << "No match\n";
                return 0;
            } else {
                // Otherwise, key is the next greatest value
                key = donations_list.CeilKey((std::stoi(argv[3]) + 1));
                // Get value for that key
                value = donations_list.Get(key);
            }
        } else if (temp[0] == '-') {  // If precursor to who is '-'
            // Take negative of the given key to avoid errors
            int neg_key = std::stoi(argv[3])*(-1);
            if (neg_key <= donations_list.MinKey()) {
                // If neg_key is less than the minimum key
                // Put out no match since there can be no smaller key
                std::cout << "No match\n";
                return 0;
            } else {
                // Otherwise, key is the next smallest key
                key = donations_list.FloorKey((neg_key - 1));
                // Get value of that key
                value = donations_list.Get(key);
            }
        } else {  // If no precursor to 'who'
            if (donations_list.ContainsKey(std::stoi(argv[3])) != true) {
                // If key not in tree put out no match
                std::cout << "No match\n";
                return 0;
            } else {
                // Otherwise, return that key
                key = std::stoi(argv[3]);
                // Get the value of that key
                value = donations_list.Get(key);
            }
        }
    }
    // Print out key-value pair corresponding to the given command
    std::cout << value << " (" << key << ")\n";
    // Close file
    donations_file.close();

    return 0;
}
