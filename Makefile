all:
	clang++ leveldb.cpp -I /usr/local//Cellar/dart/1.5.3/libexec/ -I /Users/dmorilha/leveldb/include/  -fPIC -shared -rdynamic -undefined dynamic_lookup -L /Users/dmorilha/leveldb/ -l leveldb -g -o libleveldb.dylib
