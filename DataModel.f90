module DataModel

    implicit none
    INTEGER :: ios, fh
    
    
    
    !INTEGER :: N = 1140404
    !INTEGER :: M = 1140407
    INTEGER :: N = 285935
    INTEGER :: M = 285950
    INTEGER :: Length  = 29 * 10  * 1000
    !INTEGER :: Length = 12 * 100 * 1000

    integer :: N_Length = 800

    !INTEGER :: N = 1000
    !INTEGER :: M = 1007
    !INTEGER :: Length = 12 * 100

    INTEGER :: trial = 1000!*10  !00

    INTEGER :: N_Loops = 285935

    !INTEGER :: c = 0
    !Length = 300*1000
    

    integer :: chunk_size

    INTEGER :: mi, ma
    CHARACTER(100) :: num1char
    INTEGER :: num_of_threads

    !Enumeration types
    INTEGER :: SMALL_SIZE = 29 * 10000 ! for 285935
    INTEGER :: BIG_SIZE = 12 * 100 * 1000! for 1140404
    
    contains

end module DataModel