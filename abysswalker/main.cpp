// abysswalker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <conio.h>
#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
using namespace std;

// Global Variables
string textSeparator = "\n================================================================================\n\n";
string titleDisplay[6] = {
    "   _____ ___.                                        .__   __                 ",
"  /  _  \\\\_ |__ ___.__. ______ ________  _  _______  |  | |  | __ ___________ ",
" /  /_\\  \\| __ <   |  |/  ___//  ___/\\ \\/ \\/ /\\__  \\ |  | |  |/ // __ \\_  __ \\",
"/    |    \\ \\_\\ \\___  |\\___ \\ \\___ \\  \\     /  / __ \\|  |_|    <\\  ___/|  | \\/ ",
"\\____|__  /___  / ____/____  >____  >  \\/\\_/  (____  /____/__|_ \\\\___  >__|",
"        \\/    \\/\\/         \\/     \\/               \\/          \\/    \\/       "
};

string setArea = "Darkroot Depths";
string setKeepsake = "None";
string setOldSoul = "Soul of the Wolf Knight";
int currentAreaDay = 1;
string currentAreaDayOrNight = "Day";
string currentAreaTime = "9:00";
bool gameStarted = false;

map<int, map<string, string>> saveSlots;
map<string, map<string, string>> keepsakesList;
map<string, map<string, string>> oldSoulsList;
map<string, map<string, string>> regionsList;

vector<vector<string>>  qPressInfo = {
    { "View Itemlist", "Change Region", "Change Keepsake", "Change Old Soul", "Start", "Life Ring", "Fire Gem", "Black Firebombs", "Test Item" },
    { "Items are the main way of getting stronger in ABYSSWALKER. There are many that can be unlocked by meeting specific conditions, allowing them to appear in-game.", "There are 3 regions to travel through in ABYSSWALKER. Each has different locations, enemies, bosses, and mechanics. To unlock the next region, you must achieve victory in the prior region.", "Keepsakes are items that you start with. They usually provide passive effects, but can sometimes be used in specific situations. More keepsakes can be unlocked by completing specific objectives.", "Old Souls are the powerful souls of heroes long gone. If one is selected, you will begin with different stats and items. Old Souls can be found during gameplay, unlocking them permanently.", "Select this option to begin the game! Make sure you have chosen your starting gear well.", "Begin with +3 HP.", "Consumable item. Use at a Blacksmith to add the Fire effect to your current weapon.", "Every other turn in combat, attack a second time, inflicting 2 damage. Triggers up to 3 times in one fight.", "Unknown." }
};
int playerCoords[2] = { 4, 4 };
int playerHP = 10;
int playerATK = 1;
int playerDEF = 0;
int playerSPD = 0;
string playerWeapon = "None";
vector<string> playerInventory = { "Empty", "Empty", "Empty", "Empty" };

const string INACTIVE = "\033[0;90m";
const string RED = "\033[0;31m";
const string GREEN = "\033[0;32m";
const string YELLOW = "\033[0;33m";
const string BLUE = "\033[0;36m";
const string RESET = "\033[0m";
const string PLAYER_TILE = " A ";
const string OPEN_TILE = "   ";
const string CLOSED_TILE = "NNN";

vector<vector<string>> worldMap = {
        {CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, OPEN_TILE, CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, OPEN_TILE,},
        {CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, OPEN_TILE, CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, OPEN_TILE,},
        { CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, OPEN_TILE, OPEN_TILE, OPEN_TILE, OPEN_TILE, OPEN_TILE, OPEN_TILE, },
        { CLOSED_TILE, OPEN_TILE, CLOSED_TILE, CLOSED_TILE, OPEN_TILE, CLOSED_TILE, OPEN_TILE, CLOSED_TILE, OPEN_TILE, CLOSED_TILE, },
        { CLOSED_TILE, OPEN_TILE, OPEN_TILE, OPEN_TILE, OPEN_TILE, CLOSED_TILE, OPEN_TILE, CLOSED_TILE, OPEN_TILE, CLOSED_TILE, },
        { CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, OPEN_TILE, CLOSED_TILE, CLOSED_TILE, OPEN_TILE, CLOSED_TILE, OPEN_TILE, OPEN_TILE, },
        { CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, OPEN_TILE, CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, OPEN_TILE, CLOSED_TILE, },
        { CLOSED_TILE, OPEN_TILE, CLOSED_TILE, OPEN_TILE, CLOSED_TILE, CLOSED_TILE, OPEN_TILE, OPEN_TILE, OPEN_TILE, CLOSED_TILE, },
        { CLOSED_TILE, OPEN_TILE, OPEN_TILE, OPEN_TILE, CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, },
        { CLOSED_TILE, OPEN_TILE, CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, CLOSED_TILE, },
};

