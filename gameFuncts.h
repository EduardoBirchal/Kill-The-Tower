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
#include <libintl.h>

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

#define NUM_STATUSES 11
#define NUM_ENEMY_STATUSES 1

#define INV_SIZE 6
#define MAX_ITEM 25
#define MAX_DESC_ITEM 75

#define NUM_CLASSES 4
#define MAX_NARRATE 200
#define MAX_DESC_CLASS 160

#define TAB "      "
#define TEXT_SPEED 15
#define BAR_SIZE 64

#define CONFIG_AMT 2

#define MAX_ENEMY_SKILL 25
#define NUM_ENEMY_ACTIONS 4

#define _(String) gettext(String)


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
    int hpMax, hp, dmgDiceNum, dmgDice, dmgMod, atkMod, atkNum, armor, skillMod, manaMax, mana, tacticalsInARow;
    char name[MAX_NAME], atkName[MAX_ENEMY_SKILL];
    int status[NUM_ENEMY_STATUSES];
    int advantage;

    struct enemySkill_s *knownSkills;
    int *skillCodes;
    int skillNum;
} enemyS;


/* ==== Typedefs e Enums ==== */

enum statusNums {mageArmS, mageShldS, tripAtkS, parryAtkS, rdntSmiteS, hungerOfTheVoidS, azathothDreamS, searingLightS, btlTranceS, poisonedS, weakenedS};
enum enemyStatusNums {poisonedSE};

enum classes {warrior, wizard, warlock, paladin};

enum spells {fireBlt, sonicBlst, mageArm, mageShld, magicMsl, blessWpn, rdntSmite, voidHunger, yogSothothSight, cthulhuFire, azathothDream, srngLight};
enum skills {doubleStrk, tripAtk, selfDmg, parryAtk, scndWind, dvnGuidance, bldOffering, adrnlSurge, prcStrk, btlTrance, siphonPwr, dvnIntervention};

enum enemySkills {eFireBlt, eRegenerate, eleechAtk};

typedef int bool;
typedef bool (*sklFunct) (); // Define bool (*coisa) () como só sklFunct. É um ponteiro de função.


/* ==== Variáveis globais ==== */

extern int showDesc;
extern playerS player;
extern enemyS enemy;


/* ==== Funções ==== */

/* ==== Combate - funções de combate ==== */

    // Libera tudo do player
    void freePlayer();

    // Libera tudo do inimigo 
    void freeEnemy();

    // Imprime uma barra de vida
    void printHp (int hpMax, int hp);

    // Imprime uma barra de mana
    void printMana (int manaMax, int mana);

    // Coloca o número dentro de um máximo e um mínimo
    int checkRange (int num, int min, int max);

    // Conserta o HP pra não ficar negativo
    int updateValues ();

    // Rola [num] dados de [size] lados
    int rollDice(int size, int num, int mod, int adv);

    // Faz o ataque do player
    int playerAtk();

    // Retorna true e imprime uma mensagem se alguém morreu
    bool battleIsOver ();

    
/* ==== Skills - habilidades especiais não-mágicas ==== */

    // Checa se uma habilidade já está em efeito
    bool isInEffect (int index);

    // Ataca duas vezes.
    int doubleStrike ();

    // Ataca e ganha vantagem nos ataques no turno seguinte.
    int tripAttack ();

    // Ataca e ganha um bônus de armadura no turno seguinte.
    int parryAttack ();

    // Recupera HP.
    int secondWind ();

    // Ataca com um bônus de ataque e dano.
    int divineGuidance ();

    // Recebe dano e recupera mana.
    int bloodOffering ();

    // Uma skill de cura genérica.
    int skillHeal (int healDie, int healDieNum, int healMod, char* str);

    // Imprime a lista de habilidades.
    void printSkills();

    // Imprime as habilidades e lê a escolha do player.
    int playerSkl ();

    // Diminui os cooldowns de toda habilidade do inimigo.
    void updateEnemyCooldown ();

    // Diminui os cooldowns de toda habilidade do player.
    void updatePlayerCooldown ();

    // Diminui os cooldowns de toda habilidade.
    void updateCooldowns ();

    // Inicializa o vetor de skills do player.
    void initSkills ();

    // Adiciona uma skill no vetor de skills do player.
    void addSkill (int index);

    // Define o nome e descrição das skills do player.
    void defineSkillStrings ();


