# Compiler and linker
CXX = g++
CXXFLAGS = -I./crawler -I./database -I/usr/include/curlpp -I/usr/include/curl $(shell pkg-config --cflags libmongocxx)
LDLIBS = -lgumbo -lcurlpp -lcurl -lpthread $(shell pkg-config --libs libmongocxx)

# Target executable
TARGET = output

# Source files
SRCS = crawler/crawler.cpp database/mongo_db.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Build the target executable
$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDLIBS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)
