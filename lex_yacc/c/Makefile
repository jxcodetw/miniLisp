all: y.tab.o lex.yy.o eval.o stack.o driver.o
	gcc -o csmli driver.o eval.o stack.o y.tab.o lex.yy.o -ll

driver.o: driver.c
	gcc -o driver.o -c driver.c

stack.o: stack.c
	gcc -o stack.o -c stack.c

eval.o: eval.c
	gcc -o eval.o -c eval.c

lex.yy.o: lex.yy.c
	gcc -c -g -I.. lex.yy.c

lex.yy.c: csmli.l
	flex -o lex.yy.c csmli.l

y.tab.o: y.tab.c
	gcc -c -g -I.. y.tab.c

y.tab.c: csmli.y
	bison -d -o y.tab.c csmli.y

test: all
	./test.sh

clean:
	rm lex.yy.c lex.yy.o y.tab.c  y.tab.h  y.tab.o eval.o stack.o driver.o csmli
