#pragma once
#include <string>
#include <vector>
using namespace std;

class List
{
private:
	string newlySelectedItem, currentSelectedItem;
	int selectedItemIndex, listLength;
	vector<string> listItems;
	bool importFromFile;
public:
	List(vector<string> list, bool import = false);

	void displayItems();
};

