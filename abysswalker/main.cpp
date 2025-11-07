
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

// Game Loop
int main()
{
    /*checkForSave();
    displayGamePreparations();
    generateWorld();*/

    MainMenu menu(setArea, setKeepsake, setOldSoul);
    menu.displayMenuOptions();
}