#pragma once
#include <string>
#include <vector>
#include <tuple>
#include <functional>
#include <memory>
#include "List.h"
using namespace std;

class Location
{
public:
	string locationType, icon;
	bool active;

	Location();
	Location(tuple<int, int> coords, tuple<int, int> sector);

	void pause(int milliseconds);

	virtual string interactStart(bool locationDisplaySelected, function<void()> infoCallback = {}) { return "null"; }
	//virtual unique_ptr<Location> clone() const = 0;
public:
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

	virtual string interactStart(bool locationDisplaySelected, function<void()> infoCallback = {}) override;
};

class Crystal : public Location
{
private:
	vector<string> finalItemsList;
	List itemsOptions;
public:
	Crystal();
	Crystal(tuple<int, int> coords, tuple<int, int> sector);

	virtual string interactStart(bool locationDisplaySelected, function<void()> infoCallback = {}) override;
};

class Grave : public Location
{
private:
	vector<string> finalItemsList;
	List itemsOptions;
public:
	Grave();
	Grave(tuple<int, int> coords, tuple<int, int> sector);

	virtual string interactStart(bool locationDisplaySelected, function<void()> infoCallback = {}) override;
};
