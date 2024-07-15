# Compiler and linker
CXX = g++
CXXFLAGS = -I/usr/include/curlpp -I/usr/include/curl
LDLIBS = -lgumbo -lcurlpp -lcurl -lpthread

# Target executable
TARGET = output

# Source files
SRCS = crawler.cpp

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
