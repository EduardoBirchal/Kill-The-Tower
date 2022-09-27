#include "gameFuncts.h"


/* ==== Defines ==== */

#define NUM_SKILLS 3
#define MAX_SKILL 25
#define MAX_DESC_SKILL 100

#define NUM_SPELLS 5
#define MAX_SPELL 25
#define MAX_DESC_SPELL 100


/* ==== Typedefs ==== */

typedef struct skill_s {
    sklFunct funct;
    char name[MAX_SKILL];
    char desc[MAX_DESC_SKILL];
} skillS;

typedef struct spell_s {
    sklFunct funct;
    char name[MAX_SPELL];
    char desc[MAX_DESC_SPELL];

    int cost;
} spellS;


/* ======= Combate ======= */

    // Imprime uma barra de vida
    void printHp (int hpMax, int hp) {
        for(int i=1; i<=hpMax; i++) {
            if(i <= (hp)) {
                printf("\033[102m "); // Imprime em verde as células de HP cheio e o resto (hp vazio) imprime em verde escuro
            }
            else {
                printf("\033[42m ");
            }
        }
        printf("\033[0m");
        printf("\n\n");
    }

    // Imprime uma barra de mana
    void printMana (int manaMax, int mana) {
        printf(" ");
        for(int i=1; i<=manaMax; i++) {
            if(i <= (mana)) {
                printf("\033[104m "); // Imprime em azul as células de mana cheio e o resto (mana vazio) imprime em azul escuro
            }
            else {
                printf("\033[44m ");
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

        if(enemy->hp == 0) return 1; // Retorna >0 se alguem morreu
        if(player->hp == 0) return 2;
        return 0;
    }

    // Rola [num] dados de [size] lados
    int rollDice(int size, int num, int mod, int adv) {
        int total;
        int roll = 0;

        for(int j=0; j<num; j++) {
            roll += 1 + rand()%size;
            //printf("\t%io dado: %i\n", j+1, roll); // Só usar pra debug.
        }
        total = roll;

        for(int i=0; i<adv; i++) {
            roll = 0;
            for(int j=0; j<num; j++) {
                roll += 1 + rand()%size;
                //printf("\t%io dado: %i\n", j+1, roll); // Só usar pra debug.
            }
            if (roll>total) total=roll;
        }

        return total+mod;
    }

    // Faz o ataque do player
    int playerAtk(playerS *player, enemyS *enemy) {
        int atkRoll = rollDice(20, 1, player->atkMod, player->advantage); // Rola o ataque
        int dmgRoll = 0;

        printSlow("Rolagem de ataque - \033[36mrolando ");
        printf("1d20%+i", player->atkMod);
        rollSlow(atkRoll);
        
        // Se for 17 natural pra cima, é um acerto crítico
        if(atkRoll-player->atkMod > 17) {
            printSlow(" \033[33;4mAcerto Critico!\033[0m Dados de dano dobrados pra esse ataque.\n\n");
            dmgRoll = rollDice(player->dmgDice, player->dmgDiceNum*2, player->dmgMod*2, 0);
            printSlow("Rolagem de dano - \033[36mrolando ");
            printf("%id%i%+i", player->dmgDiceNum*2, player->dmgDice, player->dmgMod*2);
            rollSlow(dmgRoll);
            printSlow("\n\nSua lamina atinge o alvo com precisao brutal, causando dano massivo.\n\n");

            enemy->hp -= dmgRoll;
            return 2;
        } 
        // Se for acima da armadura do inimigo, acerta
        else if (atkRoll >= enemy->armor) { 
            printSlow(" \033[33;4mAcerto!\033[0m\n\n");
            dmgRoll = rollDice(player->dmgDice, player->dmgDiceNum, player->dmgMod, 0);
            printSlow("Rolagem de dano - \033[36mrolando ");
            printf("%id%i%+i", player->dmgDiceNum, player->dmgDice, player->dmgMod);
            rollSlow(dmgRoll);
            printSlow("\n\nVoce ataca a criatura, que falha em se esquivar e recua com um grito.\n\n");

            enemy->hp -= dmgRoll;
            return 1;
        }
        else {
            printSlow(" \033[33;4mFalha...\033[0m\n\nO oponente desvia agilmente do seu golpe, saltando para o lado. A lamina encontra apenas terra.\n\n");
            return 0;
        }
    }


/* ======= Skills ======= */

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

    // Ataca duas vezes.
    int doubleStrike (playerS* player, enemyS *enemy) {
        playerAtk(player, enemy);
        playerAtk(player, enemy);
        
        return 1;
    }

    // Ataca e ganha vantagem nos ataques no turno seguinte.
    int tripAttack (playerS* player, enemyS *enemy) {
        if (playerAtk(player, enemy)) {
            player->status[tripAtk] = true;
            player->advantage++;
        }

        return 1;
    }

    // Dá dano no player, pra debug.
    int debugSelfDmg (playerS* player, enemyS *enemy) {
        player->hp -= 10;
        printSlow("Ai.\n\n");

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
        {
            &debugSelfDmg,
            "Auto-esfaquear",
            "Da dano em si mesmo. Ai."
        },
    };

    /* ==== Imprimir menu de habilidades ==== */

    // Imprime o menu de skills
    void printSkills() {
        int option = 0, i = 0;

        printf("\n");
        for(i=0; i<NUM_SKILLS; i++) {
            printf("\033[33m%i:\033[0m ", i+1);
            puts(skills[i].name);
        }
        printf("\033[33m%i: \033[36mCancelar\033[0m\n", i+1);
    }

    // Lê a escolha de skill do player
    int readSkill(playerS *player, enemyS *enemy) {
        int option = 1;

        while (1) {
            printf("\nEscolha um feitico:\n> "); // Lê a opção
            scanf("%i", &option);
            printf("\n");

            if(option>0 && option<=NUM_SKILLS) {                  // Se a opção é um habilidade, usa ela.
                printInfo(*player, *enemy);
                if(skills[option-1].funct (player, enemy)) break; // Se ele retornar 1, acaba o loop. habilidades retornam 0 se eles não funcionam 
            }                                                     // (Exemplo: usa Rasteira quando ela já está em efeito)
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


/* ======= Feitiços ====== */

    /* ==== Funções de feitiço ==== */

    // Rola o ataque pra um feitiço.
    int spellAtk (playerS* player) {
        int atkRoll = rollDice(20, 1, player->magMod, player->advantage);

        printSlow("Rolagem de ataque magico - \033[36mrolando ");
        printf("1d20%+i", player->magMod);
        rollSlow(atkRoll);

        return atkRoll;
    }

    // Um feitiço de dano genérico. Retorna se acertou ou errou, pra efeitos adicionais.
    int spellDmg (playerS* player, enemyS *enemy, int dmgDie, int dmgDieNum, char* strHit, char* strMiss) {
        int dmgRoll = 0;
        int atkRoll = spellAtk (player);

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


    /* ==== Lista de feitiços ==== */

    // Um feitiço simples, que dá uma boa quantidade de dano.
    int fireBolt (playerS* player, enemyS *enemy) {
        spellDmg (player, enemy, 6, 2,
        "\n\nO dardo de fogo acerta a criatura, estalando e criando um estouro de chamas.\n\n", 
        " \033[33;4mFalha...\033[0m\n\nO alvo se abaixa para fora do caminho do projetil, que dispara por cima dele e se dissipa numa nuvem de brasas.\n\n");

        return 1;
    }

    // Dá dano médio e diminui armadura.
    int sonicBlast (playerS* player, enemyS *enemy) {
        int result = spellDmg (player, enemy, 8, 1,
        "\n\nO pulso sonico colide com o alvo, com um impacto pesado. O som ressoante rasga um buraco em sua armadura de couro. \033[33mArmadura do inimigo -1!\033[0m\n\n", 
        " \033[33;4mFalha...\033[0m\n\nO feixe atinge o escudo de bronze da criatura, estourando num zumbido estrondoso.\n\n");

        if (result) {
            enemy->armor--;
        }

        return 1;
    }

    // Aumenta a armadura do player. Não acumula.
    int mageArmor (playerS *player, enemyS *enemy) {
        if(player->status[mageArm]) {
            printSlow("Esse feitico ja esta em efeito!\n\n");
            return 0;
        } else {
            player->status[mageArm] = true;
            player->armor += 2;
            printSlow("Voce conjura uma armadura translucida de energia protetiva em volta de si. \033[33mArmadura +2!\033[0m\n\n");
        }

        return 1;
    }

    // Aumenta muito a armadura do player, por 1 turno. Não acumula.
    int mageShield (playerS *player, enemyS *enemy) {
        if(player->status[mageShld]) {
            printSlow("Esse feitico ja esta em efeito!\n\n");
            return 0;
        } else {
            player->status[mageShld] = 2;
            player->armor += 5;
            printSlow("Com uma runa protetora, voce conjura um escudo flutuante de energia arcana. \033[33mArmadura +5!\033[0m\n\n");
        }

        return 1;
    }

    // Dá dano baixo e acerta sempre, sem precisar rolar ataque.
    int magicMissile (playerS *player, enemyS *enemy) {
        int dmgRoll = rollDice(4, 3, 1, 0);

        printSlow("Rolagem de dano - \033[36mrolando ");
        printf("%id%i%+i", 3, 4, 1);
        rollSlow(dmgRoll);
        printSlow("\n\nTres dardos de energia se materializam, se curvando pelo ar e liberando pulsos de forca arcana ao atingirem o alvo.\n\n");
        enemy->hp -= dmgRoll;

        return 1;
    }

    /* ==== Criar o array de feitiços ==== */

    spellS spells[NUM_SPELLS] = {
        {
            &fireBolt, // Função de quando o feitiço é conjurado
            "Dardo de Fogo", // Nome do feitiço
            "Causa dano alto.", // Descrição do feitiço
            5 // Custo de mana do feitiço
        },
        {
            &sonicBlast,
            "Pulso Sonico",
            "Causa dano medio e diminui a armadura do alvo em -1.",
            6
        },
        {
            &mageArmor,
            "Armadura Arcana",
            "Aumenta sua armadura em +2 permanentemente.",
            2
        },
        {
            &mageShield,
            "Escudo Arcano",
            "Aumenta sua armadura em +5 por 1 turno.",
            1
        },
        {
            &magicMissile,
            "Misseis Magicos",
            "Causa dano baixo e nunca erra, nao importa a armadura do alvo.",
            3
        }
    };

    /* ==== Imprimir menu de feitiços ==== */

    void printSpells() {
        int option = 0, i = 0;

        printf("\n");
        for(i=0; i<NUM_SPELLS; i++) {
            printf("\033[33m%i:\033[0m ", i+1);
            fputs(spells[i].name, stdout);
            printf(" \033[94m(%i mana)\033[0m\n", spells[i].cost);
        }
        printf("\033[33m%i: \033[36mCancelar\033[0m\n", i+1);
    }

    int readSpell(playerS *player, enemyS *enemy) {
        int option = 1;

        while (1) {
            printf("\nEscolha um feitico:\n> "); // Lê a opção
            scanf("%i", &option);
            printf("\n");

            if(option>0 && option<=NUM_SPELLS) {    // Se a opção é um feitiço, conjura ele.
                if(player->mana >= spells[option-1].cost) {
                    printInfo(*player, *enemy);

                    if(spells[option-1].funct (player, enemy)) {
                        player->mana -= spells[option-1].cost;
                        break;  // Se ele retornar 1, acaba o loop. Feitiços retornam 0 se eles não funcionam 
                    }           // (Exemplo: conjura Armadura Arcana quando ela já está em efeito)
                }                                                     
                else {
                    printf("Voce nao tem mana o suficiente para usar esse feitico!\n"); // Se não tem mana suficiente, não usa o feitiço
                }
            }
            else if(option==NUM_SPELLS+1) {  
                return 1; // Se a opção for cancelar, volta pro menu
            } 
            else {
                printf("Opcao invalida! (tem que ser um numero de 1 a %i).\n", NUM_SPELLS+1); // Se não for válida, pede pra colocar outra
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


/* ======= Status ======= */

    // Checa o array de status do player e faz os efeitos de cada status
    void updateStatus(playerS *player) {
        for (int i=0; i<NUM_STATUSES; i++) {
            switch (i)
            {
            case mageShld:
                if (player->status[mageShld] == 1) player->armor -= 5; // Quando mageShld está pra acabar, diminui a armadura em 5.
                if (player->status[mageShld] > 0) player->status[mageShld]--; // Mage Shield dura pelo turno que ele foi ativado e o próximo turno.
                
                break;

            case tripAtk:
                if (player->status[tripAtk] == true) {
                    player->status[tripAtk] == false;
                    player->advantage--;
                }
                
                break;                                                    
            
            default:
                break;
            }
        }
    }