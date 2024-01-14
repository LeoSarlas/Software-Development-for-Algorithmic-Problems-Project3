CC = g++
CFLAGS = -IHeader_files  # Specify the "TEST" directory as the header file directory

# List of all your source files, including the main program and the additional source files
SRCS1 = mainLSH.cpp LSH.cpp Hashtable.cpp 
SRCS2 = mainHC.cpp HyperCube.cpp Hashtable.cpp HashMap.cpp

# Automatically generate the list of object files from source files
OBJS1 = $(SRCS1:.cpp=.o)
OBJS2 = $(SRCS2:.cpp=.o)

# Rules to compile .cpp files to object files
# with empty line printing for better visual separation
%.o: %.cpp
	@echo
	@echo "Compiling $<..."
	$(CC) -c -o $@ $< $(CFLAGS)
	@echo "Compiled $<."
	@echo

# The main program depends on all object files
# with empty line printing for better visual separation
LSH: $(OBJS1)
	@echo
	@echo "Linking object files to create LSH..."
	$(CC) -o lsh $^ $(CFLAGS)
	@echo "Linked object files to create LSH."
	@echo

.PHONY: clean
HC: $(OBJS2)
	@echo
	@echo "Linking object files to create HC..."
	$(CC) -o cube $^ $(CFLAGS)
	@echo "Linked object files to create HC."
	@echo

.PHONY: clean
clean:
	@echo
	@echo "Cleaning up..."
	rm -f cube   # Remove the executable
	rm -f lsh   # Remove the executable
	rm -f *.o       # Remove object files
	@echo "Cleanup completed."
	@echo