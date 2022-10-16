#include "gameFuncts.h"


/* ==== Defines ==== */

#define NUM_SKILLS 10
#define MAX_SKILL 25
#define MAX_DESC_SKILL 100

#define NUM_SPELLS 12
#define MAX_SPELL 25
#define MAX_DESC_SPELL 100


/* ==== Typedefs ==== */

typedef struct skill_s {
    sklFunct funct; // sklFunct é um ponteiro de função. Esse tipo é usado para feitiços, habilidades e itens.
    char name[MAX_SKILL];
    char desc[MAX_DESC_SKILL];

    int maxCooldown;
    int cooldown;
} skillS;

typedef struct spell_s {
    sklFunct funct;
    char name[MAX_SPELL];
    char desc[MAX_DESC_SPELL];

    int cost;
} spellS;

static int showDesc = true; // Mostrar descrição dos feitiços e habilidades


/* ======= Combate ======= */

    // Imprime a barra de HP
    void printHpBar (int hpMax, int hp) {
        // Imprime em verde as células de HP cheio e o resto (hp vazio) imprime em verde escuro
        for(int i=1; i<=hpMax; i++) {
            if(i <= (hp)) {
                printf("\033[102m "); // Célula verde
            }
            else {
                printf("\033[42m "); // Célula verde escura
            }
        }
    }

    // Imprime a quantidade de HP embaixo da barra, por exemplo: 'HP: 6/10'
    void printHpNum (int hpMax, int hp) {
        // Calcula o comprimento da string
        int hpMaxLen = digitNum(hpMax);
        int hpLen = digitNum(hp);
        hpLen = hpLen + hpMaxLen; // hpLen é o comprimento da string do valor do HP

        // Imprime a quantidade de HP
        printf("\033[0m\n");
        centerText(strlen("HP: ") + hpLen, BORDER_LEN); //  Centraliza o texto a ser imprimido
        printf("HP: %i/%i\n\n", hp, hpMax);
    }

    // Imprime uma barra de HP e a quantidade embaixo
    void printHp (int hpMax, int hp) {
        printHpBar(hpMax, hp);
        printHpNum(hpMax, hp);
    }

    // Imprime a barra de mana
    void printManaBar (int manaMax, int mana) {
        // Imprime em azul as células de mana cheio e o resto (mana vazio) imprime em azul escuro
        for(int i=1; i<=manaMax; i++) {
            if(i <= (mana)) {
                printf("\033[104m "); // Célula azul
            }
            else {
                printf("\033[44m "); // Célula azul escuro
            }
        }
    }

    // Imprime a quantidade de mana embaixo da barra, por exemplo: 'Mana: 6/10'
    void printManaNum (int manaMax, int mana) {
        // Calcula o comprimento da string
        int manaMaxLen = digitNum(manaMax);
        int manaLen = digitNum(mana);
        manaLen = manaLen + manaMaxLen; // manaLen é o comprimento da string do valor de mana

        // Imprime a quantidade de HP
        printf("\033[0m\n");
        centerText(strlen("Mana: ") + manaLen, BORDER_LEN);
        printf("Mana: %i/%i\n\n", mana, manaMax);
    }

    // Imprime uma barra de mana
    void printMana (int manaMax, int mana) {
        printManaBar(manaMax, mana);
        printManaNum(manaMax, mana);
    }

    // Conserta o HP pra não ficar negativo
    int updateHp (playerS *player, enemyS *enemy) {
        // Conserta HP do inimigo
        if(enemy->hp < 0) enemy->hp = 0; 
        if(enemy->hp > enemy->hpMax) enemy->hp = enemy->hpMax;

        // Conserta HP do player
        if(player->hp < 0) player->hp = 0;
        if(player->hp > player->hpMax) player->hp = player->hpMax;

        // Conserta mana do player
        if(player->mana < 0) player->mana = 0;
        if(player->mana > player->manaMax) player->mana = player->manaMax;

        if(enemy->hp == 0) return 1; // Retorna >0 se alguem morreu
        if(player->hp == 0) return 2;
        return 0;
    }

    // Rola [num] dados de [size] lados, com vantagem igual a [adv], e soma [mod] ao total
    int rollDice(int size, int num, int mod, int adv) {
        int total;
        int roll = 0;

        for(int j=0; j<num; j++) {
            roll += 1 + rand()%size; // Rola o dado

            //printf("\t%io dado: %i\n", j+1, roll); // Só usar pra debug.
        }
        total = roll;

        if(adv) printf("\033[92m(Rolando com vantagem x%i)\033[0m\n", adv);
        
        // Rola com vantagem, ou seja, rola [adv] vezes e escolhe o maior resultado
        for(int i=0; i<adv; i++) {
            roll = 0;

            for(int j=0; j<num; j++) {
                roll += 1 + rand()%size; // Rola o dado

                //printf("\t%io dado: %i\n", j+1, roll); // Só usar pra debug.
            }
            if (roll>total) total=roll; // Substitui o total se essa rolagem foi maior que as outras rolagens
        }

        return total+mod; // Adiciona o modificador ao total
    }

    // Dá o dano de Radiant Smite se o player tiver usado
    int rdntSmiteDmg(playerS *player, enemyS *enemy) {
        if (player->status[rdntSmiteS]) {
            player->status[rdntSmiteS] = false;
            int rdntRoll = rollDice(6, 2, 0, 0);

            printSlow("Rolagem de dano (Golpe Radiante) - \033[36mrolando ");
            printf("%id%i", 2, 6);
            rollSlow(rdntRoll);

            enemy->hp -= rdntRoll;

            printSlow("Voce canaliza toda a energia divina acumulada para a sua lamina, liberando-a num pulso reverberante de poder radiante.\n\n");
        }
    }

    // Acerto crítico do player
    int playerCrit(playerS *player, enemyS *enemy) {
        // Rola o dado
        int dmgRoll = rollDice(player->dmgDice, player->dmgDiceNum*2, player->dmgMod*2, 0);

        // Imprime as coisas
        printSlow("Rolagem de dano - \033[36mrolando ");
        printf("%id%i%+i", player->dmgDiceNum*2, player->dmgDice, player->dmgMod*2);
        rollSlow(dmgRoll);
        printSlow(player->critString);

        // Se o player usou Radiant Smite, dá o dano extra
        rdntSmiteDmg(player, enemy);

        // Dá o dano
        enemy->hp -= dmgRoll;
    }

    // Acerto não-crítico do player
    int playerHit(playerS *player, enemyS *enemy) {
        // Rola o dado
        int dmgRoll = rollDice(player->dmgDice, player->dmgDiceNum, player->dmgMod, 0);

        // Imprime as coisas
        printSlow("Rolagem de dano - \033[36mrolando ");
        printf("%id%i%+i", player->dmgDiceNum, player->dmgDice, player->dmgMod);
        rollSlow(dmgRoll);
        printSlow(player->hitString);

        // Se o player usou Radiant Smite, dá o dano extra
        rdntSmiteDmg(player, enemy);

        // Dá o dano
        enemy->hp -= dmgRoll;
    }

    // Faz o ataque do player
    int playerAtk(playerS *player, enemyS *enemy) {
        // Rola o ataque
        int atkRoll = rollDice(20, 1, player->atkMod, player->advantage);

        // Imprime a rolagem
        printSlow("Rolagem de ataque - \033[36mrolando ");
        printf("1d20%+i", player->atkMod);
        rollSlow(atkRoll);
        
        // Se for 17 natural pra cima, é um acerto crítico
        if(atkRoll-player->atkMod > 17) {
            printSlow(" \033[33;4mAcerto Critico!\033[0m Dados de dano dobrados pra esse ataque.\n\n");

            playerCrit(player, enemy);
            return 2;
        } 
        // Se for acima da armadura do inimigo, acerta
        else if (atkRoll >= enemy->armor) { 
            printSlow(" \033[33;4mAcerto!\033[0m\n\n");
            
            playerHit(player, enemy);
            return 1;
        }
        // Senão, erra
        else {
            printSlow(" \033[33;4mFalha...\033[0m");

            printSlow(player->missString);
            return 0;
        }
    }


