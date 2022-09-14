#include "gameFuncts.h"

#define NUM_SPELLS 5
#define MAX_SPELL 20

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

int mageArmor (playerS *player, enemyS *enemy) {
    if(player->status[mageArm]) {
        printSlow("Esse feitico ja esta em efeito!\n\n");
    } else {
        player->status[mageArm] = true;
        player->armor += 2;
        printSlow("Voce conjura uma armadura translucida de energia protetiva em volta de si. \033[33mArmadura +2!\033[0m\n\n");
    }
}

int mageShield (playerS *player, enemyS *enemy) {
    if(player->status[mageShld]) {
        printSlow("Esse feitico ja esta em efeito!\n\n");
    } else {
        player->status[mageShld] = 2;
        player->armor += 5;
        printSlow("Com uma runa protetora, voce conjura um escudo flutuante de energia arcana. \033[33mArmadura +5!\033[0m\n\n");
    }
}

int magicMissile (playerS *player, enemyS *enemy) {
    int dmgRoll = rollDice(4, 3, 1);

    printSlow("Rolagem de dano - \033[36mrolando ");
    printf("%id%i%+i", 3, 4, 1);
    rollSlow(dmgRoll);
    printSlow("Voce invoca tres dardos de energia que se curvam pelo ar e atingem o alvo, liberando pulsos de forca arcana no impacto.\n\n");
    enemy->hp -= dmgRoll;
}

magFunct spells[10] = {&fireBolt, &sonicBlast, &mageArmor, &mageShield, &magicMissile};


/* ==== Imprimir menu de feitiços ==== */

