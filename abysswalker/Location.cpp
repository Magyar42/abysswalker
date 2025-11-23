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
}

Location::Location(tuple<int, int> coords, tuple<int, int> sector)
{
	locationCoords = coords;
	sectorCoords = sector;
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

	vector<string> fullItemsList = keepsakesVector;
	for (int i = 0; i < 3; i++) {
		int newItemIndex = rand() % fullItemsList.size();
		finalItemsList.push_back(fullItemsList[newItemIndex]);
	}

	itemsOptions = List("Location", finalItemsList);
}

string Ruins::interactStart(bool locationDisplaySelected, function<void()> infoCallback)
{
	clearScreen();
	displayTitle();
	cout << " You have arrived at a " << colourText(locationType, YELLOW) << ".\n\n";
	cout << " You explore the site, coming across a few items of use. Choose one to take with you: \n";

	if (locationDisplaySelected) {
		string returnedItem = itemsOptions.displayItems(infoCallback);
		return returnedItem;
	}
	else {
		for (string item : finalItemsList) {
			cout << "   " << colourText(item, INACTIVE) << "\n";
		}
		cout << textSeparator;
	}
}