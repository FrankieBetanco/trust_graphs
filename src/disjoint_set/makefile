all: libdisjoint.a

LIB=../../lib
INC=../../include

libdisjoint.a: disjoint_set.cpp
	g++ -c disjoint_set.cpp -I$(INC)
	ar -rcs libdisjoint_set.a disjoint_set.o
	mv libdisjoint_set.a $(LIB)

clean:
	rm *.o
