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
#define true 1
#define false 0

#define MAX_NAME 25
#define BORDER_LEN 96

#define MAX_OPTION 15
#define OPTION_AMT 6

#define NUM_STATUSES 2
#define MAX_WPN_SMN 3


/* ==== Structs ==== */
typedef struct wpnSmn_s {
    int dmgDiceNum, dmgDice;
    char name[MAX_NAME];
} wpnSmnS;

typedef struct player_s {
    int hpMax, hp, dmgDiceNum, dmgDice, dmgMod, atkMod, atkNum, armor, magMod;
    char name[MAX_NAME];
    int status[NUM_STATUSES];

    wpnSmnS wpnSmns[MAX_WPN_SMN];
} playerS;

typedef struct enemy_s {
    int hpMax, hp, dmgDiceNum, dmgDice, dmgMod, atkMod, atkNum, armor, magMod;
    char name[MAX_NAME];
    int status[NUM_STATUSES];
} enemyS;

/* ==== Typedefs ==== */
enum statNums {mageArm = 0, mageShld};

typedef int (*magFunct) (playerS *player, enemyS *enemy); // define int (*coisa) (playerS *player, enemyS *enemy) como só magFunct. É um ponteiro de função.
typedef int bool;

/* ==== Funções ==== */

/* ==== term.c - funções de terminal ==== */

// Imprime para apertar enter e não continua até o usuário apertar
void requestEnter();

// Sleep que funciona em Windows e Linux
void trueSleep(int ms);

// Um regex que limpa o terminal, muitas vezes mais rápido que o system("clear") e é multiplataforma
void clearTerm();

// Imprime espaços pra centralizar a próxima impressão em uma margem
void centerText (int len, int borderPos);

// Conta o número de dígitos de um número recursivamente
int digitNum (int num);

// Imprime uma mensagem devagar
void printSlow (char string[]);

// Imprime o resultado de um dado devagar
void rollSlow (int result);

// Imprime uma margem
void printBorder ();

// Imprime os atributos do player
void playerInfo(playerS player);

// Imprime os atributos do inimigo
void enemyInfo(enemyS enemy);

// Imprime o menu de atributos
void printInfo (playerS player, enemyS enemy);

/* ==== status.c - as funções de cada status do player ==== */

// Checa o array de status do player e faz os efeitos de cada status
void updateStatus(playerS *player);

/* ==== combat.c - funções de combate ==== */

// Imprime uma barra de vida
void printHp (int hpMax, int hp);

// Conserta o HP pra não ficar negativo
int updateHp (playerS *player, enemyS *enemy);

// Rola [num] dados de [size] lados
int rollDice(int size, int num, int mod);