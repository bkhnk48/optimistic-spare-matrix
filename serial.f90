PROGRAM serial
    USE ReadFile
    USE PrintAll
    
   
    use, intrinsic :: iso_c_binding
    IMPLICIT NONE
    

    INTEGER :: i, c

    DOUBLE PRECISION, DIMENSION (:), ALLOCATABLE :: X, XA1, XA2, Y 

    
    double precision :: wct_start,wct_end,cput_start,cput_end,runtime

    INTEGER, DIMENSION (:), ALLOCATABLE :: G1, G2, recv_request
    REAL :: Mflops

    
    i = 0
    c = 0
    Length = 12 * 100 * 1000
    N = 1140404
    M = 1140407 
    
    
    CALL LoadArray(X, XA1, XA2, Y)

        
    CALL LoadGLOSEG(G1, G2, Length)
    !CALL FindMIN_MAX(G1, G2, ma, mi)

    
    N_Length = N
    N_Loops = N
    !print *, N_Length, ' ', trial, ' ', N_Loops
    
    call timing(wct_start,cput_start)

    DO c = 1, trial
        
        DO i = 1 , N_Length
            X(G1(i)) = X(G1(i)) + XA1(i)*Y(G2(i))
            X(G2(i)) = X(G2(i)) + XA2(i)*Y(G1(i))
        ENDDO

        IF(G1(i-1) - M > M) THEN
            CALL dummy(X, XA1, XA2, Y)
        ENDIF
        
    ENDDO !DO c = 1, trial

        
        
    call timing(wct_end,cput_end)

    runtime = wct_end-wct_start
    print *, "Time = ", runtime, "seconds"
    print *,"Performance: ", dble(trial)*N_Loops*2/runtime/1000000.d0," MFlop/s"
    

    
    
    
        
    DEALLOCATE (G1)
        
        
    DEALLOCATE (XA1)
        
    DEALLOCATE (XA2)

        !if(associated(Y )) then
    DEALLOCATE (Y)
        !endif
    DEALLOCATE (G2)
    DEALLOCATE (X)
    
END PROGRAM 