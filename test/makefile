CXX := g++
CCFLAGS := -O3 -I../include/util -I../include -I../sample -std=c++0x
LDFLAGS := -lgtest -lgtest_main -lpthread
TARGETS := pathfinding_test 
SOURCES := $(wildcard *_test.cc)
OBJS :=  $(SOURCES:.cc=.o)

all: $(TARGETS)

pathfinding_test: $(OBJS) 
	echo $(OBJS)
	$(CXX) $(CCFLAGS) $^ $(LDFLAGS) -o $@

%.o: %.cc
	$(CXX) $(CCFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(TARGETS) $(OBJS)
