FLAG = -std=c99 -Wall -pedantic
COMP = gcc
DEBUG = -g

test:	test.c getWord.c getWord.h sort.c sort.h
	$(COMP) $(FLAG) test.c getWord.c sort.c -lpthread

getWord.o: getWord.c getWord.h
	$(COMP) $(FLAG) getWord.c 

sort.o: 	sort.c sort.h
	$(COMP) $(FLAG) sort.c -lpthread

clean:
	rm *.o test
