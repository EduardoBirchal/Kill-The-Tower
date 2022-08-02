#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_NAME 25
#define BORDER_LEN 96

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

void clearTerm() {
    printf("\033[2J\033[1;1H");
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

void centerText (int len, int borderPos) {
    for(int i=0; i<((borderPos/2) - (len/2)); i++) {
        printf(" ");
    }
}

void playerInfo(playerS player) {
    int hpMaxLen = (int) log10f(player.hpMax) + 1;
    int hpLen = (int) log10f(player.hp) + 1;
    hpLen = hpLen + hpMaxLen + 4; // hpLen é o comprimento da string do valor do hp do player, que vai ser escrita depois do nome. O +4 é por causa do espaço, dos dois parênteses e da barra.

    // Imprime o nome
    centerText(strlen(player.name) + hpLen, BORDER_LEN);
    printf("%s (%i/%i)\n", player.name, player.hp, player.hpMax);

    // Imprime o hp e os atributos
    centerText(player.hpMax + 2, BORDER_LEN); // O +2 é por causa das duas barras
    printHp(player.hpMax, player.hp);
    printf("Attack Roll: 1d20%+i\tDamage Roll: %id%i%+i\tArmor: %i\n", player.atkMod, player.dmgDiceNum, player.dmgDice, player.dmgMod, player.armor);
}

void enemyInfo(enemyS enemy) {
    int hpMaxLen = (int) log10f(enemy.hpMax) + 1;
    int hpLen = (int) log10f(enemy.hp) + 1;
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

int main(int argc, char* argv) {
    // Declarando variáveis
    int comando;

    // Declarando criaturas
    playerS player = {32, 30, 4, 6, 5, 3, 1, 16, "Bababooey"};
    enemyS enemy = {21, 9, 2, 10, 5, -2, 2, 10, "Bobao Feio"};

    while(comando != 69) {
        printInfo(player, enemy);
        printf("> ");
        scanf("%i", &comando);
        player.hp += comando;
        //turnPlayer();
        //turnEnemy();
    }

    return EXIT_SUCCESS;
}