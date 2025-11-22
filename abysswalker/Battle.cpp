#include "Battle.h"
#include "Misc.h"
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

Battle::Battle(int hp, int atk, int def, int spd, vector<string> inv, string wpn)
{
	playerATK = atk;
	playerDEF = def;
	playerHP = hp;
	playerSPD = spd;
	playerInv = inv;
	playerWpn = wpn;
	playerMaxHP = hp;

	enemyHP = 10;
	enemyATK = 1;
	enemyDEF = 0;
	enemySPD = 0;
	enemyAbility = "";
	enemyMaxHP = 10;
}

void Battle::displayBattleInfo()
{
	cout << colourText(" [Player Stats] ", BLUE) << "\n";
	cout << getStatsBattle(to_string(playerHP), to_string(playerMaxHP), to_string(playerATK), to_string(playerDEF), to_string(playerSPD)) << "\n\n";

	cout << colourText(" [Enemy Stats] ", RED) << "\n";
	cout << getStatsBattle(to_string(enemyHP), to_string(enemyMaxHP), to_string(enemyATK), to_string(enemyDEF), to_string(enemySPD)) << "\n";
	cout << " [INFO]: " << enemyAbility << "\n";

	cout << textSeparator;
}

void Battle::pauseBattle(int milliseconds)
{
	this_thread::sleep_for(chrono::milliseconds(1000));
}

void Battle::startBattle(Enemy enemyType)
{
	enemyHP = stoi(enemyType.HP);
	enemyMaxHP = enemyHP;
	enemyATK = stoi(enemyType.ATK);
	enemyDEF = stoi(enemyType.DEF);
	enemySPD = stoi(enemyType.SPD);
	enemyAbility = enemyType.ability;

	clearScreen();
	displayTitle();
	displayBattleInfo();

	cout << " A " << colourText(enemyType.enemyName, RED) << " blocks your path!";
	pauseBattle(DELAY_TIME);
	cout << "\n\n Battle Start!\n";
}
