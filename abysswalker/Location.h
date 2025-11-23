#pragma once
#include <string>
#include <vector>
#include <tuple>
#include <functional>
#include "List.h"
using namespace std;

class Location
{
public:
	string locationType, icon;

	Location();
	Location(tuple<int, int> coords, tuple<int, int> sector);

	void pause(int milliseconds);

	virtual string interactStart(bool locationDisplaySelected, function<void()> infoCallback = {}) { return "null"; }
protected:
	tuple<int, int> locationCoords;
	tuple<int, int> sectorCoords;
};

class Ruins : public Location
{
private:
	vector<string> finalItemsList;
	List itemsOptions;
public:
	Ruins();
	Ruins(tuple<int, int> coords, tuple<int, int> sector);

	virtual string interactStart(bool locationDisplaySelected, std::function<void()> infoCallback = {}) override;
};
