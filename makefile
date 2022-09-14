
AULALISTA: main.o term.o status.o spells.o combat.o
	gcc main.o term.o status.o spells.o combat.o -o main

main.o: main.c
	gcc -c main.c

term.o: term.c gameFuncts.h
	gcc -c term.c

status.o: status.c gameFuncts.h
	gcc -c status.c

spells.o: spells.c gameFuncts.h
	gcc -c spells.c

combat.o: combat.c gameFuncts.h
	gcc -c combat.c