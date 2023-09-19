/**
* @file: enemy
* @brief: Funções de controle do inimigo
* @author: Eduardo Santos Birchal
*/
#include "gameFuncts.h"


/* ==== Typedefs e Defines ==== */

#define NUM_ENEMY_SKILLS 6

#define BASE_ATTACK_WEIGHT 3
#define BASE_ATTACK_BIAS 0

#define DAMAGE_BIAS 1
#define HEAL_BIAS 0
#define DEFENSIVE_BIAS -1
#define TACTICAL_BIAS -1

enum enemyActions {damage, heal, defend, tactical};

extern playerS player;
extern enemyS enemy;

typedef struct enemySkill_s {
    sklFunct funct; // sklFunct é um ponteiro de função. Esse tipo é usado para feitiços, habilidades e itens.
    char name[MAX_ENEMY_SKILL];

    int maxCooldown;
    int manaCost;
    bool signature; // Se uma skill é signature, ela é específica ao inimigo e tem uma descrição narrativa.

    int actionWeights[NUM_ENEMY_ACTIONS]; // Os pesos pra cada ação. Uma skill de dar dano e diminuir ataque teria, por exemplo, 2 no índice damage e 1 no índice debuff

    int cooldown;
} enemySkillS;


/* ==== Funções gerais de habilidade ==== */

    // Anuncia o ataque do inimigo
    void announceAtkE() {
        printf("\033[91m");
        printSlow(enemy.name);
        printSlow(_("\033[0m usa \033[91m"));
        printSlow(enemy.atkName);
        printSlow(_("\033[0m!\n\n"));
    }

    // Acerto crítico do inimigo
    int enemyCrit() {
        // Rola o dado
        int dmgRoll = rollDice(enemy.dmgDice, enemy.dmgDiceNum*2, enemy.dmgMod*2, 0);

        // Imprime as coisas
        printSlow(_("Rolagem de dano - \033[36mrolando "));
        printf("%id%i%+i", enemy.dmgDiceNum*2, enemy.dmgDice, enemy.dmgMod*2);
        printCustomResult(dmgRoll, "dano");
        printf("\n\n");

        // Dá o dano
        player.hp -= dmgRoll;
    }

    // Acerto não-crítico do inimigo
    int enemyHit() {
        // Rola o dado
        int dmgRoll = rollDice(enemy.dmgDice, enemy.dmgDiceNum, enemy.dmgMod, 0);

        // Imprime as coisas
        printSlow(_("Rolagem de dano - \033[36mrolando "));
        printf("%id%i%+i", enemy.dmgDiceNum, enemy.dmgDice, enemy.dmgMod);
        printCustomResult(dmgRoll, "dano");
        printf("\n\n");

        // Dá o dano
        player.hp -= dmgRoll;
    }

    // Um ataque do inimigo
    int enemyAtk() {
        // Rola o ataque
        int atkRoll = rollDice(20, 1, enemy.atkMod, enemy.advantage);

        // Imprime a rolagem
        announceAtkE(enemy);

        printSlow(_("Rolagem de ataque do inimigo - \033[36mrolando "));
        printf("1d20%+i", enemy.atkMod);
        printRollResult(atkRoll);
        
        // Se for 17 natural pra cima, é um acerto crítico
        if(atkRoll-enemy.atkMod > 17) {
            printSlow(_(" \033[33;4mAcerto Critico!\033[0m Dados de dano dobrados pra esse ataque.\n\n"));

            enemyCrit();
            requestEnter();
            return 2;
        }
        // Se for acima da armadura do player, acerta
        else if (atkRoll >= player.armor) { 
            printSlow(_(" \033[33;4mAcerto!\033[0m\n\n"));
            
            enemyHit();
            requestEnter();
            return 1;
        }
        // Senão, erra
        else {
            printSlow(_(" \033[33;4mFalha!\033[0m O ataque erra!\n\n"));

            requestEnter();
            return 0;
        }
    }


