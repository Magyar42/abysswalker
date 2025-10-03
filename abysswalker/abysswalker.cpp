// abysswalker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>
#include <fstream>
#include <string>
using namespace std;

//map<string, string>SaveFile;
string textSeparator = "\n========================================\n\n";
map<int, map<string, string>> saveSlots;
string titleDisplay[6] = {
    "   _____ ___.                                        .__   __                 ",
"  /  _  \\\\_ |__ ___.__. ______ ________  _  _______  |  | |  | __ ___________ ",
" /  /_\\  \\| __ <   |  |/  ___//  ___/\\ \\/ \\/ /\\__  \\ |  | |  |/ // __ \\_  __ \\",
"/    |    \\ \\_\\ \\___  |\\___ \\ \\___ \\  \\     /  / __ \\|  |_|    <\\  ___/|  | \\/ ",
"\\____|__  /___  / ____/____  >____  >  \\/\\_/  (____  /____/__|_ \\\\___  >__|",
"        \\/    \\/\\/         \\/     \\/               \\/          \\/    \\/       "
};

bool checkForSave()
{
    int selectedSave = -1;
    string currentLine = "";
    int currentSlot = -1;
    int saveSlotArray[3] = { 0, 0, 0 };

    // Find savefile.json (if possible), create the file if not present
    ifstream saveFile("savefile.json");
    if (!saveFile.is_open()) {
        cout << "No save found. Creating new file.\n";
        return false;
    }

    // Read through the file
    while (getline(saveFile, currentLine)) {
        //cout << currentLine + "\n";
        if (currentLine.empty()) continue;

        if (currentLine.find("[Slot") != string::npos) {
            currentSlot = stoi(currentLine.substr(5, 1));
            //saveSlots[currentSlot] = map < string, string >> "()";
        }

        // Use npos!!
        // If ":" is in this line, find it and get the key + value for dictionary
        else if (currentSlot != -1 && currentLine.find(":") != string::npos) {
            size_t colonPosition = currentLine.find(":");
            string key = currentLine.substr(0, colonPosition);
            string value = currentLine.substr(colonPosition + 2);
            saveSlots[currentSlot][key] = value;

            // Mark save slot as not empty (for display)
            if (saveSlotArray[currentSlot] == 0) {
                saveSlotArray[currentSlot] = 1;
            }
        }
    }

    saveFile.close();

    // Print saves to choose from
    cout << textSeparator << "Choose a save slot.\n\n";
    for (int i = 0; i < 3; i++) {
        cout << "Slot #" << i + 1 << ":\n";

        if (saveSlotArray[i] == 1) {
            cout << " - " << saveSlots[i]["items"] << "/56 Items\n";
            cout << " - " << saveSlots[i]["old_souls"] << "/6 Old Souls\n";
        }
        else {
            cout << " Empty\n";
        }
    }
    cout << textSeparator;
    cin >> selectedSave;

    return true;
}

int main()
{
    for (string titleLine : titleDisplay) {
        cout << titleLine << "\n";
    }

    checkForSave();
}