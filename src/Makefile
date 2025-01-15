TARGET=editor
CC=gcc

OBJ = main.o draw.o term.o 

$(TARGET): $(OBJ)
	mkdir -p ../build
	$(CC) -o $(TARGET) $(OBJ) -g
	mv $(TARGET) ../build/

main.o: term.h draw.h
term.o: term.h draw.h
draw.o: term.h draw.h

.PHONY: clean
clean:
	rm -rf ../build $(OBJ)
