all:
	gcc src/main.c src/foundation/*.c src/parser/*.c src/standard/*.c -g -lm -o pewter
install:
	mv pewter /usr/local/bin
	mkdir -R /usr/local/include/pewter
	cp tests/standard.prg /usr/local/include/pewter/
