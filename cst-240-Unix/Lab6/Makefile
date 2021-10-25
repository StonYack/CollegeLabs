COPTS = -g -O0 -Wall -std=c99 -lreadline

all: build

client: client.c
	gcc $(COPTS)  -o client client.c

server: server.c
	gcc $(COPTS)  -o server server.c
testcmd: testcmd.c
	gcc $(COPTS)  -o testcmd testcmd.c

build:  client server test testcmd

test:   test.py
	python3 test.py

clean:
	rm -f test
	rm -f *.o
