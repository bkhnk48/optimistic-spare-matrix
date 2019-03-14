MODULE ReadFile
    USE DataModel
contains

SUBROUTINE readREAL(ios, fh, X, fn)
    IMPLICIT NONE
    CHARACTER (len=*), INTENT(IN) :: fn !file name
    INTEGER, INTENT(INOUT) :: ios  
    INTEGER, INTENT(IN) :: fh
    INTEGER :: i, count  
    DOUBLE PRECISION :: t
    
    DOUBLE PRECISION, DIMENSION(:), INTENT(INOUT) :: X

    i = 0
    count = 0
    OPEN(fh, file=fn, iostat=ios, action='READ')
    print *, 'File name: ', fn

    IF (ios /= 0) STOP 1
    DO i = 1, N
        READ(fh, *, iostat=ios) t
        IF (ios /= 0) exit
        if(i.eq.(N-1)) then
            print *, i
            exit
        endif
        X(i) = t
        count = count + 1
    ENDDO
    print *, 'count = ', count
    CLOSE(fh)
ENDSUBROUTINE

SUBROUTINE dummy(X, XA1, XA2, Y)
    IMPLICIT NONE
    DOUBLE PRECISION, DIMENSION(:), INTENT(INOUT) :: X, XA1, XA2, Y

    X = X + X
    print *, X
ENDSUBROUTINE


SUBROUTINE LoadArray(X, XA1, XA2, Y)!, G1, G2)
    IMPLICIT NONE
    DOUBLE PRECISION, DIMENSION(:), ALLOCATABLE, INTENT(INOUT) :: X, XA1, XA2, Y
    !INTEGER, DIMENSION (:), ALLOCATABLE, INTENT(INOUT) :: G1, G2
    INTEGER :: i

    !ALLOCATE (G1(Length))
    !ALLOCATE (G2(Length))

    ALLOCATE (XA1(Length))
    ALLOCATE (XA2(Length))
    ALLOCATE (Y(Length))

    ALLOCATE (X(Length))

    fh = 13
    !print *, SMALL_SIZE, ' ', Length
    if(Length.eq.SMALL_SIZE) then
        CALL readREAL(ios, fh, XA1, 'd1541XA1') 
    elseif(Length.eq.BIG_SIZE) then    
        CALL readREAL(ios, fh, XA1, 'xa1') 
    endif

    fh = 14
    if(Length.eq.SMALL_SIZE) then
        CALL readREAL(ios, fh, XA2, 'd1541XA2') 
    elseif(Length.eq.BIG_SIZE) then
        CALL readREAL(ios, fh, XA2, 'xa2') 
    endif



    fh = 16
    if(Length.eq.SMALL_SIZE) then
        CALL readREAL(ios, fh, Y, 'd1541Y')
    elseif(Length.eq.BIG_SIZE) then    
        CALL readREAL(ios, fh, Y, 'y')
    endif    

    DO i = 1 , Length
       X(i) = 0
    ENDDO
ENDSUBROUTINE

SUBROUTINE LoadGLOSEG(G1, G2, L)
IMPLICIT NONE
    INTEGER, DIMENSION (:), ALLOCATABLE, INTENT(INOUT) :: G1, G2
    INTEGER :: L

    ALLOCATE (G1(Length))
    ALLOCATE (G2(Length))

    fh = 12
    if(L.eq.SMALL_SIZE) then
        CALL readINT(ios, fh, G1, 'd1541_3077528') 
    elseif(L.eq.BIG_SIZE) then
        CALL readINT(ios, fh, G1, 'g1s') 
    endif

    fh = 15
    if(L.eq.SMALL_SIZE) then
        CALL readINT(ios, fh, G2, 'd1541G2')
    elseif(L.eq.BIG_SIZE) then
        CALL readINT(ios, fh, G2, 'g2s')
    endif

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

SUBROUTINE FindMIN_MAX(G1, G2, ma, mi)
    IMPLICIT NONE
        INTEGER, INTENT(INOUT) :: ma, mi
        INTEGER, DIMENSION (:), INTENT(IN) :: G1, G2
        INTEGER :: i
    mi = MIN(G1(1), G2(1))
    ma = MAX(G1(1), G2(1))
    DO i = 2 , N
        mi = MIN(mi, G1(i))
        ma = MAX(ma, G1(i))
        mi = MIN(mi, G2(i))
        ma = MAX(ma, G2(i))
    ENDDO
ENDSUBROUTINE

SUBROUTINE LoadIndexes(L1, L1_1, L2, L2_1, VAL_1, VAL_2, COL_1, COL_2, row1, row2, G1, G2, ma, mi)
    IMPLICIT NONE
    INTEGER, DIMENSION (:), ALLOCATABLE, INTENT(INOUT) :: L1, L1_1, L2, L2_1
    INTEGER, DIMENSION (:), ALLOCATABLE, INTENT(INOUT) :: VAL_1, VAL_2, COL_1, COL_2
    INTEGER, DIMENSION (:), ALLOCATABLE, INTENT(INOUT) :: row1, row2
    INTEGER, DIMENSION (:), INTENT(IN) :: G1, G2
    INTEGER, INTENT(INOUT) :: ma, mi
    INTEGER :: i, c

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

    CALL FindMIN_MAX(G1, G2, ma, mi)
    

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
ENDSUBROUTINE


SUBROUTINE LoadSpareMatrix(L1, L1_1, L2, L2_1, VAL_1, VAL_2, COL_1, COL_2, row1, row2, G1, G2, mi)
    IMPLICIT NONE
    INTEGER, DIMENSION (:), INTENT(IN) :: L1, L2
    INTEGER, DIMENSION (:), INTENT(INOUT) :: VAL_1, VAL_2, COL_1, COL_2
    INTEGER, DIMENSION (:), INTENT(INOUT) :: row1, row2
    INTEGER, DIMENSION (:), INTENT(INOUT) :: L1_1, L2_1
    INTEGER, DIMENSION (:), INTENT(IN) :: G1, G2
    INTEGER, INTENT(IN) :: mi
    INTEGER :: i, k, offset, j

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
ENDSUBROUTINE    

END MODULE
