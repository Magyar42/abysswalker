#pragma once
#include <map>
#include <vector>
#include <string>
using namespace std;

class MainMenu
{
private:
public:
	map<int, map<string, string>> saveSlots;
	vector<int> saveSlotArray;
	string setArea, setKeepsake, setOldSoul;
	bool gameStarted;

	//MainMenu();
	//MainMenu(string setArea);
	//MainMenu(string setArea, string setKeepsake);
	MainMenu(string setArea, string setKeepsake, string setOldSoul);

	void displaySaves();
	void displayMenuOptions();
};

