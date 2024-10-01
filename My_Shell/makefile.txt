CC = gcc

CFLAGS = -g 

OBJS = mysh.o structures.o input_reader.o

EXEC = mysh

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(CFLAGS)

run: $(EXEC)
	./$(EXEC) $(ARGS)

clean:
	rm -f $(OBJS) $(EXEC)