PROGRAM no_mpi_modification
    USE ReadFile
    USE PrintAll

    use omp_lib
    
    use, intrinsic :: iso_c_binding
    
    IMPLICIT NONE
    

    INTEGER :: c
    INTEGER :: i, j, k, offset

    double precision :: wct_start,wct_end,cput_start,cput_end,runtime

    DOUBLE PRECISION, DIMENSION (:), ALLOCATABLE :: X, XA1, XA2, Y 
    
    INTEGER, DIMENSION (:), ALLOCATABLE :: L1, L1_1, L2, L2_1 

    INTEGER, DIMENSION (:), ALLOCATABLE :: VAL_1, VAL_2

    INTEGER, DIMENSION (:), ALLOCATABLE :: G1, G2


    INTEGER, DIMENSION (:), ALLOCATABLE :: row1, row2, COL_1, COL_2

    !INTEGER :: mi, ma
    
    num_of_threads = 8
    CALL get_command_argument(1, num1char)
    read (num1char, *) num_of_threads
    !print *, ":", command_argument_count()
    !print *,"Enter number of threads: "
    !read (*, *) num_of_threads
    
    i = 0

    c = 0

    ALLOCATE (G1(Length))
    ALLOCATE (G2(Length))

    ALLOCATE (XA1(Length))
    ALLOCATE (XA2(Length))
    ALLOCATE (Y(Length))

    ALLOCATE (X(Length))

    ALLOCATE (L1(Length))
    ALLOCATE (L1_1(Length))

    ALLOCATE (L2(Length))
    ALLOCATE (L2_1(Length))


    ALLOCATE (VAL_1(Length))
    ALLOCATE (COL_1(Length))

    ALLOCATE (VAL_2(Length))
    ALLOCATE (COL_2(Length))

    ALLOCATE(row1(length))
    ALLOCATE(row2(length))

    fh = 12
    !CALL readINT(ios, fh, G1, 'd1541_3077528') 
    CALL readINT(ios, fh, G1, 'g1s') 

    fh = 13
    !CALL readREAL(ios, fh, XA1, 'd1541XA1') 
    CALL readREAL(ios, fh, XA1, 'xa1') 

    fh = 14
    !CALL readREAL(ios, fh, XA2, 'd1541XA2') 
    CALL readREAL(ios, fh, XA2, 'xa2') 

    fh = 15
    !CALL readINT(ios, fh, G2, 'd1541G2')
    CALL readINT(ios, fh, G2, 'g2s')

    fh = 16
    !CALL readREAL(ios, fh, Y, 'd1541Y')
    CALL readREAL(ios, fh, Y, 'y')

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
        L1(i) = 0
        L1_1(i) = 0

        L2(i) = 0
        L2_1(i) = 0
    ENDDO

    DO i=1, Length
        VAL_1(i) = 0
        COL_1(i) = 0 
        VAL_2(i) = 0
        COL_2(i) = 0 
    enddo

    DO i = 1, N
        L1(G1(i) - mi + 1) = L1(G1(i) - mi + 1) + 1
        L2(G2(i) - mi + 1) = L2(G2(i) - mi + 1) + 1
    ENDDO

    DO i = 1, ma - mi + 1
        L1_1(i) = L1(i)
        L2_1(i) = L2(i)
    ENDDO

    c = 1
    row1(1) = 1
    do i = 1, ma - mi + 1
        c = c + L1(i)
        row1(i+1) = c
    enddo

    c = 1
    row2(1) = 1
    do i = 1, ma - mi + 1
        c = c + L2(i)
        row2(i+1) = c
    enddo


    do i = 1, N 
        k = G1(i) - mi + 1
        j = row1(k)
        offset = L1(k) - L1_1(k)
        VAL_1(j + offset) = i
        COL_1(j + offset) = G2(i)
        L1_1(k) = L1_1(k) - 1
        
        k = G2(i) - mi + 1
        j = row2(k)
        offset = L2(k) - L2_1(k)
        VAL_2(j + offset) = i
        COL_2(j + offset) = G1(i)
        L2_1(k) = L2_1(k) - 1
    enddo

    

    !print *, "# of threads: ", num_of_threads
    call OMP_SET_NUM_THREADS(num_of_threads)
    

    deallocate(L1)
    deallocate(L1_1)
    deallocate(L2)
    deallocate(L2_1)
    N_Length = ma - mi + 1
    N_Loops = N
    !print *, ma, " ", mi, " ", trial

    !if ( rank == 0 ) then

        call timing(wct_start,cput_start)
        DO c = 1, trial
            !$omp parallel do schedule(static)
            !!$omp parallel do schedule(dynamic)
            do i = 1, ma - mi + 1 !N_Length !
                do j = row1(i), row1(i + 1) - 1
                    X(i) = X(i) + XA1(VAL_1(j))* Y(COL_1(j))
                enddo

                do j = row2(i), row2(i + 1) - 1
                    X(i) = X(i) + XA2(VAL_2(j))* Y(COL_2(j))
                enddo
            enddo
            !$omp end parallel do

            IF(i - M > M) THEN
                CALL dummy(X, XA1, XA2, Y)
            ENDIF

        
        ENDDO !DO c = 1, trial

        call timing(wct_end,cput_end)
        runtime = wct_end-wct_start
        print *, "Time = ", runtime, "seconds"
        !print *,"Performance: ", dble(trial)*N*2/runtime/1000000.d0," MIt/s"
        print *,"Performance: ", dble(trial)*N_Loops*2/runtime/1000000.d0," MFlop/s"

    !endif

    !call MPI_Finalize ( ierr )

    DEALLOCATE (G1)
    DEALLOCATE (XA1)
    DEALLOCATE (XA2)

    DEALLOCATE (Y)
    DEALLOCATE (G2)

    DEALLOCATE (X)

    DEALLOCATE (VAL_1)
    DEALLOCATE (COL_1)

    DEALLOCATE (VAL_2)
    DEALLOCATE (COL_2)

    DEALLOCATE(row1)
    DEALLOCATE(row2)
    
END PROGRAM no_mpi_modification