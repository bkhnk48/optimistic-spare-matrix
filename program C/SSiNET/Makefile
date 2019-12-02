all: serial

serial: 
	gcc -g -c ErrorReport.c
	gcc -g -c DataStructure.h
	gcc -g -c TrafficPattern.c
	gcc -g -c Graph.c 
	gcc -g -c RAlgorithm.c
	gcc -g -c Init.c
	gcc -g -c Host.c
	gcc -g -c Switch.c
	gcc -g -c Link.c
	gcc -g -c RoutingTable.c 
	gcc -g -c RoutingPath.c
	gcc -g -c Simulation.c
#gcc serialNS.c -o serialNS.exe  DataStructures.o
	gcc serialNS.c -g -o serialNS.exe 
r:
	./serialNS.exe
clean: 
	-rm *.exe
	-rm *.o
clear: 
	-rm *.exe
	-rm *.o