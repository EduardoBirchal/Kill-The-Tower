/**
* @file: main
* @brief: Arquivo principal do jogo
* @author: Eduardo Santos Birchal
*/

#include "gameFuncts.h"


/* ==== Main ==== */

int main(int argc, char** argv) {
    // Semeando o rand()
    srand(time(NULL));

    // Declarando variáveis
    int battleState = 0;

    // Declarando criaturas
    playerS player = createPlayer();
    enemyS enemy = createEnemy ((rand()%3) + 1);
    

    while (1) {
        updateValues(&player, &enemy);
        updateStatus(&player, &enemy);
        updateCooldown(&player);
        battleState = updateValues(&player, &enemy); // Faz o updateValues antes e depois de atualizar os status, pra nao mostrar HP nem mana negativo.
        if(battleState) {
            printInfo(player, enemy); // Se alguém morreu, imprime a tela pra mostrar quem foi.
            if (battleState == 1) {
                printSlow("O inimigo cai no chao, derrotado.\n");
            }
            else if (battleState == 2) {
                printSlow("Voce sucumbe aos seus ferimentos e desmaia.\n");
            }
            break;
        }
        if (turnPlayer(&player, &enemy)) {
            break;
        }
        enemyAtk(&player, &enemy);
        //turnEnemy(&player, &enemy);
    }
    printf("\n");

    free (player.inventory);
    return EXIT_SUCCESS;
}
