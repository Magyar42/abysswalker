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
	playerBurn = 0;
	playerStun = 0;
	playerPoison = 0;
	playerDMGDealt = 0;

	enemyHP = 10;
	enemyATK = 1;
	enemyDEF = 0;
	enemySPD = 0;
	enemyAbility = "";
	enemyMaxHP = 10;
	enemyBurn = 0;
	enemyStun = 0;
	enemyPoison = 0;
	enemyDMGDealt = 0;

	playerExposed = false;
	enemyExposed = false;
	playerWounded = false;
	enemyWounded = false;

	playerWon = false;
	battleOver = false;
	numTurn = 0;
	numSouls = 100;

	blacksmithCoalUsed = false;
	profaneGunpowderActive = false;
	carthusBeaconActive = false;
}

void Battle::playerAttack()
{
	cout << "\n\n You attack...";
	pauseBattle(DELAY_TIME);

	int damageDealtToDEF = playerATK;
	int damageDealtToHP = 0;
	if (damageDealtToDEF < 1) { damageDealtToDEF = 1; }

	enemyDEF -= damageDealtToDEF;
	checkEffectsEnemyChangedStat("DEF", -damageDealtToDEF);
	if (enemyDEF < 0) {
		damageDealtToHP = abs(enemyDEF);
		enemyDEF = 0;
		enemyHP -= damageDealtToHP;
		damageDealtToDEF -= damageDealtToHP;
	}
	playerDMGDealt += damageDealtToDEF + damageDealtToHP;
	if (enemyHP < 0) { enemyHP = 0; }

	clearScreen();
	displayTitle();
	displayBattleInfo();

	cout << " You attack, dealing " << colourText(to_string(damageDealtToHP+damageDealtToDEF), RED) << " damage!\n";
	pauseBattle(DELAY_TIME_SHORT);

	displayDamageInfo(false, damageDealtToDEF, damageDealtToHP);

	checkEffectsPlayerStrikes();

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
	checkEffectsPlayerChangedStat("DEF", -damageDealtToDEF);
	if (playerDEF < 0) {
		damageDealtToHP = abs(playerDEF);
		playerDEF = 0;
		playerHP -= damageDealtToHP;
		damageDealtToDEF -= damageDealtToHP;
	}
	enemyDMGDealt += damageDealtToDEF + damageDealtToHP;
	if (playerHP < 0) { playerHP = 0; }

	clearScreen();
	displayTitle();
	displayBattleInfo();

	cout << " The enemy attacks, dealing " << colourText(to_string(damageDealtToHP + damageDealtToDEF), RED) << " damage!\n";
	pauseBattle(DELAY_TIME_SHORT);

	displayDamageInfo(true, damageDealtToDEF, damageDealtToHP);

	checkEffectsEnemyStrikes();

	if (playerHP == 0) {
		cout << "\n\n You lost!";
		playerWon = false;
		battleOver = true;
		pauseBattle(DELAY_TIME);
	}
}

// Used for various other sources of damage like Burn, Poison, Abilities, etc
void Battle::inflictDamage(bool isPlayer, int numDamage, int numTimes)
{
	for (int i = 0; i < numTimes; i++) {
		int damageDealtToDEF = numDamage;
		int damageDealtToHP = 0;
		if (damageDealtToDEF < 1) { damageDealtToDEF = 1; }

		if (!isPlayer && carthusBeaconActive) {
			cout << "\n Your " << colourText("Carthus Beacon", YELLOW) << " increases the non-strike damage dealt by +2!";
			damageDealtToDEF += 2;
			pauseBattle(DELAY_TIME_SHORT);
		}

		if (isPlayer) {
			playerDEF -= damageDealtToDEF;
			checkEffectsPlayerChangedStat("DEF", -damageDealtToDEF);
			if (playerDEF < 0) {
				damageDealtToHP = abs(playerDEF);
				playerDEF = 0;
				playerHP -= damageDealtToHP;
				damageDealtToDEF -= damageDealtToHP;
			}
			enemyDMGDealt += damageDealtToDEF + damageDealtToHP;
			if (playerHP < 0) { playerHP = 0; }
		}
		else {
			enemyDEF -= damageDealtToDEF;
			checkEffectsEnemyChangedStat("DEF", -damageDealtToDEF);
			if (enemyDEF < 0) {
				damageDealtToHP = abs(enemyDEF);
				enemyDEF = 0;
				enemyHP -= damageDealtToHP;
				damageDealtToDEF -= damageDealtToHP;
			}
			playerDMGDealt += damageDealtToDEF + damageDealtToHP;
			if (enemyHP < 0) { enemyHP = 0; }
		}

		displayDamageInfo(isPlayer, damageDealtToDEF, damageDealtToHP);
	}
}

