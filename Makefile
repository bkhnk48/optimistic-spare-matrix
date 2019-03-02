a:
	gfortran -g -o t.exe  test.f90
clean:
	-rm *.o
	-rm *.mod
	-rm *.exe
	
