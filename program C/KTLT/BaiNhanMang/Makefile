all: serial

serial: 
	gcc -c timing.c
	gcc Original.c -o out
	export OMP_NUM_THREADS=4
	gcc Optimization.c -o opt -fopenmp
	gcc MultiMatrix.c -o mm.o
	gcc ReduceCacheSize.c -o rcs.o
c:
	rm out
	rm opt
	rm *.o
bp:
	gcc -c timing.c
	gcc binarysearch.c -DDO_PREFETCH -o with-prefetch
	gcc binarysearch.c -o no-prefetch
