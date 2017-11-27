all: run

run: clearMain compile
	./main

compile:
	gcc main.c -Wall -o main

clearMain:
	rm -rf main