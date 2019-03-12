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


SUBROUTINE LoadArray(X, XA1, XA2, Y, G1, G2)
    IMPLICIT NONE
    DOUBLE PRECISION, DIMENSION(:), ALLOCATABLE, INTENT(INOUT) :: X, XA1, XA2, Y
    INTEGER, DIMENSION (:), ALLOCATABLE, INTENT(INOUT) :: G1, G2
    INTEGER :: i

    fh = 12
    ALLOCATE (G1(Length))
    ALLOCATE (G2(Length))

    ALLOCATE (XA1(Length))
    ALLOCATE (XA2(Length))
    ALLOCATE (Y(Length))

    ALLOCATE (X(Length))


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