/* ==== Funções de habilidades genéricas ==== */

    // Rola o ataque pra uma habilidade que não é um ataque básico.
    int enemySkillAtk () {
        int atkRoll = rollDice(20, 1, enemy.skillMod, enemy.advantage);

        printSlow(_("Rolagem de ataque - \033[36mrolando "));
        printf("1d20%+i", enemy.skillMod);
        printRollResult(atkRoll);

        return atkRoll;
    }

    // Dá dano de uma skill
    int enemySkillDmg (int dmgDie, int dmgDieNum) {
        int dmgRoll = rollDice(dmgDie, dmgDieNum, enemy.dmgMod, 0);

        printSlow(_("Rolagem de dano - \033[36mrolando "));
        printf("%id%i%+i", dmgDieNum, dmgDie, enemy.dmgMod);
        printCustomResult(dmgRoll, "dano");
        
        player.hp -= dmgRoll;
        return dmgRoll;
    }

    // Uma habilidade de dano genérica. Retorna o dano, pra efeitos adicionais.
    int enemySkillAtkDmg (int dmgDieNum, int dmgDie) {
        int atkRoll = enemySkillAtk (enemy);

        if (atkRoll >= player.armor) { // Se acertou, dá dano e retorna true
            printSlow(_(" \033[33;4mAcerto!\033[0m\n\n"));

            return enemySkillDmg(dmgDie, dmgDieNum);
        }
        else { // Senão, retorna 0
            printSlow(_(" \033[33;4mFalha! O ataque erra!\033[0m\n\n"));

            return 0;
        }
    }

    // Uma skill de cura genérica
    int enemySkillHeal (int healDieNum, int healDie, int healMod) {
        int healRoll = rollDice(healDieNum, healDie, healMod, 0);

        printSlow(_("Rolagem de cura - \033[36mrolando "));
        printf("%id%i%+i", healDieNum, healDie, healMod);
        printCustomResult(healRoll, "cura");

        enemy.hp += healRoll;
        return healRoll;
    }


/* ==== Skills não-signature ==== */

    // Dá dano médio
    bool fireBoltE () { // Toda skill de inimigo tem 'E' no final, pra diferenciar das de player
        enemySkillAtkDmg (2, 8);

        return true;
    }

    // Cura o usuário
    bool regenerateE () {
        enemySkillHeal (3, 4, enemy.skillMod);

        return true;
    }

    // Dá dano e cura metade do dano causado
    bool leechAttackE () {
        int healAmt = enemySkillAtkDmg(2, 6);

        if (healAmt) {
            printSlow(_("O inimigo recebe "));
            printf("\033[36m%i\033[0m", healAmt/2);
            printSlow(_(" de cura!\n\n"));

            enemy.hp += healAmt/2;
        }
        
        return true;
    }

    // Aumenta a armadura em +1
    bool fortifyE () {
        enemy.armor++;
        printSlow(_("\033[33mArmadura do inimigo +1!\033[0m\n\n"));

        return true;
    }

    // Ataca e aplica um efeito de veneno se acertar

    bool poisonAtkE () {
        if (enemySkillAtkDmg(2, 6)) {
            int psnRoll = rollDice(4, 1, 0, 0);

            printSlow(_("Rolagem de veneno - \033[36mrolando "));
            printf("%id%i%+i", 1, 4, 0);
            printCustomResult(psnRoll, "veneno");

            player.status[poisonedS] += psnRoll;
        }
    }

    // Aplica um efeito de enfraquecimento 
    bool sapStrengthE () {
        int wknRoll = rollDice(1, 4, 1, 0);

        printSlow(_("Rolagem de enfraquecimento - \033[36mrolando "));
        printf("%id%i%+i", 1, 4, 1);
        printCustomResult(wknRoll, "enfraquecimento");

        player.status[weakenedS] += wknRoll;
    }


