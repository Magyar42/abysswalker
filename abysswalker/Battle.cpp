#include "Battle.h"
#include "Misc.h"
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <conio.h>

Battle::Battle(int hp, int maxHp, int atk, int def, int spd, vector<string> inv, string wpn)
{
	playerATK = atk;
	playerDEF = def;
	playerHP = hp;
	playerSPD = spd;
	playerInv = inv;
	playerWpn = wpn;
	playerMaxHP = maxHp;

	enemyHP = 10;
	enemyATK = 1;
	enemyDEF = 0;
	enemySPD = 0;
	enemyAbility = "";
	enemyMaxHP = 10;

	playerExposed = false;
	enemyExposed = false;
	playerWounded = false;
	enemyWounded = false;

	playerWon = false;
	battleOver = false;
}

void Battle::playerAttack()
{
	cout << "\n\n You attack...";
	pauseBattle(DELAY_TIME);

	int damageDealtToDEF = playerATK;
	int damageDealtToHP = 0;
	if (damageDealtToDEF < 1) { damageDealtToDEF = 1; }

	enemyDEF -= damageDealtToDEF;
	if (enemyDEF < 0) {
		damageDealtToHP = abs(enemyDEF);
		enemyDEF = 0;
		enemyHP -= damageDealtToHP;
		damageDealtToDEF -= damageDealtToHP;
	}
	if (enemyHP < 0) { enemyHP = 0; }

	clearScreen();
	displayTitle();
	displayBattleInfo();

	cout << " You attack, dealing " << colourText(to_string(damageDealtToHP+damageDealtToDEF), RED) << " damage!\n";
	pauseBattle(DELAY_TIME_SHORT);

	if (damageDealtToDEF > 0) {
		cout << "\n Enemy "<< colourText("DEF:", BLUE) << " -" << damageDealtToDEF;
		pauseBattle(DELAY_TIME_SHORT);

		if (enemyDEF == 0 && !enemyExposed) {
			cout << "\n Enemy is exposed! \n";
			enemyExposed = true;
			pauseBattle(DELAY_TIME_SHORT);
		}
	}
	if (damageDealtToHP > 0) {
		cout << "\n Enemy " << colourText("HP:", GREEN) << " -" << damageDealtToHP;
		pauseBattle(DELAY_TIME_SHORT);

		if (enemyHP <= (enemyMaxHP / 2) && !enemyWounded) {
			cout << "\n Enemy is wounded! \n";
			enemyWounded = true;
			pauseBattle(DELAY_TIME_SHORT);
		}
	}
	if (enemyHP == 0) {
		cout << "\n\n Victory!";
		playerWon = true;
		battleOver = true;
		pauseBattle(DELAY_TIME);
	}
}

void Battle::enemyAttack()
{
	cout << "\n\n The enemy attacks...";
	pauseBattle(DELAY_TIME);

	int damageDealtToDEF = enemyATK;
	int damageDealtToHP = 0;
	if (damageDealtToDEF < 1) { damageDealtToDEF = 1; }

	playerDEF -= damageDealtToDEF;
	if (playerDEF < 0) {
		damageDealtToHP = abs(playerDEF);
		playerDEF = 0;
		playerHP -= damageDealtToHP;
		damageDealtToDEF -= damageDealtToHP;
	}
	if (playerHP < 0) { playerHP = 0; }

	clearScreen();
	displayTitle();
	displayBattleInfo();

	cout << " The enemy attacks, dealing " << colourText(to_string(damageDealtToHP + damageDealtToDEF), RED) << " damage!\n";
	pauseBattle(DELAY_TIME_SHORT);

	if (damageDealtToDEF > 0) {
		cout << "\n Your " << colourText("DEF:", BLUE) << " -" << damageDealtToDEF;
		pauseBattle(DELAY_TIME_SHORT);

		if (playerDEF == 0 && !playerExposed) {
			cout << "\n You are exposed! \n";
			playerExposed = true;
			pauseBattle(DELAY_TIME_SHORT);
		}
	}
	if (damageDealtToHP > 0) {
		cout << "\n Your " << colourText("HP:", GREEN) << " -" << damageDealtToHP;
		pauseBattle(DELAY_TIME_SHORT);

		if (playerHP <= (playerMaxHP / 2) && !playerWounded) {
			cout << "\n You are wounded! \n";
			playerWounded = true;
			pauseBattle(DELAY_TIME_SHORT);
		}
	}
	if (playerHP == 0) {
		cout << "\n\n You lost!";
		playerWon = false;
		battleOver = true;
		pauseBattle(DELAY_TIME);
	}
}

void Battle::displayBattleInfo()
{
	cout << colourText(" [Player] ", BLUE) << "\n";
	cout << getStatsBattle(to_string(playerHP), to_string(playerMaxHP), to_string(playerATK), to_string(playerDEF), to_string(playerSPD)) << "\n\n";

	cout << colourText(" [Enemy] ", RED) << "\n";
	cout << getStatsBattle(to_string(enemyHP), to_string(enemyMaxHP), to_string(enemyATK), to_string(enemyDEF), to_string(enemySPD)) << "\n";
	cout << " [ABILITY]: " << enemyAbility << "\n";

	cout << textSeparator;
}

void Battle::pauseBattle(int milliseconds)
{
	this_thread::sleep_for(chrono::milliseconds(1000));
}

void Battle::startBattle(Enemy enemyType, bool bossFight)
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

	if (bossFight) {
		cout << " The " << colourText("[!] " + enemyType.enemyName, RED) << " stands before you!";
	}
	else {
		cout << " A " << colourText(enemyType.enemyName, RED) << " blocks your path!";
	}
	pauseBattle(DELAY_TIME);
	cout << "\n\n Press any key to begin the battle...";
	_getch();

	if (playerSPD >= enemySPD) {
		while (!battleOver) {
			playerAttack();
			if (enemyHP <= 0) break;
			enemyAttack();
		}
	}
	else {
		while (!battleOver) {
			enemyAttack();
			if (playerHP <= 0) break;
			playerAttack();
		}
	}

	if (battleOver && playerWon) {
		cout << "\n You gained +100 Souls.";
	}
	else if (battleOver && !playerWon) {
		cout << "\n You were defeated by the " << colourText(enemyType.enemyName, RED) << "...\n";
	}

	pauseBattle(DELAY_TIME_SHORT);
	cout << "\n\n Press any key to continue...";
}
