#pragma 
#include <string>
#include <vector>
#include <map>
using namespace std;

extern const string textSeparator;
extern const string titleDisplay[6];
extern const string lossDisplay[9];
extern const string winDisplay[9];
extern const string RESET;
extern const string INACTIVE;
extern const string RED;
extern const string GREEN;
extern const string YELLOW;
extern const string BLUE;

extern const int DELAY_TIME;
extern const int DELAY_TIME_SHORT;
extern const int charMovementPerTimeCounter;
extern const int bossEncounterDays[3];

extern const string timesOfDay[7];

extern const string PLAYER_TILE;
extern const string OPEN_TILE;
extern const string CLOSED_TILE;
extern const string LOCATION_TILE;

extern vector<vector<string>>  qPressInfo;
extern map<string, map<string, string>> keepsakesMap;
extern map<string, map<string, string>> oldSoulsMap;
extern map<string, map<string, string>> regionsMap;
extern map<string, map<string, string>> enemiesMap;

extern vector<string> allItemsVector;
extern vector<string> keepsakesVector;
extern vector<string> oldSoulsVector;
extern vector<string> regionsVector;
extern vector<string> enemiesVector;

string colourText(const string& text, const string& colour, const string& reset = RESET);
string getStats(string HP, string ATK, string DEF, string SPD);
string getStatsBattle(string HP, string maxHP, string ATK, string DEF, string SPD);
void clearScreen();
void displayTitle();
void displayEndscreen(bool victory);
void qPressCheck(string currentSelection);