c:
	gcc -O3 -fargument-noalias -c timing.c
	mpif90 -O3 -o a isend_irecv.f90
	mpif90 -O3 -o ompi fpi_parallel.f90  timing.o 
	mpif90 -O3 -o mpi fmpi_pi.f90 timing.o

r:
#mpiexec -np 4 ./a 100
	mpiexec -np 4 ./mpi

clean: 
	-rm *.o
	-rm *.mod
	-rm ompi
	-rm mpi