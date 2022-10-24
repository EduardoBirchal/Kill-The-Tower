/**
* @file: gameFuncts
* @brief: Arquivo header de funções, defines e typedefs
* @author: Eduardo Santos Birchal
*/

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
#define OPTION_AMT 5

#define NUM_STATUSES 9
#define MAX_WPN_SMN 3

#define INV_SIZE 6
#define MAX_ITEM 25
#define MAX_DESC_ITEM 75

#define NUM_CLASSES 4
#define MAX_NARRATE 200
#define MAX_DESC_CLASS 160

#define TAB "      "
#define TEXT_SPEED 15

#define CONFIG_AMT 2

#define MAX_ENEMY_SKILL 25


/* ==== Structs ==== */

struct item_s; // itemS depende de playerS e playerS depende de itemS, então eu dei forward declaration. Mesma coisa pra spell_s e skill_s
struct spell_s;
struct skill_s;

struct enemySkill_s;

typedef struct player_s {
    int hpMax, hp, dmgDiceNum, dmgDice, dmgMod, atkMod, atkNum, armor, magMod, manaMax, mana;
    char name[MAX_NAME];
    int status[NUM_STATUSES];
    int advantage; // Se vantagem=0, não tem vantagem.

    struct item_s *inventory;
    int invFill;

    struct spell_s *knownSpells;
    int spellNum;

    struct skill_s *knownSkills;
    int skillNum;

    int class;

    char hitString[MAX_NARRATE];
    char missString[MAX_NARRATE];
    char critString[MAX_NARRATE];
} playerS;

typedef struct enemy_s {
    int hpMax, hp, dmgDiceNum, dmgDice, dmgMod, atkMod, atkNum, armor, skillMod;
    char name[MAX_NAME], atkName[MAX_ENEMY_SKILL];
    int status[NUM_STATUSES];
    int advantage;

    struct enemySkill_s *knownSkills;
    int skillNum;
} enemyS;


/* ==== Typedefs ==== */

enum statNums {mageArmS, mageShldS, tripAtkS, parryAtkS, rdntSmiteS, hungerOfTheVoidS, azathothDreamS, searingLightS, btlTranceS};
enum classes {warrior, wizard, warlock, paladin};

enum spells {fireBlt, sonicBlst, mageArm, mageShld, magicMsl, blessWpn, rdntSmite, voidHunger, yogSothothSight, cthulhuFire, azathothDream, srngLight};
enum skills {doubleStrk, tripAtk, selfDmg, parryAtk, scndWind, dvnGuidance, bldOffering, adrnlSurge, prcStrk, btlTrance};

typedef int bool;
typedef bool (*sklFunct) (playerS *player, enemyS *enemy); // define bool (*coisa) (playerS *player, enemyS *enemy) como só sklFunct. É um ponteiro de função.


/* ==== Funções ==== */

/* ==== Combate - funções de combate ==== */

    // Imprime uma barra de vida
    void printHp (int hpMax, int hp);

    // Imprime uma barra de mana
    void printMana (int manaMax, int mana);

    // Conserta o HP pra não ficar negativo
    int updateValues (playerS *player, enemyS *enemy);

    // Rola [num] dados de [size] lados
    int rollDice(int size, int num, int mod, int adv);

    // Faz o ataque do player
    int playerAtk(playerS *player, enemyS *enemy);

    
/* ==== Skills - habilidades especiais não-mágicas ==== */


    // Ataca duas vezes.
    int doubleStrike (playerS* player, enemyS *enemy);

    // Ataca e ganha vantagem nos ataques no turno seguinte.
    int tripAttack (playerS* player, enemyS *enemy);

    // Ataca e ganha um bônus de armadura no turno seguinte.
    int parryAttack (playerS* player, enemyS *enemy);

    // Recupera HP.
    int secondWind (playerS* player, enemyS *enemy);

    // Ataca com um bônus de ataque e dano.
    int divineGuidance (playerS* player, enemyS *enemy);

    // Recebe dano e recupera mana.
    int bloodOffering (playerS* player, enemyS *enemy);

    // Uma skill de cura genérica.
    int skillHeal (playerS* player, enemyS *enemy, int healDie, int healDieNum, int healMod, char* str);

    // Imprime a lista de habilidades.
    void printSkills(playerS *player);

    // Imprime as habilidades e lê a escolha do player.
    int playerSkl (playerS *player, enemyS *enemy);

    // Diminui os cooldowns de toda habilidade.
    void updateCooldown (playerS *player);

    // Inicializa o vetor de skills do player.
    void initSkills (playerS *player);

    // Adiciona uma skill no vetor de skills do player.
    void addSkill (playerS *player, int index);


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

    // Aumenta o dano da arma permanentemente.
    int blessWeapon (playerS *player, enemyS *enemy);

    // Amplifica o dano do seu próximo ataque.
    int radiantSmite (playerS *player, enemyS *enemy);

    // Lê a escolha de feitiços do player.
    int readSpell(playerS *player, enemyS *enemy);
    
    // Imprime os feitiços e lê a escolha do player.
    int playerMag (playerS *player, enemyS *enemy);

    // Inicializa o vetor de feitiços do player.
    void initSpells (playerS *player);

    // Adiciona um feitiço no vetor de feitiços do player.
    void addSpell (playerS *player, int index);


