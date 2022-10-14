#include "gameFuncts.h"

enemyS createEnemy(int index) {
    FILE *file;
    int linesDetected=0;
    char stringScan[6];

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

    // Pegando nome
    fgets(enemy.name, MAX_NAME, file);
    enemy.name[strcspn(enemy.name, "\n")] = 0; // Deleta o \n do nome.

    // Pegando outros atributos
    fscanf(file, "%*s %i", &(enemy.hpMax));
    fscanf(file, "%*s %i", &(enemy.dmgDiceNum));
    fscanf(file, "%*s %i", &(enemy.dmgDice));
    fscanf(file, "%*s %i", &(enemy.dmgMod));
    fscanf(file, "%*s %i", &(enemy.atkMod));
    fscanf(file, "%*s %i", &(enemy.atkNum));
    fscanf(file, "%*s %i", &(enemy.armor));
    fscanf(file, "%*s %i", &(enemy.magMod));
    
    fclose(file);

    for(int i=0; i<NUM_STATUSES; i++) { // Eu vou ser honesto, o jogo nunca acessa o array de status do inimigo, mas por algum motivo se eu não fizer esse loop, o programa dá segfault.
        enemy.status[i] = false;        // Eu não tenho ideia do porque isso aconteceu e porque esse loop conserta, mas tá aí.
    }

    return enemy;
}