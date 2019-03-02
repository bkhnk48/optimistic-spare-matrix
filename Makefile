all: cprintarr readAll printAll alternative a 

readAll:  
	gfortran -pg -c -g ReadAll.f90

a: 
	gfortran -pg -g ReadAll.o PrintAll.o Replacement.o readdata.f90  -o a

printAll:
	gfortran -pg -c -g PrintAll.f90

alternative:
	gfortran -g -c -O3 cprintarr.o Replacement.f90

cprintarr:
	gcc -g -c -ftree-vectorize cprintarr.c -O3

e: 
	gfortran -c ReadAll.f90
#ifort -c ReadAll.f90
#/opt/intel/compilers_and_libraries/linux/mpi/intel64/bin/mpif90  -c ReadAll.f90
	gfortran -c PrintAll.f90
	gcc -O3 -fargument-noalias -c timing.c
#ifort -c PrintAll.f90
#/opt/intel/compilers_and_libraries/linux/mpi/intel64/bin/mpif90  -c PrintAll.f90
	mpif90 -O3 -o a readdata.f90 ReadAll.o PrintAll.o timing.o
#/opt/intel/compilers_and_libraries/linux/mpi/intel64/bin/mpif90 -O3 -o a readdata.f90 ReadAll.o PrintAll.o 
	mpif90 -fopenmp -O3 -o m modification.f90 ReadAll.o PrintAll.o timing.o

d: 
	gfortran -g -c ReadAll.f90
	gfortran -g -c PrintAll.f90
	gcc -g -fargument-noalias -c timing.c
	mpif90 -g -o a readdata.f90 ReadAll.o PrintAll.o timing.o
	mpif90 -g -o m modification.f90 ReadAll.o PrintAll.o timing.o


t:
	gfortran -g -c Reproducibility.f90
	gcc -g -c cprintarr.c 
	gfortran -g -c cprintarr.o OptimizedLib.f90 -lc
	gfortran -pg -c -g ReadAll.f90
	gfortran -O3 -g -o t testcase.f90 OptimizedLib.o ReadAll.o Reproducibility.o cprintarr.o -lc

r:
#mpiexec -np 1 a
	mpiexec -np 1 ./a
s:
#mpiexec -np 1 a
	mpiexec -np 1 ./m

clean: 
	-rm *.o
	-rm *.mod
	-rm a
	-rm m