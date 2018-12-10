build:
	gcc *.c -o test -lm -lcriterion

test: build
	./test
clean:
	rm -f *.out *o dci test
