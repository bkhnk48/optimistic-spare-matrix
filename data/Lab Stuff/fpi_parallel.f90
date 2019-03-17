program compute_pi

     use, intrinsic :: iso_c_binding

     implicit none
     integer n, i
     double precision :: wct_start,wct_end,cput_start,cput_end,runtime
     
     double precision w, x, sum, pi, f, a
     !function to integrate
     
     f(a) = 4.d0 / (1.d0 + a*a)
     
     print *, 'Enter number of intervals: '
     
     read *, n
     !calculate the interval size
     call timing(wct_start,cput_start)

     w = 1.0d0/n
     
     sum = 0.0d0
     !$OMP PARALLEL DO PRIVATE(x), SHARED(w)
     !$OMP& REDUCTION(+: sum)
     
     do i= 1, n
          x = w * (i - 0.5d0)
          sum = sum + f(x)
     enddo
     
     pi = w * sum

     call timing(wct_end,cput_end)
     
     print *, 'computed pi = ', pi
     
     runtime = wct_end-wct_start
     print *, "Time = ", runtime, "seconds"
     
     stop
     
end