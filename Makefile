CC = gcc
CFLAGS = -Wall -Wextra -g
SOURCES = delete.c search.c functii.c main.c
EXE = tema1

build:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXE)
run: $(EXE)
	./$(EXE)
clean:
	rm -f $(EXE)
