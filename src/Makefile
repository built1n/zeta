SOURCES=zeta.o exec.o interface.o
CFLAGS=-I. -fPIC -g -std=gnu11 -O3 -g
CXXFLAGS=-I. -fPIC -fpermissive -g
zeta: $(SOURCES)
	gcc $(SOURCES) -o zeta -lstdc++ -g
all: zeta
clean:
	rm -f *.o
	rm -f *~
	rm a.out -f
	rm zeta -f
