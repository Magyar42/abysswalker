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

	MainMenu(string area, string keepsake, string soul);

	void displaySaves();
	void displayMenuOptions();
	void initItems();
};

