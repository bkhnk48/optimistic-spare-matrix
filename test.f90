PROGRAM test
    implicit none
    integer :: i, c 
    INTEGER :: j, k, offset

    DOUBLE PRECISION, DIMENSION (:), ALLOCATABLE :: X, XA1, XA2, Y 
    
    INTEGER, DIMENSION (:), ALLOCATABLE :: L, L1 !, Index

    DOUBLE PRECISION, DIMENSION (:), ALLOCATABLE :: V, B

    INTEGER, DIMENSION (:), ALLOCATABLE :: G1, G2


    INTEGER, DIMENSION (:), ALLOCATABLE :: row

    INTEGER :: mi, ma

    INTEGER :: Length
    INTEGER :: N

    N = 10
    Length = 30

    ALLOCATE (G1(Length))
    ALLOCATE (G2(Length))

    ALLOCATE (XA1(Length))
    ALLOCATE (XA2(Length))
    ALLOCATE (Y(Length))

    ALLOCATE (X(Length))

    ALLOCATE (L(Length))
    ALLOCATE (L1(Length))

    !ALLOCATE (I(Length))

    ALLOCATE (V(Length + Length))
    ALLOCATE (B(Length + Length))

    ALLOCATE (row(length))


    print *, "test some hypothesises:"
    print *, "do i=1, 1:"
    do i=1, 1
        print *, "       1111 111111"
    enddo
    print *, "do min = max:"
    do i=7, 7
        print *, "       FFFFFF"
    enddo

    print *, "do min < max:"
    do i=7, 6
        print *, "      <<<<<"
    enddo

    N = 5
    Length = 10

    DO i = 1 , Length
        X(i) = 0
        XA1(i) = 2*i
        XA2(i) = 2*i - 1
    ENDDO

    DO i = 1 , Length
        G1(i) = mod(irand(), N) + 1
        G2(i) = mod(irand(), N) + 1
    ENDDO

    do i = 1, N
        print *, 'i =', i, 'G1 =', G1(i), ' G2=', &
            G2(i)
    enddo

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
        V(i) = 0
    ENDDO

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
        V(j + offset) = XA1(i)
        L1(k) = L1(k) - 1
    enddo

    do i = 1, N 
        k = G2(i) - mi + 1
        j = row(k)
        offset = L(k) - L1(k)
        V(j + offset) = XA2(i)
        L1(k) = L1(k) - 1
    enddo

    do i = 1, N
        print *, 'G1 =', G1(i), ' G2=', &
            G2(i), ' row=', row(i)
    enddo
END PROGRAM test