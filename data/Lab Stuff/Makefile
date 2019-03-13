c:
	mpif90 -O3 -o a isend_irecv.f90

r:
	mpiexec -np 4 ./a 100

clean: 
	-rm *.o
	-rm *.mod
	-rm a
	-rm m