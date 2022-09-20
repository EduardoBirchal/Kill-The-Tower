#include "gameFuncts.h"

#define NUM_SKILLS 2
#define MAX_SKILL 25
#define MAX_DESC_SKILL 100

typedef struct skill_s {
    sklFunct funct;
    char name[MAX_SKILL];
    char desc[MAX_DESC_SKILL];
} skillS;


/* ==== Funções de habilidade ==== */

// Rola o ataque pra um habilidade.
int skillAtk (playerS* player) {
    int atkRoll = rollDice(20, 1, player->magMod, 0);

    printSlow("Rolagem de ataque magico - \033[36mrolando ");
    printf("1d20%+i", player->magMod);
    rollSlow(atkRoll);

    return atkRoll;
}

// Um habilidade de dano genérico. Retorna se acertou ou errou, pra efeitos adicionais.
int skillDmg (playerS* player, enemyS *enemy, int dmgDie, int dmgDieNum, char* strHit, char* strMiss) {
    int dmgRoll = 0;
    int atkRoll = skillAtk (player);

    if (atkRoll >= enemy->armor) { // Se acertou, dá dano e imprime 'strHit'.
        printSlow(" \033[33;4mAcerto!\033[0m\n\n");
        dmgRoll = rollDice(dmgDie, dmgDieNum, player->magMod, 0);
        printSlow("Rolagem de dano - \033[36mrolando ");
        printf("%id%i%+i", dmgDieNum, dmgDie, player->magMod);
        rollSlow(dmgRoll);
        printSlow(strHit);
        enemy->hp -= dmgRoll;

        return 1;
    }
    else { // Senão, imprime 'strMiss'.
        printSlow(strMiss);
        return 0;
    }
}


/* ==== Lista de habilidades ==== */

// Um habilidade simples, que dá uma boa quantidade de dano.
int doubleStrike (playerS* player, enemyS *enemy) {
    playerAtk(player, enemy);
    playerAtk(player, enemy);
    
    return 1;
}

// Dá dano médio e diminui armadura.
int tripAttack (playerS* player, enemyS *enemy) {
    if (playerAtk(player, enemy)) {
        player->status[tripAtk] = true;
        player->advantage++;
    }

    return 1;
}

/* ==== Criar o array de habilidades ==== */

skillS skills[NUM_SKILLS] = {
    {
        &doubleStrike, // Função de quando o habilidade é conjurado
        "Golpe Duplo", // Nome do habilidade
        "Ataca duas vezes." // Descrição do habilidade
    },
    {
        &tripAttack,
        "Rasteira",
        "Da uma rasteira no alvo, fazendo-o cair e te dando vantagem no proximo ataque."
    },
};

/* ==== Imprimir menu de habilidades ==== */

void printSkills() {
    int option = 0, i = 0;

    printf("\n");
    for(i=0; i<NUM_SKILLS; i++) {
        printf("\033[33m%i:\033[0m ", i+1);
        puts(skills[i].name);
    }
    printf("\033[33m%i: \033[36mCancelar\033[0m\n", i+1);
}

int readSkill(playerS *player, enemyS *enemy) {
    int option = 1;

    while (1) {
        printf("\nEscolha um feitico:\n> "); // Lê a opção
        scanf("%i", &option);
        printf("\n");

        if(option>0 && option<=NUM_SKILLS) {                  // Se a opção é um habilidade, conjura ele.
            if(skills[option-1].funct (player, enemy)) break; // Se ele retornar 1, acaba o loop. habilidades retornam 0 se eles não funcionam 
        }                                                     // (Exemplo: conjura Armadura Arcana quando ela já está em efeito)
        else if(option==NUM_SKILLS+1) {  
            return 1; // Se a opção for cancelar, volta pro menu
        } 
        else {
            printf("Opcao invalida! (tem que ser um numero de 1 a %i).\n", NUM_SKILLS+1); // Se não for válida, pede pra colocar outra
        }
    }

    return 0;
}

int playerSkl (playerS *player, enemyS *enemy) {
    printSkills();
    if(readSkill(player, enemy)) {
        return 1; // Retorna 1, fazendo com que o menu imprima de novo
    }

    return 0;
}