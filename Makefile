build: testLib.c
	gcc testLib.c dciCommon.c -O2 -o test.out -lm -lcriterion
test: 
	./test.out
