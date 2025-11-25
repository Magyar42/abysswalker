#pragma once
#include <string>
#include <tuple>
#include <vector>
using namespace std;

class Enemy
{
public:
    string enemyName, icon, ability;
    string HP, ATK, DEF, SPD;
	tuple<int, int> sectorPos, mapPos, mapPosPrev;

    explicit Enemy(string type, tuple<int,int> sector, tuple<int, int> pos, bool isBoss=false);
    void updateMovement(const vector<vector<string>>& sector);
};