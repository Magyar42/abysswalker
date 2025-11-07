#include "List.h"
#include "Misc.h"
#include <string>
#include <conio.h>
#include <iostream>
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
            string testValue = itemSelectionEffect();
            return testValue;
            break;
        }
    }

    return "no burger";
}

string List::displayImportedItems(string filename)
{
    return string();
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
    if (listType == "Main Menu") {
        clearScreen();
        return currentSelectedItem;
        /*newlySelectedItem = displayListSetup(currentSelectedItem);
        if (currentSelectedItem == "Change Keepsake") { setKeepsake = newlySelectedItem; }
        else if (currentSelectedItem == "Change Old Soul") { setOldSoul = newlySelectedItem; }
        else if (currentSelectedItem == "Change Region") { setArea = newlySelectedItem; }
        else if (currentSelectedItem == "Start") { gameStarted = true; }*/
    }
}

string List::selectSublist(string newList)
{
    // TODO FINISH TS!!
    vector<string>keepsakesTest = { "Life Ring", "Fire Gem", "Black Firebombs", "Test Item"};
    string selectedItem = "";
    if (newList == "Keepsakes") {
        // selectedItem = displayList(listType, "keepsakes.json", keepsakesList);
        List keepsakesList(newList, keepsakesTest, true);
        selectedItem = keepsakesList.displayImportedItems("keepsakes.json");
    }
    /*else if (listType == "Change Old Soul") { selectedItem = displayList(listType, "oldsouls.json", oldSoulsList); }
    else if (listType == "Change Region") { selectedItem = displayList(listType, "regions.json", regionsList); }*/
    return selectedItem;
}
