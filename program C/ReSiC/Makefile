all: serial

serial: 
	gcc -g -c ErrorReport.c
	gcc -g -c ShiftBitAPI.c
	gcc ShiftBitAPITest.c -g -o apiTest.out
	gcc TestUpdateEXBHost.c -g -o updateEXBHost.out
	gcc -g -c FatTreeTest.c
	gcc -g -c Event.c
	gcc -g -c Splay.c
	gcc FatTree.c -g -o out  
r:
	./out 1
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