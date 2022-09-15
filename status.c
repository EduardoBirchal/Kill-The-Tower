#include "gameFuncts.h"

// Checa o array de status do player e faz os efeitos de cada status
void updateStatus(playerS *player) {
    for (int i=0; i<NUM_STATUSES; i++) {
        switch (i)
        {
        case mageShld:
            if (player->status[mageShld] == 1) player->armor -= 5; // Quando mageShld está pra acabar, diminui a armadura em 5.
            if (player->status[mageShld] > 0) player->status[mageShld]--; // Mage Shield dura pelo turno que ele foi ativado e o próximo turno.
            
            break;                                                        
        
        default:
            break;
        }
    }
}