CFLAGS= -lcriterion -lm -Wall -Wextra -Wpedantic -Wcast-align -Winit-self -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wundef -Wwrite-strings -Wpointer-arith -Wmissing-declarations -Wuninitialized -Wold-style-definition -Wstrict-prototypes -Wmissing-prototypes -Wswitch-default -Wbad-function-cast -Wnested-externs -Wconversion -Wunreachable-code
buildRun:
	gcc main.c dciCommon.c -O2 -o decoder.out $(CFLAGS)
buildTest:
	gcc Test.c dciCommon.c -O2 -o test.out  $(CFLAGS)
test: buildTest
	./test.out
runDCI0: buildRun
	echo "48720800" | ./decoder.out dci0 10
runDCI1: buildRun
	echo "7ffffffffe" | ./decoder.out dci1 20
runDCI60: buildRun
	
clean:
	rm -f *.out *o dci test.out
