SOURCES=zeta.o exec.o interface.o
HEADERS=zeta.h util.h
CFLAGS=-I. -fPIC -g -std=gnu11 -O3 -g
CXXFLAGS=-I. -fPIC  -g
zeta: $(SOURCES) $(HEADERS) Makefile
	gcc $(SOURCES) -o zeta -lstdc++ -g
all: zeta
clean:
	rm -f *.o
	rm -f *~
	rm a.out -f
	rm zeta -f
