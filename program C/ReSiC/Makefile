all: serial

serial: 
	gcc -g -c ErrorReport.c
	gcc -g -c ShiftBitAPI.c
	gcc ShiftBitAPITest.c -g -o apiTest.out
	gcc TestUpdateEXBHost.c -g -o updateEXBHost.out
	gcc -g -c State.c
	gcc -g -c FatTreeTest.c
	gcc -g -c Event.c
	gcc -g -c Splay.c
	gcc -g -c timing.c
	gcc TestPerformance.c -g -o per.o
	gcc FatTree.c -g -o out  
	gcc SplayTest.c -g -o splay.o
	gcc -g -c OptimizationSplay.c
	gcc OptimizationSplayTest.c -g -o optest.o
	gcc -g -c VoidSplay.c 
	gcc VoidSplayTest.c -g -o vstest.o
n:
	gcc -c ErrorReport.c
	gcc -c State.c
	gcc -c FatTreeTest.c
	gcc -c Event.c
	gcc -c Splay.c
	gcc -c timing.c
	gcc FatTree.c -o out  
	gcc SplayTest.c -o splay.o
	gcc -c OptimizationSplay.c
	gcc OptimizationSplayTest.c -o optest.o
	gcc -c VoidSplay.c 
	gcc VoidSplayTest.c -o vstest.o
r:
	./out 1
s:
	./splay
clean: 
	-rm *.o
	-rm out
	-rm *.out
clear: 
	-rm *.o
	-rm out
	-rm *.out
c: 
	-rm *.o
	-rm out
	-rm *.out
t:
	./apiTest.out