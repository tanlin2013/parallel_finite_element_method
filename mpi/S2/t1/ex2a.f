      implicit REAL*8 (A-H,O-Z)
      include  'mpif.h'

      integer(kind=4) :: my_rank, PETOT, NEIB
      real   (kind=8) :: VEC(36)

      integer(kind=4), dimension(MPI_STATUS_SIZE,1) :: stat_send
      integer(kind=4), dimension(MPI_STATUS_SIZE,1) :: stat_recv
      integer(kind=4), dimension(1)    :: request_send
      integer(kind=4), dimension(1)    :: request_recv

      integer(kind=4) :: start_send, length_send
      integer(kind=4) :: start_recv, length_recv

      call MPI_INIT      (ierr)
      call MPI_COMM_SIZE (MPI_COMM_WORLD, PETOT, ierr )
      call MPI_COMM_RANK (MPI_COMM_WORLD, my_rank, ierr )

      if (my_rank.eq.0) then
        NEIB= 1
        start_send=   1
        length_send=  11
        start_recv=  length_send + 1
        length_recv=  25
        do i= 1, 36
          VEC(i)= 100 + i
        enddo
      endif

      if (my_rank.eq.1) then
        NEIB= 0
         start_send=  1
        length_send= 25
         start_recv= length_send + 1
        length_recv= 11
        do i= 1, 36
          VEC(i)= 200 + i
        enddo
      endif

      do i= 1, 36
        write (*,'(i1,a,i2,f10.0)') my_rank, ' #BEFORE# ', i,VEC(i)
      enddo

      call MPI_ISEND (VEC(start_send), length_send,                     &
     &                MPI_DOUBLE_PRECISION, NEIB, 0, MPI_COMM_WORLD,    &
     &                request_send(1), ierr)
      call MPI_IRECV (VEC(start_recv), length_recv,                     &
     &                MPI_DOUBLE_PRECISION, NEIB, 0, MPI_COMM_WORLD,    &
     &                request_recv(1), ierr)

      call MPI_WAITALL (1, request_recv, stat_recv, ierr)
      call MPI_WAITALL (1, request_send, stat_send, ierr)
   
      do i= 1, 36
        write (*,'(i1,a,i2,f10.0)') my_rank, ' #AFTER # ', i,VEC(i)
      enddo

      call MPI_FINALIZE (ierr)

      end 



