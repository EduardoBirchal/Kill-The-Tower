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
int rollDice(int size, int num, int mod) {
    int total = 0;

    for(int i=0; i<num; i++) {
        srand(time(NULL) + i); // O +i é pra seed ser diferente todo loop. Não tenho certeza do porquê de ela ser igual todo loop sem esse +i, já que ele dá o srand todo loop, mas isso faz funcionar.
        total += 1 + rand()%size;
        //printf("\t%io dado: %i\n", i+1, total); // Só usar pra debug.
    }
    return total+mod;
}