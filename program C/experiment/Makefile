all: serial

serial: 
	gcc -g -c Event.c
	gcc -g -c timing.c
	gcc -g -c OptimizationSplay.c
	gcc OptimizationSplayTest.c -g -o optest.o
	gcc -g -c VoidSplay.c 
	gcc VoidSplayTest.c -g -o vstest.o
	gcc DeleteSplayTest.c -g -o delS.o
	gcc NormalSplay.c -g -o no.o
	gcc -g -c MultiOpSplay.c
n:
	gcc -c Event.c
	gcc -c timing.c
	gcc -c OptimizationSplay.c
	gcc OptimizationSplayTest.c -o optest.o
	gcc -c VoidSplay.c 
	gcc VoidSplayTest.c -o vstest.o
	gcc DeleteSplayTest.c -o delS.o
	gcc test.c -o test.o
	gcc NormalSplay.c -o no.o
	gcc -c MultiOpSplay.c
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

