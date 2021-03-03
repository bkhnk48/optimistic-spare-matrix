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
	gcc -pg -c VoidSplay.c
	gcc -pg -g -c MergeSplay.c
	gcc -pg -g -c ControlFlow.c
	gcc -pg -c Node.c
	gcc -pg -c -g ProcessData.c
	gcc -pg -c CalendarQueue.c
	gcc -pg -c CAQueue.c
	gcc randomFile.c -pg -o ran.o
	gcc -pg -g -c TestAPI.c
	gcc -pg -g -c Throughput.c
	gcc -pg -g -c -fbounds-check -fno-strict-aliasing PairStrategies.c
	@echo "==================="
	@echo "Done for *.o in lib"
	@echo "==================="
n:
	gcc -c ShiftBits.c
	gcc -c Event.c
	gcc -c memory.c -lm
	gcc -c timing.c
	gcc -c NetworkNode.c
	gcc -c FatTreeGraph.c
	gcc -c FatTreeTables.c
	gcc -c OptimizationSplay.c
	gcc -c VoidSplay.c
	gcc -c MergeSplay.c
	gcc -c ControlFlow.c
	gcc -c Node.c
	gcc -c ProcessData.c
	gcc -c CalendarQueue.c
	gcc -c CAQueue.c
	gcc randomFile.c -o ran.o
	gcc -c TestAPI.c
	gcc -c Throughput.c
	gcc -c -fbounds-check -fno-strict-aliasing PairStrategies.c
	@echo "==================="
	@echo "Done for *.o in lib"
	@echo "==================="
c: 
	-rm *.o
