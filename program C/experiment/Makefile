all: serial

serial: 
	gcc -pg -c -g ShiftBits.c
	gcc -pg -c Event.c
	gcc -pg -c memory.c -lm
	gcc -pg -c timing.c
	gcc -pg -g -c NetworkNode.c
	gcc -pg -c FatTreeGraph.c
	gcc -pg -g -c FatTreeTables.c
	gcc -pg -c OptimizationSplay.c
	gcc OptimizationSplayTest.c -pg -o optest.o -lm
	gcc -pg -c VoidSplay.c 
	gcc VoidSplayTest.c -pg -o vstest.o -lm
	gcc -pg -g -c MergeSplay.c
	gcc -pg -g -c ControlFlow.c
	export OMP_NUM_THREADS=4
	gcc MergeSplayTest.c -g -o mstest.o -lm -fopenmp
	gcc -pg -c Node.c
	gcc -pg -c LinkedList.c
	gcc -pg -c CalendarQueue.c
	gcc CalendarQueueTest.c -pg -o cqtest.o -lm
	gcc -pg -c CAQueue.c
	gcc randomFile.c -pg -o ran.o
	gcc -ggdb3 -O0 -std=c99 -Wall -Wextra -pedantic -o main.out main.c
	gcc FatTreeTest.c -g -o ftg.o
#gcc FatTree4PortsTest.c -g -o f4t.o -lm
	gcc EventTest.c -g -o e.o -lm
n:
	gcc -c ShiftBits.c
	gcc -c Event.c
	gcc -c memory.c -lm
	gcc -c timing.c
	gcc -c NetworkNode.c
	gcc -c FatTreeGraph.c
	gcc -c FatTreeTables.c
	gcc -c OptimizationSplay.c
	gcc OptimizationSplayTest.c -o optest.o -lm
	gcc -c VoidSplay.c -lm
	gcc VoidSplayTest.c -o vstest.o -lm
	gcc -pg -c MergeSplay.c
	gcc -pg -c ControlFlow.c
	export OMP_NUM_THREADS=4
	gcc MergeSplayTest.c -o mstest.o -lm -fopenmp
	gcc -c Node.c
	gcc -c LinkedList.c
	gcc -c CalendarQueue.c
	gcc CalendarQueueTest.c -o cqtest.o -lm
	gcc -c CAQueue.c
	gcc randomFile.c -o ran.o
	gcc -ggdb3 -O0 -std=c99 -Wall -Wextra -pedantic -o main.out main.c
	gcc FatTreeTest.c -o ftg.o
	gcc FatTree4PortsTest.c -o f4t.o -lm
	gcc EventTest.c -o e.o -lm
clean: 
	-rm *.o
	-rm out
	-rm *.out
	-rm *.txt
clear: 
	-rm *.o
	-rm out
	-rm *.out
	-rm *.txt
c: 
	-rm *.o
	-rm out
	-rm *.out
	-rm *.txt
k:
	dmesg | egrep -i "killed process"
