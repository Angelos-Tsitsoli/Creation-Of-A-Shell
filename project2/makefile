CC = gcc
CFLAGS = -g3 -lpthread

OBJS = poller.o 
EXEC = poller
ARGS = 5635 8 16 pollLog.txt pollStats.txt

OBJS2 = pollSwayer.o 
EXEC2 = pollSwayer
ARGS2 = localhost 5635 inputFile.txt

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