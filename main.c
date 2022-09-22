#include "gameFuncts.h"


/* ==== Funções do turno do jogador ==== */

// Imprime as opções do player
void printOptions() {
    char options[OPTION_AMT][MAX_OPTION] = {"ATACAR", "HABILIDADES", "MAGIA", "INVENTARIO", "ACOES", "FECHAR JOGO"};
    int option = 0;

    printf("\n");
    for(int i=0; i<OPTION_AMT; i++) {
        printf("%s\t", options[i]);
    }
    printf("\n");
}

// Lê a opção que o player escolheu
int readOption(playerS *player, enemyS *enemy) {
    int option = 1;

    while (1) {
        printf("\nEscolha uma opcao:\n> ");
        scanf("%i", &option);
        printf("\n");

        switch (option)
        {
        // Ataque
        case 1: 
            printInfo(*player, *enemy);
            playerAtk(player, enemy);
            requestEnter();
            return 0;
            break;

        // Skills
        case 2: 
            printInfo(*player, *enemy);
            if (playerSkl(player, enemy)) {
                printInfo(*player, *enemy);
                return 0;
            }
            requestEnter();
            return 0;
            break;

        // Magia
        case 3: 
            printInfo(*player, *enemy);
            if (playerMag(player, enemy)) {
                printInfo(*player, *enemy);
                return 0;
            }
            requestEnter();
            return 0;
            break;

        // Inventário
        case 4: 
            if (playerInv(player, enemy)) {
                printInfo(*player, *enemy);
                return 0;
            }
            requestEnter();
            return 0;
            break;

        // Ações
        case 5: 
            printInfo(*player, *enemy);
            //playerAct(player);
            requestEnter();
            return 0;
            break;

        // Cancelar
        case 6: 
            return 1;
            break;

        // Opção inválido
        default:
            printInfo(*player, *enemy);
            printf("Invalid option! (has to be number between 1 and %i).\n", OPTION_AMT);
            break;
        }
    }
}

int turnPlayer(playerS *player, enemyS *enemy) {
    printOptions();
    if(readOption(player, enemy)) {
        return 1; // Retorna 1, fazendo com que o programa feche
    }

    return 0;
}

playerS createPlayer() {
    // Inicializando os atributos
    playerS player = {21, 21, 1, 8, 3, 5, 1, 16, 6, "Voce", 0};
    
    // Inicializando os status
    for(int i=0; i<NUM_STATUSES; i++) { 
        player.status[i] = false;
    }

    // Inicializando o inventário
    initInv(&player);
    fillInv(&player);

    return player;
}

/* ==== Main ==== */

int main(int argc, char** argv) {
    // Declarando variáveis
    int battleState = 0;

    // Declarando criaturas
    playerS player = createPlayer();
    enemyS enemy = {96, 96, 1, 4, 2, 4, 2, 14, 1, "Ze Pequeno, o Anao Denso"};

    while (1) {
        battleState = updateHp(&player, &enemy);
        updateStatus(&player);
        printInfo(player, enemy);
        if(battleState) {
            if (battleState == 1) {
                printSlow("O goblin cai no chao, derrotado.\n");
            }
            else if (battleState == 2) {
                printSlow("Voce sucumbe aos seus ferimentos e desmaia.\n");
            }
            break;
        }
        if (turnPlayer(&player, &enemy)) {
            free (player.inventory);
            break;
        }
        //turnEnemy(&player, &enemy);
    }
    printf("\n");

    return EXIT_SUCCESS;
}
