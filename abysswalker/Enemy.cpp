#include "Enemy.h"
#include "Misc.h"
#include <tuple>
#include <iostream>
#include <ctime>
using namespace std;

Enemy::Enemy(string type, tuple<int, int> sector, tuple<int, int> pos, bool isBoss)
{
	enemyName = type;
	sectorPos = sector;
	mapPos = pos;
	mapPosPrev = pos;
    if (isBoss) {
        HP = bossesMap[enemyName]["HP"];
        ATK = bossesMap[enemyName]["ATK"];
        DEF = bossesMap[enemyName]["DEF"];
        SPD = bossesMap[enemyName]["SPD"];
        icon = "<" + bossesMap[enemyName]["map_icon"] + ">";
        ability = bossesMap[enemyName]["ability"];
	}
    else {
        HP = enemiesMap[enemyName]["HP"];
        ATK = enemiesMap[enemyName]["ATK"];
        DEF = enemiesMap[enemyName]["DEF"];
        SPD = enemiesMap[enemyName]["SPD"];
        icon = "<" + enemiesMap[enemyName]["map_icon"] + ">";
        ability = enemiesMap[enemyName]["ability"];
    }
}

void Enemy::updateMovement(const vector<vector<string>>& sector)
{
    srand(time(0));
    tuple<int, int> newPos = mapPos;
    mapPosPrev = mapPos;
    int movementChance = rand() % 100;

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
                newPos = mapPos;
                continue;
            }

            if (sector[row][column] == CLOSED_TILE) {
                newPos = mapPos;
                continue;
            }

            mapPos = newPos;
            break;
        }
    }
}