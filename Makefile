all: cprintarr ReadFile printAll alternative a 

ReadFile:  
	gfortran -pg -c -g ReadFile.f90

a: 
	gfortran -pg -g ReadFile.o PrintAll.o Replacement.o readdata.f90  -o a

printAll:
	gfortran -pg -c -g PrintAll.f90

alternative:
	gfortran -g -c -O3 cprintarr.o Replacement.f90

cprintarr:
	gcc -g -c -ftree-vectorize cprintarr.c -O3

e: 
	gfortran -c ReadFile.f90
#ifort -c ReadFile.f90
#/opt/intel/compilers_and_libraries/linux/mpi/intel64/bin/mpif90  -c ReadFile.f90
	gfortran -c PrintAll.f90
	gfortran -c DataModel.f90
	gcc -O3 -fargument-noalias -c timing.c
#ifort -c PrintAll.f90
#/opt/intel/compilers_and_libraries/linux/mpi/intel64/bin/mpif90  -c PrintAll.f90
	mpif90 -O3 -o a readdata.f90 ReadFile.o PrintAll.o timing.o DataModel.o
#/opt/intel/compilers_and_libraries/linux/mpi/intel64/bin/mpif90 -O3 -o a readdata.f90 ReadFile.o PrintAll.o 
	mpif90 -fopenmp -O3 -o m modification.f90 ReadFile.o PrintAll.o timing.o DataModel.o
	mpif90 -fopenmp -O3 -o d_locality d_locality_modification.f90 ReadFile.o PrintAll.o timing.o DataModel.o

00:
	gfortran -c ReadFile.f90
	gfortran -c PrintAll.f90
	gcc -O0 -fargument-noalias -c timing.c
	mpif90 -O0 -o a readdata.f90 ReadFile.o PrintAll.o timing.o
	mpif90 -fopenmp -O0 -o m modification.f90 ReadFile.o PrintAll.o timing.o
	mpif90 -g -O0 -o d_locality d_locality_modification.f90 ReadFile.o PrintAll.o timing.o

d: 
	gfortran -g -c ReadFile.f90
	gfortran -g -c PrintAll.f90
	gcc -g -fargument-noalias -c timing.c
	mpif90 -g -o a readdata.f90 ReadFile.o PrintAll.o timing.o
	mpif90 -g -o m modification.f90 ReadFile.o PrintAll.o timing.o
	mpif90 -g -o d_locality d_locality_modification.f90 ReadFile.o PrintAll.o timing.o

t:
	gfortran -g -c Reproducibility.f90
	gcc -g -c cprintarr.c 
	gfortran -g -c cprintarr.o OptimizedLib.f90 -lc
	gfortran -pg -c -g ReadFile.f90
	gfortran -O3 -g -o t testcase.f90 OptimizedLib.o ReadFile.o Reproducibility.o cprintarr.o -lc

r:
#mpiexec -np 1 a
	mpiexec -np 1 ./a
s:
#mpiexec -np 1 a
	mpiexec -np 1 ./m
n:
	mpiexec -np 1 ./d_locality	

clean: 
	-rm *.o
	-rm *.mod
	-rm a
	-rm m