CC = gcc

CFLAGS = -g -lpthread

OBJS = poller2.o 

EXEC = poller2

OBJS2 = pollSwayer.o 

EXEC2 = pollSwayer



$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(CFLAGS)

run: $(EXEC)
	./$(EXEC) $(ARGS)

clean:
	rm -f $(OBJS) $(EXEC)

run2: $(EXEC2)
	./$(EXEC2) $(ARGS)

clean2:
	rm -f $(OBJS2) $(EXEC2)	