build: testLib.c
	gcc *.c -O2 -o test.out -lm -lcriterion
test: 
	./test.out
