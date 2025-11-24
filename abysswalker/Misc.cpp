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
const string lossDisplay[9] = {
    " __      __  ______   __    __        _______   ______  ________  _______  ",
    "/  \\    /  |/      \\ /  |  /  |      /       \\ /      |/        |/       \\ ",
    "$$  \\  /$$//$$$$$$  |$$ |  $$ |      $$$$$$$  |$$$$$$/ $$$$$$$$/ $$$$$$$  |",
    " $$  \\/$$/ $$ |  $$ |$$ |  $$ |      $$ |  $$ |  $$ |  $$ |__    $$ |  $$ |",
    "  $$  $$/  $$ |  $$ |$$ |  $$ |      $$ |  $$ |  $$ |  $$    |   $$ |  $$ |",
    "   $$$$/   $$ |  $$ |$$ |  $$ |      $$ |  $$ |  $$ |  $$$$$/    $$ |  $$ |",
    "    $$ |   $$ \\__$$ |$$ \\__$$ |      $$ |__$$ | _$$ |_ $$ |_____ $$ |__$$ |",
    "    $$ |   $$    $$/ $$    $$/       $$    $$/ / $$   |$$       |$$    $$/ ",
    "    $$/     $$$$$$/   $$$$$$/        $$$$$$$/  $$$$$$/ $$$$$$$$/ $$$$$$$/  "
};
const string winDisplay[9] = {
    " __     __  ______   ______   ________  ______   _______   __      __ ",
    "/  |   /  |/      | /      \\ /        |/      \\ /       \\ /  \\    /  |",
    "$$ |   $$ |$$$$$$/ /$$$$$$  |$$$$$$$$//$$$$$$  |$$$$$$$  |$$  \\  /$$/ ",
    "$$ |   $$ |  $$ |  $$ |  $$/    $$ |  $$ |  $$ |$$ |__$$ | $$  \\/$$/  ",
    "$$  \\ /$$/   $$ |  $$ |         $$ |  $$ |  $$ |$$    $$<   $$  $$/   ",
    " $$  /$$/    $$ |  $$ |   __    $$ |  $$ |  $$ |$$$$$$$  |   $$$$/    ",
    "  $$ $$/    _$$ |_ $$ \\__/  |   $$ |  $$ \\__$$ |$$ |  $$ |    $$ |    ",
    "   $$$/    / $$   |$$    $$/    $$ |  $$    $$/ $$ |  $$ |    $$ |    ",
    "    $/     $$$$$$/  $$$$$$/     $$/    $$$$$$/  $$/   $$/     $$/     "
};

const string RESET = "\033[0m";
const string INACTIVE = "\033[0;90m";
const string RED = "\033[0;31m";
const string GREEN = "\033[0;32m";
const string YELLOW = "\033[0;33m";
const string BLUE = "\033[0;36m";

const string PLAYER_TILE = "<A>";
const string OPEN_TILE = "   ";
const string CLOSED_TILE = "NNN";
const string LOCATION_TILE = "[L]";

const int DELAY_TIME = 800;
const int DELAY_TIME_SHORT = 400;
const int charMovementPerTimeCounter = 15;
const int bossEncounterDays[3] = { 3, 6, 9 };

const string timesOfDay[7] = { "Dawn", "Morning", "Midday", "Afternoon", "Dusk", "Evening", "Night" };

