PROGRAM Example_MPI
 
  use mpi
  implicit none
  integer ierr,ntasks,taskid,itask,status(MPI_STATUS_SIZE)
  integer i,j,k,buffsize,sentid,send_request,recv_request
  character argtmp*12
  real(8) inittime,recvtime,totaltime,rand,buffsum
 
  real(8),allocatable,dimension(:) :: recvtimes
  real(8),allocatable,dimension(:) :: buffsums
 
  real(8),allocatable,dimension(:) :: sendbuff
  real(8),allocatable,dimension(:) :: recvbuff
 
  !---------------------------------------------------------------
  ! MPI Initialisation. It's important to put this call at the
  ! beginning of the program, after variable declarations.
  call MPI_INIT( ierr )
 
  !---------------------------------------------------------------
  ! Get the number of MPI processes and the taskid of this process.
  call MPI_COMM_SIZE(MPI_COMM_WORLD,ntasks,ierr)
  call MPI_COMM_RANK(MPI_COMM_WORLD,taskid,ierr)
 
  !---------------------------------------------------------------
  ! Get buffsize value from program arguments.
  call getarg(1,argtmp)
  read(argtmp,'(I12)')buffsize
 
  !---------------------------------------------------------------
  ! Printing out the description of the example.
  if ( taskid.eq.0 )then
    write(6,'(A)')
    write(6,'(A)')"##########################################################"
    write(6,'(A)')
    write(6,'(A)')" Example 6"
    write(6,'(A)')
    write(6,'(A)')" Point-to-point communication: MPI_Isend MPI_Irecv"
    write(6,'(A)')
    write(6,'(A,I12)')" Vector size:",buffsize
    write(6,'(A,I5)')" Number of processes:",ntasks
    write(6,'(A)')
    write(6,'(A)')"##########################################################"
    write(6,'(A)')
    write(6,'(A)')"                --> BEFORE COMMUNICATION <--"
    write(6,'(A)')
  endif
 
   !---------------------------------------------------------------
   ! Memory allocation. 
    allocate( sendbuff(0:buffsize-1) )
    allocate( recvbuff(0:buffsize-1) )
    allocate( recvtimes(0:ntasks-1) )
    allocate( buffsums(0:ntasks-1) )
 
   !-----------------------------------------------------------------
   ! Vectors and matrices initialisation.
   call srand(taskid*10)
   do i=0,buffsize-1
    sendbuff(i)=rand();
   end do
 
   !-----------------------------------------------------------------
   ! Print out before communication.
 
   buffsum=0.0
   do i=0,buffsize-1
      buffsum=buffsum+sendbuff(i)
   end do
 
   call MPI_Gather(buffsum,1,MPI_REAL8,buffsums,1, MPI_REAL8,0,MPI_COMM_WORLD,ierr)
 
   if(taskid.eq.0)then
    do itask=0,ntasks-1
      sentid=itask+1
      if(itask.eq.(ntasks-1))sentid=0
      write(6,'(A,I3,A,I3,A,E14.8)')  &
      &       "Process",itask,": Sum of vector sent to ",sentid," =", buffsums(itask)
    end do
   end if
 
   !-----------------------------------------------------------------
   ! Communication
 
   inittime = MPI_Wtime();
 
   if ( taskid.eq.0 )then
     call MPI_Irecv(recvbuff,buffsize,MPI_REAL8,ntasks-1,MPI_ANY_TAG, &
     &             MPI_COMM_WORLD,recv_request,ierr)
     recvtime = MPI_Wtime()
     call MPI_Isend(sendbuff,buffsize,MPI_REAL8,taskid+1,0,MPI_COMM_WORLD,send_request,ierr)  
   else if( taskid.eq.(ntasks-1) )then
     call MPI_Isend(sendbuff,buffsize,MPI_REAL8,0,0,MPI_COMM_WORLD,send_request,ierr)
     call MPI_Irecv(recvbuff,buffsize,MPI_REAL8,taskid-1,MPI_ANY_TAG, &
     &             MPI_COMM_WORLD,recv_request,ierr)
     recvtime = MPI_Wtime()
   else
     call MPI_Irecv(recvbuff,buffsize,MPI_REAL8,taskid-1,MPI_ANY_TAG, &
     &             MPI_COMM_WORLD,recv_request,ierr)
     recvtime = MPI_Wtime()
     call MPI_Isend(sendbuff,buffsize,MPI_REAL8,taskid+1,0,MPI_COMM_WORLD,send_request,ierr)
   end if
 
   call MPI_Wait(send_request,status,ierr);
   call MPI_Wait(recv_request,status,ierr);
 
   totaltime=MPI_Wtime()
 
   !-----------------------------------------------------------------
   ! Print out after communication.
 
   buffsum=0.0
   do i=0,buffsize-1
      buffsum=buffsum+recvbuff(i)
   end do
   if ( taskid.eq.0 )then
    do itask=0,ntasks-1
      buffsums(itask)=0.0
    end do     
   end if
 
   call MPI_Gather(buffsum,1,MPI_REAL8,buffsums,1, MPI_REAL8,0,MPI_COMM_WORLD,ierr)
 
   call MPI_Gather(recvtime,1,MPI_REAL8,recvtimes,1, MPI_REAL8,0,MPI_COMM_WORLD,ierr)
 
  if(taskid.eq.0)then
        write(6,*)
    write(6,'(A)')"##########################################################"
    write(6,*)
    write(6,'(A)')"                --> AFTER COMMUNICATION <--"
    write(6,*)
    do itask=0,ntasks-1
      write(6,'(A,I3,A,F5.2,A,e14.8)')"Process ",itask,": Vector received at",recvtimes(itask), &
      &                             " seconds: Sum = ",buffsums(itask)
    end do
    write(6,*)
    write(6,'(A)')"##########################################################"
    write(6,'(A,F5.2,A)')" Communication Time: ",totaltime," seconds"
    write(6,'(A)')"##########################################################"
    write(6,*)
  end if
 
  !-----------------------------------------------------------------
  ! Free the allocated memory
  deallocate(sendbuff)
  deallocate(recvbuff)
  deallocate(recvtimes)
  deallocate(buffsums)
 
  !-----------------------------------------------------------------
  ! MPI finalisation
  
  call MPI_FINALIZE( ierr )
 
end
