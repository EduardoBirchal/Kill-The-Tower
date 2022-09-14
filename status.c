#include "gameFuncts.h"

// Checa o array de status do player e faz os efeitos de cada status
void updateStatus(playerS *player) {
    for (int i=0; i<NUM_STATUSES; i++) {
        switch (i)
        {
        /*case mageShld:
            if (player->status[mageShld] > 0) player->status[mageShld]--; // Mage Shield dura pelo turno que ele foi ativado e o próximo turno.
            else player->armor -= 5;                                      // O status mageShld começa em 2 e diminui todo início de turno, até chegar em 0 e o seu efeito acabar.
            
            break;                                                        
        
        */
        default:
            break;
        }
    }
}