// Utility Functions
string colourText(const string& text, const string& colour, const string& reset = RESET)
{
    return colour + text + reset;
}

void clearScreen()
{
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

string displayList(string listInfo, string fileName, map<string, map<string, string>> listName)
{
    int selectionIndex = 0;
    bool endDisplayList = false;
    string selectedItem = "";

    while (true) {
        displayTitle();
        for (int i = 0; i < qPressInfo[0].size(); ++i) {
            if (qPressInfo[0][i] == listInfo) {
                cout << qPressInfo[1][i] << "\n";
            }
        }
        cout << textSeparator;

        string currentLine = "";
        string currentItem = "";
        vector<string> displayList = {};
        vector<string> fullList = {};

        // Loop through each item in .json, fill out the dictionary with keys and values
        ifstream itemFile(fileName);
        while (getline(itemFile, currentLine)) {
            if (currentLine.empty()) continue;

            if (currentLine.find("[") != string::npos) {
                currentItem = currentLine.substr(1, currentLine.length()-2);
                //cout << currentItem;
                fullList.push_back(currentItem);
            }

            else if (currentItem != "" && currentLine.find(":") != string::npos) {
                size_t colonPosition = currentLine.find(":");
                string key = currentLine.substr(0, colonPosition);
                string value = currentLine.substr(colonPosition + 2);
                listName[currentItem][key] = value;
            }

        }
        itemFile.close();

        int unknownItemCounter = 1;
        // Loop through updated dictionary, display all unlocked items
        for (string item : fullList) {
            if (listName[item]["unlocked"] == "true") { displayList.push_back(item); }
            else {
                displayList.push_back(colourText("Undiscovered #" + to_string(unknownItemCounter), RED));
                ++unknownItemCounter;
            }
        }
        if (listInfo != "Change Region") { displayList.push_back("None"); }

        string selectionValue = displayList[selectionIndex];
        for (string selectionItem : displayList) {
            if (selectionValue == selectionItem) {
                cout << colourText(" > " + selectionItem, YELLOW) << "\n";
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
                int newSelectionIndex = selectionIndexUpdate(selectionIndex, displayList.size(), qCheck);
                selectionIndex = newSelectionIndex;
                clearScreen();
                break;
            }
            else if (qCheck == 'e') {
                if (displayList[selectionIndex].find("Undiscovered")==string::npos) {
                    selectedItem = displayList[selectionIndex];
                    endDisplayList = true;
                    clearScreen();
                    break;
                }
            }
        }
        if (endDisplayList) {
            break;
        }
    }
    return selectedItem;
}

string displayListSetup(string listType)
{
    string selectedItem = "";
    if (listType == "Change Keepsake") { selectedItem = displayList(listType, "keepsakes.json", keepsakesList); }
    else if (listType == "Change Old Soul") { selectedItem = displayList(listType, "oldsouls.json", oldSoulsList); }
    else if (listType == "Change Region") { selectedItem = displayList(listType, "regions.json", regionsList); }
    return selectedItem;
}

// Main Loop Functions
bool checkForSave()
{
    displayTitle();

    //int selectedSave = -1;
    string currentLine = "";
    int currentSlot = -1;
    int saveSlotArray[3] = { 0, 0, 0 };

    ifstream saveFile("savefile.json");
    if (!saveFile.is_open()) {
        cout << "No save found. Creating new file.\n";
        return false;
        clearScreen();
    }

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

    string slotName = "";
    // Print saves to choose from
    cout << "Enter the index of a save slot to continue.\n\n";
    for (int i = 1; i < 4; i++) {
        slotName = "Slot #" + to_string(i);
        cout << colourText(slotName, YELLOW) << ":\n";

        if (saveSlotArray[i-1] == 1) {
            cout << " - " << saveSlots[i-1]["items"] << "/56 Items\n";
            cout << " - " << saveSlots[i-1]["old_souls"] << "/6 Old Souls\n";
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

void displayGamePreparations()
{
    string newlySelectedItem = "";
    string selectionArray[] = { "View Itemlist", "Change Region", "Change Keepsake", "Change Old Soul", "Start" };
    int selectionArrayLength = sizeof(selectionArray) / sizeof(selectionArray[0]);
    int selectionIndex = 0;

    while (true) {
        displayTitle();
        cout << " Prepare for your journey before starting. You can press Q for more information about the currently selected line at any time.\n\n";
        cout << colourText(" Region: ", BLUE) << setArea << "\n";
        cout << colourText(" Keepsake: ", BLUE) << setKeepsake << "\n";
        cout << colourText(" Old Soul: ", BLUE) << setOldSoul << "\n";
        cout << textSeparator;

        string selectionValue = selectionArray[selectionIndex];
        //cout << "  Change Region\n  Change Keepsake\n  Change Old Soul\n  Start\n";
        for (string selectionItem : selectionArray) {
            if (selectionValue == selectionItem) {
                cout << colourText(" > " + selectionItem, YELLOW) << "\n";
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
            else if (qCheck == 'e') {
                clearScreen();
                newlySelectedItem = displayListSetup(selectionValue);
                if (selectionValue == "Change Keepsake") { setKeepsake = newlySelectedItem; }
                else if (selectionValue == "Change Old Soul") { setOldSoul = newlySelectedItem; }
                else if (selectionValue == "Change Region") { setArea = newlySelectedItem; }
                else if (selectionValue == "Start")  { gameStarted = true; }
                break;
            }
        }
        if (gameStarted) { break; }
    }
}

string selectBoss(string area, int day)
{
    string currentBoss = "";
    if (area == "Darkroot Depths"){
        switch (day) {
            case 1:
                currentBoss = "Titanite Demon";
                break;
            case 2:
                currentBoss = "Moonlight Butterfly";
                break;
            case 3:
                currentBoss = "Hydra of the Basin";
                break;
        }
    }
    return currentBoss;
}

void setupPlayer()
{
    if (setOldSoul == "Soul of the Wolf Knight") {
        playerHP = 10;
        playerATK = 4;
        playerDEF = 5;
        playerSPD = 1;
        playerWeapon = "Greatsword";
    }
    
    playerInventory.at(0) = setKeepsake;
}

tuple<int, int> displayMap(vector<vector<string>> map, string reset_colour)
{
    cout << "\nTest.\n";
    tuple<int, int> mapSize;
    mapSize = make_tuple(map[0].size(), map.size());
    map[playerCoords[0]][playerCoords[1]] = colourText(PLAYER_TILE, BLUE, reset_colour);

    for (const auto& row: map) {
        cout << " ";
        for (const auto& tile : row) {
            cout << tile;
        }
        cout << endl;
    }

    return mapSize;
}

void playerMovementUpdate(char input, tuple<int, int> mapSize)
{
    int newCoords[2];
    copy(playerCoords, playerCoords + 2, newCoords);
    // cout << newCoords[0] << " | " << newCoords[1];

    if (input == 'w') { --newCoords[0]; }
    else if (input == 's') { ++newCoords[0]; }
    else if (input == 'a') { --newCoords[1]; }
    else if (input == 'd') { ++newCoords[1]; }

    if (newCoords[0] >= 0 && newCoords[1] >= 0) {
        if (newCoords[0] < get<0>(mapSize) && newCoords[1] < get<1>(mapSize)) {
            if (worldMap[newCoords[0]][newCoords[1]] == OPEN_TILE) {
                playerCoords[0] = newCoords[0];
                playerCoords[1] = newCoords[1];
            }
        }
    }
}

void generateWorld()
{
    tuple<int, int> mapSize;
    string currentBoss = "";
    currentBoss = selectBoss(setArea, currentAreaDay);
    setupPlayer();

    bool mapSelected = true;
    while (true) {
        cout << colourText("", RESET);
        displayTitle();
        string dayInfo = currentAreaTime + " [" + currentAreaDayOrNight + " " + to_string(currentAreaDay) + "]";

        string reset_colour = "";
        if (mapSelected == true) {
            reset_colour = RESET;
        }
        else {
            reset_colour = INACTIVE;
        }

        cout << colourText(" Next Boss: ", BLUE, reset_colour) << currentBoss << " [Q]\n";
        cout << colourText(" Current Time: ", BLUE, reset_colour) << dayInfo << "\n";
        cout << colourText("\n HP: ", GREEN, reset_colour) << playerHP << " | " << colourText("ATK: ", RED, reset_colour) << playerATK << " | " << colourText("DEF: ", BLUE, reset_colour) << playerDEF << " | " << colourText("SPD: ", YELLOW, reset_colour) << playerSPD << "\n";
        mapSize = displayMap(worldMap, reset_colour);
        cout << colourText("", RESET);
        cout << textSeparator;

        if (mapSelected) { cout << colourText("", INACTIVE, INACTIVE); }

        cout << playerWeapon << endl;
        int index = 1;
        for (string item : playerInventory) {
            cout << "[" << index << "] " << item << endl;
            index++;
        }

        while (true) {
            char input = _getch();
            if (mapSelected) {
                if (input == 'q') {
                    qPressCheck(currentBoss); // todo
                }
                else if (input == 'w' || input == 'a' || input == 's' || input == 'd') {
                    playerMovementUpdate(input, mapSize);
                    clearScreen();
                    break;
                }
                else if (input == 'e') {
                    clearScreen();
                    mapSelected = false;
                    break;
                }
            }
            else {
                if (input == 'e') {
                    clearScreen();
                    mapSelected = true;
                    break;
                }
            }
        }
    }

}

// Game Loop
int main()
{
    checkForSave();

    displayGamePreparations();

    generateWorld();
}