PROGRAM pureMPI
    USE ReadFile
    USE PrintAll
    
   
    use mpi
    use, intrinsic :: iso_c_binding
    IMPLICIT NONE
    

    INTEGER :: i, c

    DOUBLE PRECISION, DIMENSION (:), ALLOCATABLE :: X, XA1, XA2, Y 

    double precision :: wct_start,wct_end,cput_start,cput_end,runtime

    INTEGER, DIMENSION (:), ALLOCATABLE :: G1, G2
    REAL :: Mflops

    integer ierr
    integer num_procs
    integer rank

    i = 0
    c = 0


    CALL LoadArray(X, XA1, XA2, Y, G1, G2)

    !
    !  Initialize MPI.
    !
    call MPI_Init ( ierr )
        
    !
    !  Determine this process's rank.
    !
    call MPI_Comm_rank ( MPI_COMM_WORLD, rank, ierr )
    !
    !  Find out the number of processes available.
    !
    call MPI_Comm_size ( MPI_COMM_WORLD, num_procs, ierr )

    N_Length = N
    if ( rank == 0 ) then
        call timing(wct_start,cput_start)
        DO c = 1, trial
        
            DO i = 1 , N_Length
                X(G1(i))= X(G1(i))+ XA1(i)*Y(G2(i))
                X(G2(i))= X(G2(i))+XA2(i)*Y(G1(i))
            ENDDO

            IF(G1(i-1) - M > M) THEN
                CALL dummy(X, XA1, XA2, Y)
            ENDIF
        
        ENDDO !DO c = 1, trial


        call timing(wct_end,cput_end)



        
        runtime = wct_end-wct_start
        print *, "Time = ", runtime, "seconds"
        print *,"Performance: ", dble(trial)*N_Loops*2/runtime/1000000.d0," MFlop/s"
    ENDIF

    call MPI_Finalize ( ierr )

    DEALLOCATE (G1)
    DEALLOCATE (XA1)
    DEALLOCATE (XA2)

    DEALLOCATE (Y)
    DEALLOCATE (G2)

    DEALLOCATE (X)
    
END PROGRAM pureMPI