/* ==== Array de habilidades ==== */

    const enemySkillS skillsE[NUM_ENEMY_SKILLS] = {
        {
            &fireBoltE,         // Função
            "Dardo de Fogo",    // Nome
            3,                  // Cooldown
            0,                  // Custo de mana (só skills poderosas custam mana)
            false,              // É signature
            {3, 0, 0, 0},       // Pesos de cada ação (dano, cura, defensiva, tática)
        },
        {
            &regenerateE,
            "Regenerar",
            4,
            7,
            false,
            {0, 3, 0, 0},
        },
        {
            &leechAttackE,
            "Roubar Vitalidade",
            2,
            0,
            false,
            {2, 1, 0, 0},
        },
        {
            &fortifyE,
            "Fortificar",
            2,
            5,
            false,
            {0, 0, 3, 0},
        },
        {
            &poisonAtkE,
            "Ataque Venenoso",
            4,
            0,
            false,
            {1, 0, 0, 2},
        },
        {
            &sapStrengthE,
            "Extrair Forca",
            3,
            3,
            false,
            {0, 0, 0, 3},
        }
    };


/* ==== Funções de uso de habilidade ==== */

    // Anuncia a skill do inimigo
    void announceSkillE (enemySkillS skill) {
        printf("\033[91m");
        printSlow(enemy.name);
        printSlow(_("\033[0m usa "));

        if(skill.signature) printf("\033[33m"); // Se for uma skill signature, coloca ela em amarelo em vez de magenta
        else printf("\033[35m");

        printSlow(skill.name);
        printSlow("\033[0m!\n\n");
    }

    // Anuncia e usa uma skill
    void useSkillE (int index) {
        enemy.mana -= enemy.knownSkills[index].manaCost;
        enemy.knownSkills[index].cooldown = enemy.knownSkills[index].maxCooldown;
        announceSkillE (enemy.knownSkills[index]);
        enemy.knownSkills[index].funct();
        requestEnter();
    }

    // Aloca e preenche o vetor de skills do inimigo
    void initSkillsE () {
        enemy.knownSkills = malloc(sizeof(enemySkillS) * enemy.skillNum);  // Aloca o vetor de habilidades  

        for (int i=0; i<enemy.skillNum; i++) {
            enemy.knownSkills[i] = skillsE[enemy.skillCodes[i]]; // Pega o código na posição 'i' em skillCodes e coloca a skill correspondente ao código em knownSkills
            enemy.knownSkills[i].cooldown = 0; // Coloca o cooldown em 0
        }
    }


/* ==== Criação do inimigo ==== */

    // Cria o inimigo
    void createEnemy(int index) {
        FILE *file;
        int linesDetected = 0; // linesDetected guarda o número de linhas puladas até chegar no inimigo certo no arquivo.
        char stringScan[6]; // stringScan é a string que foi lida para ser comparada

        // Abrindo o arquivo
        file = fopen("enemies.txt", "r");
        if (file == NULL) {
            printf(_("\033[91mERRO: \033[0mArquivo 'enemies.txt' nao encontrado! Fechando programa...\n\n"));
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
                printf(_("\033[91mERRO: \033[0mInimigo %i nao encontrado no arquivo! Fechando programa...\n\n"), index);
                fclose(file);
                exit(1);
            }
        }

        getStringFromFile(file, MAX_NAME, enemy.name);                     // Pegando nome
        
        // Pegando outros atributos            
        fscanf(file, "%*s %i", &(enemy.hpMax));                            // Máximo de HP
        fscanf(file, "%*s %i", &(enemy.dmgDiceNum));                       // Número de dados de dano
        fscanf(file, "%*s %i", &(enemy.dmgDice));                          // Tamanho dos dados de dano
        fscanf(file, "%*s %i", &(enemy.dmgMod));                           // Modificador de dano
        fscanf(file, "%*s %i", &(enemy.atkMod));                           // Modificador de ataque
        fscanf(file, "%*s %i", &(enemy.atkNum));                           // Número de ataques por turno
        fscanf(file, "%*s %i", &(enemy.armor));                            // Armadura
        fscanf(file, "%*s %i", &(enemy.skillMod));                         // Modificador de habilidade
        fscanf(file, "%*s %i", &(enemy.skillNum));                         // Número de habilidades
        fscanf(file, "%*s %i", &(enemy.manaMax));                          // Máximo de Mana

        enemy.skillCodes = malloc(sizeof(int) * enemy.skillNum);           // Aloca o vetor de códigos de habilidades

        getArrayFromFile(file, enemy.skillNum, enemy.skillCodes);          // Códigos de habilidades     
        initSkillsE(&enemy);

        fgetc(file);                                                       // Pega um \n pra não atrapalhar os fgets
 
        getStringFromFile(file, MAX_ENEMY_SKILL, enemy.atkName);           // Nome do ataque básico

        // Fechando o arquivo
        fclose(file);

        // Inicializa o vetor de status do inimigo
        for(int i=0; i<NUM_ENEMY_STATUSES; i++) {
            enemy.status[i] = false;     
        }
        
        enemy.advantage = 0;
        enemy.hp = enemy.hpMax;
        enemy.mana = enemy.manaMax;
        enemy.tacticalsInARow = 0;
    }


