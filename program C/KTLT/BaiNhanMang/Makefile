all: serial

serial: 
	gcc -c timing.c
	gcc Original.c -o out
	export OMP_NUM_THREADS=4
	gcc Optimization.c -o opt -fopenmp
c:
	rm out
	rm opt
	rm *.o