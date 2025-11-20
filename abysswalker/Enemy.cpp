#include "Enemy.h"
#include "Misc.h"
#include <iostream>
using namespace std;

Enemy::Enemy(string type)
{
	enemyType = type;
	HP = enemiesMap[enemyType]["HP"];
	ATK = enemiesMap[enemyType]["ATK"];
	DEF = enemiesMap[enemyType]["DEF"];
	SPD = enemiesMap[enemyType]["SPD"];
	icon = "<" + enemiesMap[enemyType]["icon"] + ">";
}
