CC = gcc
CFLAGS = -g -lpthread

OBJS = poller2.o 
EXEC = poller2
ARGS = 5634 8 16

OBJS2 = pollSwayer.o 
EXEC2 = pollSwayer
ARGS2 = localhost 5634 inputFile.txt

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(CFLAGS)

$(EXEC2): $(OBJS2)
	$(CC) $(OBJS2) -o $(EXEC2) $(CFLAGS)


run: $(EXEC)
	./$(EXEC) $(ARGS)

run2: $(EXEC2)
	./$(EXEC2) $(ARGS2)

clean:
	rm -f $(OBJS) $(EXEC)

clean2:
	rm -f $(OBJS2) $(EXEC2)	