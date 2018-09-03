CC = g++
CFLAGS = -Wall -std=c++1z
DEPS = CommandMode.h 
OBJ = CommandMode.o NormalMode.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

NormalMode: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

