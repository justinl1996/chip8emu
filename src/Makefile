#Extra Sources to be added
SOURCES = $(PROJ).c $(SRCS).c

#Extra Include directories
INCLUDE= -I.

#Libraries to be added
LIBS= -lpthread -lX11 -lSDL2 

#Compiler Option flags
CFLAGS=-Wall -std=c++14 
CC=g++

chip8emu: main.cpp chip8.o gui.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(INCLUDE)

chip8.o: chip8.cpp
	$(CC) $(CFLAGS) -c $^ -o $@ $(LIBS) $(INCLUDE)	

gui.o: gui.cpp
	$(CC) $(CFLAGS) -c $^ -o $@ $(LIBS) $(INCLUDE)	

clean:
	$(RM) chip8emu 
	$(RM) *.o
