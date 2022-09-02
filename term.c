#include "gameFuncts.h"

// Imprime para apertar enter e não continua até o usuário apertar
void requestEnter() {
    printf("Press ENTER to continue.");
    getchar(); // Primeiro getchar pega o \n da ultima mensagem
    getchar();
}

// Sleep que funciona em Windows e Linux
void trueSleep(int ms) {
    #ifdef _WIN32
        //Sleep(1000); // Sleep 1 segundo
        Sleep(ms);
    #else
        //sleep(1); // Sleep 1 segundo
        usleep(ms*1000);
    #endif
}

// Um regex que limpa o terminal, muitas vezes mais rápido que o system("clear") e é multiplataforma
void clearTerm() {
    printf("\033[1;1H\033[2J"); 
}

// Imprime espaços pra centralizar a próxima impressão em uma margem
void centerText (int len, int borderPos) {
    for(int i=0; i<((borderPos/2) - (len/2)); i++) {
        printf(" ");
    }
}

// Imprime uma barra de vida
void printHp (int hpMax, int hp) {
    for(int i=1; i<=hpMax; i++) {
        if(i <= (hp)) {
            printf("\033[102m ");
        }
        else {
            printf("\033[42m ");
        }
    }
    printf("\033[0m");
    printf("\n\n");
}

// Conserta o HP pra não ficar negativo
int updateHp (playerS *player, enemyS *enemy) {
    if(enemy->hp < 0) enemy->hp = 0;
    if(enemy->hp > enemy->hpMax) enemy->hp = enemy->hpMax;
    if(player->hp < 0) player->hp = 0;
    if(player->hp > player->hpMax) player->hp = player->hpMax;

    if(enemy->hp == 0) return 1;
    if(player->hp == 0) return 2;
    return 0;
}

// Conta o número de dígitos de um número recursivamente
int digitNum (int num) {
    int digits = 1;

    if (num > 10) {
        return digits + digitNum(num/10);
    }
    else {
        return digits;
    }
}

// Imprime uma mensagem devagar
void printSlow (char string[]) {
    for(int i=0; string[i]!='\0'; i++) {
        printf("%c", string[i]);
        fflush(stdout);
        if(string[i] != '\n') {
            trueSleep(25);
        }
    }
}

// Imprime o resultado de um dado devagar
void rollSlow (int result) {
    printSlow("...");
    fflush(stdout);
    trueSleep(800);
    printf(" %i.\033[0m ", result);
    fflush(stdout);
    trueSleep(150);
}

// Imprime uma margem
void printBorder () {
    printf("\033[93m ");
    for(int i=0; i<BORDER_LEN; i++) { 
        printf("-");
    }
    printf("\033[0m ");
    printf("\n");
}

// Imprime os atributos do player
void playerInfo(playerS player) {
    int hpMaxLen = digitNum(player.hpMax);
    int hpLen = digitNum(player.hp);
    hpLen = hpLen + hpMaxLen + 4; // hpLen é o comprimento da string do valor do hp do player, que vai ser escrita depois do nome. O +4 é por causa do espaço, dos dois parênteses e da barra

    // Imprime o nome
    centerText(strlen(player.name) + hpLen, BORDER_LEN);
    printf("\033[1m ");
    printf("%s (%i/%i)\n", player.name, player.hp, player.hpMax);
    printf("\033[0m ");

    // Imprime o hp e os atributos
    centerText(player.hpMax, BORDER_LEN);
    printHp(player.hpMax, player.hp);
    printf("Rolagem de Ataque: \033[4m1d20%+i\033[0m\tRolagem de Dano: \033[4m%id%i%+i\033[0m\tArmadura: \033[4m%i\033[0m\n", player.atkMod, player.dmgDiceNum, player.dmgDice, player.dmgMod, player.armor);
}

// Imprime os atributos do inimigo
void enemyInfo(enemyS enemy) {
    int hpMaxLen = digitNum(enemy.hpMax); //(int) log10f(enemy.hpMax) + 1;
    int hpLen = digitNum(enemy.hp); //(int) log10f(enemy.hp) + 1;
    hpLen = hpLen + hpMaxLen + 4; 

    // Imprime a margem de cima
    printBorder();

    // Imprime o nome
    centerText(strlen(enemy.name) + hpLen, BORDER_LEN);
    printf("\033[1m ");
    printf("%s (%i/%i)\n", enemy.name, enemy.hp, enemy.hpMax);
    printf("\033[0m ");

    // Imprime o hp e os atributos
    centerText(enemy.hpMax, BORDER_LEN);
    printHp(enemy.hpMax, enemy.hp);
    printf("Rolagem de Ataque: \033[4m1d20%+i\033[0m\tRolagem de Dano: \033[4m%id%i%+i\033[0m\tArmadura: \033[4m%i\033[0m\n", enemy.atkMod, enemy.dmgDiceNum, enemy.dmgDice, enemy.dmgMod, enemy.armor);

}

// Imprime o menu de atributos
void printInfo (playerS player, enemyS enemy) {
    // Limpa a tela
    clearTerm(); 

    // Imprime a margem de cima
    printBorder();

    // Imprime a informação
    playerInfo(player);
    enemyInfo(enemy);

    // Imprime a margem de baixo
    printBorder();
}