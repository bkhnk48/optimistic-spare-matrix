all: serial

serial: 
	make -C lib
	gcc OptimizationSplayTest.c -pg -o optest.o -lm -I lib
	gcc VoidSplayTest.c -pg -o vstest.o -lm -I lib
	gcc MergeSplayTest.c -g -o mstest.o -lm -I lib
	gcc CalendarQueueTest.c -pg -g -o cqtest.o -lm -I lib
	gcc FatTreeTest.c -g -o ftg.o -I lib
	gcc FatTree4PortsTest.c -g -fno-strict-aliasing -o f4t.o -lm -I lib
	@echo "====================="
	@echo "      All done!"
	@echo "====================="
#gcc EventTest.c -g -o e.o -lm
n:
	make n -C lib
	gcc OptimizationSplayTest.c -o optest.o -lm -I lib
	gcc VoidSplayTest.c -o vstest.o -lm -I lib
	export OMP_NUM_THREADS=4
	gcc MergeSplayTest.c -o mstest.o -lm -fopenmp -I lib
	gcc CalendarQueueTest.c -o cqtest.o -lm -I lib
	gcc FatTreeTest.c -o ftg.o -I lib
	gcc FatTree4PortsTest.c -fno-strict-aliasing -o f4t.o -lm -I lib
	@echo "===================="
	@echo "All done - no debug!"
	@echo "===================="
clean: 
	-rm *.o
	-rm *.txt
	make c -C lib
clear: 
	-rm *.o
	-rm *.out
	-rm *.txt
	make c -C lib
c: 
	-rm *.o
	-rm *.txt
	make c -C lib
k:
	dmesg | egrep -i "killed process"
