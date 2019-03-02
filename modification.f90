PROGRAM modification
    USE ReadAll
    USE PrintAll
    use mpi
    
    use, intrinsic :: iso_c_binding
    
    IMPLICIT NONE
    

    INTEGER :: ios, fh, Length
    INTEGER :: N, i, c, trial, M
    INTEGER :: j, k, offset

    double precision :: wct_start,wct_end,cput_start,cput_end,runtime

    DOUBLE PRECISION, DIMENSION (:), ALLOCATABLE :: X, XA1, XA2, Y 
    
    INTEGER, DIMENSION (:), ALLOCATABLE :: L, L1 !, Index

    DOUBLE PRECISION, DIMENSION (:), ALLOCATABLE :: VAL, COL_IDX

    INTEGER, DIMENSION (:), ALLOCATABLE :: G1, G2


    INTEGER, DIMENSION (:), ALLOCATABLE :: row

    INTEGER :: mi, ma

    integer ierr
    integer num_procs
    integer rank
    

    fh = 12
    N = 285896
    M = N + 3
    i = 0
    trial = 10000
    !trial = 10
    c = 0
    Length = 300*1000
    ALLOCATE (G1(Length))
    ALLOCATE (G2(Length))

    ALLOCATE (XA1(Length))
    ALLOCATE (XA2(Length))
    ALLOCATE (Y(Length))

    ALLOCATE (X(Length))

    ALLOCATE (L(Length))
    ALLOCATE (L1(Length))

    !ALLOCATE (I(Length))

    ALLOCATE (VAL(Length + Length))
    ALLOCATE (COL_IDX(Length + Length))

    ALLOCATE (row(length + Length))


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
    ENDDO

    mi = MIN(G1(1), G2(1))
    ma = MAX(G1(1), G2(1))
    DO i = 2 , N
        mi = MIN(mi, G1(i))
        ma = MAX(ma, G1(i))
        mi = MIN(mi, G2(i))
        ma = MAX(ma, G2(i))
    ENDDO

    DO i = 1, ma - mi + 1
        L(i) = 0
        L1(i) = 0
    ENDDO

    DO i=1, Length + Length 
        VAL(i) = 0
        COL_IDX(i) = 0 
    enddo

    DO i = 1, N
        L(G1(i) - mi + 1) = L(G1(i) - mi + 1) + 1
        L(G2(i) - mi + 1) = L(G2(i) - mi + 1) + 1
    ENDDO

    DO i = 1, ma - mi + 1
        L1(i) = L(i)
    ENDDO

    c = 1
    row(1) = 1
    do i = 1, ma - mi + 1
        c = c + L(i)
        row(i+1) = c
    enddo


    do i = 1, N 
        k = G1(i) - mi + 1
        j = row(k)
        offset = L(k) - L1(k)
        VAL(j + offset) = XA1(i)
        COL_IDX(j + offset) = G2(i)
        L1(k) = L1(k) - 1
        
        k = G2(i) - mi + 1
        j = row(k)
        offset = L(k) - L1(k)
        VAL(j + offset) = XA2(i)
        COL_IDX(j + offset) = G1(i)
        L1(k) = L1(k) - 1
    enddo

    deallocate(L1)

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

        call timing(wct_start,cput_start)
        DO c = 1, trial
            !DO i = 1, 2*N
            !    V(i) = 0
            !    B(i) = 0
            !ENDDO
            IF(i - M > M) THEN
                CALL dummy(X, XA1, XA2, Y)
            ENDIF

        
        ENDDO !DO c = 1, trial

        call timing(wct_end,cput_end)
        runtime = wct_end-wct_start
        print *, "Time = ", runtime, "seconds"
        !print *,"Performance: ", dble(trial)*N/runtime/1000000.d0," MIt/s"
        

    endif

    call MPI_Finalize ( ierr )

    DEALLOCATE (G1)
    DEALLOCATE (XA1)
    DEALLOCATE (XA2)

    DEALLOCATE (Y)
    DEALLOCATE (G2)

    DEALLOCATE (X)

    DEALLOCATE (VAL)
    DEALLOCATE (COL_IDX)
    
END PROGRAM modification