/* ==== IA do inimigo ==== */

    // Retorna a segurança do inimigo ou player em relação a HP, numa escala de 0 a 10
    int calcHpSafety (int max, int current) {
        double safety = (double) current / (double) max; // Ganha uma fração entre o HP máximo e o atual

        safety *= 10;  // Multiplica por 10, movendo ela uma casa decimal pra frente 
        round(safety); // Arredonda pra ficar na escala, ficando no mínimo 0 e no máximo 10

        return (int) safety;
    }

    // Retorna a segurança do inimigo em relação a ser acertado por ataques, numa escala de 0 a 10
    int calcHitSafety (int armor, int atkMod, int magMod, int atkNum) {
        int highestMod = (atkMod > magMod) ? atkMod : magMod; // Pega o maior modificador entre o de magia e o de ataque, já que o player provavelmente vai usar o maior

        double safety = armor - highestMod; // Chance, em 20, do inimigo ser acertado em um ataque 
        
        safety /= 2;
        round(safety); // Divide por 2 pra ficar chance em 10 e arredonda
        safety /= atkNum;
        round(safety); // Divide pelo número de ataques pra ganhar a chance de ser acertado em um turno

        return (int) safety;
    }

    // Retorna o quão recomendável é pro inimigo tomar uma ação de dano, numa escala de 0 a 10 
    int damageWeight () {
        int enemySafety = calcHpSafety (enemy.hpMax, enemy.hp) + calcHitSafety (enemy.armor, player.atkMod, player. magMod, player.atkNum);
        int playerSafety = calcHitSafety (player.armor, enemy.atkMod, enemy.skillMod, enemy.atkNum);
        int weight = 0;

        enemySafety /= 2;
        weight = (enemySafety-playerSafety+10) / 2;

        return weight+DAMAGE_BIAS; // Divide por 2 porque a escala original é -10 a 10. Subtrai playerSafety porque se o player está muito seguro, é melhor tomar uma ação tática. Adiciona 10 porque originalmente ficaria -10 a 20 em vez de 0 a 30
    }

    // Retorna o quão recomendável é pro inimigo tomar uma ação de cura, numa escala de 0 a 10
    int healWeight () {
        int hpSafety = calcHpSafety (enemy.hpMax, enemy.hp);
        int weight = 0;

        weight = 10-hpSafety;

        return weight+HEAL_BIAS; // Quanto menos seguro o inimigo, melhor é ele se curar. Sendo assim, subtrai hpSafety de 10 pra ficar no máximo 10 e no mínimo 0
    }

    // Retorna o quão recomendável é pro inimigo tomar uma ação defensiva, numa escala de 0 a 10
    int defendWeight () {
        int hitSafety = calcHitSafety (enemy.armor, player.atkMod, player. magMod, player.atkNum);
        int weight = 0;

        weight = 10-hitSafety;

        return weight+DEFENSIVE_BIAS;
    }

    // Retorna o quão recomendável é pro inimigo tomar uma ação tática, numa escala de -5 a 5
    int tacticalWeight () {
        int playerSafety = calcHitSafety (player.armor, enemy.atkMod, enemy.skillMod, enemy.atkNum);
        int numTacticals = enemy.tacticalsInARow;
        int weight = 0;

        weight = 10-playerSafety-numTacticals; // Subtrai o número de táticas seguidas pra evitar spam de táticas

        return weight+TACTICAL_BIAS; 
    }

    // Insere, num array de ações, os pesos de cada ação que o inimigo pode tomar
    void calcActionWeights (int *actionArray) {
        actionArray[damage] = damageWeight ();       // Ação de dano
        actionArray[heal] = healWeight ();           // Ação de cura
        actionArray[defend] = defendWeight ();       // Ação defensiva
        actionArray[tactical] = tacticalWeight ();   // Ação tática (buff em si mesmo ou debuff no player)
    }

    int calcSkillWeight (int *actionArray, enemySkillS skill) {
        int weight = 0;

        //char typeNames[4][12] = {"\033[31matk", "\033[32mheal", "\033[94mdef", "\033[35mtact"}; // Debug

        for (int i=0; i<NUM_ENEMY_ACTIONS; i++) {
            //printf("%s\033[0m = %i*%i | ", typeNames[i], skill.actionWeights[i], actionArray[i]); // Debug
            weight += skill.actionWeights[i] * actionArray[i];
        }

        //printf("\n"); // Debug
        return weight;
    }

    int decideActionE (int *actionArray) {
        int atkWeight = (BASE_ATTACK_WEIGHT * actionArray[damage]) + BASE_ATTACK_BIAS;
        int skillChoice = -1, skillWeight = atkWeight; // skillWeight começa como atkWeight, porque se nenhuma skill for muito preferível, o inimigo simplesmente ataca

        for(int i=0; i<enemy.skillNum; i++) {
            //printf("Skill: %s\n", enemy.knownSkills[i].name); // Debug
            int currentWeight = calcSkillWeight(actionArray, enemy.knownSkills[i]);
            //printf("currentWeight: \033[93m%i\033[0m, skillWeight: %i\n\n", currentWeight, skillWeight); // Debug

            // Só escolhe a skill se ela tiver maior prioridade que as outras e não custar mais mana do que o inimigo pode gastar e não estiver em cooldown
            if (enemy.knownSkills[i].manaCost <= enemy.mana && enemy.knownSkills[i].cooldown <= 0) {
                if (currentWeight > skillWeight) {
                    //printf("Substituicao "); // Debug
                    skillWeight = currentWeight;
                    skillChoice = i;
                }
                else if (currentWeight == skillWeight && (rand()%3)) { // Se for de igual prioridade, tem 1/3 de chance de substituir mesmo assim
                    skillWeight = currentWeight;
                    skillChoice = i;
                }
            }
        }

        return skillChoice;
    }

    // Aumenta o número de skills táticas seguidas se a skill for tática ou reseta se não for
    void updateTacticalCount (int choice) {
        if(enemy.knownSkills[choice].actionWeights[tactical] > 1) { // Se o peso de tática da skill for 2 ou mais, aumenta o número de táticas usadas
            enemy.tacticalsInARow++;
        }
        else {
            enemy.tacticalsInARow = 0; // Senão, reseta o número
        }
    }

    // Executa as funções do turno do inimigo
    void turnEnemy () {
        int actionWeights[NUM_ENEMY_ACTIONS];
        int choice;

        calcActionWeights(actionWeights);
        choice = decideActionE(actionWeights);

        if (choice < 0) {
            enemyAtk();
        }
        else {
            updateTacticalCount(choice);
            useSkillE(choice);
        }
    }

    // Diminui os cooldowns de toda habilidade do inimigo.
    void updateEnemyCooldown () {
        for(int i=0; i<enemy.skillNum; i++) {
            if(enemy.knownSkills[i].cooldown) enemy.knownSkills[i].cooldown--;
        }
    }