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
            for(int i=0; i<player->atkNum; i++) {
                playerAtk(player, enemy);
            }
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
    char descricoes[NUM_CLASSES][MAX_DESC_CLASS] = {
    "Especialista em combate. Tem poucas magias, mas muitas habilidades e dano alto.",
    "Arcanista com uma variedade de feiticos uteis e poderosos.",
    "Usa poderes profanos vindos de um pacto com criaturas extraplanares.",
    "Guerreiro que canaliza o poder de uma divindade para se fortalecer em combate.",
    "Assassino treinado para atacar com letalidade. Nao acerta ataques nao-criticos mas causa dano enorme."}; // Lista das descrições das classes

    printf("\033[1mEscolha sua classe:\033[0m\n\n");

    for(int i=0; i<NUM_CLASSES; i++) {
        printf("\033[33m%i: \033[0m", i+1);
        fputs(nomes[i], stdout);
        printf(" - \033[32m");
        puts(descricoes[i]);
        printf("\033[0m");
    }
}


void chooseClass (playerS *player) {
    int escolha;
    printClasses();

    while (1) {
        printf("> ");
        scanf("%i", &escolha);
        escolha--; // O enum começa em 0, então diminui em 1.

        switch (escolha)
        {
        case warrior:
            // Stats do player
            player->armor = 18;
            player->atkMod = 6;
            player->atkNum = 1;
            player->dmgDice = 6;
            player->dmgDiceNum = 2;
            player->dmgMod = 5;
            player->hpMax = 25;
            player->magMod = -1;
            player->manaMax = 10;

            // Feitiços do player
            addSpell (player, fireBlt);
            addSpell (player, mageShld);

            // Skills do player
            addSkill (player, doubleStrk);
            addSkill (player, tripAtk);
            addSkill (player, parryAtk);
            addSkill (player, scndWind);
            addSkill (player, selfDmg);
            
            // Mensagens de ataque
            strcpy(player->hitString, "\n\nA lamina do seu machado atinge o inimigo, que falha em se esquivar e recua com um grito.\n\n");
            strcpy(player->critString, "\n\nGirando sua arma com as duas maos, voce faz um corte letal no alvo, causando dano massivo.\n\n");
            strcpy(player->missString, "\n\nO oponente desvia agilmente do seu golpe, saltando para o lado. A lamina encontra apenas terra.\n\n");

            break;
        
        case wizard:
            player->armor = 14;
            player->atkMod = 0;
            player->atkNum = 1;
            player->dmgDice = 6;
            player->dmgDiceNum = 1;
            player->dmgMod = -2;
            player->hpMax = 15;
            player->magMod = 6;
            player->manaMax = 25;

            addSpell (player, fireBlt);
            addSpell (player, sonicBlst);
            addSpell (player, mageArm);
            addSpell (player, mageShld);
            addSpell (player, magicMsl);

            addSkill (player, parryAtk);
            addSkill (player, scndWind);
            addSkill (player, selfDmg);

            strcpy(player->hitString, "\n\nApesar da sua falta de treinamento marcial, voce consegue atingir a criatura com o seu cajado, fazendo-a recuar.\n\n");
            strcpy(player->critString, "\n\nVoce acerta o alvo com um giro do seu cajado, com um impacto brutal e o som de ossos quebrando.\n\n");
            strcpy(player->missString, "\n\nVoce golpeia pra frente com a ponta do bastao, mas a armadura do inimigo absorve o impacto do ataque.\n\n");
            
            break;

        case warlock:
            player->armor = 12;
            player->atkMod = 0;
            player->atkNum = 1;
            player->dmgDice = 8;
            player->dmgDiceNum = 1;
            player->dmgMod = 2;
            player->hpMax = 15;
            player->magMod = 6;
            player->manaMax = 20;

            addSpell (player, mageArm);
            addSpell (player, voidHunger);
            addSpell (player, yogSothothSight);
            addSpell (player, cthulhuFire);

            addSkill (player, doubleStrk);
            addSkill (player, tripAtk);
            addSkill (player, bldOffering);
            addSkill (player, selfDmg);
            
            strcpy(player->hitString, "\n\nVoce invoca um feixe de energia sombria que dispara erraticamente pelo ar, atingindo o inimigo e queimando-o.\n\n");
            strcpy(player->critString, "\n\nCom uma palavra profana voce conjura um raio faiscante de sombra, que atinge o alvo em cheio e o empurra pra tras numa chuva de faiscas.\n\n");
            strcpy(player->missString, "\n\nNo calor da batalha, voce nao consegue se concentrar para evocar as energias extraplanares do seu patrono, e o raio se dissipa com um chiado.\n\n");

            break;

        case paladin:
            player->armor = 16;
            player->atkMod = 5;
            player->atkNum = 1;
            player->dmgDice = 8;
            player->dmgDiceNum = 1;
            player->dmgMod = 3;
            player->hpMax = 20;
            player->magMod = 3;
            player->manaMax = 20;

            strcpy(player->hitString, "\n\nO golpe da sua espada acerta o alvo com um corte amplo, abrindo um ferimento e jorrando sangue por onde a lamina rasga.\n\n");
            strcpy(player->critString, "\n\nVoce acerta a criatura com o seu escudo numa investida e finca sua espada num ponto vital, causando um ferimento gravissimo.\n\n");
            strcpy(player->missString, "\n\nO som de metal com metal ressoa pelo campo de batalha quando o seu golpe e bloqueado pelo escudo do inimigo.\n\n");
            
            break;

        case rogue:
            player->armor = 14;
            player->atkMod = 6;
            player->atkNum = 1;
            player->dmgDice = 3;
            player->dmgDiceNum = 4;
            player->dmgMod = 3;
            player->hpMax = 15;
            player->magMod = 2;
            player->manaMax = 15;

            strcpy(player->critString, "\n\nCom precisao cirurgica voce acerta um ataque letal com a sua adaga, girando-a e abrindo um ferimento horrivel.\n\n");
            strcpy(player->missString, "\n\nO seu corte e bloqueado pelo inimigo, raspando contra sua armadura de couro.\n\n");
            
            break;
        
        default:
            printf("Opcao invalida! (tem que ser um numero entre 1 e %i).\n", NUM_CLASSES);
            continue;
            break;
        }

        break;
    }

    player->class = escolha;
}

playerS createPlayer() {
    playerS player;

    // Inicializando os arrays
    initSpells(&player);
    initSkills(&player);
    initInv(&player);
    fillInv(&player);

    // Escolhendo a classe
    chooseClass (&player);

    // Inicializando os atributos
    player.advantage = 0;
    player.hp = player.hpMax;
    player.mana = player.manaMax;
    strcpy(player.name, "Voce");
    

    // Inicializando os status
    for(int i=0; i<NUM_STATUSES; i++) { 
        player.status[i] = false;
    }

    return player;
}

/* ==== Main ==== */

int main(int argc, char** argv) {
    // Declarando variáveis
    int battleState = 0;

    // Declarando criaturas
    playerS player = createPlayer();

    enemyS enemy = {96, 96, 1, 4, 2, 4, 2, 14, 1, "Ze Pequeno, o Anao Denso"};
    for(int i=0; i<NUM_STATUSES; i++) { // Eu vou ser honesto, o jogo nunca acessa o array de status do inimigo, mas por algum motivo se eu não fizer esse loop, o programa dá segfault.
        enemy.status[i] = false;        // Eu não tenho ideia do porque isso aconteceu e porque esse loop conserta, mas tá aí.
    }

    while (1) {
        battleState = updateHp(&player, &enemy);
        updateStatus(&player, &enemy);
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
