#include "gameFuncts.h"

#define NUM_SPELLS 5
#define MAX_SPELL 25
#define MAX_DESC 100

typedef struct spell_s {
    magFunct funct;
    char name[MAX_SPELL];
    char desc[MAX_DESC];
} spellS;

/* ==== Lista de feitiços ==== */

// Rola o ataque pra um feitiço.
int spellAtk (playerS* player) {
    int atkRoll = rollDice(20, 1, player->magMod);

    printSlow("Rolagem de ataque magico - \033[36mrolando ");
    printf("1d20%+i", player->magMod);
    rollSlow(atkRoll);

    return atkRoll;
}

// Um feitiço de dano genérico. Retorna se acertou ou errou, pra efeitos adicionais.
int spellDmg (playerS* player, enemyS *enemy, int dmgDie, int dmgDieNum, char* strHit, char* strMiss) {
    int dmgRoll = 0;
    int atkRoll = spellAtk (player);

    if (atkRoll >= enemy->armor) {
        printSlow(" \033[33;4mAcerto!\033[0m\n\n");
        dmgRoll = rollDice(dmgDie, dmgDieNum, player->magMod);
        printSlow("Rolagem de dano - \033[36mrolando ");
        printf("%id%i%+i", dmgDieNum, dmgDie, player->magMod);
        rollSlow(dmgRoll);
        printSlow(strHit);
        enemy->hp -= dmgRoll;

        return 1;
    }
    else {
        printSlow(strMiss);
        return 0;
    }
}

// Um feitiço simples, que dá uma boa quantidade de dano.
int fireBolt (playerS* player, enemyS *enemy) {
    spellDmg (player, enemy, 6, 2,
    "\n\nO dardo de fogo acerta a criatura, estalando e criando um estouro de chamas.\n\n", 
    " \033[33;4mFalha...\033[0m\n\nO alvo se abaixa para fora do caminho do projétil, que dispara por cima dele e se dissipa numa nuvem de brasas.\n\n");
}

// Dá dano médio e diminui armadura.
int sonicBlast (playerS* player, enemyS *enemy) {
    int result = spellDmg (player, enemy, 8, 1,
    "\n\nO pulso sonico colide com o alvo, com um impacto pesado. O som ressoante rasga um buraco em sua armadura de couro. \033[33mArmadura do inimigo -1!\033[0m\n\n", 
    " \033[33;4mFalha...\033[0m\n\nO feixe atinge o escudo de bronze da criatura, estourando num zumbido estrondoso.\n\n");

    if (result) {
        enemy->armor--;
    }
}

// Aumenta a armadura do player. Não acumula.
int mageArmor (playerS *player, enemyS *enemy) {
    if(player->status[mageArm]) {
        printSlow("Esse feitico ja esta em efeito!\n\n");
    } else {
        player->status[mageArm] = true;
        player->armor += 2;
        printSlow("Voce conjura uma armadura translucida de energia protetiva em volta de si. \033[33mArmadura +2!\033[0m\n\n");
    }
}

// Aumenta muito a armadura do player, por 1 turno. Não acumula.
int mageShield (playerS *player, enemyS *enemy) {
    if(player->status[mageShld]) {
        printSlow("Esse feitico ja esta em efeito!\n\n");
    } else {
        player->status[mageShld] = 2;
        player->armor += 5;
        printSlow("Com uma runa protetora, voce conjura um escudo flutuante de energia arcana. \033[33mArmadura +5!\033[0m\n\n");
    }
}

// Dá dano baixo e acerta sempre, sem precisar rolar ataque.
int magicMissile (playerS *player, enemyS *enemy) {
    int dmgRoll = rollDice(4, 3, 1);

    printSlow("Rolagem de dano - \033[36mrolando ");
    printf("%id%i%+i", 3, 4, 1);
    rollSlow(dmgRoll);
    printSlow("\nVoce invoca tres dardos de energia que se curvam pelo ar e atingem o alvo, liberando pulsos de forca arcana no impacto.\n\n");
    enemy->hp -= dmgRoll;
}

/* ==== Criar o array de feitiços ==== */

spellS spells[NUM_SPELLS] = {
    {
        &fireBolt, // Função de quando o feitiço é conjurado
        "Dardo de Fogo", // Nome do feitiço
        "Causa dano alto." // Descrição do feitiço
    },
    {
        &sonicBlast,
        "Pulso Sonico",
        "Causa dano medio e diminui a armadura do alvo em -1."
    },
    {
        &mageArmor,
        "Armadura Arcana",
        "Aumenta sua armadura em +2 permanentemente."
    },
    {
        &mageShield,
        "Escudo Arcano",
        "Aumenta sua armadura em +5 por 1 turno."
    },
    {
        &magicMissile,
        "Misseis Magicos",
        "Causa dano baixo e nunca erra, nao importa a armadura do alvo."
    }
};

/* ==== Imprimir menu de feitiços ==== */

void printSpells() {
    int option = 0, i = 0;

    printf("\n");
    for(i=0; i<NUM_SPELLS; i++) {
        printf("\033[33m%i:\033[0m ", i+1);
        puts(spells[i].name);
    }
    printf("\033[33m%i: \033[36mCancelar\033[0m\n", i+1);
}

int readSpell(playerS *player, enemyS *enemy) {
    int option = 1;

    while (1) {
        printf("\nEscolha um feitico:\n> ");
        scanf("%i", &option);
        printf("\n");

        /*while (1) {
            uint8_t *state = SDL_GetKeyboardState(NULL);
            if (state[80]) {
                option--;
            }
            if (state[79]) {
                option++;
                printf("%i ", option);
            }
        }*/
        if(option>0 && option<=NUM_SPELLS) {
            spells[option-1].funct (player, enemy); // Se a opção é um feitiço, conjura ele
            break;
        } 
        else if(option=NUM_SPELLS+1) {  
            return 1;
        } 
        else {
            printf("Opcao invalida! (tem que ser um numero de 1 a %i).\n", NUM_SPELLS+1);
        }
    }

    return 0;
}

int playerMag (playerS *player, enemyS *enemy) {
    printSpells();
    if(readSpell(player, enemy)) {
        return 1; // Retorna 1, fazendo com que o menu imprima de novo
    }

    return 0;
}