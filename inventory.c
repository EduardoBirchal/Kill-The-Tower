/**
* @file: inventory
* @brief: Funções de controle do inventário
* @author: Eduardo Santos Birchal
*/

#include "gameFuncts.h"

#define NUM_ITEMS 3

typedef struct item_s { // Tá definido aqui porque depende do tipo sklFunct
    sklFunct funct;
    char name[MAX_ITEM];
    char desc[MAX_DESC_ITEM];

    int num; // O número de itens no slot. Se for 0, o slot está vazio.
} itemS;

extern playerS player;
extern enemyS enemy;


// Um item de dano genérico.
int itemDmg (int dmgDie, int dmgDieNum, int dmgMod, char* strHit) {
    int dmgRoll = rollDice(dmgDieNum, dmgDie, dmgMod, 0);

    printSlow(_("Rolagem de dano - \033[36mrolando "));
    printf("%id%i%+i", dmgDieNum, dmgDie, dmgMod);
    printRollResult(dmgRoll);
    printSlow(strHit);
    enemy.hp -= dmgRoll;
}

// Um item de cura genérico.
int itemHeal (int healDie, int healDieNum, int healMod, char* str) {
    int healRoll = rollDice(healDieNum, healDie, healMod, 0);

    printSlow(_("Rolagem de cura - \033[36mrolando "));
    printf("%id%i%+i", healDieNum, healDie, healMod);
    printRollResult(healRoll);
    printSlow(str);

    player.hp += healRoll;
}

    /* ==== Lista de feitiços ==== */

    // Aumenta a armadura do player. Não acumula.
    int healPotion () {
        itemHeal(4, 3, 5,
       _("A pocao tem um gosto forte e enjoativo, mas alivia sua dor e seus ferimentos fecham em segundos.\n\n"));

        return 1;
    }

    // Aumenta muito a armadura do player, por 1 turno. Não acumula.
    int armorRune () {
        player.armor += 2;
        printSlow(_("Com um comando mental, o disco runado se ativa. O simbolo brilhante projeta um campo de protecao magico em volta de voce. \033[33mArmadura +2!\033[0m\n\n"));

        return 1;
    }

    // Dá dano baixo e acerta sempre, sem precisar rolar ataque.
    int acidFlask () {
        itemDmg (6, 4, 4, 
       _("Voce puxa uma alca no frasco, retirando o tecido que separa dois liquidos dentro. Os fluidos se misturam e borbulham cada vez mais, e voce arremessa o frasco que explode numa enxurrada de acido.\n\n"));

        return 1;
    }


/* ==== Criar o array de itens ==== */

itemS items[INV_SIZE] = {
    {
        &healPotion, // Função de quando o feitiço é conjurado
        "Pocao de Cura", // Nome do feitiço
        "Cura o usuario." // Descrição do feitiço
    },
    {
        &armorRune,
        "Runa de Protecao",
        "Aumenta a armadura do usuario em +2."
    },
    {
        &acidFlask,
        "Frasco de Acido",
        "Causa dano alto no alvo."
    }
};

/* ==== Funções de inventário ==== */

// Cria um inventário vazio.
void initInv () {
    player.inventory = (itemS*) malloc (sizeof(itemS) * INV_SIZE);

    for(int i=0; i<INV_SIZE; i++) {
        player.inventory[i].num = 0;
    }
    
    player.invFill = 0; // invFill é o número de slots ocupados no inventário.
}

// Coloca um número de itens num slot.
void setInvSlot (int slot, int item, int itemNum) {
    player.inventory[slot] = items[item];
    player.inventory[slot].num = itemNum;

    player.invFill++;
}

// Pra debug, enche o inventário com uma lista pré-pronta de itens.
void fillInv () {
    setInvSlot(0, 0, 4);
    setInvSlot(1, 1, 1);
    setInvSlot(2, 2, 2);
}

// Usa um item, diminuindo a quantidade do item no slot.
int useItem (int item) {   
    int deuCerto = player.inventory[item].funct (); // Executa a função do item e retorna o que ela retornar (guardado no deuCerto).
    if(player.inventory[item].num <= 1) { // Se está usando o último item do slot, deleta ele do vetor.
        int i = item;
        
        for (i=item; i<INV_SIZE; i++) {
            if (player.inventory[i+1].num <= 0) break; // Se o próximo item do inventário tem 0 de num (ou seja, se o item atual é o último item do inventário, saia do loop)
            player.inventory[i] = player.inventory[i+1]; // Senão, o item atual é igual ao próximo.
        }
        player.inventory[i].num = 0; // O último item do inventário vira um item qualquer de num 0 e o invFill diminui em 1.
        player.invFill--;
    }
    else player.inventory[item].num--; // Se não estiver usando o último item do slot, só diminui o número de itens.

    return deuCerto;
}

/* ==== Imprimir menu de itens ==== */

// Imprime o menu de itens.
void printItems() {
    int option = 0, i = 0;

    printInfo();
    printf(_("\nDigite 0 para mostrar/parar de mostrar descricoes.\n\n"));

    for(i=0; i<player.invFill; i++) {
        printf("\033[33m%i:\033[0m ", i+1);
        fputs(player.inventory[i].name, stdout);
        printf(" \033[36m(x%i)\033[0m", player.inventory[i].num);

        // Imprime a descrição, se showDesc for verdadeiro
        if(showDesc) {
            printf("\033[90m - %s\033[0m", player.inventory[i].desc);
        }
        printf("\n");
    }

    printf(_("\033[33m%i: \033[36mCancelar\033[0m\n"), i+1);
}

// Lê a escolha do player.
int readItem() {
    int option = 1;
    int size = player.invFill;

    while (1) {
        printf(_("\nEscolha um item:\n> ")); // Lê a opção
        scanf("%i", &option);
        printf("\n");

        if(option>0 && option<=size) {                  // Se a opção é um item, usa ele.
            printInfo();
            if(useItem(option-1)) break; // Se ele retornar 1, acaba o loop. Itens retornam 0 se eles não funcionam 
        }                                               // (Exemplo: usa Runa de Proteção quando ela já está em efeito)
        else if(option == size+1) {  
            return 1; // Se a opção for cancelar, volta pro menu
        } 
        else if(option == 0) { // Se a opção for 0, inverte showDesc e chama a função de novo
            showDesc = !showDesc;
            printItems();
            return readItem();
        }
        else {
            printf(_("Opcao invalida! (tem que ser um numero de 1 a %i).\n"), size+1); // Se não for válida, pede pra colocar outra
        }
    }

    return 0;
}

// Imprime o menu e lê a escolha de item.
int playerInv () {
    printItems();
    if(readItem()) {
        return 1; // Retorna 1, fazendo com que o menu imprima de novo
    }

    return 0;
}