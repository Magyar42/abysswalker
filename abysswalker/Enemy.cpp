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
	ability = enemiesMap[enemyName]["ability"];
}

void Enemy::updateMovement(const vector<vector<string>>& sector)
{
    tuple<int, int> newPos = mapPos;
    mapPosPrev = mapPos;
    int movementChance = rand() % 100;

    // todo: fix enemies wrongly believing some tiles are closed/open when they are not
    if (movementChance < 50) {
        int attempts = 0;
        const int maxAttempts = 10;

        while (attempts++ < maxAttempts) {
            int direction = rand() % 4;
            if (direction == 0) get<1>(newPos) -= 1; // left
            else if (direction == 1) get<1>(newPos) += 1; // right
            else if (direction == 2) get<0>(newPos) -= 1; // up
            else if (direction == 3) get<0>(newPos) += 1; // down

            int row = get<0>(newPos);
            int column = get<1>(newPos);

            // Bounds check before indexing
            if (row < 0 || column < 0 || row >= static_cast<int>(sector.size()) ||
                column >= static_cast<int>(sector[row].size())) {
                cout << "Enemy " << enemyName << " tried to move to closed tile at (" << get<0>(newPos) << ", " << get<1>(newPos) << ")\n";
                newPos = mapPos;
                continue;
            }

            if (sector[row][column] == CLOSED_TILE) {
                cout << "Enemy " << enemyName << " tried to move to closed tile at (" << get<0>(newPos) << ", " << get<1>(newPos) << ")\n";
                newPos = mapPos;
                continue;
            }

            mapPos = newPos;
            break;
        }
    }
}