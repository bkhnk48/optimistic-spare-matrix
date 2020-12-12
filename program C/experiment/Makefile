all: serial

serial: 
	gcc -pg -c Event.c
	gcc -pg -c memory.c -lm
	gcc -pg -c timing.c
	gcc -pg -c OptimizationSplay.c
	gcc OptimizationSplayTest.c -pg -o optest.o -lm
	gcc -pg -c VoidSplay.c 
	gcc VoidSplayTest.c -pg -o vstest.o -lm
	gcc DeleteSplayTest.c -pg -o delS.o -lm
#gcc NormalSplay.c -g -o no.o
#gcc -g -c MultiOpSplay.c
#gcc MultiOpSplayTest.c -g -o mtest.o
	gcc -pg -c ListSplay.c
	gcc ListSplayTest.c -pg -o ltest.o -lm
	gcc -pg -c Node.c
	gcc -pg -c LinkedList.c
	gcc -pg -c CalendarQueue.c
	gcc CalendarQueueTest.c -pg -o cqtest.o -lm
	gcc -pg -c CAQueue.c
	gcc -pg -c CASplay.c
	gcc CASplayTest.c -pg -o catest.o -lm
	gcc randomFile.c -pg -o ran.o
	gcc -ggdb3 -O0 -std=c99 -Wall -Wextra -pedantic -o main.out main.c
n:
	gcc -c Event.c
	gcc -c memory.c -lm
	gcc -c timing.c
	gcc -c OptimizationSplay.c
	gcc OptimizationSplayTest.c -o optest.o -lm
	gcc -c VoidSplay.c -lm
	gcc VoidSplayTest.c -o vstest.o -lm
	gcc DeleteSplayTest.c -o delS.o -lm
	gcc test.c -o test.o -lm
#gcc NormalSplay.c -o no.o
#gcc -c MultiOpSplay.c
#gcc MultiOpSplayTest.c -o mtest.o
	gcc -c ListSplay.c
	gcc ListSplayTest.c -DDO_PREFETCH -o ltest.o -lm
	gcc -c Node.c
	gcc -c LinkedList.c
	gcc -c CalendarQueue.c
	gcc CalendarQueueTest.c -o cqtest.o -lm
	gcc -c CAQueue.c
	gcc -c CASplay.c
	gcc CASplayTest.c -o catest.o -lm
	gcc randomFile.c -o ran.o
	gcc -ggdb3 -O0 -std=c99 -Wall -Wextra -pedantic -o main.out main.c
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
