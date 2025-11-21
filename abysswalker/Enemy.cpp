#include "Enemy.h"
#include "Misc.h"
#include <tuple>
#include <iostream>
using namespace std;

Enemy::Enemy(string type, tuple<int, int> sector, tuple<int, int> pos)
{
	enemyName = type;
	sectorPos = sector;
	mapPos = pos;
	mapPosPrev = pos;
	HP = enemiesMap[enemyName]["HP"];
	ATK = enemiesMap[enemyName]["ATK"];
	DEF = enemiesMap[enemyName]["DEF"];
	SPD = enemiesMap[enemyName]["SPD"];
	icon = "<" + enemiesMap[enemyName]["map_icon"] + ">";
}

void Enemy::updateMovement(vector<vector<string>> sector)
{
	// todo: prevent enemy from moving into walls or off map
	// also, prevent enemies from overlapping
	// to fix: enemies seem to only be moving around their initial position, as if the function is using their initial position as a reference rather than their current position
	mapPosPrev = mapPos;
	cout << sector[get<1>(mapPos)][get<0>(mapPos)] << " -> ";
	int movementChance = rand() % 100;
	if (movementChance < 50) { // todo: adjust chance based on enemy type
		int direction = rand() % 4;
		if (direction == 0) { 
			get<1>(mapPos) -= 1;
		}
		else if (direction == 1) {
			get<1>(mapPos) += 1;
		}
		else if (direction == 2) {
			get<0>(mapPos) -= 1;
		}
		else if (direction == 3) {
			get<0>(mapPos) += 1;
		}
	}
}