/* ======= Skills ======= */

    /* ==== Funções de habilidade ==== */

    // Uma skill de cura genérica.
    int skillHeal (playerS* player, enemyS *enemy, int healDie, int healDieNum, int healMod, char* str) {
        int healRoll = rollDice(healDieNum, healDie, healMod, 0);

        printSlow("Rolagem de cura - \033[36mrolando ");
        printf("%id%i%+i", healDieNum, healDie, healMod);
        rollSlow(healRoll);
        printSlow(str);

        player->hp += healRoll;
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
            player->status[tripAtkS] = true;
            player->advantage++;

            printSlow("O seu ataque faz o inimigo cair no chao, vulneravel. \033[33mVantagem +1!\033[0m\n\n");
        }

        return 1;
    }

    // Dá dano no player, pra debug.
    int debugSelfDmg (playerS* player, enemyS *enemy) {
        player->hp -= 10;
        printSlow("Ai.\n\n");

        return 1;
    }

    // Ataca e ganha um bônus de armadura no turno seguinte.
    int parryAttack (playerS* player, enemyS *enemy) {
        playerAtk(player, enemy);

        player->status[parryAtkS] = 2;
        player->armor += 3;

        printSlow("Voce posiciona sua arma para bloquear o proximo ataque. \033[33mArmadura +3!\033[0m\n\n");

        return 1;
    }

    // Recupera HP.
    int secondWind (playerS* player, enemyS *enemy) {
        itemHeal(player, enemy, 6, 1, 2,
        "Voce recua e respira, recuperando seu folego e se sentindo descansado.\n\n");

        return 1;
    }

    // Ataca com um bônus de ataque e dano.
    int divineGuidance (playerS* player, enemyS *enemy) {
        player->atkMod += 5;
        player->dmgMod += 5;
        printSlow("Voce se concentra na sua divindade, deixando conhecimento divino guiar seu ataque. \033[33mAtaque e dano +5!\033[0m\n\n");

        playerAtk(player, enemy);

        player->atkMod -= 5;
        player->dmgMod -= 5;

        return 1;
    }

    // Recebe dano e recupera mana.
    int bloodOffering (playerS* player, enemyS *enemy) {
        int sacrifice;  

        printf("\nQuanto de HP sacrificar? (1 de HP recupera 3 Mana)\n\n> ");
        scanf("%i", &sacrifice);

        player->hp -= sacrifice;
        player->mana += sacrifice*3;

        printSlow("Voce corta o seu braco, deixando sangue sair. O sangue evapora ao tocar o chao, e voce sente energia magica fluindo para suas reservas.\n\n");
    
        return 1;
    }

    // Toma dano e recupera os cooldowns de tudo.
    int adrenalineSurge (playerS *player, enemyS *enemy) {
        printSlow("Apesar de estar exausto, voce obriga seus musculos a continuarem por pura forca de vontade.\n\n");

        // Repete o loop pra cada skill que o player tem
        for (int i=0; i<player->skillNum; i++) {
            // Se o player não vai morrer ao tomar o dano da skill, dá o dano
            if(player->hp > player->knownSkills[i].cooldown && player->hp > 4) {
                if(player->knownSkills[i].cooldown > 4) { 
                    player->hp -= player->knownSkills[i].cooldown; // Se o cooldown é maior que 4, toma o dano do cooldown
                }
                else if (player->knownSkills[i].cooldown > 0){     // Senão, dá 4 de dano
                    player->hp -= 4;
                }
                player->knownSkills[i].cooldown = 0; // Reseta o cooldown da skill
            }
            // Se o player fosse morrer ao tomar o dano, cancela a skill
            else {
                printSlow("Voce chegou ao seu limite. Se voce se exercesse mais ainda, voce morreria. (Nao tem HP suficiente para regenerar todas as habilidades)");
                break;
            }
        }
    }

    // Acerta o ataque sem precisar rolar.
    int precisionStrike (playerS *player, enemyS *enemy) {
        printSlow("Com um movimento gracioso, voce se posiciona para aproveitar uma brecha na defesa do alvo.\n");
        playerHit(player, enemy);
    }

    // Dá um boost de dano e ataque mas coloca todas as habilidades em cooldown por 4 turnos.
    int battleTrance (playerS *player, enemyS *enemy) {
        printSlow("Esquecendo toda a tecnica, voce confia o controle do seu corpo inteiramente aos seus instintos.\n");

        player->atkMod += 3;
        player->dmgMod += 3;

        // Coloca o cooldown de cada skill em no mínimo 4
        for (int i=0; i<player->skillNum; i++) {
            if(player->knownSkills[i].maxCooldown > 4) {
                player->knownSkills[i].cooldown = player->knownSkills[i].maxCooldown;
            }
            else {
                player->knownSkills[i].cooldown = 4;
            }
        }

        player->status[btlTranceS] = 4;
    }

    /* ==== Criar o array de habilidades ==== */

    const skillS skills[NUM_SKILLS] = {
        {
            &doubleStrike, // Função de quando a habilidade é usada
            "Golpe Duplo", // Nome da habilidade
            "Ataca duas vezes.", // Descrição da habilidade
            5 // Cooldown da habilidade
        },
        {
            &tripAttack,
            "Rasteira",
            "Da uma rasteira no alvo, fazendo-o cair e te dando vantagem no proximo ataque.",
            4
        },
        {
            &debugSelfDmg,
            "Auto-esfaquear",
            "Da dano em si mesmo. Ai.",
            0
        },

        {
            &parryAttack,
            "Golpe Defensivo",
            "Ataca e assume uma posicao defensiva, aumentando sua armadura no proximo turno.",
            2
        },

        {
            &secondWind,
            "Recuperar Folego",
            "Descansa e se cura.",
            5
        },

        {
            &divineGuidance,
            "Orientacao Divina",
            "Ataca e ganha um bonus na rolagem de ataque e de dano.",
            3
        },

        {
            &bloodOffering,
            "Oferenda de Sangue",
            "Sacrifica HP por Mana.",
            1
        },

        {
            &adrenalineSurge,
            "Surto de Adrenalina",
            "Sacrifica HP e regenera todos os cooldowns.",
            3
        },

        {
            &precisionStrike,
            "Ataque Preciso",
            "Acerta um ataque sem precisar rolar, mas nao pode ser critico.",
            6
        },

        {
            &battleTrance,
            "Instinto Brutal",
            "Ganha um aumento de ataque e dano por 4 turnos, mas coloca todas as habilidades em cooldown.",
            7
        },
    };

    /* ==== Imprimir menu de habilidades ==== */

    // Imprime o menu de skills
    void printSkills(playerS *player) {
        int option = 0, i = 0;

        printf("\nDigite 0 para mostrar/parar de mostrar descricoes.\n\n");
        for(i=0; i<player->skillNum; i++) {
            if(player->knownSkills[i].cooldown == 0) {
                printf("\033[33m%i:\033[0m ", i+1);
                fputs(player->knownSkills[i].name, stdout);
            }
            else {
                printf("\033[90m%i: ", i+1);
                fputs(player->knownSkills[i].name, stdout);
                printf(" (%i turnos para recarregar)\033[0m", player->knownSkills[i].cooldown);
            }
            if(showDesc) {
                printf("\033[90m - %s\033[0m", player->knownSkills[i].desc);
            }
            printf("\n");
        }
        printf("\033[33m%i: \033[36mCancelar\033[0m\n", i+1);
    }

    // Lê a escolha de skill do player
    int playerSkl(playerS *player, enemyS *enemy) {
        int option = 1;

        while (1) {
            printSkills(player);

            printf("\nEscolha uma habilidade:\n> "); // Lê a opção
            scanf("%i", &option);
            printf("\n");

            if(option>0 && option<=player->skillNum) {    // Se a opção é uma habilidade, usa ela.
                if(player->knownSkills[option-1].cooldown == 0) {
                    printInfo(*player, *enemy);

                    if(player->knownSkills[option-1].funct (player, enemy)) {
                        player->knownSkills[option-1].cooldown = player->knownSkills[option-1].maxCooldown + 1; // Coloca o cooldown no máximo ao usar a habilidade.
                        break;  // Se ele retornar 1, acaba o loop. Habilidades retornam 0 se elas não funcionam 
                    }           // (Exemplo: usa Rasteira quando já está em efeito)
                }                                                     
                else {
                    printf("Essa habilidade precisa recarregar!\n"); // Se está em cooldown, não usa a habilidade
                }
            }
            else if(option==player->skillNum+1) {  
                return 1; // Se a opção for cancelar, volta pro menu
            } 
            else if(option == 0) {
                showDesc = !showDesc;
                printInfo(*player, *enemy);
                return playerSkl(player, enemy);
            }
            else {
                printf("Opcao invalida! (tem que ser um numero de 1 a %i).\n", player->skillNum+1); // Se não for válida, pede pra colocar outra
            }
        }

        return 0;
    }

    // Diminui os cooldowns de toda habilidade.
    void updateCooldown (playerS *player) {
        for(int i=0; i<player->skillNum; i++) {
            if(player->knownSkills[i].cooldown>0) player->knownSkills[i].cooldown--;
        }
    }


    /* ==== Funções de skills conhecidas ==== */

    // Inicializa o vetor de skills do player.
    void initSkills (playerS *player) {
        player->knownSkills = (skillS*) malloc(sizeof(skillS));
        player->skillNum = 0;
    }

    // Adiciona uma skill no vetor de skills do player.
    void addSkill (playerS *player, int index) {
        player->skillNum++;
        player->knownSkills = (skillS*) realloc(player->knownSkills, sizeof(skillS)*(player->skillNum)); // Realoca o vetor pra caber mais uma skill.

        player->knownSkills[player->skillNum-1] = skills[index];
        player->knownSkills[player->skillNum-1].cooldown = 0; // Inicializa o cooldown como 0.
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
        spellDmg (player, enemy, 8, 2,
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
        if(player->status[mageArmS]) {
            printf("Esse feitico ja esta em efeito!\n\n");
            return 0;
        } else {
            player->status[mageArmS] = true;
            player->armor += 2;
            printSlow("Voce conjura uma armadura translucida de energia protetiva em volta de si. \033[33mArmadura +2!\033[0m\n\n");
        }

        return 1;
    }

    // Aumenta muito a armadura do player, por 1 turno. Não acumula.
    int mageShield (playerS *player, enemyS *enemy) {
        if(player->status[mageShldS]) {
            printf("Esse feitico ja esta em efeito!\n\n");
            return 0;
        } else {
            player->status[mageShldS] = 2;
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

    // Aumenta o dano da arma permanentemente.
    int blessWeapon (playerS *player, enemyS *enemy) {
        player->dmgMod += 2;
        printSlow("Com uma oracao voce abencoa sua arma, manipulando as forcas do destino para favorecer sua lamina. \033[33mDano +2!\033[0m\n\n");

        return 1;
    }

    // Amplifica o dano do seu próximo ataque.
    int radiantSmite (playerS *player, enemyS *enemy) {
        player->status[rdntSmiteS] = true;
        printSlow("Voce se concentra e canaliza as energias da sua divindade. Seu corpo comeca a emitir centelhas de poder divino, que voce sente fluir pelas suas veias. \033[33mDano do proximo ataque +2d6!\033[0m\n\n");

        return 1;
    }

    // Causa dano contínuo no inimigo até o player castar um feitiço
    int hungerOfTheVoid (playerS *player, enemyS *enemy) {
        if(player->status[hungerOfTheVoidS]) {
            printf("Esse feitico ja esta em efeito!\n\n");
            return 0;
        }
        else {
            printSlow("Sua mente se torna inundada de visoes do Longinquo, o vazio alem do Multiverso. Com um grito terrivel, voce evoca essas visoes para o mundo fisico e conjura um portal sombrio.\n\n");
            player->status[hungerOfTheVoidS] = true;
        }

        return 1;
    }

    // Diminui o custo dos feitiços do player.
    int sightOfYogSothoth (playerS *player, enemyS *enemy) {
        printSlow("Por um instante voce consegue ver com olhos transcendentes, entendendo um pedaco da natureza do mundo e aprofundando a sua comunhao com os Grandes Ancestrais. \033[33mCusto dos feiticos -1!\033[0m\n\n");

        for (int i=0; i<player->spellNum; i++) {
            if(player->knownSpells[i].cost>1 && player->knownSpells[i].funct != &sightOfYogSothoth) { // Só reduz o custo até 1 e não reduz o custo de Visão de Yog-Sothoth.
                player->knownSpells[i].cost--;
            }
        }

        return 1;
    }

    // Causa dano alto.
    int fireOfCthulhu (playerS *player, enemyS *enemy) {
        spellDmg (player, enemy, 8, 3,
        "\n\nSeus olhos se enchem de escuridao, liberando um cone de fogo e trevas que envolve o inimigo por completo, queimando-o com fogo necrotico.\n\n", 
        " \033[33;4mFalha...\033[0m\n\nA criatura bloqueia o raio de chamas sombrias com seu escudo, e o fogo produz um cheiro forte de enxofre.\n\n");

        return 1;
    }

    // Diminui o dano do inimigo.
    int dreamOfAzathoth (playerS *player, enemyS *enemy) {
        if(!player->status[azathothDreamS]) {
            player->status[azathothDreamS] = true;
            printSlow("Voce cria uma conexao entre a mente do inimigo e os sonhos do Grande Ancestral Azathoth, aflingindo-o com visoes perturbadoras e tornando dificil para a criatura discernir entre realidade e sonho. \033[33mDano do inimigo -");
            printf("%i", player->magMod);
            printSlow("!\033[0m\n\n");

            enemy->dmgMod -= player->magMod;
            return 1;
        }
        else {
            printf("Esse feitico ja esta em efeito!\n\n");
            return 0;
        }

    }

    // Causa dano baixo constante.
    int searingLight (playerS *player, enemyS *enemy) {
        if(!player->status[searingLightS]) {
            player->status[searingLightS] = true;
            printSlow("Voce invoca a furia de sua divindade contra o inimigo, e um feixe de radiancia divina desce dos ceus sobre a criatura.\n\n");

            return 1;
        }
        else {
            printf("Esse feitico ja esta em efeito!\n\n");
            return 0;
        }
    }

    /* ==== Criar o array de feitiços ==== */

    const spellS spells[NUM_SPELLS] = {
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
        },
        {
            &blessWeapon,
            "Abencoar Arma",
            "Aumenta o dano da sua arma pelo resto do combate.",
            4
        },
        {
            &radiantSmite,
            "Golpe Radiante",
            "Carrega sua arma com energia divina, aumentando o dano do seu proximo ataque.",
            5
        },
        {
            &hungerOfTheVoid,
            "Fome do Vazio",
            "Invoca tentaculos alienigenas que causam dano continuo enquanto voce nao conjurar outro feitico.",
            5
        },
        {
            &sightOfYogSothoth,
            "Visao de Yog-Sothoth",
            "Diminui o preco das suas magias pelo resto do combate.",
            3
        },
        {
            &fireOfCthulhu,
            "Fogo de Cthulhu",
            "Causa dano alto.",
            7
        },
        {
            &dreamOfAzathoth,
            "Sonho de Azathoth",
            "Diminui o dano do inimigo pelo resto do combate.",
            6
        },
        {
            &searingLight,
            "Luz Queimante",
            "Causa dano constante pelo resto do combate.",
            5
        }
    };

    /* ==== Imprimir menu de feitiços ==== */

    void printSpells(playerS *player) {
        int option = 0, i = 0;

        printf("\nDigite 0 para mostrar/parar de mostrar descricoes.\n\n");
        for(i=0; i<player->spellNum; i++) {
            printf("\033[33m%i:\033[0m ", i+1);
            fputs(player->knownSpells[i].name, stdout);
            printf(" \033[94m(%i mana)\033[0m", player->knownSpells[i].cost);

            if(showDesc) {
                printf("\033[90m - %s\033[0m", player->knownSpells[i].desc);
            }
            printf("\n");
        }
        printf("\033[33m%i: \033[36mCancelar\033[0m\n", i+1);
    }

    int playerMag(playerS *player, enemyS *enemy) {
        int option = 1;

        while (1) {
            printSpells(player);
            printf("\nEscolha um feitico:\n> "); // Lê a opção
            scanf("%i", &option);
            printf("\n");

            if(option>0 && option<=player->spellNum) {    // Se a opção é um feitiço, conjura ele.
                if(player->mana >= player->knownSpells[option-1].cost) {
                    printInfo(*player, *enemy);

                    if (player->status[hungerOfTheVoidS]) { // Se o player estava se concentrando em H. of Hadar, quebra a concentração.
                        player->status[hungerOfTheVoidS] = false;
                        printSlow("Voce se concentra em outro feitico, e o portal para o vacuo fecha.\n");
                    }

                    if(player->knownSpells[option-1].funct (player, enemy)) {
                        player->mana -= player->knownSpells[option-1].cost;
                        break;  // Se ele retornar 1, acaba o loop. Feitiços retornam 0 se eles não funcionam 
                    }           // (Exemplo: conjura Armadura Arcana quando ela já está em efeito)
                }                                                     
                else {
                    printf("Voce nao tem mana o suficiente para usar esse feitico!\n"); // Se não tem mana suficiente, não usa o feitiço
                }
            }
            else if(option==player->spellNum+1) {  
                return 1; // Se a opção for cancelar, volta pro menu
            } 
            else if(option == 0) {
                showDesc = !showDesc;
                
                printInfo(*player, *enemy);
                return playerMag(player, enemy);
            }
            else {
                printf("Opcao invalida! (tem que ser um numero de 1 a %i).\n", player->spellNum+1); // Se não for válida, pede pra colocar outra
            }
        }

        return 0;
    }

    /* ==== Funções de feitiços conhecidos ==== */

    // Inicializa o vetor de feitiços do player.
    void initSpells (playerS *player) {
        player->knownSpells = (spellS*) malloc(sizeof(spellS));
        player->spellNum = 0;
    }

    // Adiciona um feitiço no vetor de feitiços do player.
    void addSpell (playerS *player, int index) {
        player->spellNum++;
        player->knownSpells = realloc(player->knownSpells, sizeof(spellS)*(player->spellNum+1)); // Realoca o vetor pra caber mais um feitiço.

        player->knownSpells[player->spellNum-1] = spells[index];
    }

/* ======= Status ======= */

    // Se 'evento' for falso, transforma em true e atualiza o terminal.
    int rodarEvento(int evento, playerS player, enemyS enemy) {
        if(!evento) {
            evento = true;
            printInfo(player, enemy);
        }
        
        return evento;
    }

    // Checa o array de status do player e faz os efeitos de cada status
    void updateStatus(playerS *player, enemyS *enemy) {
        int evento = false; // Se teve algum evento que o player precisa ler, evento = true e o jogo pede um enter depois de todos os eventos.

        for (int i=0; i<NUM_STATUSES; i++) {
            switch (i)
            {
            case mageShldS:
                if (player->status[mageShldS] == 1) player->armor -= 5; // Quando mageShld está pra acabar, diminui a armadura em 5.
                if (player->status[mageShldS] > 0) player->status[mageShldS]--; // Mage Shield dura pelo turno que ele foi ativado e o próximo turno.
                
                break;

            case tripAtkS:
                if (player->status[tripAtk] == true) {
                    player->status[tripAtk] == false;
                    player->advantage--;
                }
                
                break;    

            case parryAtkS:
                if (player->status[parryAtkS] == 1) player->armor -= 3;
                if (player->status[parryAtkS] > 0) player->status[parryAtkS]--;
                
                break;       

            case hungerOfTheVoidS:
                if (player->status[hungerOfTheVoidS] == true) {
                    evento = rodarEvento(evento, *player, *enemy);

                    printSlow("Tentaculos do Longinquo se estendem pelo portal, dilacerando o inimigo com os dentes afiados de suas bocas disformes.\n");

                    int dmgRoll = rollDice(6, 1, player->magMod, 0);
                    printSlow("Rolagem de dano - \033[36mrolando ");
                    printf("%id%i%+i", 1, 6, player->magMod);
                    rollSlow(dmgRoll);
                    enemy->hp -= dmgRoll;
                    printf("\n\n");
                }
                
                break;     

            case searingLightS:
                if (player->status[searingLightS] == true) {
                    evento = rodarEvento(evento, *player, *enemy);

                    printSlow("A coluna cintilante de luz persiste, queimando enquanto brilha.\n");
                    enemy->hp -= player->magMod;
                }
                
                break;           
            
            case btlTranceS:
                if (player->status[btlTranceS] == 1) {
                    player->atkMod -= 3;
                    player->dmgMod -= 3;
                }

                if (player->status[btlTranceS] > 0) player->status[btlTranceS]--;
                break;

            default:
                break;
            }
        }

        if (evento) {
            printf("Press ENTER to continue.");
            getchar(); // Se usasse a função requestEnter, ela ia pedir enter duas vezes, porque ela tem dois getchar pra pegar o \n da msg anterior primeiro
        }              // já que os eventos acontecem depois de algum outro requestEnter, tem que usar só um getchar
    }


/* ==== UI do player ==== */

    // Imprime as opções do player
    void printOptions(playerS *player, enemyS *enemy) {
        char options[OPTION_AMT][MAX_OPTION] = {"ATACAR", "HABILIDADES", "MAGIA", "INVENTARIO", "FECHAR JOGO"};
        int option = 0;

        printInfo(*player, *enemy);
        printf("\n");
        for(int i=0; i<OPTION_AMT; i++) {
            printf("\033[33m(%i)\033[0m %s%s", i+1, options[i], TAB); // O \t não dá um número consistente de espaços, então eu fiz uma string contante TAB que é só 6 espaços. É feio, mas só custa um pouquinho de tempo de compilação.
        }
        printf("\n");
    }
    
    // Lê a opção que o player escolheu
    int readOption(playerS *player, enemyS *enemy) {
        int option = 1;

        while (1) {
            option = getOption();

            switch (option)
            {
            // Ataque
            case 1: 
                printInfo(*player, *enemy);
                for(int i=0; i<player->atkNum; i++) {
                    playerAtk(player, enemy);
                }
                requestEnter();
                return 0;
                break;

            // Skills
            case 2: 
                printInfo(*player, *enemy);
                if (playerSkl(player, enemy)) {
                    return 1;
                }
                requestEnter();
                return 0;
                break;

            // Magia
            case 3: 
                printInfo(*player, *enemy);
                if (playerMag(player, enemy)) {
                    return 1;
                }
                requestEnter();
                return 0;
                break;

            // Inventário
            case 4: 
                if (playerInv(player, enemy)) {
                    return 1;
                }
                requestEnter();
                return 0;
                break;

            // Cancelar
            case 5: 
                return 2;
                break;

            // Opção inválido
            default:
                printInfo(*player, *enemy);
                printf("Opcao invalida! (tem que ser um numero entre 1 e %i).\n", OPTION_AMT);
                break;
            }
        }
    }

    // Turno do player
    int turnPlayer(playerS *player, enemyS *enemy) {
        while(1) {
            printOptions(player, enemy);
            switch (readOption(player, enemy))
            {
            case 0:
                return 0;
                break; // O player escolheu e executou uma opção.
            
            case 1:
                break; // O player escolheu uma opção e cancelou.

            case 2:
                return 1;
                break; // O player executou "Fechar Jogo".
            }
        }

        return 0;
    }


/* ==== Criação do player ==== */

    // Imprime as classes pro player escolher
    void printClasses() {
        clearTerm();

        centerText (strlen("Escolha sua classe:"), BORDER_LEN);
        char nomes[NUM_CLASSES][MAX_ITEM] = {"Guerreiro", "Mago", "Bruxo", "Paladino"}; // Lista dos nomes das classes
        char descricoes[NUM_CLASSES][MAX_DESC_CLASS] = {
        "Especialista em combate. Tem poucas magias, mas muitas habilidades e dano alto.",
        "Arcanista com uma variedade de feiticos uteis e poderosos.",
        "Usa poderes profanos vindos de um pacto com criaturas extraplanares.",
        "Guerreiro que canaliza o poder de uma divindade para se fortalecer em combate."}; // Lista das descrições das classes

        printf("\033[1mEscolha sua classe:\033[0m\n\n");

        for(int i=0; i<NUM_CLASSES; i++) {
            printf("\033[33m%i: \033[0m", i+1);
            fputs(nomes[i], stdout);
            printf(" - \033[32m");
            puts(descricoes[i]);
            printf("\033[0m");
        }
    }

    // Configura a classe do player
    void chooseClass (playerS *player) {
        int escolha;
        printClasses();

        while (1) {
            printf("> ");
            scanf("%i", &escolha);
            escolha--; // O enum começa em 0, então diminui em 1.

            switch (escolha)
            {
            case warrior:
                // Stats do player
                player->armor = 18;
                player->atkMod = 6;
                player->atkNum = 1;
                player->dmgDice = 6;
                player->dmgDiceNum = 0;
                player->dmgMod = 5;
                player->hpMax = 25;
                player->magMod = -1;
                player->manaMax = 15;

                // Feitiços do player
                addSpell (player, fireBlt);
                addSpell (player, mageShld);

                // Skills do player
                addSkill (player, doubleStrk);
                addSkill (player, tripAtk);
                addSkill (player, parryAtk);
                addSkill (player, scndWind);
                addSkill (player, adrnlSurge);
                addSkill (player, prcStrk);
                addSkill (player, btlTrance);
                
                // Mensagens de ataque
                strcpy(player->hitString, "\n\nA lamina do seu machado atinge o inimigo, que falha em se esquivar e recua com um grito.\n\n");
                strcpy(player->critString, "\n\nGirando sua arma com as duas maos, voce faz um corte letal no alvo, causando dano massivo.\n\n");
                strcpy(player->missString, "\n\nO oponente desvia agilmente do seu golpe, saltando para o lado. A lamina encontra apenas terra.\n\n");

                break;
            
            case wizard:
                player->armor = 14;
                player->atkMod = 0;
                player->atkNum = 1;
                player->dmgDice = 6;
                player->dmgDiceNum = 1;
                player->dmgMod = -2;
                player->hpMax = 15;
                player->magMod = 6;
                player->manaMax = 30;

                addSpell (player, fireBlt);
                addSpell (player, sonicBlst);
                addSpell (player, mageArm);
                addSpell (player, mageShld);
                addSpell (player, magicMsl);

                addSkill (player, parryAtk);
                addSkill (player, scndWind);

                strcpy(player->hitString, "\n\nApesar da sua falta de treinamento marcial, voce consegue atingir a criatura com o seu cajado, fazendo-a recuar.\n\n");
                strcpy(player->critString, "\n\nVoce acerta o alvo com um giro do seu cajado, com um impacto brutal e o som de ossos quebrando.\n\n");
                strcpy(player->missString, "\n\nVoce golpeia pra frente com a ponta do bastao, mas a armadura do inimigo absorve o impacto do ataque.\n\n");
                
                break;

            case warlock:
                player->armor = 12;
                player->atkMod = 0;
                player->atkNum = 1;
                player->dmgDice = 8;
                player->dmgDiceNum = 1;
                player->dmgMod = 2;
                player->hpMax = 15;
                player->magMod = 6;
                player->manaMax = 25;

                addSpell (player, mageArm);
                addSpell (player, voidHunger);
                addSpell (player, yogSothothSight);
                addSpell (player, cthulhuFire);
                addSpell (player, azathothDream);

                addSkill (player, doubleStrk);
                addSkill (player, tripAtk);
                addSkill (player, bldOffering);
                
                strcpy(player->hitString, "\n\nVoce invoca um feixe de energia sombria que dispara erraticamente pelo ar, atingindo o inimigo e queimando-o.\n\n");
                strcpy(player->critString, "\n\nCom uma palavra profana voce conjura um raio faiscante de sombra, que atinge o alvo em cheio e o empurra pra tras numa chuva de faiscas.\n\n");
                strcpy(player->missString, "\n\nNo calor da batalha, voce nao consegue se concentrar para evocar as energias extraplanares do seu patrono, e o raio se dissipa com um chiado.\n\n");

                break;

            case paladin:
                player->armor = 16;
                player->atkMod = 5;
                player->atkNum = 1;
                player->dmgDice = 8;
                player->dmgDiceNum = 1;
                player->dmgMod = 3;
                player->hpMax = 20;
                player->magMod = 3;
                player->manaMax = 25;

                addSpell (player, mageArm);
                addSpell (player, blessWpn);
                addSpell (player, rdntSmite);
                addSpell (player, srngLight);

                addSkill (player, parryAtk);
                addSkill (player, tripAtk);
                addSkill (player, dvnGuidance);
                addSkill (player, bldOffering);

                strcpy(player->hitString, "\n\nO golpe da sua espada acerta o alvo com um corte amplo, abrindo um ferimento e jorrando sangue por onde a lamina rasga.\n\n");
                strcpy(player->critString, "\n\nVoce acerta a criatura com o seu escudo numa investida e finca sua espada num ponto vital, causando um ferimento gravissimo.\n\n");
                strcpy(player->missString, "\n\nO som de metal com metal ressoa pelo campo de batalha quando o seu golpe e bloqueado pelo escudo do inimigo.\n\n");
                
                break;
            
            default:
                printf("Opcao invalida! (tem que ser um numero entre 1 e %i).\n", NUM_CLASSES);
                continue;
                break;
            }

            break;
        }

        player->class = escolha;
    }

    // Cria o player
    playerS createPlayer() {
        playerS player;

        // Inicializando os arrays
        initSpells(&player);
        initSkills(&player);
        initInv(&player);
        fillInv(&player);

        // Escolhendo a classe
        chooseClass (&player);

        // Inicializando os atributos
        player.advantage = 0;
        player.hp = player.hpMax;
        player.mana = player.manaMax;
        strcpy(player.name, "Voce");
        

        // Inicializando os status
        for(int i=0; i<NUM_STATUSES; i++) { 
            player.status[i] = false;
        }

        return player;
    }