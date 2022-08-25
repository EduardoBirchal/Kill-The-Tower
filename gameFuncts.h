/* ==== Includes ==== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
#endif


/* ==== Defines ==== */

#define MAX_NAME 25
#define BORDER_LEN 96
#define MAX_OPTION 15
#define OPTION_AMT 6


/* ==== Structs ==== */

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
typedef int (*magFunct) (playerS *player, enemyS *enemy); // define int (*coisa) (playerS *player, enemyS *enemy) como só magFunct. É um ponteiro de função.


/* ==== Funções ==== */

// Imprime para apertar enter e não continua até o usuário apertar
void requestEnter();

// Sleep que funciona em Windows e Linux
void trueSleep(int ms);

// Um regex que limpa o terminal, muitas vezes mais rápido que o system("clear") e é multiplataforma
void clearTerm();

// Imprime espaços pra centralizar a próxima impressão em uma margem
void centerText (int len, int borderPos);

// Imprime uma barra de vida
void printHp (int hpMax, int hp);

// Conserta o HP pra não ficar negativo
int updateHp (playerS *player, enemyS *enemy);

// Conta o número de dígitos de um número recursivamente
int digitNum (int num);

// Imprime uma mensagem devagar
void printSlow (char string[]);

// Imprime o resultado de um dado devagar
void rollSlow (int result);

// Imprime uma margem
void printBorder ();
