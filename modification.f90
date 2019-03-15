PROGRAM modification
    USE ReadFile
    USE PrintAll
    use mpi
    !use mpiifort
    use omp_lib
    
    use, intrinsic :: iso_c_binding
    
    IMPLICIT NONE
    

    !INTEGER :: ios, fh, Length
    !INTEGER :: N, c, trial, M
    INTEGER :: c
    INTEGER :: i, j, k, offset

    double precision :: wct_start,wct_end,cput_start,cput_end,runtime

    DOUBLE PRECISION, DIMENSION (:), ALLOCATABLE :: X, XA1, XA2, Y 
    
    INTEGER, DIMENSION (:), ALLOCATABLE :: L1, L1_1, L2, L2_1 

    INTEGER, DIMENSION (:), ALLOCATABLE :: VAL_1, VAL_2

    INTEGER, DIMENSION (:), ALLOCATABLE :: G1, G2


    INTEGER, DIMENSION (:), ALLOCATABLE :: row1, row2, COL_1, COL_2

    
    

    integer :: ierr
    integer :: num_procs
    integer :: rank
    
    
    i = 0

    c = 0
    Length = 12 * 100 * 1000
    N = 1140404
    M = 1140407

    print *, 'N = ', N, ' trial: ', trial

    CALL LoadArray(X, XA1, XA2, Y)!, G1, G2)
    CALL LoadGLOSEG(G1, G2, Length)

    CALL LoadIndexes(L1, L1_1, L2, L2_1, VAL_1, & 
            VAL_2, COL_1, COL_2, row1, row2,&
                 G1, G2, ma, mi)



    CALL LoadSpareMatrix(L1, L1_1, L2, L2_1, &
            VAL_1, VAL_2, COL_1, COL_2, row1,&
                row2, G1, G2, mi)

    

    num_of_threads = 8
    
    print *,"Enter number of threads: "
    read (*, *) num_of_threads

    call OMP_SET_NUM_THREADS(num_of_threads)
    

    deallocate(L1)
    deallocate(L1_1)
    deallocate(L2)
    deallocate(L2_1)

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

    N_Length = ma - mi + 1
    N_Loops = N

    if ( rank == 0 ) then

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
        print *,"Performance: ", dble(trial)*N_Loops*2/runtime/1000000.d0," MFlop/s"

    endif

    call MPI_Finalize ( ierr )

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
    
END PROGRAM modification