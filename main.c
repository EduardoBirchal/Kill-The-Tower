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
            printf("Opcao invalida! (tem que ser um numero entre 1 e %i).\n", OPTION_AMT);
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

// Imprime as classes pro player escolher
void printClasses() {
    clearTerm();

    centerText (strlen("Escolha sua classe:"), BORDER_LEN);
    char nomes[NUM_CLASSES][MAX_ITEM] = {"Guerreiro", "Mago", "Bruxo", "Paladino", "Ladrao"}; // Lista dos nomes das classes

    printf("\033[1mEscolha sua classe:\033[0m\n\n");

    for(int i=0; i<NUM_CLASSES; i++) {
        printf("\033[33m%i: \033[0m", i+1);
        puts(nomes[i]);
    }
}


playerS chooseClass (playerS player) {
    int escolha;
    printClasses();

        printf("> ");
        scanf("%i", &escolha);
        escolha--; // O enum começa em 0, então diminui em 1.

    while (1) {
        switch (escolha)
        {
        case warrior:
            player.armor = 18;
            player.atkMod = 6;
            player.atkNum = 1;
            player.dmgDice = 6;
            player.dmgDiceNum = 2;
            player.dmgMod = 6;
            player.hpMax = 25;
            player.magMod = -1;
            player.manaMax = 10;
            
            break;
        
        case wizard:
            player.armor = 14;
            player.atkMod = 0;
            player.atkNum = 1;
            player.dmgDice = 6;
            player.dmgDiceNum = 1;
            player.dmgMod = -2;
            player.hpMax = 15;
            player.magMod = 6;
            player.manaMax = 25;
            
            break;

        case warlock:
            player.armor = 12;
            player.atkMod = 0;
            player.atkNum = 1;
            player.dmgDice = 8;
            player.dmgDiceNum = 1;
            player.dmgMod = 2;
            player.hpMax = 15;
            player.magMod = 6;
            player.manaMax = 20;
            
            break;

        case paladin:
            player.armor = 16;
            player.atkMod = 5;
            player.atkNum = 1;
            player.dmgDice = 8;
            player.dmgDiceNum = 1;
            player.dmgMod = 3;
            player.hpMax = 20;
            player.magMod = 3;
            player.manaMax = 20;
            
            break;

        case rogue:
            player.armor = 14;
            player.atkMod = 6;
            player.atkNum = 1;
            player.dmgDice = 4;
            player.dmgDiceNum = 1;
            player.dmgMod = 4;
            player.hpMax = 15;
            player.magMod = 2;
            player.manaMax = 15;
            
            break;
        
        default:
            printf("Opcao invalida! (tem que ser um numero entre 1 e %i).\n", NUM_CLASSES);
            continue;
            break;
        }

        break;
    }

    player.class = escolha;
    return player;
}

playerS createPlayer() {
    playerS player;
    player = chooseClass (player);

    // Inicializando os atributos
    player.advantage = 0;
    player.hp = player.hpMax;
    player.mana = player.manaMax;
    strcpy(player.name, "Voce");
    
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
