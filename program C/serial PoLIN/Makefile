all: serial

serial: 
	gcc -c ErrorReport.c
	gcc -c DataStructures.c 
	gcc -c GenerationOfPackets.c 
	gcc -c Switches.c
	gcc -c Init.c
#gcc serialNS.c -o serialNS.exe  DataStructures.o
	gcc serialNS.c -o serialNS.exe 
r:
	./serialNS.exe
clean: 
	-rm *.exe
	-rm *.o