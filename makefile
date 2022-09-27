
AULALISTA: main.o term.o combat.o inventory.o
	gcc term.o combat.o inventory.o main.o -o main

term.o: term.c gameFuncts.h
	gcc -c term.c

combat.o: combat.c gameFuncts.h
	gcc -c combat.c

inventory.o: inventory.c gameFuncts.h
	gcc -c inventory.c
	
main.o: main.c
	gcc -c main.c