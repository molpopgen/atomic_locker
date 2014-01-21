CXX=c++
CXXFLAGS=-O2 -Wall -W -I.

all: atomic_locker.o locking_routines.o
	$(CXX) $(CXXFLAGS) -o atomic_locker atomic_locker.o locking_routines.o

clean:
	 rm -f *.o atomic_locker