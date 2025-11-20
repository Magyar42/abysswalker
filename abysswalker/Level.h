#pragma once
#include <string>
#include <vector>
#include <tuple>
using namespace std;

class Level
{
private:
	struct mapSector {
		vector<string> sectorTileRows;
		tuple<int, int> sectorCoords;
	};

	string setArea, setKeepsake, setOldSoul;
	string currentAreaDayOrNight, currentAreaTime, currentBoss, playerTilePrev;
	int currentAreaDay;
	tuple<int, int> playerCoords, mapSectorCoords;
	bool gameStarted, mapSelected;
	vector<mapSector> levelSectors;
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

	void initWorldMap();
	void initEnemies();
	string selectBoss(string area, int day);
	void displayMap(string reset_colour);
	void playerSetup();
	void updateMovement(char input);
	void displayWorld();
	void displayInventory(vector<string> inventory);
	void displayPlayerInfo();
	vector<string> initInvDisplay();
	void getPlayerInput();
	void assignSectorToMap(tuple<int, int> numSector);
	void loadNewSector(tuple<int, int> coords);
public:
	Level(string area, string keepsake, string oldSoul);

	int playerHP, playerATK, playerDEF, playerSPD;
	string playerWeapon;
	vector<string> playerInventory;

	void display();
};

