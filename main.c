#include <stdio.h>
#include <stdlib.h>

struct player_s {
    int hpMax, hp, dmgMin, dmgMax, atkMod, atkNum, armor;
}

int main(int argc, char* argv) {
    while(combat) {
        printInfo();
        turnPlayer();
        turnEnemy();
    }

    return EXIT_SUCCESS;
}