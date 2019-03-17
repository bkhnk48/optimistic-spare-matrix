program fmpi_pi
    use mpi
    double precision mypi, pi, wu, sum, x, f, a
    integer n, myid, numprocs, i, rc
    !function to integrate
    f(a) = 4.d0 / (1.d0 + a*a)
    call MPI_INIT( ierr )
    call MPI_COMM_RANK( MPI_COMM_WORLD, myid, ierr )
    call MPI_COMM_SIZE( MPI_COMM_WORLD, numprocs, ierr )
    if ( myid .eq. 0 ) then
        print *, 'Enter number of intervals: '
        read *, n
    endif

    call timing(wct_start,cput_start)
    call MPI_BCAST(n,1,MPI_INTEGER, 0,MPI_COMM_WORLD,ierr)
    ! calculate the interval size
    w= 1.0d0/n
    sum = 0.0d0
    do i = myid + 1, n, numprocs
        x = w * (i - 0.5d0)
        sum = sum + f(x)
    enddo
    mypi = w * sum
    ! collect all the partial sums
    Call MPI_REDUCE(mypi ,pi , 1, MPI_DOUBLE_PRECISION ,MPI_SUM,0, &
            MPT_COMM_WORLD, ierr)
    ! node 0 prints the anaver.
    if (myid .eq. 0) then
        call timing(wct_end,cput_end)
        print *, 'computed pi = ', pi
        runtime = wct_end-wct_start
        print *, "Time = ", runtime, "seconds"
    endif
    call MPI_FINALIZE(rc)
    stop
end