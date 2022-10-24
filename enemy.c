/**
* @file: enemy
* @brief: Funções de controle do inimigo
* @author: Eduardo Santos Birchal
*/
#include "gameFuncts.h"


/* ==== Typedefs ==== */

typedef struct enemySkill_s {
    sklFunct funct; // sklFunct é um ponteiro de função. Esse tipo é usado para feitiços, habilidades e itens.
    char name[MAX_ENEMY_SKILL];

    int maxCooldown;
    int cooldown;

    bool signature; // Se uma skill é signature, ela é específica ao inimigo e tem uma descrição narrativa.
} enemySkillS;


/* ==== Criação do inimigo ==== */

    // Pega uma string de um arquivo
    void getStringFromFile(FILE* file, int max, char* string) {
        fgets(string, max, file);
        string[strcspn(string, "\n")] = 0;
    }

    // Cria o inimigo
    enemyS createEnemy(int index) {
        FILE *file;
        int linesDetected = 0; // linesDetected guarda o número de linhas puladas até chegar no inimigo certo no arquivo
        char stringScan[6]; // A string que foi lida para ser comparada

        enemyS enemy;

        // Abrindo o arquivo
        file = fopen("enemies.txt", "r");
        if (file == NULL) {
            printf("\033[91mERRO: \033[0mArquivo 'enemies.txt' nao encontrado! Fechando programa...\n\n");
            exit(1);
        }

        // A string "====" marca o início de um inimigo no arquivo. Esse loop lê os "====" até chegar no [index]o inimigo. Se ele não achar, dá erro.
        while (linesDetected < index) {
            if (fgets(stringScan, 6, file)) {
                if (strstr(stringScan, "====") != 0) {
                    linesDetected++;
                }
            }
            else {
                printf("\033[91mERRO: \033[0mInimigo %i nao encontrado no arquivo! Fechando programa...\n\n", index);
                fclose(file);
                exit(1);
            }
        }

        getStringFromFile(file, MAX_NAME, enemy.name);          // Pegando nome

        // Pegando outros atributos
        fscanf(file, "%*s %i", &(enemy.hpMax));                 // Máximo de HP
        fscanf(file, "%*s %i", &(enemy.dmgDiceNum));            // Número de dados de dano
        fscanf(file, "%*s %i", &(enemy.dmgDice));               // Tamanho dos dados de dano
        fscanf(file, "%*s %i", &(enemy.dmgMod));                // Modificador de dano
        fscanf(file, "%*s %i", &(enemy.atkMod));                // Modificador de ataque
        fscanf(file, "%*s %i", &(enemy.atkNum));                // Número de ataques por turno
        fscanf(file, "%*s %i", &(enemy.armor));                 // Armadura
        fscanf(file, "%*s %i", &(enemy.skillMod));              // Modificador de habilidade

        // Pegando strings de ataque
        fgetc(file);                                            // Pega um \n pra não atrapalhar os fgets

        getStringFromFile(file, MAX_ENEMY_SKILL, enemy.atkName);    // Nome do ataque básico

        // Fechando o arquivo
        fclose(file);

        // Inicializa o vetor de status do inimigo
        for(int i=0; i<NUM_STATUSES; i++) {
            enemy.status[i] = false;     
        }
        
        enemy.advantage = 0;
        enemy.hp = enemy.hpMax;

        return enemy;
    }


