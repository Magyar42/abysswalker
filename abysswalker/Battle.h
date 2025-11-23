#pragma once
#include <string>
#include <vector>
#include "Enemy.h"
using namespace std;

class Battle
{
private:
	int playerATK, playerDEF, playerSPD, playerMaxHP;
	int enemyHP, enemyATK, enemyDEF, enemySPD, enemyMaxHP;
	vector<string> playerInv;
	string playerWpn, enemyAbility;
	bool battleOver, playerExposed, enemyExposed, playerWounded, enemyWounded;

	void playerAttack();
	void enemyAttack();
	void displayBattleInfo();
	void pauseBattle(int milliseconds);
public:
	bool playerWon;
	int playerHP;

	explicit Battle(int hp, int atk, int def, int spd, vector<string> inv, string wpn);
	void startBattle(Enemy enemyType);
};

