#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h> // For usleep()
#include <iomanip> // For setprecision()
#include "Global.h"
#include "BNO.h"
#include "Tools.h"

// Catches 'x' keyboard hit to go back one menu
#define GO_BACK (kbhit() && getchar() == 'x')

void BNOMenu();
bool validSelection(string);
