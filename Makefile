CC = gcc
CFLAGS = -Wall -g -pthread
OBJ = mmsv.o validator.o
EXEC = run

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

mmsv.o : mmsv.c validator.h
	$(CC) -c mmsv.c $(CFLAGS)

validator.o : validator.c validator.h
	$(CC) -c validator.c $(CFLAGS)

clean:
	rm -f $(EXEC) $(OBJ)
