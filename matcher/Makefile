all: matcher-test

CXXFLAGS=-std=c++14 -Wall -Werror
LIB_SRCS := match_ast.cc match_state.cc match_parser.cc matcher.cc

LIB_OBJS := $(LIB_SRCS:.cc=.o)

libmatcher.a: $(LIB_OBJS)
	$(AR) rcs $@ $^

TEST_SRCS := main.cc
TEST_OBJS = $(TEST_SRCS:.cc=.o)

matcher-test: libmatcher.a $(TEST_OBJS) 
	$(CXX) -o $@ $(TEST_OBJS) -lmatcher -L.

clean:
	rm -f $(TEST_OBJS)
	rm -f $(LIB_OBJS)
	rm -f matcher-test
	rm -f libmatcher.a