// abysswalker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <conio.h>
#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

//map<string, string>SaveFile;
string textSeparator = "\n========================================\n\n";
string titleDisplay[6] = {
    "   _____ ___.                                        .__   __                 ",
"  /  _  \\\\_ |__ ___.__. ______ ________  _  _______  |  | |  | __ ___________ ",
" /  /_\\  \\| __ <   |  |/  ___//  ___/\\ \\/ \\/ /\\__  \\ |  | |  |/ // __ \\_  __ \\",
"/    |    \\ \\_\\ \\___  |\\___ \\ \\___ \\  \\     /  / __ \\|  |_|    <\\  ___/|  | \\/ ",
"\\____|__  /___  / ____/____  >____  >  \\/\\_/  (____  /____/__|_ \\\\___  >__|",
"        \\/    \\/\\/         \\/     \\/               \\/          \\/    \\/       "
};

string setArea = "Deeproot Depths";
string setKeepsake = "None";
string setOldSoul = "None";
bool gameStarted = false;

map<int, map<string, string>> saveSlots;
vector<vector<string>>  qPressInfo = {
    { "Change Region", "Change Keepsake", "Change Old Soul", "Start" },
    { "There are 3 regions to travel through in Abysswalker. Each has different locations, enemies, bosses, and mechanics. To unlock the next region, you must achieve victory in the prior region.", "Keepsakes are items that you start with. They usually provide passive effects, but can sometimes be used in specific situations. More keepsakes can be unlocked by completing specific objectives.", "Old Souls are the powerful souls of heroes long gone. If one is selected, you will begin with different stats and items. Old Souls can be found during gameplay, unlocking them permanently.", "Select this option to begin the game! Make sure you have chosen your starting gear well." }
};
int qPressInfoArrayLength = sizeof(qPressInfo[0]) / sizeof(qPressInfo[0][0]);

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void displayTitle()
{
    for (string titleLine : titleDisplay) {
        cout << titleLine << "\n";
    }
    cout << textSeparator;
}

bool checkForSave()
{
    displayTitle();

    //int selectedSave = -1;
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
    cout << "Enter the index of a save slot to continue.\n\n";
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

    char selectedSave = _getch();
    cout << "Loading save slot " << selectedSave << "...\n";

    clearScreen();
    return true;
}

int selectionIndexUpdate(int currentIndex, int arrayLength, char input)
{
    int newArrayLength = arrayLength - 1;
    // char selectionIndexChange = _getch();

    // W and S press will cycle through list of options
    if (input == 's') {
        if (currentIndex >= newArrayLength) { currentIndex = 0; }
        else { ++currentIndex; }
    }
    else if (input == 'w') {
        if (currentIndex <= 0) { currentIndex = newArrayLength; }
        else { --currentIndex; }
    }

    return currentIndex;
}

void qPressCheck(string currentSelection)
{
    for (int i = 0; i < qPressInfo[0].size(); ++i) {
        if (qPressInfo[0][i] == currentSelection) {
            cout << "\n [INFO]: " << qPressInfo[1][i] << "\n";
        }
    }
}

void displayGamePreparations()
{
    string selectionArray[4] = { "Change Region", "Change Keepsake", "Change Old Soul", "Start" };
    int selectionArrayLength = sizeof(selectionArray) / sizeof(selectionArray[0]);
    int selectionIndex = 0;

    while (gameStarted == false) {
        displayTitle();
        cout << " Prepare for your journey before starting. You can press Q for more information about the currently selected line at any time.\n\n";
        cout << " Region: " << setArea << "\n";
        cout << " Keepsake: " << setKeepsake << "\n";
        cout << " Old Soul: " << setOldSoul << "\n";
        cout << textSeparator;

        string selectionValue = selectionArray[selectionIndex];
        //cout << "  Change Region\n  Change Keepsake\n  Change Old Soul\n  Start\n";
        for (string selectionItem : selectionArray) {
            if (selectionValue == selectionItem) {
                cout << " > " << selectionItem << "\n";
            }
            else {
                cout << "   " << selectionItem << "\n";
            }
        }

        while (true) {
            char qCheck = _getch();
            if (qCheck == 'q') {
                qPressCheck(selectionValue);
            }
            else if (qCheck == 's' || qCheck == 'w') {
                int newSelectionIndex = selectionIndexUpdate(selectionIndex, selectionArrayLength, qCheck);
                selectionIndex = newSelectionIndex;
                clearScreen();
                break;
            }
        }
    }
}

int main()
{
    checkForSave();

    displayGamePreparations();
}