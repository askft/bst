CC	= gcc
CFLAGS	= -g -std=c99 -Wall -Wextra -pedantic -O3
CFLAGS	+= -fprofile-arcs -ftest-coverage	# For `gcov`
SRC	= bst.c main.c
OBJS	= bst.o main.o
OUT	= out

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT)

run:
	./$(OUT)

cov:
	gcov -b $(SRC)
	mkdir -p gcov
	mv *.gcov gcov

clean:
	rm -f $(OBJS) $(OUT) *.gcda *.gcno
	rm -rf $(OUT).dSYM

