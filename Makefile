build: Test.c
	gcc Test.c dciCommon.c -O2 -o test.out -lcriterion
test: build
	./test.out
clean:
	rm -f *.out *o dci test.out
