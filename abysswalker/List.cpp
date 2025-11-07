#include "List.h"
#include "Misc.h"
#include <string>
#include <conio.h>
#include <iostream>
#include <fstream>
using namespace std;

List::List(string type, vector<string> list, bool import)
{
    listType = type;
	listItems = list;
	importFromFile = import;

	selectedItemIndex = 0;
	currentSelectedItem = listItems[selectedItemIndex];
	newlySelectedItem = "";
	listLength = listItems.size();
}

string List::displayItems()
{
	currentSelectedItem = listItems[selectedItemIndex];
	for (string item : listItems) {
		if (currentSelectedItem == item) {
			cout << colourText(" > " + item, YELLOW) << "\n";
		}
		else {
			cout << "   " << item << "\n";
		}
	}

    while (true) {
        char charInput = _getch();
        if (charInput == 'q') {
            qPressCheck(currentSelectedItem);
        }
        else if (charInput == 's' || charInput == 'w') {
            int newItemIndex = selectionIndexUpdate(selectedItemIndex, listLength, charInput);
            selectedItemIndex = newItemIndex;
            clearScreen();
            break;
        }
        else if (charInput == 'e') {
            if (currentSelectedItem.find("Undiscovered") == string::npos) {
                string selectedItem = itemSelectionEffect();
                return selectedItem;
                break;
            }
        }
    }

    return "null";
}

string List::displayImportedItems(string filename, map<string, map<string, string>> map)
{
    int selectionIndex = 0;
    bool endDisplayList = false;
    string selectedItem = "";

    while (true) {
        string currentLine = "";
        string currentItem = "";
        vector<string> displayList = {};
        vector<string> fullList = {};

        // Loop through each item in .json, fill out the dictionary with keys and values
        ifstream itemFile(filename);
        while (getline(itemFile, currentLine)) {
            if (currentLine.empty()) continue;

            if (currentLine.find("[") != string::npos) {
                currentItem = currentLine.substr(1, currentLine.length() - 2);
                fullList.push_back(currentItem);
            }

            else if (currentItem != "" && currentLine.find(":") != string::npos) {
                size_t colonPosition = currentLine.find(":");
                string key = currentLine.substr(0, colonPosition);
                string value = currentLine.substr(colonPosition + 2);
                map[currentItem][key] = value;
            }

        }
        itemFile.close();

        int unknownItemCounter = 1;
        // Loop through updated dictionary, display all unlocked items
        for (string item : fullList) {
            if (map[item]["unlocked"] == "true") { displayList.push_back(item); }
            else {
                displayList.push_back(colourText("Undiscovered #" + to_string(unknownItemCounter), RED));
                ++unknownItemCounter;
            }
        }
        if (listType != "Change Region") { displayList.push_back("None"); }

        listItems = displayList;
        listLength = listItems.size();
        while (true) {
            displayTitle();
            for (int i = 0; i < qPressInfo[0].size(); ++i) {
                if (qPressInfo[0][i] == listType) {
                    cout << qPressInfo[1][i] << "\n";
                }
            }
            cout << textSeparator;
            selectedItem = displayItems();
            if (selectedItem != "null") { break; }
        }
        return selectedItem;
    }
}

int List::selectionIndexUpdate(int currentIndex, int arrayLength, char input)
{
    int newArrayLength = arrayLength - 1;

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

string List::itemSelectionEffect()
{
    string selectedItem = "";
    if (listType == "Main Menu") {
        clearScreen();
        /*newlySelectedItem = displayListSetup(currentSelectedItem);
        if (currentSelectedItem == "Change Keepsake") { setKeepsake = newlySelectedItem; }
        else if (currentSelectedItem == "Change Old Soul") { setOldSoul = newlySelectedItem; }
        else if (currentSelectedItem == "Change Region") { setArea = newlySelectedItem; }
        else if (currentSelectedItem == "Start") { gameStarted = true; }*/
        if (currentSelectedItem == "Change Keepsake" || currentSelectedItem == "Change Old Soul" || currentSelectedItem == "Change Region") {
            selectedItem = selectSublist(currentSelectedItem);
        }
        else {
            selectedItem = "start";
        }
    }
    else if (listType == "Change Keepsake") {
        clearScreen();
        selectedItem = currentSelectedItem;
    }

    return selectedItem;
}

string List::selectSublist(string newList)
{
    string selectedItem = "";
    if (newList == "Change Keepsake") {
        List keepsakesList(newList, keepsakesVector, true);
        selectedItem = keepsakesList.displayImportedItems("keepsakes.json", keepsakesMap);
        selectedItem += "|keepsake";
    }
    /*else if (listType == "Change Old Soul") { selectedItem = displayList(listType, "oldsouls.json", oldSoulsList); }
    else if (listType == "Change Region") { selectedItem = displayList(listType, "regions.json", regionsList); }*/
    return selectedItem;
}