void Battle::applyStatusEffects(bool isPlayer, string item)
{
	// To be implemented: apply status effects based on item or ability used
}

void Battle::displayDamageInfo(bool isPlayer, int damageDealtToDEF, int damageDealtToHP)
{
	if (isPlayer) {
		if (damageDealtToDEF > 0) {
			cout << "\n Your " << colourText("DEF:", BLUE) << " -" << damageDealtToDEF;
			pauseBattle(DELAY_TIME_SHORT);

			if (playerDEF == 0 && !playerExposed) {
				cout << "\n You are exposed! ";
				playerExposed = true;
				pauseBattle(DELAY_TIME_SHORT);
				checkEffectsPlayerExposed();
			}
		}
		if (damageDealtToHP > 0) {
			cout << "\n Your " << colourText("HP:", GREEN) << " -" << damageDealtToHP;
			pauseBattle(DELAY_TIME_SHORT);

			if (playerHP <= (playerMaxHP / 2) && !playerWounded) {
				cout << "\n You are wounded! ";
				playerWounded = true;
				pauseBattle(DELAY_TIME_SHORT);
				checkEffectsPlayerWounded();
			}
		}
	}
	else {
		if (damageDealtToDEF > 0) {
			cout << "\n Enemy " << colourText("DEF:", BLUE) << " -" << damageDealtToDEF;
			pauseBattle(DELAY_TIME_SHORT);

			if (enemyDEF == 0 && !enemyExposed) {
				cout << "\n Enemy is exposed! ";
				enemyExposed = true;
				pauseBattle(DELAY_TIME_SHORT);
				checkEffectsEnemyExposed();
			}
		}
		if (damageDealtToHP > 0) {
			cout << "\n Enemy " << colourText("HP:", GREEN) << " -" << damageDealtToHP;
			pauseBattle(DELAY_TIME_SHORT);

			if (enemyHP <= (enemyMaxHP / 2) && !enemyWounded) {
				cout << "\n Enemy is wounded! ";
				enemyWounded = true;
				pauseBattle(DELAY_TIME_SHORT);
				checkEffectsEnemyWounded();
			}
		}
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

int Battle::startBattle(Enemy enemyType, bool bossFight)
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

	checkEffectsBattleStart();

	if (playerSPD >= enemySPD) {
		while (!battleOver) {
			numTurn++;
			checkEffectsPlayerTurn();
			playerAttack();
			if (enemyHP <= 0) break;
			checkEffectsEnemyTurn();
			enemyAttack();
		}
	}
	else {
		while (!battleOver) {
			numTurn++;
			checkEffectsEnemyTurn();
			enemyAttack();
			if (playerHP <= 0) break;
			checkEffectsPlayerTurn();
			playerAttack();
		}
	}

	checkEffectsBattleOver();

	if (battleOver && playerWon) {
		cout << "\n You gained +100 Souls.";
	}
	else if (battleOver && !playerWon) {
		cout << "\n You were defeated by the " << colourText(enemyType.enemyName, RED) << "...\n";
	}

	pauseBattle(DELAY_TIME_SHORT);
	cout << "\n\n Press any key to continue...";

	return numSouls;
}

// Item & Ability Triggers //

void Battle::checkEffectsBattleStart()
{
	for (string item : playerInv) {
		if (item == "Millwood Cloak") {
			if (playerMaxHP > playerHP) {
				playerHP += 2;
				if (playerHP > playerMaxHP) { playerHP = playerMaxHP; }
				cout << "\n Your " << item << " restored 2 HP!";
				pauseBattle(DELAY_TIME_SHORT);
			}
		}
		else if (item == "Carthus Boots") {
			playerSPD += 2;
			checkEffectsPlayerChangedStat("SPD", 2);
			cout << "\n Your " << item << " increased your SPD by +2 and dealt 2 damage to you!";
			pauseBattle(DELAY_TIME_SHORT);
			inflictDamage(true, 2);
		}
		else if (item == "Woodgrain Talisman") {
			if (playerSPD < enemySPD) {
				playerATK += playerSPD;
				cout << "\n Your " << item << " increased your ATK by your current SPD (" << playerSPD << ")!";
				pauseBattle(DELAY_TIME_SHORT);
			}
		}
		else if (item == "Crown of the Ivory King") {
			playerMaxHP += playerDEF;
			playerHP += playerDEF;
			cout << "\n Your " << item << " increased your max HP by your current DEF (" << playerDEF << ")!";
			pauseBattle(DELAY_TIME_SHORT);
		}
		else if (item == "Izalith Gauntlets") {
			enemyBurn += 2;
			cout << "\n Your " << item << " inflicted 2 stacks of Burn on the enemy!";
			pauseBattle(DELAY_TIME_SHORT);
		}
		else if (item == "Barrier of Chaos") {
			playerBurn += 2;
			cout << "\n Your " << item << " inflicted 2 stacks of Burn on yourself!";
			pauseBattle(DELAY_TIME_SHORT);
		}
		else if (item == "Dung Pie") {
			enemyPoison += 6;
			playerPoison += 3;
			cout << "\n Your " << item << " inflicted 6 stacks of Poison on the enemy and 3 stacks of Poison on yourself!";
			pauseBattle(DELAY_TIME_SHORT);
		}
		else if (item == "Relic of the Ancients") {
			enemyStun += 4;
			playerStun += 2;
			cout << "\n Your " << item << " inflicted 4 stacks of Stun on the enemy and 2 stacks of Stun on yourself!";
			pauseBattle(DELAY_TIME_SHORT);
		}
		else if (item == "Black Firebombs") {
			playerATK += 1;
			cout << "\n Your " << item << " increased your ATK by +1 for the next 3 turns!";
			pauseBattle(DELAY_TIME_SHORT);

			if (profaneGunpowderActive) {
				playerATK += 1;
				cout << "\n Your Profane Gunpowder further increased your ATK by +1!";
				pauseBattle(DELAY_TIME_SHORT);
			}
		}
		else if (item == "Ironstone Bracelet") {
			enemyATK -= 2;
			if (enemyATK < 0) { enemyATK = 0; }
			cout << "\n Your " << item << " decreased enemy ATK by 2 while you have DEF!";
			pauseBattle(DELAY_TIME_SHORT);
		}
		else if (item == "Catarina Greaves" && playerDEF > 0 && !catarinaGreavesActive) {
			playerATK += 2;
			catarinaGreavesActive = true;
			cout << "\n Your " << item << " increased your ATK by +2 as your DEF is above 0!";
			pauseBattle(DELAY_TIME_SHORT);
		}
		else if (item == "Spell: Carthus Beacon") {
			carthusBeaconActive = true;
		}
		else if (item == "Profane Gunpowder") {
			profaneGunpowderActive = true;
		}
	}
}

void Battle::checkEffectsPlayerTurn()
{
	checkEffectsAnyTurn();

	if (playerWpn == "Enchanted Blades") {
		int effectChance = rand() % 2;
		if (effectChance == 1 || numTurn % 2 != 0) {
			enemyBurn += 1;
			cout << "\n Your " << playerWpn << " inflicted 1 stack of Burn on the enemy!";
			pauseBattle(DELAY_TIME_SHORT);
		}
	}
	else if (playerWpn == "Sunlight Talisman") {
		int effectChance = rand() % 2;
		if (effectChance == 1 || numTurn % 2 != 0) {
			playerMaxHP += 1;
			playerHP += 1;
			cout << "\n Your " << playerWpn << " increased your max HP by +1!";
			pauseBattle(DELAY_TIME_SHORT);
		}
	}

	for (string item : playerInv) {
		if (item == "Ring of the Evil Eye") {
			if (numTurn % 2 != 0 && playerMaxHP > playerHP) {
				playerHP += 1;
				cout << "\n Your " << item << " restored 1 HP!";
				pauseBattle(DELAY_TIME_SHORT);
			}
		}
		else if (item == "Dragonscale Ring") {
			if (numTurn % 2 != 0) {
				playerDEF += 1;
				checkEffectsPlayerChangedStat("DEF", 1);
				cout << "\n Your " << item << " increased your DEF by +1!";
				pauseBattle(DELAY_TIME_SHORT);
			}
		}
		else if (item == "Carthus Bloodring") {
			if (numTurn % 2 != 0) {
				playerSPD += 1;
				checkEffectsPlayerChangedStat("SPD", 1);
				cout << "\n Your " << item << " increased your SPD by +1!";
				pauseBattle(DELAY_TIME_SHORT);
			}
		}
		else if (item == "Black Firebombs") {
			if (numTurn == 4) {
				playerATK -= 1;
				cout << "\n The effect of your " << item << " ran out. Your ATK has been reduced by 1.";
				pauseBattle(DELAY_TIME_SHORT);
			}
		}
		else if (item == "Aged Feather") {
			if (playerMaxHP > playerHP) {
				playerHP += 2;
				playerSPD -= 1;
				if (playerHP > playerMaxHP) { playerHP = playerMaxHP; }
				checkEffectsPlayerChangedStat("SPD", -1);
				cout << "\n Your " << item << " restored 2 HP and decreased your SPD by 1!";
				pauseBattle(DELAY_TIME_SHORT);
			}
		}
		else if (item == "Corrosive Buckler") {
			if (playerDEF > 0) {
				enemyPoison += 2;
				playerDEF -= 1;
				checkEffectsPlayerChangedStat("DEF", -1);
				cout << "\n Your " << item << " inflicted 2 stacks of Poison on the enemy and decreased your DEF by 1!";
				pauseBattle(DELAY_TIME_SHORT);
			}
		}
	}
}

void Battle::checkEffectsEnemyTurn()
{
	checkEffectsAnyTurn();
}

void Battle::checkEffectsAnyTurn()
{
}

void Battle::checkEffectsPlayerExposed()
{
	for (string item : playerInv) {
		if (item == "Catarina Armour") {
			playerDEF += 3;
			checkEffectsPlayerChangedStat("DEF", 3);
			cout << "\n Your " << item << " increased your DEF by +3!";
			pauseBattle(DELAY_TIME_SHORT);
		}
		else if (item == "Millwood Helmet") {
			if (playerMaxHP > playerHP) {
				playerHP += 3;
				if (playerHP > playerMaxHP) { playerHP = playerMaxHP; }
				cout << "\n Your " << item << " restored 3 HP!";
				pauseBattle(DELAY_TIME_SHORT);
			}
		}
		else if (item == "Spiked Shield") {
			cout << "\n Your " << item << " dealt 4 damage to you!";
			pauseBattle(DELAY_TIME_SHORT);
			inflictDamage(true, 4);
		}
		else if (item == "Spell: Iron Flesh") {
			playerDEF += 7;
			playerSPD -= 1;
			if (playerSPD < 0) { playerSPD = 0; }
			checkEffectsPlayerChangedStat("DEF", 7);
			checkEffectsPlayerChangedStat("SPD", -1);
			cout << "\n Your " << item << " increased your DEF by +7 and reduced your SPD by 1!";
			pauseBattle(DELAY_TIME_SHORT);
		}
		else if (item == "Roped Firebomb") {
			cout << "\n Your " << item << " dealt 5 damage to the enemy!";
			pauseBattle(DELAY_TIME_SHORT);
			inflictDamage(false, 5);

			if (profaneGunpowderActive) {
				cout << "\n Your Profane Gunpowder dealt a further 5 damage!";
				pauseBattle(DELAY_TIME_SHORT);
				inflictDamage(false, 5);
			}
		}
		else if (item == "Spell: Wrath of the Gods") {
			cout << "\n Your " << item << " activated, dealing 1 damage to the enemy 3 times!";
			pauseBattle(DELAY_TIME_SHORT);
			inflictDamage(false, 1, 3);
		}
		else if (item == "Wolf's Blood") {
			playerDEF += 4;
			checkEffectsPlayerChangedStat("DEF", 4);
			cout << "\n Your " << item << " increased your DEF by +4!";
			pauseBattle(DELAY_TIME_SHORT);
		}
		else if (item == "Ironstone Bracelet") {
			enemyATK += 3;
			cout << "\n Your " << item << " increased enemy ATK by +3 as you have been exposed!";
			pauseBattle(DELAY_TIME_SHORT);
		}
		else if (item == "Blacksmith's Coal") {
			if (!blacksmithCoalUsed) {
				playerExposed = false;
				playerDEF += 2;
				blacksmithCoalUsed = true;
				checkEffectsPlayerChangedStat("DEF", 2);
				cout << "\n Your " << item << " activated, allowing you to trigger exposed effects one more time and increasing your DEF by +2!";
				pauseBattle(DELAY_TIME_SHORT);
			}
		}
	}
}

void Battle::checkEffectsEnemyExposed()
{
}

void Battle::checkEffectsPlayerWounded()
{
	if (playerWpn == "Crow Talons") {
		playerATK += 3;
		cout << "\n Your " << playerWpn << " increased your ATK by +3 and dealt 4 damage to you!";
		pauseBattle(DELAY_TIME_SHORT);
		inflictDamage(true, 4);
	}

	for (string item : playerInv) {
		if (item == "Spell: Seething Chaos") {
			enemyBurn += 3;
			cout << "\n Your " << item << " inflicted 3 stacks of Burn on the enemy!";
			pauseBattle(DELAY_TIME_SHORT);
		}
		else if (item == "Darkwraith Headpiece") {
			if (playerMaxHP > playerHP) {
				playerHP += playerATK;
				if (playerHP > playerMaxHP) { playerHP = playerMaxHP; }
				cout << "\n Your " << playerWpn << " restored " << playerATK << " HP!";
				pauseBattle(DELAY_TIME_SHORT);
			}
		}
	}
}

void Battle::checkEffectsEnemyWounded()
{
	for (string item : playerInv) {
		if (item == "Spell: Curse of Londor") {
			checkEffectsPlayerWounded();
			cout << "\n Your " << item << " activated, triggering your wounded effects!";
			pauseBattle(DELAY_TIME_SHORT);
		}
	}
}

void Battle::checkEffectsPlayerStrikes()
{
	if (playerWpn == "Dark Hand") {
		if (playerMaxHP > playerHP) {
			playerHP += 1;
			if (playerHP > playerMaxHP) { playerHP = playerMaxHP; }
			cout << "\n Your " << playerWpn << " restored 1 HP!";
			pauseBattle(DELAY_TIME_SHORT);
		}
	}
	else if (playerWpn == "Dragonslayer Greatbow") {
		playerSPD -= 1;
		if (playerSPD < 0) { playerSPD = 0; }
		checkEffectsPlayerChangedStat("SPD", -1);
		cout << "\n Your " << playerWpn << " decreased your SPD by 1!";
		pauseBattle(DELAY_TIME_SHORT);

		if (playerSPD == 0) {
			playerATK -= 3;
			if (playerATK < 0) { playerATK = 0; }
			cout << "\n Your " << playerWpn << " reduced your ATK by 3 due to SPD reaching 0!";
			pauseBattle(DELAY_TIME_SHORT);
		}
	}
}

void Battle::checkEffectsEnemyStrikes()
{
	for (string item : playerInv) {
		if (item == "Putrescent Gauntlets") {
			if (numTurn == 1) {
				enemyPoison += enemyDMGDealt;
				cout << "\n Your " << item << " inflicted " << enemyDMGDealt << " stacks of Poison on the enemy!";
				pauseBattle(DELAY_TIME_SHORT);
			}
		}
		else if (item == "Divine Armour") { 
			if (numTurn == 1) {
				enemyStun += 5;
				cout << "\n Your " << item << " dealt " << enemyDMGDealt << " damage to you, but inflicted 5 stacks of Stun on the enemy!";
				pauseBattle(DELAY_TIME_SHORT);
				inflictDamage(true, enemyDMGDealt);
			}
		}
	}
}

void Battle::checkEffectsPlayerInflicted(string statusEffect)
{
	checkEffectsAnyInflicted(statusEffect);

	for (string item : playerInv) {
		if (item == "Stone Greaves" && statusEffect == "Stun") {
			cout << "\n Your " << item << " dealt 2 damage to the enemy!";
			pauseBattle(DELAY_TIME_SHORT);
			inflictDamage(false, 2);
		}
	}
}

void Battle::checkEffectsEnemyInflicted(string statusEffect)
{
	checkEffectsAnyInflicted(statusEffect);
}

void Battle::checkEffectsAnyInflicted(string statusEffect)
{
	for (string item : playerInv) {
		if (item == "Storm King Crown" && statusEffect == "Stun") {
			playerDEF += 2;
			checkEffectsPlayerChangedStat("DEF", 2);
			cout << "\n Your " << item << " increased your DEF by +2!";
			pauseBattle(DELAY_TIME_SHORT);
		}
	}
}

void Battle::checkEffectsPlayerChangedStat(string statChanged, int numChanged)
{
	for (string item : playerInv) {
		if (statChanged == "SPD") {
			if (item == "Legion Armour" && numChanged < 0) {
				playerDEF += 1;
				checkEffectsPlayerChangedStat("DEF", 1);
				cout << "\n Your " << item << " increased your DEF by +1!";
				pauseBattle(DELAY_TIME_SHORT);
			}
			else if (item == "Legion Greaves" && numChanged < 0) {
				playerMaxHP += 2;
				playerHP += 2;
				cout << "\n Your " << item << " increased your max HP by +2!";
				pauseBattle(DELAY_TIME_SHORT);
			}
		}
		else if (statChanged == "DEF") {
			if (item == "Catarina Greaves" && playerDEF <= 0 && catarinaGreavesActive) {
				playerATK -= 2;
				catarinaGreavesActive = false;
				cout << "\n Your " << item << " decreased your ATK by 2 as your DEF has dropped to 0!";
				pauseBattle(DELAY_TIME_SHORT);
			}
			else if (item == "Catarina Greaves" && playerDEF > 0 && !catarinaGreavesActive) {
				playerATK += 2;
				catarinaGreavesActive = true;
				cout << "\n Your " << item << " increased your ATK by +2 as your DEF is above 0!";
				pauseBattle(DELAY_TIME_SHORT);
			}
		}
	}
}

void Battle::checkEffectsEnemyChangedStat(string statChanged, int numChanged)
{
}

void Battle::checkEffectsBattleOver()
{
	for (string item : playerInv) {
		if (item=="Covetous Serpent Ring") {
			numSouls += 100;
			cout << "\n\n Your " << item << " granted you an additional +100 Souls!";
			pauseBattle(DELAY_TIME_SHORT);
		}
	}
}

