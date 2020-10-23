CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp DecisionTree.cpp DecisionTreeNode.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hw2

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


