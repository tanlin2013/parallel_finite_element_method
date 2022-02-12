      implicit REAL*8 (A-H,O-Z)
      real(kind=8), dimension(32) :: VEC

      open (21, file= 'a1x.all', status= 'unknown')
      do i= 1, 32
        read (21,*) VEC(i)
      enddo

      sum= 0.d0
      do i= 1, 32
        sum= sum + VEC(i)**2
      enddo
      sum= dsqrt(sum)

      write (*,'(1pe27.20)') sum

      stop
      end
