#include "gameFuncts.h"

/* ==== Opções do jogador ==== */

int playerAtk(playerS *player, enemyS *enemy){
    int atkRoll = rollDice(20, 1, player->atkMod, player->advantage);
    int dmgRoll = 0;

    printSlow("Rolagem de ataque - \033[36mrolando ");
    printf("1d20%+i", player->atkMod);
    rollSlow(atkRoll);
    
    if(atkRoll-player->atkMod > 17) {
        printSlow(" \033[33;4mAcerto Critico!\033[0m Dados de dano dobrados pra esse ataque.\n\n");
        dmgRoll = rollDice(player->dmgDice, player->dmgDiceNum*2, player->dmgMod*2, 0);
        printSlow("Rolagem de dano - \033[36mrolando ");
        printf("%id%i%+i", player->dmgDiceNum*2, player->dmgDice, player->dmgMod*2);
        rollSlow(dmgRoll);
        printSlow("\n\nSua lamina atinge o alvo com precisao brutal, causando dano massivo.\n\n");

        enemy->hp -= dmgRoll;
        return 2;
    } 
    else if (atkRoll >= enemy->armor) {
        printSlow(" \033[33;4mAcerto!\033[0m\n\n");
        dmgRoll = rollDice(player->dmgDice, player->dmgDiceNum, player->dmgMod, 0);
        printSlow("Rolagem de dano - \033[36mrolando ");
        printf("%id%i%+i", player->dmgDiceNum, player->dmgDice, player->dmgMod);
        rollSlow(dmgRoll);
        printSlow("\n\nVoce ataca a criatura, que falha em se esquivar e recua com um grito.\n\n");

        enemy->hp -= dmgRoll;
        return 1;
    }
    else {
        printSlow(" \033[33;4mFalha...\033[0m\n\nO oponente desvia agilmente do seu golpe, saltando para o lado. A lamina encontra apenas terra.\n\n");
        return 0;
    }
}


/* ==== Funções do turno do jogador ==== */

void printOptions() {
    char options[OPTION_AMT][MAX_OPTION] = {"ATACAR", "HABILIDADES", "MAGIA", "INVENTARIO", "ACOES", "FECHAR JOGO"};
    int option = 0;

    printf("\n");
    for(int i=0; i<OPTION_AMT; i++) {
        printf("%s\t", options[i]);
    }
    printf("\n");
}

int readOption(playerS *player, enemyS *enemy) {
    int option = 1;

    while (1) {
        printf("\nEscolha uma opcao:\n> ");
        scanf("%i", &option);
        printf("\n");

        /*while (1) {
            uint8_t *state = SDL_GetKeyboardState(NULL);
            if (state[80]) {
                option--;
            }
            if (state[79]) {
                option++;
                printf("%i ", option);
            }
        }*/
        switch (option)
        {
        case 1:
            printInfo(*player, *enemy);
            playerAtk(player, enemy);
            requestEnter();
            return 0;
            break;
        case 2:
            printInfo(*player, *enemy);
            if (playerSkl(player, enemy)) {
                printInfo(*player, *enemy);
                return 0;
            }
            requestEnter();
            return 0;
            break;
        case 3:
            printInfo(*player, *enemy);
            if (playerMag(player, enemy)) {
                printInfo(*player, *enemy);
                return 0;
            }
            requestEnter();
            return 0;
            break;
        case 4:
            //playerInv(player);
            requestEnter();
            return 0;
            break;
        case 5:
            printInfo(*player, *enemy);
            //playerAct(player);
            requestEnter();
            return 0;
            break;
        case 6:
            return 1;
            break;
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
    initInv(player);
    fillInv(player);

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
            break;
        }
        //turnEnemy(&player, &enemy);
    }
    printf("\n");

    return EXIT_SUCCESS;
}
