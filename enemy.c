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
        if (fgets(stringScan, 6, file)) { // AVISO PARA EU DO FUTURO: ****N  U  N  C  A**** USE FSCANF COM %S!!!!!!!!!!!!!!
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

    // Nome
    fgets(enemy.name, MAX_NAME, file);
    printf("NOME = %s", enemy.name);

    fscanf(file, "%*s %i", &(enemy.hpMax));
    
    fclose(file);

    exit(1);
    return enemy;
}