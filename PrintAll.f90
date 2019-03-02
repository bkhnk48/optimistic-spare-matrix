MODULE PrintAll
contains

SUBROUTINE printArr(X, l)
    IMPLICIT NONE
    INTEGER, INTENT(IN) :: l  
    INTEGER :: i
    REAL, DIMENSION(:), INTENT(IN) :: X


    DO i = 1, l
        print *, "Element at ", &
		i, ' is:', X(i) 
    ENDDO

ENDSUBROUTINE

END MODULE
