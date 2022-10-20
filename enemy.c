/**
* @file: enemy
* @brief: Funções de controle do inimigo
* @author: Eduardo Santos Birchal
*/
#include "gameFuncts.h"

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
    fscanf(file, "%*s %i", &(enemy.magMod));                // Modificador de magia

    // Pegando strings de ataque
    fscanf(file, "%*c");
    getStringFromFile(file, MAX_NARRATE, enemy.hitString);  // String de acerto
    getStringFromFile(file, MAX_NARRATE, enemy.missString); // String de falha
    getStringFromFile(file, MAX_NARRATE, enemy.critString); // String de crítico

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

// Acerto crítico do inimigo
int enemyCrit(playerS *player, enemyS *enemy) {
    // Rola o dado
    int dmgRoll = rollDice(enemy->dmgDice, enemy->dmgDiceNum*2, enemy->dmgMod*2, 0);

    // Imprime as coisas
    printSlow("Rolagem de dano - \033[36mrolando ");
    printf("%id%i%+i", enemy->dmgDiceNum*2, enemy->dmgDice, enemy->dmgMod*2);
    printRollResult(dmgRoll);
    printf("\n\n");
    printSlow(enemy->critString);
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
    printRollResult(dmgRoll);
    printf("\n\n");
    printSlow(enemy->hitString);
    printf("\n\n");

    // Dá o dano
    player->hp -= dmgRoll;
}

// Um ataque do inimigo
int enemyAtk(playerS *player, enemyS *enemy) {
    // Rola o ataque
    int atkRoll = rollDice(20, 1, enemy->atkMod, enemy->advantage);

    // Imprime a rolagem
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
        printSlow(" \033[33;4mFalha!\033[0m\n\n");

        printSlow(enemy->missString);
        printf("\n\n");

        requestEnter();
        return 0;
    }
}



// Rola o ataque pra um feitiço.
int enemySkillAtk (enemyS *enemy) {
    int atkRoll = rollDice(20, 1, enemy->magMod, enemy->advantage);

    printSlow("Rolagem de ataque magico - \033[36mrolando ");
    printf("1d20%+i", enemy->magMod);
    printRollResult(atkRoll);

    return atkRoll;
}

// Um feitiço de dano genérico. Retorna se acertou ou errou, pra efeitos adicionais.
int enemySkillDmg (playerS* player, enemyS *enemy, int dmgDie, int dmgDieNum, char* strHit, char* strMiss) {
    int dmgRoll = 0;
    int atkRoll = enemySkillAtk (enemy);

    if (atkRoll >= enemy->armor) { // Se acertou, dá dano e imprime 'strHit'.
        printSlow(" \033[33;4mAcerto!\033[0m\n\n");
        dmgRoll = rollDice(dmgDie, dmgDieNum, enemy->magMod, 0);
        printSlow("Rolagem de dano - \033[36mrolando ");
        printf("%id%i%+i", dmgDieNum, dmgDie, enemy->magMod);
        printRollResult(dmgRoll);
        printSlow(strHit);
        enemy->hp -= dmgRoll;

        return 1;
    }
    else { // Senão, imprime 'strMiss'.
        printSlow(strMiss);
        return 0;
    }
}