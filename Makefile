all:
	gcc src/main.c src/foundation/*.c src/parser/*.c src/standard/*.c -g -lm -o pewter
install:
	mv pewter /usr/local/bin
	mkdir -R /usr/local/include/pewter
	cp src/standard/stdlib/* /usr/local/include/pewter/
