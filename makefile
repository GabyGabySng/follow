CC=gcc
CCFLAGS=-g -W -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Isrc

# Options pour GTK 3
GTKFLAGS=`pkg-config --cflags gtk+-3.0`
GTKLIBS=`pkg-config --libs gtk+-3.0 gmodule-export-2.0`


# ---------- Interface graphique ----------

gui: build/list.o build/strhash.o build/text.o build/plsc.o build/main.o build/app.o build/affichage.o build/callbacks.o build/resources.o
	$(CC) -rdynamic build/list.o build/strhash.o build/text.o build/plsc.o build/main.o build/app.o build/affichage.o build/callbacks.o build/resources.o -o build/projetc-gui $(GTKLIBS) -lm


# ---------- Logique (src/) ----------

build/list.o: src/list.c src/list.h
	mkdir -p build
	$(CC) $(CCFLAGS) -c src/list.c -o build/list.o

build/strhash.o: src/strhash.c src/strhash.h src/list.h
	mkdir -p build
	$(CC) $(CCFLAGS) -c src/strhash.c -o build/strhash.o

build/text.o: src/text.c src/text.h src/strhash.h src/list.h
	mkdir -p build
	$(CC) $(CCFLAGS) -c src/text.c -o build/text.o

build/plsc.o: src/plsc.c src/plsc.h src/text.h src/strhash.h src/list.h
	mkdir -p build
	$(CC) $(CCFLAGS) -c src/plsc.c -o build/plsc.o


# ---------- Interface (gui/) ----------

build/main.o: gui/main.c gui/app.h
	mkdir -p build
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c gui/main.c -o build/main.o

build/app.o: gui/app.c gui/app.h gui/affichage.h src/plsc.h src/text.h
	mkdir -p build
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c gui/app.c -o build/app.o

build/affichage.o: gui/affichage.c gui/affichage.h src/plsc.h src/text.h
	mkdir -p build
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c gui/affichage.c -o build/affichage.o

build/callbacks.o: gui/callbacks.c gui/callbacks.h gui/app.h
	mkdir -p build
	$(CC) $(CCFLAGS) $(GTKFLAGS) -c gui/callbacks.c -o build/callbacks.o

# interface.glade est transformé en code C pour être intégré à l'exécutable
build/resources.o: gui/follow.gresource.xml gui/interface.glade
	mkdir -p build
	glib-compile-resources --sourcedir=gui --target=build/resources.c --generate-source gui/follow.gresource.xml
	$(CC) -g $(GTKFLAGS) -c build/resources.c -o build/resources.o


# ---------- Tests (make tests) ----------

tests: build/test_list build/test_strhash build/test_text build/test_plsc

build/test_list: tests/test_list.c tests/testListHelpers.c tests/testListHelpers.h build/list.o build/strhash.o
	$(CC) $(CCFLAGS) -Itests tests/test_list.c tests/testListHelpers.c build/list.o build/strhash.o -o build/test_list -lm

build/test_strhash: tests/test_strhash.c build/list.o build/strhash.o
	$(CC) $(CCFLAGS) tests/test_strhash.c build/list.o build/strhash.o -o build/test_strhash -lm

build/test_text: tests/test_text.c build/list.o build/strhash.o build/text.o
	$(CC) $(CCFLAGS) tests/test_text.c build/list.o build/strhash.o build/text.o -o build/test_text -lm

build/test_plsc: tests/test_plsc.c build/list.o build/strhash.o build/text.o build/plsc.o
	$(CC) $(CCFLAGS) tests/test_plsc.c build/list.o build/strhash.o build/text.o build/plsc.o -o build/test_plsc -lm


clear:
	rm -rf build