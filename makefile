
AULALISTA: main.o term.o combat.o
	gcc term.o combat.o main.o -o main

term.o: term.c gameFuncts.h
	gcc -c term.c

combat.o: combat.c gameFuncts.h
	gcc -c combat.c
	
main.o: main.c
	gcc -c main.c