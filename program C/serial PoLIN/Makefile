all: serial

serial: 
	gcc -c DataStructures.c 
	gcc -c Init.c
#gcc serialNS.c -o serialNS.exe  DataStructures.o
	gcc serialNS.c -o serialNS.exe 
r:
	./serialNS.exe
clean: 
	-rm *.exe
	-rm *.o