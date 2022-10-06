#include "gameFuncts.h"


/* ==== Funções do turno do jogador ==== */

// Imprime as opções do player
void printOptions(playerS *player, enemyS *enemy) {
    char options[OPTION_AMT][MAX_OPTION] = {"ATACAR", "HABILIDADES", "MAGIA", "INVENTARIO", "OPCOES", "FECHAR JOGO"};
    int option = 0;

    printInfo(*player, *enemy);
    printf("\n");
    for(int i=0; i<OPTION_AMT; i++) {
        printf("\033[33m(%i)\033[0m %s%s", i+1, options[i], TAB); // O \t não dá um número consistente de espaços, então eu fiz uma string contante TAB que é só 6 espaços. É feio, mas só custa um pouquinho de tempo de compilação.
    }
    printf("\n");
}

// Lê a opção que o player escolheu
int readOption(playerS *player, enemyS *enemy) {
    int option = 1;

    while (1) {
        option = getOption();

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
                return 1;
            }
            requestEnter();
            return 0;
            break;

        // Magia
        case 3: 
            printInfo(*player, *enemy);
            if (playerMag(player, enemy)) {
                return 1;
            }
            requestEnter();
            return 0;
            break;

        // Inventário
        case 4: 
            if (playerInv(player, enemy)) {
                return 1;
            }
            requestEnter();
            return 0;
            break;

        // Opções
        case 5:
            printInfo(*player, *enemy);
            if (playerConfig(player, enemy)) {
                return 1;
            }
            requestEnter();
            return 0;
            break;

            return 0;
            break;

        // Cancelar
        case 6: 
            return 2;
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
    while(1) {
        printOptions(player, enemy);
        switch (readOption(player, enemy))
        {
        case 0:
            return 0;
            break; // O player escolheu e executou uma opção.
        
        case 1:
            break; // O player escolheu uma opção e cancelou.

        case 2:
            return 1;
            break; // O player executou "Fechar Jogo".
        }
    }

    return 0;
}

// Imprime as classes pro player escolher
void printClasses() {
    clearTerm();

    centerText (strlen("Escolha sua classe:"), BORDER_LEN);
    char nomes[NUM_CLASSES][MAX_ITEM] = {"Guerreiro", "Mago", "Bruxo", "Paladino"}; // Lista dos nomes das classes
    char descricoes[NUM_CLASSES][MAX_DESC_CLASS] = {
    "Especialista em combate. Tem poucas magias, mas muitas habilidades e dano alto.",
    "Arcanista com uma variedade de feiticos uteis e poderosos.",
    "Usa poderes profanos vindos de um pacto com criaturas extraplanares.",
    "Guerreiro que canaliza o poder de uma divindade para se fortalecer em combate."}; // Lista das descrições das classes

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
            player->manaMax = 15;

            // Feitiços do player
            addSpell (player, fireBlt);
            addSpell (player, mageShld);

            // Skills do player
            addSkill (player, doubleStrk);
            addSkill (player, tripAtk);
            addSkill (player, parryAtk);
            addSkill (player, scndWind);
            addSkill (player, adrnlSurge);
            
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
            player->manaMax = 30;

            addSpell (player, fireBlt);
            addSpell (player, sonicBlst);
            addSpell (player, mageArm);
            addSpell (player, mageShld);
            addSpell (player, magicMsl);

            addSkill (player, parryAtk);
            addSkill (player, scndWind);

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
            player->manaMax = 25;

            addSpell (player, mageArm);
            addSpell (player, voidHunger);
            addSpell (player, yogSothothSight);
            addSpell (player, cthulhuFire);
            addSpell (player, azathothDream);

            addSkill (player, doubleStrk);
            addSkill (player, tripAtk);
            addSkill (player, bldOffering);
            
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
            player->manaMax = 25;

            addSpell (player, mageArm);
            addSpell (player, blessWpn);
            addSpell (player, rdntSmite);
            addSpell (player, srngLight);

            addSkill (player, parryAtk);
            addSkill (player, tripAtk);
            addSkill (player, dvnGuidance);
            addSkill (player, bldOffering);

            strcpy(player->hitString, "\n\nO golpe da sua espada acerta o alvo com um corte amplo, abrindo um ferimento e jorrando sangue por onde a lamina rasga.\n\n");
            strcpy(player->critString, "\n\nVoce acerta a criatura com o seu escudo numa investida e finca sua espada num ponto vital, causando um ferimento gravissimo.\n\n");
            strcpy(player->missString, "\n\nO som de metal com metal ressoa pelo campo de batalha quando o seu golpe e bloqueado pelo escudo do inimigo.\n\n");
            
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
    // Semeando o rand()
    srand(time(NULL));

    // Declarando variáveis
    int battleState = 0;
    static int showDesc = -1; // Se for 1, mostra as descrições das coisas.

    // Declarando criaturas
    playerS player = createPlayer();

    enemyS enemy = createEnemy ((rand()%3) + 1);
    
    for(int i=0; i<NUM_STATUSES; i++) { // Eu vou ser honesto, o jogo nunca acessa o array de status do inimigo, mas por algum motivo se eu não fizer esse loop, o programa dá segfault.
        enemy.status[i] = false;        // Eu não tenho ideia do porque isso aconteceu e porque esse loop conserta, mas tá aí.
    }

    while (1) {
        updateHp(&player, &enemy);
        updateStatus(&player, &enemy);
        updateCooldown(&player);
        battleState = updateHp(&player, &enemy); // Faz o updateHp antes e depois de atualizar os status, pra nao mostrar HP negativo.
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
            free (player.inventory);
            break;
        }
        //turnEnemy(&player, &enemy);
    }
    printf("\n");

    return EXIT_SUCCESS;
}
