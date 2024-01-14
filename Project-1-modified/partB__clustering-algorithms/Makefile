CC = g++
CFLAGS = -IHeader_files  # Specify the "TEST" directory as the header file directory

# List of all your source files, including the main program and the additional source files
SRCS = cluster.cpp helping_functions.cpp lloyds.cpp reverse_search_lsh.cpp reverse_search_random_projection.cpp

# Automatically generate the list of object files from source files
OBJS = $(SRCS:.cpp=.o)

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
cluster: $(OBJS)
	@echo
	@echo "Linking object files to create cluster..."
	$(CC) -o cluster $^ $(CFLAGS)
	@echo "Linked object files to create cluster."
	@echo

.PHONY: clean
clean:
	@echo
	@echo "Cleaning up..."
	rm -f cluster   # Remove the executable
	rm -f *.o       # Remove object files
	@echo "Cleanup completed."
	@echo
