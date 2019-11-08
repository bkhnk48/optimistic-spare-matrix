all: serial

serial: 
	gcc -g -c ErrorReport.c
#	gcc -g -c DataStructures.c 
#	gcc -g -c Host.c
#	gcc -g -c IntegratedPort.c
#	gcc -g -c GenerationOfPackets.c 
#	gcc -g -c Switches.c
	gcc -g -c Init.c
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