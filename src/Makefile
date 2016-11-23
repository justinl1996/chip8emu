#Extra Sources to be added
SOURCES = $(PROJ).c $(SRCS).c

#Extra Include directories
INCLUDE= -I.

#Libraries to be added
LIBS= -lpthread -lX11

#Compiler Option flags
CFLAGS=-Wall 
CC=g++

chip8emu: main.cpp chip8.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(INCLUDE)

chip8.o: chip8.cpp
	$(CC) $(CFLAGS) -c $^ -o $@ $(LIBS) $(INCLUDE)	

clean:
	$(RM) shell 
	$(RM) *.o
