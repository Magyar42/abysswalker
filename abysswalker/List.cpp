#include "List.h"
#include "Misc.cpp"
#include <string>
using namespace std;

List::List(vector<string> list, bool import)
{
	listItems = list;
	importFromFile = import;

	selectedItemIndex = 0;
	currentSelectedItem = listItems[selectedItemIndex];
	newlySelectedItem = "";
	listLength = listItems.size();
}

// TODO: make sure constructor is run only once, as below should be in a while loop in the MainMenu class
void List::displayItems()
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

	// TODO: add in loop for checking for player input
}
