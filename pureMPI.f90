PROGRAM pureMPI
    USE ReadFile
    USE PrintAll
    
   
    use mpi
    use, intrinsic :: iso_c_binding
    IMPLICIT NONE
    

    INTEGER :: i, c

    DOUBLE PRECISION, DIMENSION (:), ALLOCATABLE :: X, XA1, XA2, Y 

    DOUBLE PRECISION, DIMENSION (:), ALLOCATABLE :: recv1, recv2, recv3

    double precision :: wct_start,wct_end,cput_start,cput_end,runtime

    INTEGER, DIMENSION (:), ALLOCATABLE :: G1, G2, recv_request
    REAL :: Mflops

    integer :: ierr, send_request
    integer :: num_procs
    integer :: rank, status(MPI_STATUS_SIZE)
    integer :: sendAll(MPI_STATUS_SIZE, 1), recvAll(MPI_STATUS_SIZE, 1)
    integer :: offset, buffsize

    i = 0
    c = 0
    Allocate(recv1(ma - mi))
    Allocate(recv2(ma - mi))
    Allocate(recv3(ma - mi))
    Allocate(recv_request(3))

    CALL LoadArray(X, XA1, XA2, Y)!, G1, G2)
    CALL LoadGLOSEG(G1, G2, Length)

    buffsize = ma - mi + 1
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
    offset = N_Loops*rank
    if ( rank == 0 ) then
        call timing(wct_start,cput_start)
        
        call MPI_Irecv(recv1, buffsize, MPI_DOUBLE_PRECISION, 1, &
                        1, &
            &             MPI_COMM_WORLD,recv_request,ierr)
        call MPI_Irecv(recv2, buffsize, MPI_DOUBLE_PRECISION, 2, &
                        2, &
                MPI_COMM_WORLD,recv_request,ierr)
        
        call MPI_Irecv(recv3, buffsize, MPI_DOUBLE_PRECISION, 3, &
                3, &
        MPI_COMM_WORLD,recv_request,ierr)
        call MPI_WaitAll(3, recv_request,sendAll,ierr);

        DO c = 1, trial
        
            DO i = 1 , N_Length
                X(offset + G1(i))= X(offset + G1(i))+ XA1(i)*Y(G2(i))
                X(offset + G2(i))= X(offset + G2(i))+XA2(i)*Y(G1(i))
            ENDDO

            IF(G1(i-1) - M > M) THEN
                CALL dummy(X, XA1, XA2, Y)
            ENDIF
        
        ENDDO !DO c = 1, trial

        
        call timing(wct_end,cput_end)

        runtime = wct_end-wct_start
        print *, "Time = ", runtime, "seconds"
        !print *,"Performance: ", dble(trial)*N_Loops*2/runtime/1000000.d0," MFlop/s"
    ELSE 
        DO c = 1, trial
            
            DO i = 1 , N_Length
                X(offset + G1(i))= X(offset + G1(i))+ XA1(i)*Y(G2(i))
                X(offset + G2(i))= X(offset + G2(i))+XA2(i)*Y(G1(i))
            ENDDO

            IF(G1(i-1) - M > M) THEN
                CALL dummy(X, XA1, XA2, Y)
            ENDIF    
            
        ENDDO !DO c = 1, trial

        
        CALL MPI_Isend(X(offset + mi : offset + ma), buffsize, &
                MPI_REAL8,0,rank,MPI_COMM_WORLD, &
                send_request,ierr)
        call MPI_Wait(send_request,status,ierr);        
    ENDIF

    
    if(rank == 0) then
    
        DEALLOCATE (G1)
        !print *, '!!!!!!!!Come here???'
        DEALLOCATE (XA1)
        DEALLOCATE (XA2)

        DEALLOCATE (Y)
        DEALLOCATE (G2)

        DEALLOCATE (X)
    endif

    call MPI_Finalize ( ierr )
    
END PROGRAM pureMPI





