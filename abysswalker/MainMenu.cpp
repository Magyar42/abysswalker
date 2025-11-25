#include "MainMenu.h"
#include "List.h"
#include "Misc.h"
#include <string>
#include <fstream>
#include <iostream>
#include <conio.h>
using namespace std;

MainMenu::MainMenu(string area, string keepsake, string soul)
{
    setArea = area;
    setKeepsake = keepsake;
    setOldSoul = soul;
    saveSlotArray = { 0, 0, 0 };
    gameStarted = false;

    string currentLine = "";
    int currentSlot = -1;

    ifstream saveFile("savefile.json");
    while (getline(saveFile, currentLine)) {
        if (currentLine.empty()) continue;

        if (currentLine.find("[Slot") != string::npos) {
            currentSlot = stoi(currentLine.substr(5, 1));
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

    initItems();
}

void MainMenu::displaySaves()
{
    displayTitle();

    string slotName = "";
    cout << "Enter the index of a save slot to continue.\n\n";
    for (int i = 1; i < 4; i++) {
        slotName = "Slot #" + to_string(i);
        cout << colourText(slotName, YELLOW) << ":\n";

        if (saveSlotArray[i - 1] == 1) {
            cout << " - " << saveSlots[i - 1]["items"] << "/56 Items\n";
            cout << " - " << saveSlots[i - 1]["old_souls"] << "/6 Old Souls\n";
        }
        else {
            cout << " Empty\n";
        }
    }
    cout << textSeparator;

    char selectedSave = _getch();
    cout << "Loading save slot " << selectedSave << "...\n";

    clearScreen();
}

void MainMenu::initItems()
{
    string currentLine = "";
    string currentItem = "";

    ifstream file("items.txt");
    while (getline(file, currentLine)) {
        if (currentLine.empty()) continue;

		// Add to vector of all items
        if (currentLine.find("[") != string::npos) {
            currentItem = currentLine.substr(1, currentLine.length() - 2);
            allItemsVector.push_back(currentItem);
        }

		// Add to map of all items with desc, type and unlock status
        else if (currentItem != "" && currentLine.find(":") != string::npos) {
            size_t colonPosition = currentLine.find(":");
            string key = currentLine.substr(0, colonPosition);
            string value = currentLine.substr(colonPosition + 2);
            allItemsMap[currentItem][key] = value;

            // Add to separate ring, weapon, gem, default vectors
            int itemTypeID = 0;
            if (value == "ring") { itemTypeID = 1; }
            else if (value == "weapon") { itemTypeID = 3; }
            else if (value == "gem") { itemTypeID = 2; }
            allItemsByType[itemTypeID].push_back(currentItem);

            // Add to qPressInfo for item descriptions
            if (key == "desc") {
                qPressInfo[0].push_back(currentItem);
                qPressInfo[1].push_back(allItemsMap[currentItem]["desc"]);
            }
        }
    }
    file.close();
}

void MainMenu::displayMenuOptions()
{
    vector<string> selectionArray = { "View Itemlist", "Change Region", "Change Keepsake", "Change Old Soul", "Start" };

    List mainMenuOptions("Main Menu", selectionArray);
    while (true) {
        displayTitle();
        cout << " Prepare for your journey before starting. You can press Q for more information about the currently selected line at any time.\n\n";
        cout << colourText(" Region: ", BLUE) << setArea << "\n";
        cout << colourText(" Keepsake: ", BLUE) << setKeepsake << "\n";
        cout << colourText(" Old Soul: ", BLUE) << setOldSoul << "\n";
        cout << textSeparator;

        string returnValue = mainMenuOptions.displayItems();
        if (returnValue.find("|") != string::npos) {
            string token = returnValue.substr(returnValue.find("|"), returnValue.length() + 1);
            string item = returnValue.substr(0, returnValue.find("|"));
            if (token == "|keepsake") { setKeepsake = item; }
            else if (token == "|oldsoul") { setOldSoul = item; }
            else if (token == "|region") { setArea = item; }
        }
        else if (returnValue == "start") {
            gameStarted = true;
            break;
        }
    }
}
