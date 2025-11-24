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
	active = true;
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
}

Ruins::Ruins(tuple<int, int> coords, tuple<int, int> sector)
	: Location(coords, sector)
{
	locationType = "Ruins";
	icon = "[R]";
	finalItemsList = {};

	vector<string> fullItemsList = allItemsVector;
	for (int i = 0; i < 3; i++) {
		int newItemIndex = rand() % fullItemsList.size();
		finalItemsList.push_back(fullItemsList[newItemIndex]);
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