
#include "MainMenu.h"
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
    /*checkForSave();
    displayGamePreparations();
    generateWorld();*/

    MainMenu menu(setArea, setKeepsake, setOldSoul);
    menu.displayMenuOptions();

    // TODO next: re-add in basic level. should be simple, just use the old code.
    /*Level level();
    level.display();*/
}