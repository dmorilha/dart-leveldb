CXX ?= clang++

CXXFLAGS += -fPIC -g
CPPFLAGS += -I/usr/local/Cellar/dart/1.6.0/libexec/
LDFLAGS += -shared -rdynamic -undefined dynamic_lookup
LIBS = -lleveldb

test: test_leveldb.dart leveldb.dart libleveldb.dylib
	dart test_leveldb.dart;

libleveldb.dylib: leveldb.o
	$(CXX) $^ $(CXXFLAGS)  $(LDFLAGS) $(LIBS) -o $@;

leveldb.o: leveldb.cpp
	$(CXX) -c $^ $(CXXFLAGS) $(CPPFLAGS) -o $@;

clean:
	rm -Rvf *.o *.dylib*;
