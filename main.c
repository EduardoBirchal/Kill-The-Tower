/**
* @file: main
* @brief: Arquivo principal do jogo
* @author: Eduardo Santos Birchal
*/

#include "gameFuncts.h"

/*

COISAS PRA FAZER:

Interface
    - Menu principal
    - Customizar nome
    - Traduzir pra inglês
Inimigo
    - IA do Inimigo <-- FAZENDO
Player
    - Sistema de levels
    - Equipamento
Dungeon
    - Fazer múltiplas salas da dungeon
    - Imprimir o mapa da dungeon
    Recompensas
        - XP
        - Itens
        - Dinheiro
        - Skills e feitiços
        - Equipamento
    - Lojas
    - Fazer o player voltar pro menu ao morrer
    - Boss no fim do andar
    - Múltiplos andares?
Bugs
    - Escudo Arcano te dá -1 de vantagem
    - Barra de vida não imprime direito (HP tá menos que a metade mas a barrinha tá além da metade)

*/


/* ==== Main ==== */

int main(int argc, char** argv) {
    // Semeando o rand()
    srand(time(NULL));

    // Declarando variáveis
    int battleState = 0;

    // Declarando criaturas
    playerS player = createPlayer();
    enemyS enemy = createEnemy((rand()%3) + 1);
    

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
        
        turnEnemy(&player, &enemy);
    }
    printf("\n");


    // Libera arrays alocados
    freePlayer(player);
    freeEnemy(enemy);

    return EXIT_SUCCESS;
}