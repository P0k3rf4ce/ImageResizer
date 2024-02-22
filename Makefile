FLAGS = -std=c99 -Wall -Werror
PNG_FLAGS = -Ilibpng/include -Llibpng/lib -lpng

.PHONY: all clean

all: test resize

resize: optparse.o png_resizer.o main.o
	gcc ${FLAGS} ${PNG_FLAGS} -o $@ $^

test: tests/optparse_tests
	@for t in $^; do		\
		./$$t 2> tests/optparse_error.txt ||:; 		\
	done

tests/optparse_tests: optparse.o tests/optparse_tests.c 
	gcc ${FLAGS} -o $@ $^

%.o: %.c
	gcc ${FLAGS} -c $<
	
clean:
	@rm -r *.o tests/optparse_tests tests/optparse_error.txt tests/*.dSYM ||:
