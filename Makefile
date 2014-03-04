SOURCES=zeta.o exec.o interface.o
CFLAGS=-I. -fPIC
CXXFLAGS=-I. -fPIC -fpermissive
all: $(SOURCES)
	gcc $(SOURCES) -o zeta -lstdc++
clean:
	rm -f *.o
	rm -f *~
	rm a.out -f
	rm zeta -f
