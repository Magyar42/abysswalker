#pragma 
#include <string>
#include <vector>
#include <map>
using namespace std;

extern const string textSeparator;
extern const string titleDisplay[6];
extern const string RESET;
extern const string INACTIVE;
extern const string RED;
extern const string GREEN;
extern const string YELLOW;
extern const string BLUE;

extern vector<vector<string>>  qPressInfo;
extern map<string, map<string, string>> keepsakesMap;
extern map<string, map<string, string>> oldSoulsMap;
extern map<string, map<string, string>> regionsMap;
extern vector<string> keepsakesVector;

string colourText(const string& text, const string& colour, const string& reset = RESET);
string getStats(string HP, string ATK, string DEF, string SPD);
void clearScreen();
void displayTitle();
void qPressCheck(string currentSelection);