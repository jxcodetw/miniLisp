CC=g++

csmli: y.tab.o lex.yy.o eval.o driver.o
	$(CC) -o csmli driver.o eval.o y.tab.o lex.yy.o -ll

driver.o: driver.cpp
	$(CC) -o driver.o -c driver.cpp

eval.o: eval.cpp
	$(CC) -o eval.o -c eval.cpp

lex.yy.o: lex.yy.cpp
	$(CC) -c -g -I.. lex.yy.cpp

lex.yy.cpp: csmli.l
	flex -o lex.yy.cpp csmli.l

y.tab.o: y.tab.cpp
	$(CC) -c -g -I.. y.tab.cpp

y.tab.cpp: csmli.y
	bison -d -o y.tab.cpp csmli.y

output: csmli
	mkdir -p output/testdata

test: output
	./test.sh
	@rm -r output

clean:
	rm lex.yy.cpp lex.yy.o y.tab.cpp  y.tab.hpp  y.tab.o eval.o driver.o csmli
