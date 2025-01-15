TARGET=editor
CC=gcc

OBJ = main.o term.o draw.o controls.o file.o

$(TARGET): $(OBJ)
	mkdir -p ../build
	$(CC) -o $(TARGET) $(OBJ) -g
	mv $(TARGET) ../build/

main.o: include/term.h include/draw.h include/controls.h
term.o: include/term.h 
draw.o: include/term.h include/draw.h
controls.o: include/controls.h include/__defines.h include/draw.h include/term.h
file.o: include/file.h include/term.h



.PHONY: clean
clean:
	rm -rf ../build $(OBJ)
