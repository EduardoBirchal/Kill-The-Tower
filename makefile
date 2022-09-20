
AULALISTA: main.o term.o combat.o
	gcc main.o term.o combat.o -o main

main.o: main.c
	gcc -c main.c

term.o: term.c gameFuncts.h
	gcc -c term.c

combat.o: combat.c gameFuncts.h
	gcc -c combat.c