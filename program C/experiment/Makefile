all: serial

serial: 
	gcc -pg -c Event.c
	gcc -pg -c timing.c
	gcc -pg -c OptimizationSplay.c
	gcc OptimizationSplayTest.c -pg -o optest.o
	gcc -pg -c VoidSplay.c 
	gcc VoidSplayTest.c -pg -o vstest.o
	gcc DeleteSplayTest.c -pg -o delS.o
#gcc NormalSplay.c -g -o no.o
#gcc -g -c MultiOpSplay.c
#gcc MultiOpSplayTest.c -g -o mtest.o
	gcc -pg -c ListSplay.c
	gcc ListSplayTest.c -pg -o ltest.o
	gcc -pg -c Node.c
	gcc -pg -c LinkedList.c
	gcc -pg -c CalendarQueue.c
	gcc CalendarQueueTest.c -pg -o cqtest.o
	gcc -pg -c CAQueue.c
	gcc -pg -c CASplay.c
	gcc CASplayTest.c -pg -o catest.o
	gcc randomFile.c -pg -o ran.o
n:
	gcc -c Event.c
	gcc -c timing.c
	gcc -c OptimizationSplay.c
	gcc OptimizationSplayTest.c -o optest.o
	gcc -c VoidSplay.c 
	gcc VoidSplayTest.c -o vstest.o -O3
	gcc DeleteSplayTest.c -o delS.o
	gcc test.c -o test.o
#gcc NormalSplay.c -o no.o
#gcc -c MultiOpSplay.c
#gcc MultiOpSplayTest.c -o mtest.o
	gcc -c ListSplay.c
	gcc ListSplayTest.c -DDO_PREFETCH -o ltest.o
	gcc -c Node.c
	gcc -c LinkedList.c
	gcc -c CalendarQueue.c
	gcc CalendarQueueTest.c -o cqtest.o
	gcc -c CAQueue.c
	gcc -c CASplay.c
	gcc CASplayTest.c -o catest.o
	gcc randomFile.c -o ran.o
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

