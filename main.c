#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#ifdef _WIN32 || _WIN64
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

#define MAX_NAME 25
#define BORDER_LEN 96
#define MAX_OPTION 15
#define OPTION_AMT 6


/* ==== Definindo structs ==== */

struct player_s {
    int hpMax, hp, dmgDiceNum, dmgDice, dmgMod, atkMod, atkNum, armor;
    char name[MAX_NAME];
};

struct enemy_s {
    int hpMax, hp, dmgDiceNum, dmgDice, dmgMod, atkMod, atkNum, armor;
    char name[MAX_NAME];
};

typedef struct player_s playerS;
typedef struct enemy_s enemyS;


/* ==== Funções gerais de combate ==== */

int rollDice(int size, int num, int mod) {
    int total = 0;

    for(int i=0; i<num; i++) {
        srand(time(NULL) + i); // O +i é pra seed ser diferente todo loop. Não tenho certeza do porquê de ela ser igual todo loop sem esse +i, já que ele dá o srand todo loop, mas isso faz funcionar.
        total += 1 + rand()%size;
        //printf("\t%io dado: %i\n", i+1, total);
    }
    return total+mod;
}

void requestEnter() {
    printf("Press ENTER to continue.");
    getchar();
    getchar();
}


/* ==== Funções gerais de impressão ==== */

void clearTerm() {
    printf("\033[2J\033[1;1H"); // Só funciona com windows. É um regex que limpa o terminal, muitas vezes mais rápido que o system("clear")
}

void centerText (int len, int borderPos) {
    for(int i=0; i<((borderPos/2) - (len/2)); i++) {
        printf(" ");
    }
}

void printHp (int hpMax, int hp) {
    printf("|");
    for(int i=1; i<=hpMax; i++) { 
        if(i <= (hp)) {
            printf("#");
        }
        else {
            printf(" ");
        }
    }
    printf("|\n\n");
}

int updateHp (playerS *player, enemyS *enemy) {
    if(enemy->hp < 0) enemy->hp = 0;
    if(enemy->hp > enemy->hpMax) enemy->hp = enemy->hpMax;
    if(player->hp < 0) player->hp = 0;
    if(player->hp > player->hpMax) player->hp = player->hpMax;

    if(enemy->hp == 0) return 1;
    if(player->hp == 0) return 2;
    return 0;
}

int digitNum (int num) {
    int digits = 1;

    if (num > 10) {
        return digits + digitNum(num/10);
    }
    else {
        return digits;
    }
}

void trueSleep(int ms) {
    #ifdef _WIN32 || _WIN64
        //Sleep(1000); // Sleep 1 segundo
        Sleep(ms); // Sleep 0,5 segundo
      #else
        //sleep(1); // Sleep 1 segundo
        usleep(ms*1000);  // Sleep 0,5 segundo (500 milisegundos)
      #endif
}


/* ==== Funções de imprimir atributos ==== */

void playerInfo(playerS player) {
    int hpMaxLen = digitNum(player.hpMax); //(int) log10f(player.hpMax) + 1;
    int hpLen = digitNum(player.hp); //(int) log10f(player.hp) + 1;
    hpLen = hpLen + hpMaxLen + 4; // hpLen é o comprimento da string do valor do hp do player, que vai ser escrita depois do nome. O +4 é por causa do espaço, dos dois parênteses e da barra

    // Imprime o nome
    centerText(strlen(player.name) + hpLen, BORDER_LEN);
    printf("%s (%i/%i)\n", player.name, player.hp, player.hpMax);

    // Imprime o hp e os atributos
    centerText(player.hpMax + 2, BORDER_LEN); // O +2 é por causa das duas barras
    printHp(player.hpMax, player.hp);
    printf("Attack Roll: 1d20%+i\tDamage Roll: %id%i%+i\tArmor: %i\n", player.atkMod, player.dmgDiceNum, player.dmgDice, player.dmgMod, player.armor);
}

void enemyInfo(enemyS enemy) {
    int hpMaxLen = digitNum(enemy.hpMax); //(int) log10f(enemy.hpMax) + 1;
    int hpLen = digitNum(enemy.hp); //(int) log10f(enemy.hp) + 1;
    hpLen = hpLen + hpMaxLen + 4; 

    // Imprime a margem de cima
    for(int i=0; i<BORDER_LEN; i++) { 
        printf("-");
    }
    printf("\n");

    // Imprime o nome
    centerText(strlen(enemy.name) + hpLen, BORDER_LEN);
    printf("%s (%i/%i)\n", enemy.name, enemy.hp, enemy.hpMax);

    // Imprime o hp e os atributos
    centerText(enemy.hpMax + 2, BORDER_LEN);
    printHp(enemy.hpMax, enemy.hp);
    printf("Attack Roll: 1d20%+i\tDamage Roll: %id%i%+i\tArmor: %i\n", enemy.atkMod, enemy.dmgDiceNum, enemy.dmgDice, enemy.dmgMod, enemy.armor);

}

