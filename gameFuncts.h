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

#define MAX_NAME 50
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
    int advantage; // Se vantagem=0, não tem vantagem.

    wpnSmnS wpnSmns[MAX_WPN_SMN];
} playerS;

typedef struct enemy_s {
    int hpMax, hp, dmgDiceNum, dmgDice, dmgMod, atkMod, atkNum, armor, magMod;
    char name[MAX_NAME];
    int status[NUM_STATUSES];
} enemyS;

/* ==== Typedefs ==== */

enum statNums {mageArm = 0, mageShld, tripAtk};

typedef int (*sklFunct) (playerS *player, enemyS *enemy); // define int (*coisa) (playerS *player, enemyS *enemy) como só sklFunct. É um ponteiro de função.
typedef int bool;


/* ==== Funções ==== */

/* ==== Combate - funções de combate ==== */

    // Imprime uma barra de vida
    void printHp (int hpMax, int hp);

    // Conserta o HP pra não ficar negativo
    int updateHp (playerS *player, enemyS *enemy);

    // Rola [num] dados de [size] lados
    int rollDice(int size, int num, int mod, int adv);


/* ==== Skills - habilidades especiais não-mágicas ==== */

    // Rola o ataque pra um habilidade.
    int skillAtk (playerS* player);

    // Um habilidade de dano genérico. Retorna se acertou ou errou, pra efeitos adicionais.
    int skillDmg (playerS* player, enemyS *enemy, int dmgDie, int dmgDieNum, char* strHit, char* strMiss);

    // Ataca duas vezes.
    int doubleStrike (playerS* player, enemyS *enemy);

    // Ataca e ganha vantagem nos ataques no turno seguinte.
    int tripAttack (playerS* player, enemyS *enemy);

    // Imprime a lista de habilidades.
    void printSkills();

    // Lê a escolha de habilidade do player.
    int readSkill(playerS *player, enemyS *enemy);

    // Imprime as habilidades e lê a escolha do player.
    int playerSkl (playerS *player, enemyS *enemy);


/* ==== Spells - as funções de feitiços ==== */

    // Rola o ataque pra um feitiço.
    int spellAtk (playerS* player);

    // Um feitiço de dano genérico. Retorna se acertou ou errou, pra efeitos adicionais.
    int spellDmg (playerS* player, enemyS *enemy, int dmgDie, int dmgDieNum, char* strHit, char* strMiss);

    // Um feitiço simples, que dá uma boa quantidade de dano.
    int fireBolt (playerS* player, enemyS *enemy);

    // Dá dano médio e diminui armadura.
    int sonicBlast (playerS* player, enemyS *enemy);

    // Aumenta a armadura do player. Não acumula.
    int mageArmor (playerS *player, enemyS *enemy);

    // Aumenta muito a armadura do player, por 1 turno. Não acumula.
    int mageShield (playerS *player, enemyS *enemy);

    // Dá dano baixo e acerta sempre, sem precisar rolar ataque.
    int magicMissile (playerS *player, enemyS *enemy);

    // Imprime a lista de feitiços.
    void printSpells();

    // Lê a escolha de feitiços do player.
    int readSpell(playerS *player, enemyS *enemy);
    
    // Imprime os feitiços e lê a escolha do player.
    int playerMag (playerS *player, enemyS *enemy);


/* ==== Status - as funções de cada status do player ==== */

    // Checa o array de status do player e faz os efeitos de cada status
    void updateStatus(playerS *player);


/* ==== Term - funções de terminal ==== */

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