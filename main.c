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
    - Traduzir pra inglês (usar gettext. Pra refatorar o código, aprender regex)
        REGEX:
            find print(f|Slow)\(("[^"]*")
            replace print$1(_($2)
    - Fazer o terminal ser controlável via setinhas
Inimigo
    - Aprimorar IA do Inimigo
Player
    - Sistema de levels
    - Equipamento
Dungeon
    - Fazer múltiplas salas da dungeon
    - Imprimir o mapa da dungeon
    - Recompensas
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
    createPlayer();
    
    //createEnemy((rand()%3) + 1);
    createEnemy(2);
    

    while (true) {
        updateValues();
        updateStatus();
        updateCooldowns();
        
        if (battleIsOver()) break; // Se a batalha acabou, quebra o loop.

        if (turnPlayer()) {
            break;
        }

        if (battleIsOver()) break; // Checa se a batalha acabou antes e depois do turno do player, pra ninguém poder agir quando já morreu.
        
        turnEnemy();
    }
    printf("\n");


    // Libera arrays alocados
    freePlayer();
    freeEnemy();

    return EXIT_SUCCESS;
}