// Data Structures
vector<string> allItemsVector = { "Life Ring", "Fire Gem", "Black Firebombs", "Test Item"}; // todo change to dynamically load from files
vector<string> keepsakesVector = { "Life Ring", "Fire Gem", "Black Firebombs", "Test Item" };
vector<string> oldSoulsVector = { "Soul of the Wolf Knight", "Soul of the Dragonslayer", "Soul of the Chaos Flame", "Soul of the Paledrake", "Soul of the Darkmoon"};
vector<string> regionsVector = { "Darkroot Depths", "Oolacile Township", "Chasm of the Abyss" };
vector<string> enemiesVector = { "Hollow Warrior", "Balder Knight", "Demonic Foliage", "Living Mushroom", "Knight of Stone", "Crystal Golem", "Black Knight" };
map<string, map<string, string>> keepsakesMap = {};
map<string, map<string, string>> oldSoulsMap = {};
map<string, map<string, string>> regionsMap = {};
map<string, map<string, string>> enemiesMap = {};
vector<vector<string>>  qPressInfo = {
    { "View Itemlist",
    "Change Region",
    "Change Keepsake",
    "Change Old Soul",
    "Start",
    "Life Ring",
    "Fire Gem",
    "Black Firebombs",
    "Test Item",
    "Greatsword",
    "Darkroot Depths",
    "Oolacile Township",
    "Chasm of the Abyss",
    "Soul of the Wolf Knight",
    "Soul of the Dragonslayer",
    "Soul of the Chaos Flame",
    "Soul of the Paledrake",
    "Soul of the Darkmoon",
    "Titanite Demon"},

    { "Items are the main way of getting stronger in ABYSSWALKER. There are many that can be unlocked by meeting specific conditions, allowing them to appear in-game.",
    "There are 3 regions to travel through in ABYSSWALKER. Each has different locations, enemies, bosses, and mechanics. To unlock the next region, you must achieve victory in the prior region.",
    "Keepsakes are items that you start with. They usually provide passive effects, but can sometimes be used in specific situations. More keepsakes can be unlocked by completing specific objectives.",
    "Old Souls are the powerful souls of heroes long gone. If one is selected, you will begin with different stats and items. Old Souls can be found during gameplay, unlocking them permanently.",
    "Select this option to begin the game! Make sure you have chosen your starting gear well.",
    "Begin with +3 HP.",
    "Consumable item. Use at a Blacksmith to add the Fire effect to your current weapon.",
    "Every other turn in combat, attack a second time, inflicting 2 damage. Triggers up to 3 times in one fight.",
    "Unknown.",
    "Base effect: Gain +2 " + colourText("ATK", RED) + ".",
    "A dark and untamed forest with hostile, yet weak, wildlife. The basin at its end is said to hold a great danger.",
    "A decrepit town that has been afflicted by the spread of the Abyss. Its denizens roam without their minds, acting as a potential threat if not dispatched quickly.",
    "The source of Oolacile's blight. A deep and dark chasm that has an unknown source. Can an individual even survive in such a place? There's one way to find out.",
    "You are Sir Artorias, one of Lord Gwyn's Four Great Knights. Your ability with a greatsword is unmatched.",
    "Dragonslayer Ornstein was captain of the Four Great Knights, leading the war against the dragons alongside Lord Gwyn's exiled firstborn.",
    "The Witch of Izalith inherited the Soul of Life, and her daughters bear her legacy. Pyromancy comes naturally from Chaos.",
    "Seath the Scaleless was born lacking the stone scales of his brethren. Turning to forbidden sorceries for a solution, he descended into madness.",
    "Gwyndolin was the third child of Lord Gwyn. Hidden from public view, he lead the Blades of the Darkmoon to dispense justice across Lordran.",
    "Titanite Demon: " + getStats("7", "2", "7", "3") + " \n Every other attack has a 50% chance of inflicting Stun."}
};

// Misc Functions
string colourText(const string& text, const string& colour, const string& reset)
{
    return colour + text + reset;
}

string getStats(string HP, string ATK, string DEF, string SPD)
{
    return colourText(" HP:", GREEN) + " " + HP + " | " + colourText("ATK:", RED) + " " + ATK + " | " + colourText("DEF: ", BLUE) + " " + DEF + " | " + colourText("SPD: ", YELLOW) + SPD;
}

string getStatsBattle(string HP, string maxHP, string ATK, string DEF, string SPD)
{
    return colourText(" HP:", GREEN) + " " + HP + "/" + maxHP + " | " + colourText("ATK:", RED) + " " + ATK + " | " + colourText("DEF: ", BLUE) + " " + DEF + " | " + colourText("SPD: ", YELLOW) + SPD;
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

void displayEndscreen(bool victory)
{
    if (!victory) {
        for (string lossLine : lossDisplay) {
            cout << colourText(lossLine, RED) << "\n";
        }
    }
    else {
        for (string winLine : winDisplay) {
            cout << colourText(winLine, YELLOW) << "\n";
        }
    }
}

void qPressCheck(string currentSelection)
{
    for (int i = 0; i < qPressInfo[0].size(); ++i) {
        if (qPressInfo[0][i] == currentSelection) {
            cout << colourText("\n [INFO] " + qPressInfo[1][i] + "\n", RESET);
        }
    }
}