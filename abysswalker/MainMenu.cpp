#include "MainMenu.h"
#include "Misc.cpp"
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

void MainMenu::displayMenuOptions()
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
                else if (selectionValue == "Start") { gameStarted = true; }
                break;
            }
        }
        if (gameStarted) { break; }
    }
}