/* ==== Funções gerais de habilidade ==== */

    void announceAtk(enemyS *enemy) {
        printf("\033[91m");
        printSlow(enemy->name);
        printSlow("\033[0m usa \033[91m");
        printSlow(enemy->atkName);
        printSlow("\033[0m!\n\n");
    }

    // Acerto crítico do inimigo
    int enemyCrit(playerS *player, enemyS *enemy) {
        // Rola o dado
        int dmgRoll = rollDice(enemy->dmgDice, enemy->dmgDiceNum*2, enemy->dmgMod*2, 0);

        // Imprime as coisas
        printSlow("Rolagem de dano - \033[36mrolando ");
        printf("%id%i%+i", enemy->dmgDiceNum*2, enemy->dmgDice, enemy->dmgMod*2);
        printCustomResult(dmgRoll, "dano");
        printf("\n\n");

        // Dá o dano
        player->hp -= dmgRoll;
    }

    // Acerto não-crítico do inimigo
    int enemyHit(playerS *player, enemyS *enemy) {
        // Rola o dado
        int dmgRoll = rollDice(enemy->dmgDice, enemy->dmgDiceNum, enemy->dmgMod, 0);

        // Imprime as coisas
        printSlow("Rolagem de dano - \033[36mrolando ");
        printf("%id%i%+i", enemy->dmgDiceNum, enemy->dmgDice, enemy->dmgMod);
        printCustomResult(dmgRoll, "dano");
        printf("\n\n");

        // Dá o dano
        player->hp -= dmgRoll;
    }

    // Um ataque do inimigo
    int enemyAtk(playerS *player, enemyS *enemy) {
        // Rola o ataque
        int atkRoll = rollDice(20, 1, enemy->atkMod, enemy->advantage);

        // Imprime a rolagem
        announceAtk(enemy);
        
        printSlow("Rolagem de ataque do inimigo - \033[36mrolando ");
        printf("1d20%+i", enemy->atkMod);
        printRollResult(atkRoll);
        
        // Se for 17 natural pra cima, é um acerto crítico
        if(atkRoll-enemy->atkMod > 17) {
            printSlow(" \033[33;4mAcerto Critico!\033[0m Dados de dano dobrados pra esse ataque.\n\n");

            enemyCrit(player, enemy);
            requestEnter();
            return 2;
        }
        // Se for acima da armadura do player, acerta
        else if (atkRoll >= player->armor) { 
            printSlow(" \033[33;4mAcerto!\033[0m\n\n");
            
            enemyHit(player, enemy);
            requestEnter();
            return 1;
        }
        // Senão, erra
        else {
            printSlow(" \033[33;4mFalha!\033[0m O ataque erra!\n\n");

            requestEnter();
            return 0;
        }
    }

    // Rola o ataque pra uma habilidade que não é um ataque básico.
    int enemySkillAtk (enemyS *enemy) {
        int atkRoll = rollDice(20, 1, enemy->skillMod, enemy->advantage);

        printSlow("Rolagem de ataque - \033[36mrolando ");
        printf("1d20%+i", enemy->skillMod);
        printRollResult(atkRoll);

        return atkRoll;
    }

    int enemySkillDmg (playerS* player, enemyS *enemy, int dmgDie, int dmgDieNum) {
        int dmgRoll = rollDice(dmgDie, dmgDieNum, enemy->skillMod, 0);

        printSlow("Rolagem de dano - \033[36mrolando ");
        printf("%id%i%+i", dmgDieNum, dmgDie, enemy->skillMod);
        printCustomResult(dmgRoll, "dano");
        
        enemy->hp -= dmgRoll;
        return dmgRoll;
    }

    // Uma habilidade de dano genérica. Retorna o dano, pra efeitos adicionais.
    int enemySkillAtkDmg (playerS* player, enemyS *enemy, int dmgDieNum, int dmgDie) {
        int atkRoll = enemySkillAtk (enemy);

        if (atkRoll >= player->armor) { // Se acertou, dá dano e retorna true
            printSlow(" \033[33;4mAcerto!\033[0m\n\n");

            return enemySkillDmg(player, enemy, dmgDie, dmgDieNum);
        }
        else { // Senão, retorna 0
            printSlow(" \033[33;4mFalha! O ataque erra!\033[0m\n\n");

            return 0;
        }
    }

    int enemySkillHeal (enemyS *enemy, int healDieNum, int healDie, int healMod) {
        int healRoll = rollDice(healDieNum, healDie, healMod, 0);

        printSlow("Rolagem de cura - \033[36mrolando ");
        printf("%id%i%+i", healDieNum, healDie, healMod);
        printRollResult(healRoll);

        enemy->hp += healRoll;
        return healRoll;
    }

    void announceSkill (enemyS *enemy, enemySkillS skill) {
        printf("\033[91m");
        printSlow(enemy->name);
        printSlow("\033[0m usa ");

        if(skill.signature) printf("\033[33m"); // Se for uma skill signature, coloca ela em amarelo em vez de vermelho claro
        else printf("\033[91m");

        printSlow(skill.name);
        printSlow("\033[0m!\n\n");
    }


/* ==== Funções de habilidades genéricas ==== */

    // Dá dano médio
    bool fireBoltE (playerS *player, enemyS *enemy) { // Toda skill de inimigo tem 'E' no final, pra diferenciar das de player
        enemySkillAtkDmg (player, enemy, 3, 8);

        return true;
    }

    // Cura o usuário
    bool regenerateE (playerS *player, enemyS *enemy) {
        enemySkillHeal (enemy, 2, 4, enemy->skillMod);

        return true;
    }

    bool leechAttackE (playerS *player, enemyS *enemy) {
        int healAmt = enemySkillAtkDmg(player, enemy, 2, 6);

        if (healAmt) {
            printSlow("O inimigo recebe ");
            printf("\033[36m%i\033[0m", healAmt/2);
            printSlow("de cura!\n\n");

            enemy->hp += healAmt/2;
        }
        
        return true;
    }