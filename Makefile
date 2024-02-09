main:
	g++ -o main *.c

debug:
	g++ -g -o main *.c

.PHONY: main debug
