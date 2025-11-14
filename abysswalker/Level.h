#pragma once
#include <string>
#include <vector>
#include <tuple>
using namespace std;

class Level
{
private:
	string setArea, setKeepsake, setOldSoul;
	string currentAreaDayOrNight, currentAreaTime, currentBoss;
	int currentAreaDay;
	tuple<int, int> playerCoords;
	bool gameStarted, mapSelected;

	string selectBoss(string area, int day);
	tuple<int, int> displayMap(vector<vector<string>> map, string reset_colour);
	void playerSetup();
	void updateMovement(char input);
	void displayWorld();
	void displayInventory(vector<string> inventory);
	void displayPlayerInfo();
	vector<string> initInvDisplay();
	void getPlayerInput();
public:
	Level(string area, string keepsake, string oldSoul);

	int playerHP, playerATK, playerDEF, playerSPD;
	string playerWeapon;
	vector<string> playerInventory;

	void display();
};

