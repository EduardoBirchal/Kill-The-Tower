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
    - Aprimorar IA do Inimigo <-- FAZENDO
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
Balanceamento
    - É muito fácil morrer, especialmente pras classes de magia
    - Pouca variedade pro paladino e pouca quantidade de combo
    - Inimigo fortifica demais
    - Rebalancear armadura (armadura demais deixa a batalha chata, mas armadura de menos elimina qualquer estratégia que não seja atacar sem parar)
Código
    - Fazer player e inimigo serem static

*/


/* ==== Variáveis Globais ==== */
int showDesc = true; // Mostrar descrição dos feitiços e habilidades

playerS player;
enemyS enemy;


/* ==== Main ==== */

int main(int argc, char** argv) {
    // Semeando o rand()
    srand(time(NULL));

    // Declarando variáveis
    int battleState = 0;

    // Definindo criaturas
    player = createPlayer();
    //enemy = createEnemy((rand()%3) + 1);
    enemy = createEnemy(2);
    

    while (1) {
        updateValues(&player, &enemy);
        updateStatus(&player, &enemy);
        updateCooldowns(&player, &enemy);
        
        if (battleIsOver(player, enemy)) break; // Se a batalha acabou, quebra o loop.

        if (turnPlayer(&player, &enemy)) {
            break;
        }

        if (battleIsOver(player, enemy)) break; // Checa se a batalha acabou antes e depois do turno do player, pra ninguém poder agir quando já morreu.
        
        turnEnemy(&player, &enemy);
    }
    printf("\n");


    // Libera arrays alocados
    freePlayer(player);
    freeEnemy(enemy);

    return EXIT_SUCCESS;
}
