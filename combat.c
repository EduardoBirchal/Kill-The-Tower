#include "gameFuncts.h"


/* ==== Defines ==== */

#define NUM_SKILLS 8
#define MAX_SKILL 25
#define MAX_DESC_SKILL 100

#define NUM_SPELLS 12
#define MAX_SPELL 25
#define MAX_DESC_SPELL 100


/* ==== Typedefs ==== */

typedef struct skill_s {
    sklFunct funct;
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

static int showDesc = 1;


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

        int hpMaxLen = digitNum(hpMax);
        int hpLen = digitNum(hp);
        hpLen = hpLen + hpMaxLen; // hpLen é o comprimento da string do valor do hp do player, que vai ser escrita depois do nome.

        printf("\033[0m\n");
        centerText(strlen("HP: ") + hpLen, BORDER_LEN);
        printf("HP: %i/%i\n\n", hp, hpMax);
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

        int manaMaxLen = digitNum(manaMax);
        int manaLen = digitNum(mana);
        manaLen = manaLen + manaMaxLen; 

        printf("\033[0m\n");
        centerText(strlen("Mana: ") + manaLen, BORDER_LEN);
        printf("Mana: %i/%i\n\n", mana, manaMax);
    }

    // Conserta o HP pra não ficar negativo
    int updateHp (playerS *player, enemyS *enemy) {
        if(enemy->hp < 0) enemy->hp = 0;
        if(enemy->hp > enemy->hpMax) enemy->hp = enemy->hpMax;
        if(player->hp < 0) player->hp = 0;
        if(player->hp > player->hpMax) player->hp = player->hpMax;

        if(player->mana < 0) player->mana = 0;
        if(player->mana > player->manaMax) player->mana = player->manaMax;

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
            printSlow(player->critString);

            if (player->status[rdntSmiteS]) {
                player->status[rdntSmiteS] = false;
                int rdntRoll = rollDice(6, 2, 0, 0);

                printSlow("Voce canaliza toda a energia divina acumulada para a sua lamina, liberando-a num pulso reverberante de poder radiante.\n");
                printSlow("Rolagem de dano (Golpe Radiante) - \033[36mrolando ");
                printf("%id%i", 2, 6);
                rollSlow(rdntRoll);
                printf("\n\n");

                enemy->hp -= rdntRoll;
            }

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
            printSlow(player->hitString);

            if (player->status[rdntSmiteS]) {
                player->status[rdntSmiteS] = false;
                int rdntRoll = rollDice(6, 2, 0, 0);

                printSlow("Rolagem de dano (Golpe Radiante) - \033[36mrolando ");
                printf("%id%i", 2, 6);
                rollSlow(rdntRoll);

                enemy->hp -= rdntRoll;

                printSlow("Voce canaliza toda a energia divina acumulada para a sua lamina, liberando-a num pulso reverberante de poder radiante.\n\n");
            }

            enemy->hp -= dmgRoll;
            return 1;
        }
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

    int adrenalineSurge (playerS *player, enemyS *enemy) {
        printSlow("Apesar de estar exausto, voce obriga seus musculos a continuarem por pura forca de vontade.\n\n");

        for (int i=0; i<player->skillNum; i++) {
            if(player->hp > player->knownSkills[i].cooldown && player->hp > 4) {
                if(player->knownSkills[i].cooldown > 4) {
                    player->hp -= player->knownSkills[i].cooldown;
                }
                else if (player->knownSkills[i].cooldown > 0){
                    player->hp -= 4;
                }
                player->knownSkills[i].cooldown = 0;
            }
            else {
                printSlow("Voce chegou ao seu limite. Se voce se exercesse mais ainda, voce morreria. (Nao tem HP suficiente para regenerar todas as habilidades)");
                break;
            }
        }
    }

    /* ==== Criar o array de habilidades ==== */

    skillS skills[NUM_SKILLS] = {
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
        }
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
            if(showDesc == 1) {
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
                showDesc *= -1;
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

            if(showDesc == 1) {
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
                showDesc *= -1;
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
            
            default:
                break;
            }
        }

        if (evento) {
            printf("Press ENTER to continue.");
            getchar(); // Se usasse a função requestEnter, ela ia pedir enter duas vezes, porque ela tem dois getchar pra pegar o \n da msg anterior primeiro
        }              // já que os eventos acontecem depois de algum outro requestEnter, tem que usar só um getchar
    }