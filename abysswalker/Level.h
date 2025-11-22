#pragma once
#include <string>
#include <vector>
#include <tuple>
#include <memory>
#include "Enemy.h"
using namespace std;

class Level
{
private:
    struct mapSector {
        vector<string> sectorTileRows;
        tuple<int, int> sectorCoords;
    };

    string setArea, setKeepsake, setOldSoul;
    string currentAreaDayOrNight, currentAreaTime, currentBoss, playerTilePrev;
    int currentAreaDay;
    tuple<int, int> playerCoords, mapSectorCoords;
    bool gameStarted, mapSelected;
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

    // non-owning pointers to enemies in the current sector
    vector<Enemy*> currentSectorEnemies;
    // owning container of all enemies (unique_ptr ensures stable addresses for Enemy objects)
    vector<unique_ptr<Enemy>> allSectorEnemies;

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
	void startCombat();
public:
    Level(string area, string keepsake, string oldSoul);

    int playerHP, playerATK, playerDEF, playerSPD;
    int endGame;
    string playerWeapon;
    vector<string> playerInventory;

    void display();
};

