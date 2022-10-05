#include "gameFuncts.h"


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

// Lê o input em Linux e pega as setas do teclado em Windows. Dá pra fazer setas em Linux, mas precisa de ncurses e eu não tenho permissão de root pra instalar a API aqui no lab
int getOption() {
    #ifdef _WIN32
        // No windows, quando aperta uma setinha, retorna primeiro ESC, depois [, depois uma letra
        char tecla = getch();
        if (tecla == '\033') { // Se o primeiro valor é ESC (apertou a seta)
            getch(); // Ignora o [

            return getch(); // Retorna a seta apertada. A=Cima, B=Baixo, C=Direita, D=Esquerda
        }
        else {
            return tecla; // Se não for seta, retorna a tecla apertada.
        }  
    #else
        int option;
        printf("\nEscolha uma opcao:\n> "); // Se não for, usa teclado.
        scanf("%i", &option);
        printf("\n");
        return option;
    #endif 
}

// Imprime para apertar enter e não continua até o usuário apertar
void requestEnter() {
    printf("Press ENTER to continue.");
    if(getchar() == '\n') getchar(); // Primeiro getchar pega o \n da ultima mensagem
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
        fflush(stdout); // Alguns sistemas operacionais colocam a string inteira num buffer e imprimem tudo de uma vez em vez de imprimir um char por vez
        if(string[i] != '\n') {
            trueSleep(15); // Valor default = 25
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
    for(int i=0; i<BORDER_LEN; i++) { // Imprime "-" até chegar no BORDER_LEN
        printf("-");
    }
    printf("\033[0m ");
    printf("\n");
}

// Imprime os atributos do player
void playerInfo(playerS player) {
    // Imprime o nome
    centerText(strlen(player.name), BORDER_LEN);
    printf("\033[1m ");
    printf("%s\n", player.name);
    printf("\033[0m ");

    // Imprime o hp, mana e os atributos
    centerText(player.hpMax, BORDER_LEN);
    printHp(player.hpMax, player.hp);

    centerText(player.manaMax, BORDER_LEN);
    printMana(player.manaMax, player.mana);

    printf("Rolagem de Ataque: \033[4m1d20%+i\033[0m\tRolagem de Dano: \033[4m%id%i%+i\033[0m\tArmadura: \033[4m%i\033[0m\n", player.atkMod, player.dmgDiceNum, player.dmgDice, player.dmgMod, player.armor);
}

// Imprime os atributos do inimigo
void enemyInfo(enemyS enemy) {
    // Imprime a margem de cima
    printBorder();

    // Imprime o nome
    centerText(strlen(enemy.name), BORDER_LEN);
    printf("\033[1m ");
    printf("%s\n", enemy.name);
    printf("\033[0m ");

    // Imprime o hp e os atributos
    centerText(enemy.hpMax, BORDER_LEN);
    printHp(enemy.hpMax, enemy.hp);
    printf("Rolagem de Ataque: \033[4m1d20%+i\033[0m\tRolagem de Dano: \033[4m%id%i%+i\033[0m\tArmadura: \033[4m%i\033[0m\n", enemy.atkMod, enemy.dmgDiceNum, enemy.dmgDice, enemy.dmgMod, enemy.armor);
}

// Imprime o menu de atributos
void printInfo (playerS player, enemyS enemy) {
    // Código de escape (se eu não me engano é ANSI) que esconde o cursor
    printf("\33[?25l");

    // Limpa a tela
    clearTerm(); 

    // Imprime a margem de cima
    printBorder();

    // Imprime a informação
    playerInfo(player);
    enemyInfo(enemy);

    // Imprime a margem de baixo
    printBorder();

    // Reabilita o cursor
    printf("\33[?25h");
}