/* ==== Spells - as funções de feitiços ==== */

    // Rola o ataque pra um feitiço.
    int spellAtk ();

    // Um feitiço de dano genérico. Retorna se acertou ou errou, pra efeitos adicionais.
    int spellDmg (int dmgDie, int dmgDieNum, char* strHit, char* strMiss);

    // Um feitiço simples, que dá uma boa quantidade de dano.
    int fireBolt ();

    // Dá dano médio e diminui armadura.
    int sonicBlast ();

    // Aumenta a armadura do player. Não acumula.
    int mageArmor ();

    // Aumenta muito a armadura do player, por 1 turno. Não acumula.
    int mageShield ();

    // Dá dano baixo e acerta sempre, sem precisar rolar ataque.
    int magicMissile ();

    // Aumenta o dano da arma permanentemente.
    int blessWeapon ();

    // Amplifica o dano do seu próximo ataque.
    int radiantSmite ();

    // Lê a escolha de feitiços do player.
    int readSpell ();
    
    // Imprime os feitiços e lê a escolha do player.
    int playerMag ();

    // Inicializa o vetor de feitiços do player.
    void initSpells ();

    // Adiciona um feitiço no vetor de feitiços do player.
    void addSpell (int index);

    // Define o nome e descrição dos feitiços do player.
    void defineSpellStrings ();


/* ==== Status - as funções de cada status do player ==== */

    // Checa o array de status do player e faz os efeitos de cada status
    void updateStatus();


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
    void playerInfo();

    // Imprime os atributos do inimigo
    void enemyInfo();

    // Imprime o menu de atributos
    void printInfo ();

    // Pega uma string de um arquivo e tira o \n
    void getStringFromFile(FILE* file, int max, char* string);

    // Pega um array de números no arquivo, separados por espaço
    void getArrayFromFile(FILE* file, int size, int *array);


/* ==== Inventory - funções de inventário do player ==== */

    // Um item de dano genérico.
    int itemDmg (int dmgDie, int dmgDieNum, int dmgMod, char* strHit);

    // Um item de cura genérico.
    int itemHeal (int healDie, int healDieNum, int healMod, char* str);

    // Aumenta a armadura do player. Não acumula.
    int healPotion ();

    // Aumenta muito a armadura do player, por 1 turno. Não acumula.
    int armorRune ();

    // Dá dano baixo e acerta sempre, sem precisar rolar ataque.
    int acidFlask ();

    // Cria um inventário vazio.
    void initInv ();

    // Coloca um número de itens num slot.
    void setInvSlot (int slot, int item, int itemNum);

    // Pra debug, enche o inventário com uma lista pré-pronta de itens.
    void fillInv ();

    // Usa um item, diminuindo a quantidade do item no slot.
    int useItem (int item);

    // Imprime o menu de itens.
    void printItems();

    // Lê a escolha do player.
    int readItem();

    // Imprime o menu e lê a escolha de item.
    int playerInv ();

    // Define os nomes e descrições dos itens.
    void defineItemStrings();


/* ==== UI do player ==== */

    // Imprime as opções do player
    void printOptions();

    // Lê a opção que o player escolheu
    int readOption();

    // Turno do player
    int turnPlayer();


/* ==== Criação do player ==== */

    // Imprime as classes pro player escolher
    void printClasses();

    // Configura a classe do player
    void chooseClass ();

    // Cria o player
    void createPlayer();
    
    // Define os nomes e descrições dos feitiços, skills e itens
    void defineStrings();


/* ==== Inimigo ==== */

    // Anuncia o ataque do inimigo
    void announceAtkE();

    // Acerto crítico do inimigo
    int enemyCrit();

    // Acerto não-crítico do inimigo
    int enemyHit();

    // Um ataque do inimigo
    int enemyAtk();

    // Rola o ataque pra uma habilidade que não é um ataque básico.
    int enemySkillAtk ();

    // Dá dano de uma skill
    int enemySkillDmg (int dmgDie, int dmgDieNum);

    // Uma habilidade de dano genérica. Retorna o dano, pra efeitos adicionais.
    int enemySkillAtkDmg (int dmgDieNum, int dmgDie);

    // Uma skill de cura genérica
    int enemySkillHeal (int healDieNum, int healDie, int healMod);

    // Anuncia e usa uma skill
    void useSkillE (int index);

    // Aloca e preenche o vetor de skills do inimigo
    void initSkillsE ();

    // Cria o inimigo
    void createEnemy(int index);

    // Executa as funções do turno do inimigo
    void turnEnemy ();

    // Define os nomes das skills do inimigo
    void defineSkillNamesE();