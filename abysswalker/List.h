#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>
using namespace std;

class List
{
private:
	string newlySelectedItem, currentSelectedItem, listType;
	int selectedItemIndex, listLength;
	vector<string> listItems;
	bool importFromFile;

	string itemSelectionEffect();
	int selectionIndexUpdate(int currentIndex, int arrayLength, char input);
	string selectSublist(string newList);
public:
	List();
	List(string type, vector<string> list, bool import = false);

	string displayItems(function<void()> infoCallback = {});
	string displayImportedItems(string filename, map<string, map<string, string>> map);
};

