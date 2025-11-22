
#include "MainMenu.h"
#include "Level.h"
#include "Misc.h"
#include <conio.h>
#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
using namespace std;

string setArea = "Darkroot Depths";
string setKeepsake = "Fire Gem";
string setOldSoul = "Soul of the Wolf Knight";

int main()
{
    while (true) {
		clearScreen();

        MainMenu menu(setArea, setKeepsake, setOldSoul);
        menu.displayMenuOptions();

        Level level(setArea, setKeepsake, setOldSoul);
        level.display();

        clearScreen();
        if (level.endGame == 1) {
            displayEndscreen(false);
        }
        else {
            displayEndscreen(true);
        }

        string displayArray[] = { to_string(level.playerHP), to_string(level.playerATK), to_string(level.playerDEF), to_string(level.playerSPD) };
        cout << "\n\n [Stats]: ";
        cout << getStats(displayArray[0], displayArray[1], displayArray[2], displayArray[3]);

        cout << "\n [Weapon]: " << level.playerWeapon << "\n";
        cout << " [Inventory]:\n";
        for (string item : level.playerInventory) {
            cout << "  - " << item << "\n";
        }

        if (level.endGame == 1) {
            cout << colourText("\n\n 'All of you... forgive me... for I have availed you nothing.'\n", YELLOW);
        }
        else {
            cout << colourText("\n\n 'The vanquished knight left behind only wolf's blood, and legacy of duty. The Undead Legion of Farron was formed to bear his torch eternally.'\n", YELLOW);
        }
        cout << "\n\n Press any key to exit...";
        _getch();
    }
}