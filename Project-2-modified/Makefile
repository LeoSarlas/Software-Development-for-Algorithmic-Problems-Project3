CC = g++
CFLAGS = -IHeader_files  # Specify the "TEST" directory as the header file directory

# List of all your source files, including the main program and the additional source files
SRCS1 = graph_search.cpp mrng.cpp LSH.cpp 	

# Automatically generate the list of object files from source files
OBJS1 = $(SRCS1:.cpp=.o)

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
graph_search: $(OBJS1)
	@echo
	@echo "Linking object files to create LSH, HC and Graphs..."
	$(CC) -o graph_search $^ $(CFLAGS)
	@echo "Linked object files to create LSH, HC and Graphs."
	@echo


.PHONY: clean
clean:
	@echo
	@echo "Cleaning up..."
	rm -f cube  			# Remove the executable
	rm -f lsh   			# Remove the executable
	rm -f graph_search   	# Remove the executable
	rm -f *.o   			# Remove object files
	@echo "Cleanup completed."
	@echo