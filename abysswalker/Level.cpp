#include "Level.h"
#include "Misc.h"
#include "List.h"
#include "Battle.h"
#include "Location.h"
#include <tuple>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <algorithm>
#include <ctime>
using namespace std;

Level::Level(string area, string keepsake, string oldSoul)
{
	setArea = area;
	setKeepsake = keepsake;
	setOldSoul = oldSoul;

	currentAreaDay = 1;
	currentAreaTimeIndex = 0;
	currentAreaTimeCounter = 0;
	gameStarted = false;
	mapSelected = true;
	locationActive = false;
	currentLocation = { make_tuple(0, 0), make_tuple(0, 0) };
	endGame = 0;
	playerCoords = make_tuple(7, 4);
	mapSectorCoords = make_tuple(2, 2);
	playerHPBase = 10;
	playerMaxHPBase = 10;
	playerATKBase = 1;
	playerDEFBase = 0;
	playerSPDBase = 0;
	playerHP = 10;
	playerMaxHP= 10;
	playerATK = 1;
	playerDEF = 0;
	playerSPD = 0;
	playerSouls = 100;
	playerWeapon = "None";
	playerWeaponDetails = {};
	playerInventory = { "Empty", "Empty", "Empty", "Empty" };
	playerTilePrev = "";

	initEnemies();
	initWorldMap();
	currentBoss = selectBoss(setArea, currentAreaDay);
	playerSetup();
}

