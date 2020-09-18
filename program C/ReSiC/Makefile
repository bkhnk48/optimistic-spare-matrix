all: serial

serial: 
	gcc -g -c ErrorReport.c
	gcc -g -c ShiftBitAPI.c
	gcc ShiftBitAPITest.c -g -o apiTest.out
	gcc TestUpdateEXBHost.c -g -o updateEXBHost.out
	gcc -g -c FatTreeTest.c
	gcc -o splay top-down-splay.c
#gcc -g -c DataStructure.h
#gcc -g -c TrafficPattern.c
#gcc -g -c Graph.c 
#gcc -g -c RAlgorithm.c
#gcc -g -c Init.c
#gcc -g -c Host.c
#gcc -g -c Switch.c
#gcc -g -c Link.c
#gcc -g -c RoutingTable.c 
#gcc -g -c RoutingPath.c
#gcc -g -c Simulation.c
#gcc -g -c LinkSimulation.c
#gcc serialNS.c -o serialNS.exe  DataStructures.o
#gcc serialNS.c -g -o out
	gcc FatTree.c -g -o out  
r:
	./out
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