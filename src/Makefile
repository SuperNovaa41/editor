TARGET=editor
CC=gcc

OBJ = main.o term.o draw.o controls.o

$(TARGET): $(OBJ)
	mkdir -p ../build
	$(CC) -o $(TARGET) $(OBJ) -g
	mv $(TARGET) ../build/

main.o: headers/term.h headers/draw.h headers/controls.h
term.o: headers/term.h 
draw.o: headers/term.h headers/draw.h
controls.o: headers/controls.h headers/__defines.h headers/draw.h headers/term.h



.PHONY: clean
clean:
	rm -rf ../build $(OBJ)