// MAP //
void Level::initWorldMap()
{
	string currentLine = "";
	int tileIndex = 0;
	int rowIndex = 0;
	string currentLineResult = "";
	vector<string> resultVector;

	int numSectorsPerRow = 5;
	int numSectors = 25;
	int sectorX = 0;
	int sectorY = 0;
	int sectorPerRowCounter = 0;

	for (int i = 0; i < numSectors; i++) {
		sectorPerRowCounter++;
		string sectorFile = "sectors/abysswalker_level" + to_string(i) + ".csv";
		ifstream levelFile(sectorFile);

		rowIndex = 0;
		resultVector.clear();
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
				else if (tileValue == '2') {
					currentLineResult += "2";
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
		levelSectors.back().sectorCoords = make_tuple(sectorX, sectorY);

		setEnemies(levelSectors.back());

		sectorX++;
		if (sectorPerRowCounter >= numSectorsPerRow) {
			sectorPerRowCounter = 0;
			sectorX = 0;
			sectorY++;
		}
	}
}

void Level::assignSectorToMap(tuple<int, int> numSector)
{
	int rowIndex = 0;
	int tileIndex = 0;

	for (mapSector sector : levelSectors) {
		if (sector.sectorCoords == numSector) {
			for (const auto& row : sector.sectorTileRows) {
				for (char tile : row) {
					if (tile == '0') { displayedSector[rowIndex][tileIndex] = OPEN_TILE; }
					else if (tile == '1') { displayedSector[rowIndex][tileIndex] = CLOSED_TILE; }
					else if (tile == '2') {
						vector<tuple<int, int>> newLocation = { make_tuple(tileIndex, rowIndex), numSector };
						if (find(inactiveLocations.begin(), inactiveLocations.end(), newLocation) != inactiveLocations.end()) {
							displayedSector[rowIndex][tileIndex] = OPEN_TILE;
						}
						else {
							displayedSector[rowIndex][tileIndex] = colourText(LOCATION_TILE, YELLOW);
							currentSectorLocations.push_back(newLocation);
						}
					}
					tileIndex++;
				}
				rowIndex++;
				tileIndex = 0;
			}
		}
	}

	loadSectorEnemies();

}

void Level::loadNewSector(tuple<int, int> coords)
{
	int xOffsetSector = 0;
	int yOffsetSector = 0;
	int charPosX = get<0>(playerCoords);
	int charPosY = get<1>(playerCoords);

	switch (get<1>(coords)) {
	case 10:
		yOffsetSector = 1;
		charPosY = 0;
		break;
	case -1:
		yOffsetSector = -1;
		charPosY = 9;
		break;
	}
	switch (get<0>(coords)) {
	case 16:
		xOffsetSector = 1;
		charPosX = 0;
		break;
	case -1:
		xOffsetSector = -1;
		charPosX = 15;
		break;
	}

	int newCoordX = get<0>(mapSectorCoords) + xOffsetSector;
	int newCoordY = get<1>(mapSectorCoords) + yOffsetSector;
	mapSectorCoords = make_tuple(newCoordX, newCoordY);
	playerCoords = make_tuple(charPosX, charPosY);

	// loadSectorEnemies();
}

// ENEMIES //
void Level::initEnemies()
{
	string currentLine = "";
	string currentItem = "";

	ifstream file("data/enemies.txt");
	while (getline(file, currentLine)) {
		if (currentLine.empty()) continue;

		if (currentLine.find("[") != string::npos) {
			currentItem = currentLine.substr(1, currentLine.length() - 2);
			
			if (currentItem == "Titanite Demon" || currentItem == "Moonlight Butterfly" || currentItem == "Hydra of the Basin") {
				bossesVector.push_back(currentItem);
			}
			else {
				enemiesVector.push_back(currentItem);
			}
		}

		else if (currentItem != "" && currentLine.find(":") != string::npos) {
			size_t colonPosition = currentLine.find(":");
			string key = currentLine.substr(0, colonPosition);
			string value = currentLine.substr(colonPosition + 2);
			
			if (currentItem == "Titanite Demon" || currentItem == "Moonlight Butterfly" || currentItem == "Hydra of the Basin") {
				bossesMap[currentItem][key] = value;
			}
			else {
				enemiesMap[currentItem][key] = value;
			}
		}

	}
	file.close();
}

void Level::setEnemies(mapSector sector)
{
	int enemySpawnLimit = 3;
	int numEnemiesToSpawn = rand() % (enemySpawnLimit + 1);
	numEnemiesToSpawn += 1;

	for (int i = 0; i < numEnemiesToSpawn; i++) {
		int enemyTypeIndex = rand() % enemiesVector.size();

		while (true) {
			int spawnX = rand() % sector.sectorTileRows[0].length();
			int spawnY = rand() % sector.sectorTileRows.size();

			if (sector.sectorTileRows[spawnY][spawnX] == '0') {
				tuple<int, int> enemyPosInSector = make_tuple(spawnX, spawnY);
				allSectorEnemies.emplace_back(make_unique<Enemy>(enemiesVector[enemyTypeIndex], sector.sectorCoords, enemyPosInSector));
				break;
			}
		}
	}
}

void Level::loadSectorEnemies()
{
	currentSectorEnemies.clear();
	for (const auto& uptr : allSectorEnemies) {
		if (uptr->sectorPos == mapSectorCoords) {
			currentSectorEnemies.push_back(uptr.get());
		}
	}
}

// GAME SETUP //
string Level::selectBoss(string area, int day)
{
	string currentBoss = "";
	if (area == "Darkroot Depths") {
		switch (day) {
		case 1:
			currentBoss = "Titanite Demon";
			break;
		case 4:
			currentBoss = "Moonlight Butterfly";
			break;
		case 7:
			currentBoss = "Hydra of the Basin";
			break;
		}
	}
	return currentBoss;
}

void Level::playerSetup()
{
	if (setOldSoul == "Soul of the Wolf Knight") {
		playerHPBase = 10;
		playerMaxHPBase = 10;
		playerATKBase = 2;
		playerDEFBase = 0;
		playerSPDBase = 1;
		setBaseStats();
		resetWeapon("Wolf Knight Greatsword");
	}

	playerInventory.at(0) = setKeepsake;
	updatePlayerStats(true);
}

void Level::setBaseStats(bool resetHP)
{
	playerMaxHP = playerMaxHPBase;
	playerATK = playerATKBase;
	playerDEF = playerDEFBase;
	playerSPD = playerSPDBase;
	if (resetHP) { playerHP = playerMaxHP; }
}

// UPDATES //
void Level::updateMovement(char input)
{
	displayedSector[get<1>(playerCoords)][get<0>(playerCoords)] = playerTilePrev;

	tuple<int, int> mapSize = make_tuple(16, 10);

	tuple<int, int> newCoords = playerCoords;
	if (input == 'w') { --get<1>(newCoords); }
	else if (input == 's') { ++get<1>(newCoords); }
	else if (input == 'a') { --get<0>(newCoords); }
	else if (input == 'd') { ++get<0>(newCoords); }

	if (get<0>(newCoords) >= 0 && get<1>(newCoords) >= 0 && get<0>(newCoords) < get<0>(mapSize) && get<1>(newCoords) < get<1>(mapSize)) {
		if (displayedSector[get<1>(newCoords)][get<0>(newCoords)] != CLOSED_TILE) {
			get<0>(playerCoords) = get<0>(newCoords);
			get<1>(playerCoords) = get<1>(newCoords);
		}
	}
	else {
		loadNewSector(newCoords);
	}

	updateTime();
	checkPlayerLocation();
	updatePlayerStats();
}

void Level::updateTime()
{
	currentAreaTimeCounter++;
	if (currentAreaTimeCounter >= charMovementPerTimeCounter) {
		currentAreaTimeCounter = 0;
		currentAreaTimeIndex++;
		if (currentAreaTimeIndex >= 7) {
			if (currentAreaDay % 3 == 0) {
				clearScreen();
				startBossCombat(currentBoss);
			}

			currentAreaTimeIndex = 0;
			currentAreaDay++;
		}
	}
}

void Level::checkPlayerLocation()
{
	for (auto* enemy : currentSectorEnemies) {
		int enemyX = get<0>(enemy->mapPos);
		int enemyY = get<1>(enemy->mapPos);

		if (enemyX == get<0>(playerCoords) && enemyY == get<1>(playerCoords)) {
			clearScreen();
			startCombat(*enemy);
			break;
		}
	}

	for (auto location : currentSectorLocations) {
		int locationX = get<0>(location[0]);
		int locationY = get<1>(location[0]);
		int locationXSector = get<0>(location[1]);
		int locationYSector = get<1>(location[1]);

		if (locationX == get<0>(playerCoords) && locationY == get<1>(playerCoords) && locationXSector == get<0>(mapSectorCoords) && locationYSector == get<1>(mapSectorCoords)) {
			if (find(inactiveLocations.begin(), inactiveLocations.end(), location) == inactiveLocations.end()) {
				currentLocation = location;
				locationActive = true;
				break;
			}
		}
	}
}

void Level::resetWeapon(string newWeapon)
{
	playerWeaponDetails = {
		{"Name", "None"},
		{"HP", "0"},
		{"ATK", "0"},
		{"DEF", "0"},
		{"SPD", "0"},
		{ "Effect", "None"},
		{ "Gem", "None"},
		{ "NumUpgrades", "0"}
	};

	if (newWeapon != "null") {
		if (find(allItemsByType[3].begin(), allItemsByType[3].end(), newWeapon) != allItemsByType[3].end()) {
			playerWeapon = newWeapon;
			playerWeaponDetails["Name"] = newWeapon;
			playerWeaponDetails["Effect"] = allItemsMap[newWeapon]["desc"];

			string statsString = allItemsMap[newWeapon]["stats"];
			vector<string> itemStats = {};
			bool severalStats = false;

			while (statsString.find("|") != string::npos) {
				severalStats = true;
				string stat = statsString.substr(0, statsString.find("|") - 1);
				statsString.erase(0, statsString.find("|") + 2);

				itemStats.push_back(stat);
			}
			if (!severalStats) {
				itemStats.push_back(statsString);
			}
			else {
				string lastStat = statsString.substr(statsString.find_last_of("|") + 1);
				itemStats.push_back(lastStat);
			}

			for (string stat : itemStats) {
				string statChange = stat.substr(0, stat.find(" "));
				string statName = stat.substr(stat.find(" ") + 1);

				playerWeaponDetails[statName] = statChange;
			}
		}
	}
	else {
		playerWeapon = "None";
	}
}

void Level::updatePlayerStats(bool setHP)
{
	setBaseStats();
	for (string item : playerInventory) {
		if (item != "Empty" && allItemsMap[item]["stats"] != "null") {
			string statsString = allItemsMap[item]["stats"];
			vector<string> itemStats = {};
			bool severalStats = false;

			while (statsString.find("|") != string::npos) {
				severalStats = true;
				string stat = statsString.substr(0, statsString.find("|") - 1);
				statsString.erase(0, statsString.find("|") + 2);

				itemStats.push_back(stat);
			}
			if (!severalStats) {
				itemStats.push_back(statsString);
			}
			else {
				string lastStat = statsString.substr(statsString.find_last_of("|") + 1);
				itemStats.push_back(lastStat);
			}

			for (string stat : itemStats) {
				string statChange = stat.substr(0, stat.find(" "));
				string statName = stat.substr(stat.find(" ") + 1);
				
				if (statName == "HP") {
					playerMaxHP += stoi(statChange);
				}
				else if (statName == "ATK") {
					playerATK += stoi(statChange);
				}
				else if (statName == "DEF") {
					playerDEF += stoi(statChange);
				}
				else if (statName == "SPD") {
					playerSPD += stoi(statChange);
				}
			}
		}
	}

	playerMaxHP += stoi(playerWeaponDetails["HP"]);
	playerATK += stoi(playerWeaponDetails["ATK"]);
	playerDEF += stoi(playerWeaponDetails["DEF"]);
	playerSPD += stoi(playerWeaponDetails["SPD"]);

	if (setHP) { playerHP = playerMaxHP; }
	if (playerMaxHP < 0) { playerMaxHP = 0; }
	if (playerHP < 0) { playerHP = 0; }
	if (playerATK < 0) { playerATK = 0; }
	if (playerDEF < 0) { playerDEF = 0; }
	if (playerSPD < 0) { playerSPD = 0; }
}

vector<string> Level::updateInventory()
{
	vector<string> playerInvDisplay = initInvDisplay();
	return playerInvDisplay;
}

// DISPLAY //
void Level::displayWorld()
{
	displayTitle();
	string dayInfo = "Day " + to_string(currentAreaDay) + ", " + timesOfDay[currentAreaTimeIndex];
	string reset_colour = "";

	if (mapSelected) { reset_colour = RESET; }
	else { reset_colour = INACTIVE; }

	cout << colourText(" Next Boss: ", BLUE, reset_colour) << currentBoss << " [Q]\n";
	cout << colourText(" Current Time: ", BLUE, reset_colour) << dayInfo << "\n\n";

	displayMap(reset_colour);
	cout << textSeparator;
}

void Level::displayMap(string reset_colour)
{
	assignSectorToMap(mapSectorCoords);

	playerTilePrev = displayedSector[get<1>(playerCoords)][get<0>(playerCoords)];
	displayedSector[get<1>(playerCoords)][get<0>(playerCoords)] = colourText(PLAYER_TILE, BLUE, reset_colour);

	for (auto* enemy : currentSectorEnemies) {
		/*if (currentAreaTimeIndex >= 4) {
			enemy->updateMovement(displayedSector);
		}*/
		int enemyX = get<0>(enemy->mapPos);
		int enemyY = get<1>(enemy->mapPos);
		displayedSector[enemyY][enemyX] = colourText(enemy->icon, RED, reset_colour);
	}

	for (const auto& row : displayedSector) {
		cout << " ";
		for (const auto& tile : row) {
			cout << tile;
		}
		cout << endl;
	}
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

	cout << colourText(" HP: ", GREEN, reset_colour) << playerHP << "/" << playerMaxHP << " | " << colourText("ATK: ", RED, reset_colour) << playerATK << " | " << colourText("DEF: ", BLUE, reset_colour) << playerDEF << " | " << colourText("SPD: ", YELLOW, reset_colour) << playerSPD << "\n";
	cout << " Souls: " << playerSouls << "\n\n";
}

void Level::displayInfoAndInventory() // Passed into Location interactStart
{
	displayPlayerInfo();
	vector<string> playerInvDisplay = initInvDisplay();
	displayInventory(playerInvDisplay);
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

// INPUT //
void Level::getPlayerInput()
{
	string reset_colour = "";
	if (mapSelected) { reset_colour = INACTIVE; }
	else { reset_colour = RESET; }

	while (locationActive) {
		char input = _getch();
		if (input == 'f') {
			clearScreen();
			mapSelected = false;
			break;
		}
		else if (input == 'p') {
			clearScreen();
			locationActive = false;
			break;
		}
	}

	while (mapSelected && !locationActive) {
		char input = _getch();
		if (input == 'q') {
			qPressCheck(currentBoss);
		}
		else if (input == 'w' || input == 'a' || input == 's' || input == 'd') {
			updateMovement(input);
			clearScreen();
			break;
		}
		else if (input == 'f') {
			clearScreen();
			mapSelected = false;
			break;
		}
	}
}

// COMBAT //
void Level::startCombat(Enemy& enemy)
{
	Battle battleInstance(playerHP, playerMaxHP, playerATK, playerDEF, playerSPD, playerInventory, playerWeapon);
	int numSouls = battleInstance.startBattle(enemy);
	_getch();

	if (!battleInstance.playerWon) {
		endGame = 1;
	}
	else {
		auto it = remove_if(allSectorEnemies.begin(), allSectorEnemies.end(),
			[&enemy](const unique_ptr<Enemy>& e) {
				return e->sectorPos == enemy.sectorPos && e->mapPos == enemy.mapPos;
			});
		allSectorEnemies.erase(it, allSectorEnemies.end());
		loadSectorEnemies();

		playerHP = battleInstance.playerHP;
		playerSouls += numSouls;
	}
}

void Level::startBossCombat(string bossName)
{
	Battle battleInstance(playerHP, playerMaxHP, playerATK, playerDEF, playerSPD, playerInventory, playerWeapon);
	Enemy bossEnemy(bossName, make_tuple(-1, -1), make_tuple(-1, -1), true);
	battleInstance.startBattle(bossEnemy, true);
	_getch();

	if (!battleInstance.playerWon) {
		endGame = 1;
	}
	else {
		playerHP = battleInstance.playerHP;
		playerSouls += 100;
		if (bossEnemy.enemyName == "Hydra of the Basin") {
			endGame = 2;
		}
		else {
			currentBoss = selectBoss(setArea, currentAreaDay);

			// Each defeated boss unlocks 2 new inventory slots
			playerInventory.push_back("Empty");
			playerInventory.push_back("Empty");
		}
	}
}

// MAIN LOOP //
void Level::display()
{
	srand(time(0));
	vector<string> playerInvDisplay = initInvDisplay();
	List playerInv("Inventory", playerInvDisplay);

	Location* locationInstance = nullptr;

	while (true) {
		if (!locationActive) {
			if (locationInstance) {
				delete locationInstance;
				locationInstance = nullptr;
			}

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
		else {
			if (locationInstance == nullptr) {
				int locationType = rand() % 5;
				switch (locationType) {
					case 0:
						locationInstance = new Ruins(currentLocation[0], currentLocation[1]);
						break;
					case 1:
						locationInstance = new Crystal(currentLocation[0], currentLocation[1]);
						break;
					case 2:
						locationInstance = new Grave(currentLocation[0], currentLocation[1]);
						break;
					case 3:
						locationInstance = new Bonfire(currentLocation[0], currentLocation[1]);
						break;
					case 4:
						locationInstance = new Firekeeper(currentLocation[0], currentLocation[1]);
						break;
				}
			}

			string returnValue = locationInstance->interactStart(mapSelected, bind(&Level::displayInfoAndInventory, this));

			if (mapSelected) {
				if (returnValue == "true") { mapSelected = false; }
				else if (returnValue != "null") {
					locationActive = false;
					inactiveLocations.push_back(currentLocation);

					if (locationInstance->locationType == "Warrior's Grave") {
						resetWeapon(returnValue);
						updatePlayerStats();
					}
					else if (locationInstance->locationType == "Crystal Lizard" || locationInstance->locationType == "Ruins") {
						bool itemAdded = false;
						for (string& invItem : playerInventory) {
							if (invItem == "Empty") {
								invItem = returnValue;
								itemAdded = true;
								break;
							}
						}

						if (!itemAdded) {
							vector<string> discardList = {};
							for (int i = 0; i < playerInvDisplay.size(); i++) {
								if (i == 0) { continue; }
								else { discardList.push_back(playerInvDisplay[i]); }
							}
							discardList.push_back(returnValue);
							List playerDiscardList("Discard", discardList);

							while (true) {
								clearScreen();
								displayTitle();
								cout << " Inventory full! Could not add " << colourText(returnValue, YELLOW) << ". Please choose any item to discard.\n\n";

								string invItemToRemove = playerDiscardList.displayItems();
								if (invItemToRemove == returnValue) {
									break;
								}
								else {
									for (string& invItem : playerInventory) {
										if (invItem == invItemToRemove) {
											invItem = returnValue;
											itemAdded = true;
											break;
										}
									}
									if (itemAdded) { break; }
								}
							}
						}
						updatePlayerStats();
					}
					else if (locationInstance->locationType == "Bonfire") {
						playerHP += 10;
						if (playerHP > playerMaxHP) { playerHP = playerMaxHP; }

						for (int i = 0; i < 4; i++) {
							currentAreaTimeIndex++;
							if (currentAreaTimeIndex >= 7) {
								if (currentAreaDay % 3 == 0) {
									clearScreen();
									startBossCombat(currentBoss);
								}

								currentAreaTimeIndex = 0;
								currentAreaDay++;
							}
						}
					}
					else if (locationInstance->locationType == "Firekeeper") {
						if (playerSouls >= 300) {
							if (returnValue == "HP") { playerMaxHPBase += 1; }
							if (returnValue == "ATK") { playerATKBase += 1; }
							if (returnValue == "DEF") { playerDEFBase += 1; }
							if (returnValue == "SPD") { playerSPDBase += 1; }
							playerSouls -= 300;
							updatePlayerStats();
						}
					}

					// todo: when duplicate items exist, make sure to add #1, #2, etc
					playerInvDisplay = updateInventory();
					playerInv = List("Inventory", playerInvDisplay);
				}
				clearScreen();
				continue;
			}
			else {
				displayPlayerInfo();
				string returnValue = playerInv.displayItems();
				if (returnValue == "true") { mapSelected = true; }
				clearScreen();
				continue;
			}
		}

		if (endGame==1) {
			clearScreen();
			break;
		}
	}

	if (locationInstance) {
		delete locationInstance;
		locationInstance = nullptr;
	}
}
