CC	= gcc
CFLAGS	= -g -std=c99 -Wall -Wextra -pedantic
OBJS	= bst.o main.o
OUT	= out

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT)

run:
	./$(OUT)

clean:
	rm -f $(OBJS) $(OUT)
