LDLIBS=-lgmpxx -lgmp
CXXFLAGS=-Ofast -g -ggdb -gdwarf -Wall -Wextra -Wpedantic -std=c++17 -flto
LINK.o=$(LINK.cc)

prime6tuple: prime6tuple.o mr.o
