#include "Level.h"
#include "Misc.h"
#include "List.h"
#include <tuple>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

//struct Level::mapSector {
//	// todo, potentially: for sectorTileRows, use binary? bitwise innit
//	vector<string> sectorTileRows;
//	tuple<int, int> sectorCoords;
//};

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
	playerCoords = make_tuple(1, 1); // todo: add random spawning
	mapSectorCoords = make_tuple(0, 0);
	playerHP = 10;
	playerATK = 1;
	playerDEF = 0;
	playerSPD = 0;
	playerWeapon = "None";
	playerInventory = { "Empty", "Empty", "Empty", "Empty" };
	playerTilePrev = "";

	vector<mapSector> levelSectors;

	initWorldMap();
	currentBoss = selectBoss(setArea, currentAreaDay);
	playerSetup();
}

void Level::initWorldMap()
{
	string currentLine = "";
	int tileIndex = 0;
	int rowIndex = 0;
	string currentLineResult = "";
	vector<string> resultVector;

	for (int i = 0; i < 2; i++) {
		string sectorFile = "abysswalker_level" + to_string(i) + ".csv";
		ifstream levelFile(sectorFile);

		while (getline(levelFile, currentLine)) {
			if (currentLine.empty()) continue;

			for (char tileValue : currentLine) {
				if (tileValue == '1') {
					currentLineResult += "1";
					tileIndex++;
				}
				else if (tileValue == '0') {
					currentLineResult += "0";
					tileIndex++;
				}
			}
			resultVector.push_back(currentLineResult);
			rowIndex++;
			tileIndex = 0;
			currentLineResult = "";
		}
		levelFile.close();

		levelSectors.emplace_back();
		levelSectors.back().sectorTileRows = resultVector;
		levelSectors.back().sectorCoords = make_tuple(i, 0);
	}
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

void Level::assignSectorToMap(tuple<int, int> numSector)
{
	int rowIndex = 0;
	int tileIndex = 0;

	for (mapSector sector : levelSectors) {
		if (sector.sectorCoords == numSector) {
			for (const auto& row : sector.sectorTileRows) {
				for (char tile : row) {
					// todo: below crashes when switching to another map, initial loading works
					if (tile == '0') { displayedSector[rowIndex][tileIndex] = OPEN_TILE; }
					else { displayedSector[rowIndex][tileIndex] = CLOSED_TILE; }
					tileIndex++;
				}
				rowIndex++;
				tileIndex = 0;
			}
		}
	}
	
}

void Level::loadNewSector()
{
	mapSectorCoords = make_tuple(1, 0);
	vector<vector<string>> displayedSector = {
		{"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	};
}

void Level::displayMap(string reset_colour)
{
	cout << 1;
	assignSectorToMap(mapSectorCoords);
	cout << 2;

	playerTilePrev = displayedSector[get<0>(playerCoords)][get<1>(playerCoords)];
	displayedSector[get<0>(playerCoords)][get<1>(playerCoords)] = colourText(PLAYER_TILE, BLUE, reset_colour);

	for (const auto& row : displayedSector) {
		cout << " ";
		for (const auto& tile : row) {
			cout << tile;
		}
		cout << endl;
	}
}

void Level::updateMovement(char input)
{
	displayedSector[get<0>(playerCoords)][get<1>(playerCoords)] = playerTilePrev;

	tuple<int, int> mapSize = make_tuple(10, 16);

	tuple<int, int> newCoords = playerCoords;
	if (input == 'w') { --get<0>(newCoords); }
	else if (input == 's') { ++get<0>(newCoords); }
	else if (input == 'a') { --get<1>(newCoords); }
	else if (input == 'd') { ++get<1>(newCoords); }

	if (get<0>(newCoords) >= 0 && get<1>(newCoords) >= 0) {
		if (get<0>(newCoords) < get<0>(mapSize) && get<1>(newCoords) < get<1>(mapSize)) {
			if (displayedSector[get<0>(newCoords)][get<1>(newCoords)] == OPEN_TILE) {
				get<0>(playerCoords) = get<0>(newCoords);
				get<1>(playerCoords) = get<1>(newCoords);
			}
			//else { loadNewSector(); }
		}
		//else { loadNewSector(); }
	}
}

void Level::displayWorld()
{
	string dayInfo = currentAreaTime + " [" + currentAreaDayOrNight + " " + to_string(currentAreaDay) + "]";
	string reset_colour = "";

	if (mapSelected) { reset_colour = RESET; }
	else { reset_colour = INACTIVE; }

	cout << colourText(" Next Boss: ", BLUE, reset_colour) << currentBoss << " [Q]\n";
	cout << colourText(" Current Time: ", BLUE, reset_colour) << dayInfo << "\n\n";

	displayMap(reset_colour);
	cout << textSeparator;
}

void Level::displayInventory(vector<string> inventory)
{
	for (string item : inventory) {
		cout << "   " << colourText(item, INACTIVE) << "\n";
	}
}

void Level::displayPlayerInfo()
{
	string reset_colour = "";
	if (mapSelected) { reset_colour = INACTIVE; }
	else { reset_colour = RESET; }

	cout << colourText(" HP: ", GREEN, reset_colour) << playerHP << " | " << colourText("ATK: ", RED, reset_colour) << playerATK << " | " << colourText("DEF: ", BLUE, reset_colour) << playerDEF << " | " << colourText("SPD: ", YELLOW, reset_colour) << playerSPD << "\n\n";
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

void Level::getPlayerInput()
{
	while (mapSelected) {
		char input = _getch();
		if (input == 'q') {
			qPressCheck(currentBoss);
		}
		else if (input == 'w' || input == 'a' || input == 's' || input == 'd') {
			updateMovement(input);
			clearScreen();
			break;
		}
		else if (input == 'e') {
			clearScreen();
			mapSelected = false;
			break;
		}
		else if (input == 'p') {
			clearScreen();
			loadNewSector();
			break;
		}
	}
}

void Level::display()
{
	vector<string> playerInvDisplay;
	playerInvDisplay = initInvDisplay();
	List playerInv("Inventory", playerInvDisplay);

	while (true) {
		displayWorld();

		displayPlayerInfo();
		if (!mapSelected) {
			string returnValue = playerInv.displayItems();
			if (returnValue == "true") { mapSelected = true; }
			clearScreen();
			continue;
		}
		else { displayInventory(playerInvDisplay); }

		getPlayerInput();
	}
}
