CC=gcc
CFLAGS=-Wall -std=c17 -pedantic
BIN=bin/
SRC=src/
INC=include/
EXE=td
OBJECTS=$(BIN)gemme.o $(BIN)monstre.o $(BIN)projectile.o $(BIN)terrain.o $(BIN)tour.o $(BIN)joueur.o $(BIN)element.o  $(BIN)action.o $(BIN)jeu.o $(BIN)bittab.o $(BIN)generictab.o $(BIN)animation.o $(BIN)graphisme.o $(BIN)options.o 
CLIBS=-lm -lMLV

default : $(EXE)

$(EXE): $(OBJECTS) $(SRC)main.c
	$(CC) $^ -o $@ $(CFLAGS) $(CLIBS)

test: $(OBJECTS) $(SRC)main.c
	$(CC) $^ -o $(EXE) $(CFLAGS) $(CLIBS) -D_TEST_

$(BIN)options.o : $(SRC)options.c $(INC)options.h

$(BIN)action.o : $(SRC)action.c $(INC)action.h $(INC)constantes.h $(INC)terrain.h $(INC)joueur.h $(INC)monstre.h $(INC)jeu.h

$(BIN)animation.o : $(SRC)animation.c $(INC)animation.h $(INC)element.h $(INC)generictab.h

$(BIN)bittab.o : $(SRC)bittab.c $(INC)bittab.h

$(BIN)constantes.o : $(SRC)constantes.c $(INC)constantes.h

$(BIN)element.o : $(SRC)element.c $(INC)element.h

$(BIN)gemme.o : $(SRC)gemme.c $(INC)gemme.h $(INC)element.h

$(BIN)generictab.o : $(SRC)generictab.c $(INC)generictab.h

$(BIN)graphisme.o : $(SRC)graphisme.c $(INC)graphisme.h $(INC)constantes.h $(INC)element.h $(INC)tour.h $(INC)terrain.h $(INC)projectile.h $(INC)monstre.h $(INC)joueur.h $(INC)action.h $(INC)jeu.h

$(BIN)jeu.o : $(SRC)jeu.c $(INC)jeu.h $(INC)terrain.h $(INC)projectile.h $(INC)monstre.h $(INC)joueur.h

$(BIN)joueur.o : $(SRC)joueur.c $(INC)joueur.h $(INC)gemme.h $(INC)terrain.h $(INC)constantes.h $(INC)bittab.h

$(BIN)monstre.o : $(SRC)monstre.c $(INC)monstre.h $(INC)constantes.h $(INC)element.h $(INC)terrain.h $(INC)generictab.h

$(BIN)projectile.o : $(SRC)projectile.c $(INC)projectile.h $(INC)monstre.h $(INC)element.h $(INC)gemme.h $(INC)tour.h $(INC)generictab.h $(INC)animation.h

$(BIN)terrain.o : $(SRC)terrain.c $(INC)terrain.h $(INC)tour.h $(INC)constantes.h

$(BIN)tour.o : $(SRC)tour.c $(INC)tour.h $(INC)gemme.h

$(BIN)%.o: $(SRC)%.c $(INC)%.h
	$(CC) $< -c -o  $@ $(CFLAGS)

clean:
	rm -f $(OBJECTS)
	rm -f td
