all:
	gcc src/main.c src/foundation/*.c src/parser/*.c src/standard/*.c -o pewter
install:
	mv pewter /usr/local/bin
	mkdir /usr/local/include/pewter
	cp tests/standard.prg /usr/local/include/pewter/
