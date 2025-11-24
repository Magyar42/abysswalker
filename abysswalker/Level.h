#pragma once
#include <string>
#include <vector>
#include <tuple>
#include <memory>
#include "Enemy.h"
#include "Location.h"
using namespace std;

class Level
{
private:
    struct mapSector {
        vector<string> sectorTileRows;
        tuple<int, int> sectorCoords;
    };

    string setArea, setKeepsake, setOldSoul;
    string currentBoss, playerTilePrev;
    int currentAreaDay, currentAreaTimeIndex, currentAreaTimeCounter;
    tuple<int, int> playerCoords, mapSectorCoords;
    bool gameStarted, mapSelected, locationActive;
    vector<mapSector> levelSectors;
    vector<vector<string>> displayedSector = {
        {"0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"},
        {"0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"},
        {"0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"},
        {"0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"},
        {"0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"},
        {"0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"},
        {"0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"},
        {"0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"},
        {"0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"},
        {"0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0"},
    };
	// Use a smart pointer so derived types are preserved (no slicing)
	unique_ptr<Location> currentLocation;

    // non-owning pointers to enemies in the current sector
    vector<Enemy*> currentSectorEnemies;
    // owning container of all enemies (unique_ptr ensures stable addresses for Enemy objects)
    vector<unique_ptr<Enemy>> allSectorEnemies;
	vector<unique_ptr<Location>> currentSectorLocations;

    void initWorldMap();
    void initEnemies();
    string selectBoss(string area, int day);
    void displayMap(string reset_colour);
    void playerSetup();
    void updateMovement(char input);
    void displayWorld();
    void displayInventory(vector<string> inventory);
    void displayPlayerInfo();
    vector<string> initInvDisplay();
    void getPlayerInput();
    void assignSectorToMap(tuple<int, int> numSector);
    void loadNewSector(tuple<int, int> coords);
    void setEnemies(mapSector);
    void loadSectorEnemies();
	void startCombat(Enemy& enemy);
	void startBossCombat(string bossName);
    void checkPlayerLocation();
	void updateTime();
    void displayInfoAndInventory();
    vector<string> updateInventory();
public:
    Level(string area, string keepsake, string oldSoul);

    int playerHP, playerMaxHP, playerATK, playerDEF, playerSPD, playerSouls;
    int endGame;
    string playerWeapon;
    vector<string> playerInventory;

    void display();
};

