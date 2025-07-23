# The compiler we want to use
CC = gcc

# Compiler flags to help find errors
CFLAGS = -g -Wall

# The final name of our program
TARGET = c_serve

# List all the .c files that will be part of our project
SOURCES = main.c user_management.c app_logic.c location_logic.c cart_logic.c

# This line automatically creates the list of .o files from the .c files
OBJECTS = $(SOURCES:.c=.o)

# This rule builds our target program. It's the default.
all: $(TARGET)

# This rule explains HOW to create the final program from the object files
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# This is a generic rule that explains HOW to compile any .c file into a .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# This rule lets us type 'make clean' to delete all compiled files
clean:
	rm -f $(OBJECTS) $(TARGET)