MODULE ReadAll
contains

SUBROUTINE readREAL(ios, fh, X, fn)
    IMPLICIT NONE
    CHARACTER (len=*), INTENT(IN) :: fn !file name
    !CHARACTER (len=*), INTENT(IN) :: tp !type data 
    INTEGER, INTENT(INOUT) :: ios  
    INTEGER, INTENT(IN) :: fh
    INTEGER :: i  !, temp
    DOUBLE PRECISION :: t
    DOUBLE PRECISION, DIMENSION(:), INTENT(INOUT) :: X

    i = 0
    OPEN(fh, file=fn, iostat=ios, action='READ')

    IF (ios /= 0) STOP 1
    DO
        i = i + 1
        READ(fh, *, iostat=ios) t
        IF (ios /= 0) exit
        X(i) = t
        
    ENDDO
    CLOSE(fh)
ENDSUBROUTINE

SUBROUTINE dummy(X, XA1, XA2, Y)
    IMPLICIT NONE
    DOUBLE PRECISION, DIMENSION(:), INTENT(INOUT) :: X, XA1, XA2, Y

    print *, X
ENDSUBROUTINE

SUBROUTINE readINT(ios, fh, X, fn)
    IMPLICIT NONE
    CHARACTER (len=*), INTENT(IN) :: fn !file name
    
    INTEGER, INTENT(INOUT) :: ios  
    INTEGER, INTENT(IN) :: fh
    INTEGER :: i  !, temp
    INTEGER :: t
    INTEGER, DIMENSION(:), INTENT(INOUT) :: X

    i = 0
    OPEN(fh, file=fn, iostat=ios, action='READ')

    IF (ios /= 0) STOP 1
    DO
        i = i + 1
        
        READ(fh, *, iostat=ios) t
        IF (ios /= 0) exit
        X(i) = t
    ENDDO
    CLOSE(fh)
ENDSUBROUTINE

END MODULE
