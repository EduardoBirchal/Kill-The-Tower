## Jogo-RPG-Sla
# Protótipo/teste de um jogo baseado em turno pra terminal

Jogo baseado em turno feito em C com 4 classes disponíveis. Usei ele pra treinar ponteiros de função para guardar feitiços, habilidades, etc.

BIBLIOTECAS:
  - combat.c - Combate + habilidades e magia
  - enemy.c - Funções de gerar e controlar inimigo
  - GameFuncts.h - Header geral e contém todos os includes e a maioria das structs, typedefs, etc
  - inventory.c - Funções de inventário
  - term.c - Funções de terminal

Tem um tanto de printf com umas sequências estranhas de caracteres, tipo esse aqui: `printf("\033[33mHello World\033[0m")`. Essas sequências de caracter são sequências de escape que mudam a impressão do printf, por exemplo, o `\033[33m` transforma o texto em amarelo, e o `\033[0m` transforma de volta em texto normal.
