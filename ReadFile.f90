MODULE ReadFile
    USE DataModel
contains

SUBROUTINE readREAL(ios, fh, X, fn)
    IMPLICIT NONE
    CHARACTER (len=*), INTENT(IN) :: fn !file name
    INTEGER, INTENT(INOUT) :: ios  
    INTEGER, INTENT(IN) :: fh
    INTEGER :: i  
    DOUBLE PRECISION :: t
    
    DOUBLE PRECISION, DIMENSION(:), INTENT(INOUT) :: X

    i = 0
    OPEN(fh, file=fn, iostat=ios, action='READ')

    IF (ios /= 0) STOP 1
    DO i = 1, N
        READ(fh, *, iostat=ios) t
        IF (ios /= 0) exit
        X(i) = t
        
    ENDDO
    CLOSE(fh)
ENDSUBROUTINE

SUBROUTINE dummy(X, XA1, XA2, Y)
    IMPLICIT NONE
    DOUBLE PRECISION, DIMENSION(:), INTENT(INOUT) :: X, XA1, XA2, Y

    X = X + X
    print *, X
ENDSUBROUTINE

SUBROUTINE readINT(ios, fh, X, fn)
    IMPLICIT NONE
    CHARACTER (len=*), INTENT(IN) :: fn !file name
    
    INTEGER, INTENT(INOUT) :: ios  
    INTEGER, INTENT(IN) :: fh
    INTEGER :: i  
    INTEGER :: t
    INTEGER, DIMENSION(:), INTENT(INOUT) :: X

    i = 0
    OPEN(fh, file=fn, iostat=ios, action='READ')

    IF (ios /= 0) STOP 1
    DO i = 1, N
        READ(fh, *, iostat=ios) t
        IF (ios /= 0) exit
        X(i) = t
    ENDDO
    CLOSE(fh)
ENDSUBROUTINE

END MODULE
