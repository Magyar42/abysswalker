#pragma once
#include <string>
#include <vector>
#include "Enemy.h"
using namespace std;

class Battle
{
private:
	int playerHP, playerATK, playerDEF, playerSPD, playerMaxHP;
	int enemyHP, enemyATK, enemyDEF, enemySPD, enemyMaxHP;
	vector<string> playerInv;
	string playerWpn, enemyAbility;

	void playerAttack();
	void enemyAttack();
	void displayBattleInfo();
	void pauseBattle(int milliseconds);
public:
	explicit Battle(int hp, int atk, int def, int spd, vector<string> inv, string wpn);
	void startBattle(Enemy enemyType);
};

