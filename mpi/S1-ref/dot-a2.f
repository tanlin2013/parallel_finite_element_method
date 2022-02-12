      implicit REAL*8 (A-H,O-Z)
      real(kind=8), dimension(23) :: VEC

      open (21, file= 'a2x.all', status= 'unknown')

      read (21,*) nn
      do i= 1, 23
        read (21,*) VEC(i)
      enddo

      sum= 0.d0
      do i= 1, 23
        sum= sum + VEC(i)**2
      enddo
      sum= dsqrt(sum)

      write (*,'(1pe27.20)') sum

      stop
      end
