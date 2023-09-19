/**
* @file: term
* @brief: Funções gerais de terminal
* @author: Eduardo Santos Birchal
*/

#include "gameFuncts.h"

static int cursorPos = 1;

extern playerS player;
extern enemyS enemy;


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
    /*#ifdef _WIN32
        // No windows, quando aperta uma setinha, retorna primeiro ESC, depois [, depois uma letra
        char tecla = getch();
        if (tecla == '\033') { // Se o primeiro valor é ESC (apertou a seta)
            getch(); // Ignora o [

            return getch(); // Retorna a seta apertada. A=Cima, B=Baixo, C=Direita, D=Esquerda
        }
        else {
            return tecla; // Se não for seta, retorna a tecla apertada.
        }  
    #else*/
        int option;
        printf(_("\nEscolha uma opcao:\n> ")); // Se não for, usa teclado.
        scanf("%i", &option);
        printf("\n");
        return option;
    //#endif 
}

// Imprime para apertar enter e não continua até o usuário apertar
void requestEnter() {
    printf(_("Press ENTER to continue."));
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
        if (!(string[i] == ' ' && cursorPos == 1)) printf("%c", string[i]); // Se o caracter for um espaço e cursorPos for 1 (o cursor estiver no início da margem), não imprime o caracter
        fflush(stdout); // Alguns sistemas operacionais colocam a string inteira num buffer e imprimem tudo de uma vez em vez de imprimir um char por vez
        
        if(string[i] != '\n') {
            cursorPos++; // Aumenta a posição do cursor quando imprime um caracter que não é \n
            #ifdef _WIN32
                trueSleep(TEXT_SPEED/10); // O Windows imprime muito mais devagar que o linux, então eu diminui o tempo em 10
            #else
                trueSleep(TEXT_SPEED);
            #endif
        }
        else {
            cursorPos = 1; // Se der um \n, o cursor volta pro início, então muda pra 1
        }

        if(cursorPos >= BORDER_LEN) {
            printf("\n");
            cursorPos = 1;
        }
    }
}

// Imprime o resultado de um dado devagar
void printRollResult (int result) {
    printSlow("...");
    fflush(stdout);
    trueSleep(800);
    printf(" %i.\033[0m ", result);
    fflush(stdout);
    trueSleep(200);
}

// Imprime o resultado de um dado e anuncia de que é esse dado. Por exemplo, se [string] for "dano", imprime "[result] de dano"
void printCustomResult (int result, char string[]) {
    printSlow("...");
    fflush(stdout);
    trueSleep(800);
    printf(" %i\033[0m ", result);
    printSlow("de ");
    printSlow(string);
    printSlow("!\n\n");
    fflush(stdout);
    trueSleep(200);
}

// Imprime uma margem
void printBorder () {
    printf("\033[93m");
    for(int i=0; i<BORDER_LEN; i++) { // Imprime "-" até chegar no BORDER_LEN
        printf("-");
    }
    printf("\033[0m ");
    printf("\n");
}

// Imprime os atributos do player
void playerInfo() {
    // Imprime o nome
    centerText(strlen(player.name), BORDER_LEN);
    printf("\033[1m ");
    printf("%s\n", player.name);
    printf("\033[0m ");

    // Imprime o hp, mana e os atributos
    printHp(player.hpMax, player.hp);
    printMana(player.manaMax, player.mana);

    printf(_("Rolagem de Ataque: \033[4m1d20%+i\033[0m      Rolagem de Dano: \033[4m%id%i%+i\033[0m      Armadura: \033[4m%i\033[0m\n"), player.atkMod, player.dmgDiceNum, player.dmgDice, player.dmgMod, player.armor);

    if(player.status[poisonedS]) {
        printf(_("\n\033[32mEnvenenado: %i\n"), player.status[poisonedS]);
    }
}

// Imprime os atributos do inimigo
void enemyInfo() {
    // Imprime a margem de cima
    printBorder();

    // Imprime o nome
    centerText(strlen(enemy.name), BORDER_LEN);
    printf("\033[1m ");
    printf("%s\n", enemy.name);
    printf("\033[0m ");

    // Imprime o hp e os atributos
    printHp(enemy.hpMax, enemy.hp);
    printMana(enemy.manaMax, enemy.mana);

    printf(_("Rolagem de Ataque: \033[4m1d20%+i\033[0m      Rolagem de Dano: \033[4m%id%i%+i\033[0m      Armadura: \033[4m%i\033[0m\n"), enemy.atkMod, enemy.dmgDiceNum, enemy.dmgDice, enemy.dmgMod, enemy.armor);
    
    if(enemy.status[poisonedSE]) {
        printf(_("\n\033[32mEnvenenado: %i\n"), enemy.status[poisonedSE]);
    }
}

// Imprime o menu de atributos
void printInfo () {
    // Código de escape (se eu não me engano é ANSI) que esconde o cursor
    printf("\33[?25l");

    // Limpa a tela
    clearTerm(); 

    // Imprime a margem de cima
    printBorder();

    // Imprime a informação
    playerInfo();
    enemyInfo();

    // Imprime a margem de baixo
    printBorder();

    // Reabilita o cursor
    printf("\33[?25h");
}

// Imprime as opções do player
void printConfig(playerS *player, enemyS *enemy) {
    char options[CONFIG_AMT][MAX_OPTION] = {"VELOC. DO TEXTO", "MOSTRAR DESCS."};
    int option = 0;

    printInfo(*player, *enemy);
    printf("\n");
    for(int i=0; i<OPTION_AMT; i++) {
        printf("\033[33m(%i)\033[0m %s%s", i+1, options[i], TAB);
    }
    printf("\n");
}

// Pega uma string de um arquivo e tira o \n
void getStringFromFile(FILE* file, int max, char* string) {
    fgets(string, max, file);
    string[strcspn(string, "\n")] = 0;
}

// Pega um array de números no arquivo, separados por espaço
void getArrayFromFile(FILE* file, int size, int *array) {
    int currentInt;
    char currentChar;

    fscanf(file, "%*s "); // Lê a string que diz o que o array representa e joga ela fora

    // Lê números até encher o array (com um espaço sobrando) ou até currentChar não ser espaço
    for (int i=0; i<size-1; i++) {
        fscanf(file, "%i%c", &currentInt, &currentChar);

        if (currentChar != ' ') break;
        else array[i] = currentInt;
    }

    // Lê o último int, mas não um char, pra não interferir com a leitura do nome do ataque básico do inimigo
    fscanf(file, "%i", &currentInt); 
    array[size-1] = currentInt;
}