void printInfo (playerS player, enemyS enemy) {
    // Limpa a tela
    clearTerm(); 

    // Imprime a margem de cima
    for(int i=0; i<BORDER_LEN; i++) { 
        printf("-");
    }
    printf("\n");

    // Imprime a informação
    playerInfo(player);
    enemyInfo(enemy);

    // Imprime a margem de baixo
    for(int i=0; i<BORDER_LEN; i++) { 
        printf("-");
    }
    printf("\n\n");
}


/* ==== Opções do jogador ==== */

void playerAtk(playerS *player, enemyS *enemy){
    int atkRoll = rollDice(20, 1, player->atkMod);
    int dmgRoll = 0;

    printf("Attack roll result (1d20%+i): %i\n\n", player->atkMod, atkRoll);
    if(atkRoll-player->atkMod > 17) {
        printf("[Critical Hit] Rolling double the damage dice.\n");
        dmgRoll = rollDice(player->dmgDice, player->dmgDiceNum*2, player->dmgMod*2);
        printf("Damage roll result (%id%i%+i): %i\n\n", player->dmgDiceNum*2, player->dmgDice, player->dmgMod*2, dmgRoll);
        printf("Critical hit! Your blade slashes the goblin's torso with brutal strength, dealing massive damage.\n\n");
    } 
    else if (atkRoll >= enemy->armor) {
        printf("[Hit] Rolling damage dice.\n");
        dmgRoll = rollDice(player->dmgDice, player->dmgDiceNum, player->dmgMod);
        printf("Damage roll result (%id%i%+i): %i\n\n", player->dmgDiceNum, player->dmgDice, player->dmgMod, dmgRoll);
        printf("You swing at the creature, who fails to evade the attack and recoils in pain.\n\n");
    }
    else {
        printf("[Miss] You swing downwards at your opponent, who nimbly dodges to the side. The blade meets only dirt.\n\n");
    }

    enemy->hp -= dmgRoll;
}


/* ==== Funções do turno do jogador ==== */

void printOptions() {
    char options[OPTION_AMT][MAX_OPTION] = {"ATTACK", "SKILLS", "MAGIC", "INVENTORY", "ACTIONS", "CLOSE GAME"};
    int option = 0;

    for(int i=0; i<OPTION_AMT; i++) {
        printf("[%i] %s\t", i+1, options[i]);
    }
    printf("\n");
}

int readOption(playerS *player, enemyS *enemy) {
    int option = 0;

    while (1) {
        printf("Choose an option:\n> ");
        scanf("%i", &option);
        printf("\n");

        switch (option)
        {
        case 1:
            playerAtk(player, enemy);
            requestEnter();
            return 0;
            break;
        case 2:
            //playerSkl(player);
            requestEnter();
            return 0;
            break;
        case 3:
            //playerMag(player);
            requestEnter();
            return 0;
            break;
        case 4:
            //playerInv(player);
            requestEnter();
            return 0;
            break;
        case 5:
            //playerAct(player);
            requestEnter();
            return 0;
            break;
        case 6:
            return 1;
            break;
        default:
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


/* ==== Main ==== */

int main(int argc, char** argv) {
    // Declarando variáveis
    int battleState = 0;

    // Declarando criaturas
    playerS player = {21, 21, 1, 8, 3, 5, 1, 16, "You"};
    enemyS enemy = {32, 32, 1, 4, 2, 4, 2, 14, "Goblin"};

    while (1) {
        battleState = updateHp(&player, &enemy);
        if(battleState) {
            if (battleState == 1) {
                printf("The goblin falls to the ground, defeated.\n");
            }
            else if (battleState == 2) {
                printf("You succumb to your wounds and black out.\n");
            }
            break;
        }
        printInfo(player, enemy);
        if (turnPlayer(&player, &enemy)) {
            break;
        }
        //turnEnemy(&player, &enemy);
    }
    printf("\n");

    return EXIT_SUCCESS;
}
