#include "Misc.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Constants
const string textSeparator = "\n================================================================================\n\n";
const string titleDisplay[6] = {
    "   _____ ___.                                        .__   __                 ",
"  /  _  \\\\_ |__ ___.__. ______ ________  _  _______  |  | |  | __ ___________ ",
" /  /_\\  \\| __ <   |  |/  ___//  ___/\\ \\/ \\/ /\\__  \\ |  | |  |/ // __ \\_  __ \\",
"/    |    \\ \\_\\ \\___  |\\___ \\ \\___ \\  \\     /  / __ \\|  |_|    <\\  ___/|  | \\/ ",
"\\____|__  /___  / ____/____  >____  >  \\/\\_/  (____  /____/__|_ \\\\___  >__|",
"        \\/    \\/\\/         \\/     \\/               \\/          \\/    \\/       "
};
const string RESET = "\033[0m";
const string INACTIVE = "\033[0;90m";
const string RED = "\033[0;31m";
const string GREEN = "\033[0;32m";
const string YELLOW = "\033[0;33m";
const string BLUE = "\033[0;36m";

// Structs
vector<vector<string>>  qPressInfo = {
    { "View Itemlist", "Change Region", "Change Keepsake", "Change Old Soul", "Start", "Life Ring", "Fire Gem", "Black Firebombs", "Test Item", "Greatsword", "Titanite Demon"},
    { "Items are the main way of getting stronger in ABYSSWALKER. There are many that can be unlocked by meeting specific conditions, allowing them to appear in-game.", "There are 3 regions to travel through in ABYSSWALKER. Each has different locations, enemies, bosses, and mechanics. To unlock the next region, you must achieve victory in the prior region.", "Keepsakes are items that you start with. They usually provide passive effects, but can sometimes be used in specific situations. More keepsakes can be unlocked by completing specific objectives.", "Old Souls are the powerful souls of heroes long gone. If one is selected, you will begin with different stats and items. Old Souls can be found during gameplay, unlocking them permanently.", "Select this option to begin the game! Make sure you have chosen your starting gear well.", "Begin with +3 HP.", "Consumable item. Use at a Blacksmith to add the Fire effect to your current weapon.", "Every other turn in combat, attack a second time, inflicting 2 damage. Triggers up to 3 times in one fight.", "Unknown.", "Base effect: Gain +2 " + colourText("ATK", RED) + ".", "Titanite Demon: " + getStats("5", "4", "15", "1") + " \n When losing all its " + colourText("DEF", BLUE) + ", Titanite Demon loses 2 " + colourText("ATK", RED) + "."}
};

// Misc Functions

string colourText(const string& text, const string& colour, const string& reset)
{
    return colour + text + reset;
}

string getStats(string HP, string ATK, string DEF, string SPD)
{
    return colourText(" HP:", GREEN) + " " + HP + " | " + colourText("ATK:", RED) + " " + ATK + " | " + colourText("DEF : ", BLUE) + " " + DEF + " | " + colourText("SPD : ", YELLOW) + SPD;
}

void clearScreen()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void displayTitle()
{
    for (string titleLine : titleDisplay) {
        cout << titleLine << "\n";
    }
    cout << textSeparator;
}

void qPressCheck(string currentSelection)
{
    for (int i = 0; i < qPressInfo[0].size(); ++i) {
        if (qPressInfo[0][i] == currentSelection) {
            cout << colourText("\n [INFO] " + qPressInfo[1][i] + "\n", RESET);
        }
    }
}