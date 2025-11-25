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
	int playerBurn, playerStun, playerPoison, enemyBurn, enemyStun, enemyPoison;
	int numTurn, numSouls;
	int playerDMGDealt, enemyDMGDealt;
	vector<string> playerInv;
	string playerWpn, enemyAbility;
	bool battleOver, playerExposed, enemyExposed, playerWounded, enemyWounded;

	bool blacksmithCoalUsed, profaneGunpowderActive, carthusBeaconActive, catarinaGreavesActive;

	void playerAttack();
	void enemyAttack();
	void inflictDamage(bool isPlayer, int numDamage, int numTimes=1);
	void applyStatusEffects(bool isPlayer, string item);
	void displayDamageInfo(bool isPlayer, int damageDealtToDEF, int damageDealtToHP);
	void displayBattleInfo();
	void pauseBattle(int milliseconds);

	void checkEffectsBattleStart();
	void checkEffectsPlayerTurn();
	void checkEffectsEnemyTurn();
	void checkEffectsAnyTurn();
	void checkEffectsPlayerExposed();
	void checkEffectsPlayerWounded();
	void checkEffectsEnemyExposed();
	void checkEffectsEnemyWounded();
	void checkEffectsPlayerStrikes();
	void checkEffectsEnemyStrikes();
	void checkEffectsPlayerInflicted(string statusEffect);
	void checkEffectsEnemyInflicted(string statusEffect);
	void checkEffectsAnyInflicted(string statusEffect);
	void checkEffectsPlayerChangedStat(string statChanged, int numChanged=1);
	void checkEffectsEnemyChangedStat(string statChanged, int numChanged=1);
	void checkEffectsBattleOver();
public:
	bool playerWon;
	int playerHP;

	explicit Battle(int hp, int maxHp, int atk, int def, int spd, vector<string> inv, string wpn);
	int startBattle(Enemy enemyType, bool bossFight = false);
};

