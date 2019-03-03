PROGRAM readdata
    USE ReadAll
    USE PrintAll
    !USE Replacement
    !USE OptimizedLib
    !USE Reproducibility
    
    use mpi
    use, intrinsic :: iso_c_binding
    IMPLICIT NONE
    

    INTEGER :: ios, fh, Length
    INTEGER :: N, i, c, trial, M
    INTEGER(8) :: iter

    DOUBLE PRECISION, DIMENSION (:), ALLOCATABLE :: X, XA1, XA2, Y 
    

    double precision :: wct_start,wct_end,cput_start,cput_end,runtime

    INTEGER, DIMENSION (:), ALLOCATABLE :: G1, G2
    REAL :: avgT, Mflops

    integer ierr
    integer num_procs
    integer rank

    fh = 12
    N = 285896
    M = N + 3
    i = 0
    avgT = 0
    trial = 10000

    iter = 0
    c = 0
    Length = 300*1000
    ALLOCATE (G1(Length))
    ALLOCATE (G2(Length))

    ALLOCATE (XA1(Length))
    ALLOCATE (XA2(Length))
    ALLOCATE (Y(Length))

    ALLOCATE (X(Length))


    CALL readINT(ios, fh, G1, 'd1541_3077528') !, 'int')

    fh = 13
    CALL readREAL(ios, fh, XA1, 'd1541XA1') !, 'r')

    fh = 14
    CALL readREAL(ios, fh, XA2, 'd1541XA2') !, 'r')

    fh = 15
    CALL readINT(ios, fh, G2, 'd1541G2')

    fh = 16
    CALL readREAL(ios, fh, Y, 'd1541Y')

    DO i = 1 , Length
            X(i) = 0
            !Z(i) = 0
    ENDDO

    DO c = 1, trial
        DO i = 1 , N
            iter = iter + 1
        ENDDO
    ENDDO
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

    if ( rank == 0 ) then
    !start = timestamp()
        call timing(wct_start,cput_start)
        DO c = 1, trial
        
            DO i = 1 , N
                X(G1(i))= X(G1(i))+ XA1(i)*Y(G2(i))
                X(G2(i))= X(G2(i))+XA2(i)*Y(G1(i))
            ENDDO

            IF(i > M) THEN
                CALL dummy(X, XA1, XA2, Y)
            ENDIF

        
        ENDDO !DO c = 1, trial

        !finish = timestamp()

        call timing(wct_end,cput_end)

        !avgT = real(finish - start) / real(clock_rate)

        !Mflops = (4.d0*trial)*N/avgT
        !Mflops = Mflops / (1000*1000)

        !avgT1 = 11.7129688


        
        runtime = wct_end-wct_start
        print *, "Time = ", runtime, "seconds"
        !print *, "Number of iterations: ", iter, ' as ', dble(trial)*N
        !print *,"Performance: ", dble(trial)*N*2/runtime/1000000.d0," MIt/s"
        print *,"Performance: ", dble(trial)*N*2/runtime/1000000.d0," MFlop/s"
        !print *, 'ETA:  ', avgT, '(s).Mflops: ', Mflops!, avgT2, '(s) %: ', percent
        !print *, '                                            %2: ', percent2
    ENDIF

    call MPI_Finalize ( ierr )

    DEALLOCATE (G1)
    DEALLOCATE (XA1)
    DEALLOCATE (XA2)

    DEALLOCATE (Y)
    DEALLOCATE (G2)

    DEALLOCATE (X)
    
END PROGRAM readdata





