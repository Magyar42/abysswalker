#pragma once
#include <string>
#include <tuple>
#include <vector>
using namespace std;

class Enemy
{
public:
    string enemyName, icon;
    string HP, ATK, DEF, SPD;
	tuple<int, int> sectorPos, mapPos, mapPosPrev;

    explicit Enemy(string type, tuple<int,int> sector, tuple<int, int> pos);
    void updateMovement(vector<vector<string>> sector);
};