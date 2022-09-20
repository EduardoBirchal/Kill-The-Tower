#include "gameFuncts.h"

// Imprime uma barra de vida
void printHp (int hpMax, int hp) {
    for(int i=1; i<=hpMax; i++) {
        if(i <= (hp)) {
            printf("\033[102m "); // Imprime em verde as células de HP cheio e o resto (hp vazio) imprime em verde escuro
        }
        else {
            printf("\033[42m ");
        }
    }
    printf("\033[0m");
    printf("\n\n");
}

// Conserta o HP pra não ficar negativo
int updateHp (playerS *player, enemyS *enemy) {
    if(enemy->hp < 0) enemy->hp = 0;
    if(enemy->hp > enemy->hpMax) enemy->hp = enemy->hpMax;
    if(player->hp < 0) player->hp = 0;
    if(player->hp > player->hpMax) player->hp = player->hpMax;

    if(enemy->hp == 0) return 1; // Retorna >0 se alguem morreu
    if(player->hp == 0) return 2;
    return 0;
}

// Rola [num] dados de [size] lados
int rollDice(int size, int num, int mod, int adv) {
    int total;
    int roll = 0;

    for(int j=0; j<num; j++) {
        roll += 1 + rand()%size;
        //printf("\t%io dado: %i\n", j+1, roll); // Só usar pra debug.
    }
    total = roll;

    for(int i=0; i<adv; i++) {
        roll = 0;
        for(int j=0; j<num; j++) {
            roll += 1 + rand()%size;
            //printf("\t%io dado: %i\n", j+1, roll); // Só usar pra debug.
        }
        if (roll>total) total=roll;
    }

    return total+mod;
}