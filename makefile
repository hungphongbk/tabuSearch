CC=g++
LIBDIR=../../../Lib/libxl
LIBPATH=$(LIBDIR)/lib64
CFLAGS=-I $(LIBDIR)/include_cpp -L $(LIBPATH) -lxl -Wl,-rpath,$(LIBPATH)
all: tabuSearch
tabuSearch: main.o Solution.o SolutionContent.o TabuList.o
	$(CC) main.o Solution.o SolutionContent.o TabuList.o -o tabuSearch $(CFLAGS)
main.o: main.cpp
	$(CC) -c main.cpp
Solution.o: Solution.h Solution.cpp
	$(CC) -c Solution.cpp $(CFLAGS)
SolutionContent.o: SolutionContent.h SolutionContent.cpp
	$(CC) -c SolutionContent.cpp
TabuList.o: TabuList.h TabuList.cpp 
	$(CC) -c TabuList.cpp