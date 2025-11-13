#include "Level.h"
#include "Misc.h"
#include "List.h"
#include <tuple>
#include <iostream>
#include <fstream>
using namespace std;

Level::Level(string area, string keepsake, string oldSoul)
{
	setArea = area;
	setKeepsake = keepsake;
	setOldSoul = oldSoul;

	currentAreaDay = 1;
	currentAreaDayOrNight = "Day";
	currentAreaTime = "9:00";
	gameStarted = false;
	mapSelected = true;
	playerCoords = make_tuple(4, 4);
	playerHP = 10;
	playerATK = 1;
	playerDEF = 0;
	playerSPD = 0;
	playerWeapon = "None";
	playerInventory = { "Empty", "Empty", "Empty", "Empty" };

	currentBoss = selectBoss(setArea, currentAreaDay);
	playerSetup();
}

string Level::selectBoss(string area, int day)
{
	string currentBoss = "";
	if (area == "Darkroot Depths") {
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

void Level::playerSetup()
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

tuple<int, int> Level::displayMap(vector<vector<string>> map, string reset_colour)
{
	tuple<int, int> mapSize;
	mapSize = make_tuple(map[0].size(), map.size());
	map[get<0>(playerCoords)][get<1>(playerCoords)] = colourText(PLAYER_TILE, BLUE, reset_colour);

	for (const auto& row : map) {
		cout << " ";
		for (const auto& tile : row) {
			cout << tile;
		}
		cout << endl;
	}

	return mapSize;
}

void Level::updateMovement(char input, tuple<int, int> mapSize)
{
	tuple<int, int> newCoords = playerCoords;
	// copy(playerCoords, playerCoords + 2, newCoords);
	// cout << newCoords[0] << " | " << newCoords[1];

	if (input == 'w') { --get<0>(newCoords); }
	else if (input == 's') { ++get<0>(newCoords); }
	else if (input == 'a') { --get<1>(newCoords); }
	else if (input == 'd') { ++get<1>(newCoords); }

	if (get<0>(newCoords) >= 0 && get<1>(newCoords) >= 0) {
		if (get<0>(newCoords) < get<0>(mapSize) && get<1>(newCoords) < get<1>(mapSize)) {
			if (worldMap[get<0>(newCoords)][get<1>(newCoords)] == OPEN_TILE) {
				get<0>(playerCoords) = get<0>(newCoords);
				get<1>(playerCoords) = get<1>(newCoords);
			}
		}
	}
}

void Level::displayWorld()
{
	tuple<int, int> mapSize;
	string dayInfo = currentAreaTime + " [" + currentAreaDayOrNight + " " + to_string(currentAreaDay) + "]";
	string reset_colour = "";

	if (mapSelected) { reset_colour = RESET; }
	else { reset_colour = INACTIVE; }

	cout << colourText(" Next Boss: ", BLUE, reset_colour) << currentBoss << " [Q]\n";
	cout << colourText(" Current Time: ", BLUE, reset_colour) << dayInfo << "\n";
	cout << colourText(" HP: ", GREEN, reset_colour) << playerHP << " | " << colourText("ATK: ", RED, reset_colour) << playerATK << " | " << colourText("DEF: ", BLUE, reset_colour) << playerDEF << " | " << colourText("SPD: ", YELLOW, reset_colour) << playerSPD << "\n\n";

	mapSize = displayMap(worldMap, reset_colour);
	cout << textSeparator;
}

void Level::displayInventory()
{
	cout << "test";
	// todo: display inv stuff but just grey innit
}

vector<string> Level::initInvDisplay()
{
	int index = 0;
	string displayString = "";
	vector<string> displayList = {};
	int emptyItemCounter = 1;

	displayList.push_back(playerWeapon);
	for (string item : playerInventory) {
		if (item != "Empty") { displayList.push_back(item); }
		else {
			displayList.push_back("Empty #" + to_string(emptyItemCounter));
			++emptyItemCounter;
		}
	}

	return displayList;
}

void Level::display()
{
	vector<string> playerInvDisplay;
	playerInvDisplay = initInvDisplay();
	List playerInv("Inventory", playerInvDisplay);

	while (true) {
		displayWorld();

		if (mapSelected) { playerInv.displayItems(); }
		else { displayInventory(); }

		/*string test;
		cin >> test;*/
	}
}
