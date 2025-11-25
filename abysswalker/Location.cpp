#include "Location.h"
#include "Misc.h"
#include "List.h"
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <conio.h>
using namespace std;

Location::Location()
{
	locationCoords = make_tuple(0, 0);
	sectorCoords = make_tuple(0, 0);
	locationType = "None";
	active = false;
}

Location::Location(tuple<int, int> coords, tuple<int, int> sector)
{
	locationCoords = coords;
	sectorCoords = sector;
	active = true;
}

void Location::pause(int milliseconds)
{
	this_thread::sleep_for(chrono::milliseconds(1000));
}

// RUINS
Ruins::Ruins()
	: Location()
{
	locationType = "Ruins";
	icon = "[R]";
	finalItemsList = {};
	active = true;
}

Ruins::Ruins(tuple<int, int> coords, tuple<int, int> sector)
	: Location(coords, sector)
{
	locationType = "Ruins";
	icon = "[R]";
	finalItemsList = {};
	active = true;

	vector<string> fullItemsList = allItemsByType[0];
	vector<string> selectedItems = {};
	int newItemIndex = 0;
	for (int i = 0; i < 3; i++) {
		while (true) {
			newItemIndex = rand() % fullItemsList.size();

			if (find(selectedItems.begin(), selectedItems.end(), fullItemsList[newItemIndex]) != selectedItems.end()) {
				continue;
			}
			else {
				finalItemsList.push_back(fullItemsList[newItemIndex]);
				selectedItems.push_back(fullItemsList[newItemIndex]);
				break;
			}
		}
	}

	itemsOptions = List("Location", finalItemsList);
}

string Ruins::interactStart(bool locationDisplaySelected, function<void()> infoCallback)
{
	string reset_colour = "";
	if (locationDisplaySelected) { reset_colour = RESET; }
	else { reset_colour = INACTIVE; }

	clearScreen();
	displayTitle();
	cout << colourText(" You have arrived at a ", reset_colour) << colourText(locationType, YELLOW, reset_colour) << ".\n";
	cout << " You explore the derelict site, coming across a few items of use. Choose one to take with you. \n\n";

	if (locationDisplaySelected) {
		string returnedItem = itemsOptions.displayItems(infoCallback);
		if (returnedItem != "null") {active = false;}
		return returnedItem;
	}
	else {
		for (string item : finalItemsList) {
			cout << "   " << colourText(item, INACTIVE) << "\n";
		}
		cout << textSeparator;
	}
}

// CRYSTAL LIZARD
Crystal::Crystal()
	: Location()
{
	locationType = "Crystal Lizard";
	icon = "[C]";
	finalItemsList = {};
	active = true;
}

Crystal::Crystal(tuple<int, int> coords, tuple<int, int> sector)
	: Location(coords, sector)
{
	locationType = "Crystal Lizard";
	icon = "[C]";
	finalItemsList = {};
	active = true;

	vector<string> fullItemsList = allItemsByType[2];
	vector<string> selectedItems = {};
	int newItemIndex = 0;
	for (int i = 0; i < 2; i++) {
		while (true) {
			newItemIndex = rand() % fullItemsList.size();

			if (find(selectedItems.begin(), selectedItems.end(), fullItemsList[newItemIndex]) != selectedItems.end()) {
				continue;
			}
			else {
				finalItemsList.push_back(fullItemsList[newItemIndex]);
				selectedItems.push_back(fullItemsList[newItemIndex]);
				break;
			}
		}
	}

	itemsOptions = List("Location", finalItemsList);
}

string Crystal::interactStart(bool locationDisplaySelected, function<void()> infoCallback)
{
	string reset_colour = "";
	if (locationDisplaySelected) { reset_colour = RESET; }
	else { reset_colour = INACTIVE; }

	clearScreen();
	displayTitle();
	cout << colourText(" You suddenly come across a small ", reset_colour) << colourText(locationType, YELLOW, reset_colour) << ".\n";
	cout << " Its back houses several gems and minerals. If you're stealthy, you may be able to slice one off for yourself. \n\n";

	if (locationDisplaySelected) {
		string returnedItem = itemsOptions.displayItems(infoCallback);
		if (returnedItem != "null") { active = false; }
		return returnedItem;
	}
	else {
		for (string item : finalItemsList) {
			cout << "   " << colourText(item, INACTIVE) << "\n";
		}
		cout << textSeparator;
	}
}

// Warrior's Grave
Grave::Grave()
	: Location()
{
	locationType = "Warrior's Grave";
	icon = "[G]";
	finalItemsList = {};
	active = true;
}

Grave::Grave(tuple<int, int> coords, tuple<int, int> sector)
	: Location(coords, sector)
{
	locationType = "Warrior's Grave";
	icon = "[G]";
	finalItemsList = {};
	active = true;

	vector<string> fullItemsList = allItemsByType[3];
	vector<string> selectedItems = {};
	int newItemIndex = 0;
	for (int i = 0; i < 3; i++) {
		while (true) {
			newItemIndex = rand() % fullItemsList.size();

			if (find(selectedItems.begin(), selectedItems.end(), fullItemsList[newItemIndex]) != selectedItems.end()) {
				continue;
			}
			else {
				finalItemsList.push_back(fullItemsList[newItemIndex]);
				selectedItems.push_back(fullItemsList[newItemIndex]);
				break;
			}
		}
	}

	itemsOptions = List("Location", finalItemsList);
}

string Grave::interactStart(bool locationDisplaySelected, function<void()> infoCallback)
{
	string reset_colour = "";
	if (locationDisplaySelected) { reset_colour = RESET; }
	else { reset_colour = INACTIVE; }

	clearScreen();
	displayTitle();
	cout << colourText(" You arrive at a decrepit ", reset_colour) << colourText(locationType, YELLOW, reset_colour) << ".\n";
	cout << " The remains of a once-great hero lay here. Their armour is broken, but there may still be a weapon of use. \n\n";

	if (locationDisplaySelected) {
		string returnedItem = itemsOptions.displayItems(infoCallback);
		if (returnedItem != "null") { active = false; }
		return returnedItem;
	}
	else {
		for (string item : finalItemsList) {
			cout << "   " << colourText(item, INACTIVE) << "\n";
		}
		cout << textSeparator;
	}
}