/* ==== Status - as funções de cada status do player ==== */

    // Checa o array de status do player e faz os efeitos de cada status
    void updateStatus(playerS *player, enemyS *enemy);


/* ==== Term - funções de terminal ==== */


    // Sleep que funciona em Windows e Linux
    void trueSleep(int ms);

    // Lê o input em Linux e pega as setas do teclado em Windows. Dá pra fazer setas em Linux, mas precisa de ncurses e eu não tenho permissão de root pra instalar a API aqui no lab
    int getOption();

    // Imprime para apertar enter e não continua até o usuário apertar
    void requestEnter();

    // Um regex que limpa o terminal, muitas vezes mais rápido que o system("clear") e é multiplataforma
    void clearTerm();

    // Imprime espaços pra centralizar a próxima impressão em uma margem
    void centerText (int len, int borderPos);

    // Conta o número de dígitos de um número recursivamente
    int digitNum (int num);

    // Imprime uma mensagem devagar
    void printSlow (char string[]);

    // Imprime o resultado de um dado devagar
    void printRollResult (int result);

    // Imprime o resultado de um dado e anuncia de que é esse dado. Por exemplo, se [string] for "dano", imprime "[result] de dano"
    void printCustomResult (int result, char string[]);

    // Imprime uma margem
    void printBorder ();

    // Imprime os atributos do player
    void playerInfo(playerS player);

    // Imprime os atributos do inimigo
    void enemyInfo(enemyS enemy);

    // Imprime o menu de atributos
    void printInfo (playerS player, enemyS enemy);


/* ==== Inventory - funções de inventário do player ==== */

    // Um item de dano genérico.
    int itemDmg (playerS* player, enemyS *enemy, int dmgDie, int dmgDieNum, int dmgMod, char* strHit);

    // Um item de cura genérico.
    int itemHeal (playerS* player, enemyS *enemy, int healDie, int healDieNum, int healMod, char* str);

    // Aumenta a armadura do player. Não acumula.
    int healPotion (playerS *player, enemyS *enemy);

    // Aumenta muito a armadura do player, por 1 turno. Não acumula.
    int armorRune (playerS *player, enemyS *enemy);

    // Dá dano baixo e acerta sempre, sem precisar rolar ataque.
    int acidFlask (playerS *player, enemyS *enemy);

    // Cria um inventário vazio.
    void initInv (playerS *player);

    // Coloca um número de itens num slot.
    void setInvSlot (playerS *player, int slot, int item, int itemNum);

    // Pra debug, enche o inventário com uma lista pré-pronta de itens.
    void fillInv (playerS *player);

    // Usa um item, diminuindo a quantidade do item no slot.
    int useItem (playerS *player, enemyS *enemy, int item);

    // Imprime o menu de itens.
    void printItems(playerS *player);

    // Lê a escolha do player.
    int readItem(playerS *player, enemyS *enemy);

    // Imprime o menu e lê a escolha de item.
    int playerInv (playerS *player, enemyS *enemy);


/* ==== UI do player ==== */

    // Imprime as opções do player
    void printOptions(playerS *player, enemyS *enemy);

    // Lê a opção que o player escolheu
    int readOption(playerS *player, enemyS *enemy);

    // Turno do player
    int turnPlayer(playerS *player, enemyS *enemy);


/* ==== Criação do player ==== */

// Imprime as classes pro player escolher
void printClasses();

// Configura a classe do player
void chooseClass (playerS *player);

// Cria o player
playerS createPlayer();


/* ==== Inimigo ==== */

// Cria o inimigo, lendo de um arquivo
enemyS createEnemy(int index);

// Um ataque do inimigo
int enemyAtk(playerS *player, enemyS *enemy);