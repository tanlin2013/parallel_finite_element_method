!C
!C   1D Steady-State Heat Transfer 
!C   FEM with Piece-wise Linear Elements
!C   CG (Conjugate Gradient) Method 
!C
!C   d/dx(CdT/dx) + Q = 0
!C   T=0@x=0  
!C
      program heat1D
      implicit REAL*8 (A-H,O-Z)

      integer :: N, NPLU, ITERmax
      integer :: R, Z, P, Q, DD

      real(kind=8) :: dX, RESID, EPS 
      real(kind=8) :: AREA, QV, COND
      real(kind=8), dimension(:), allocatable :: PHI, RHS, X
      real(kind=8), dimension(:  ), allocatable :: DIAG, AMAT
      real(kind=8), dimension(:,:), allocatable :: W

      real(kind=8), dimension(2,2) :: KMAT, EMAT

      integer, dimension(:), allocatable :: ICELNOD
      integer, dimension(:), allocatable :: INDEX, ITEM

!C
!C +-------+
!C | INIT. |
!C +-------+
!C===
      open  (11, file='input.dat', status='unknown')
       read (11,*) NE
       read (11,*) dX, QV, AREA, COND
       read (11,*) ITERmax
       read (11,*) EPS
      close (11)

      N= NE + 1
      allocate (PHI(N), DIAG(N), AMAT(2*N-2), RHS(N))
      allocate (ICELNOD(2*NE), X(N))
      allocate (INDEX(0:N), ITEM(2*N-2), W(N,4))
       PHI= 0.d0
      AMAT= 0.d0
      DIAG= 0.d0
       RHS= 0.d0
         X= 0.d0

      do i= 1, N
        X(i)= dfloat(i-1)*dX
      enddo

      do icel= 1, NE
        ICELNOD(2*icel-1)= icel
        ICELNOD(2*icel  )= icel + 1
      enddo

      KMAT(1,1)= +1.d0
      KMAT(1,2)= -1.d0
      KMAT(2,1)= -1.d0
      KMAT(2,2)= +1.d0
!C===

!C
!C +--------------+
!C | CONNECTIVITY |
!C +--------------+
!C===
      INDEX   = 2

      INDEX(0)= 0
      INDEX(1)= 1 
      INDEX(N)= 1

      do i= 1, N
        INDEX(i)= INDEX(i) + INDEX(i-1)      
      enddo

      NPLU= INDEX(N)

      do i= 1, N
        jS= INDEX(i-1)
        if (i.eq.1) then
          ITEM(jS+1)= i+1
         else if                                                        &
     &     (i.eq.N) then
          ITEM(jS+1)= i-1
         else
          ITEM(jS+1)= i-1            
          ITEM(jS+2)= i+1     
        endif
      enddo
!C===

!C
!C +-----------------+
!C | MATRIX ASSEMBLE |
!C +-----------------+
!C===
      do icel= 1, NE
        in1= ICELNOD(2*icel-1)
        in2= ICELNOD(2*icel  )
        X1 = X(in1)
        X2 = X(in2)
        DL = dabs(X2-X1)

        cK= AREA*COND/DL
        EMAT(1,1)= Ck*KMAT(1,1)
        EMAT(1,2)= Ck*KMAT(1,2)
        EMAT(2,1)= Ck*KMAT(2,1)
        EMAT(2,2)= Ck*KMAT(2,2)

        DIAG(in1)= DIAG(in1) + EMAT(1,1)
        DIAG(in2)= DIAG(in2) + EMAT(2,2)

        if (icel.eq.1) then
          k1= INDEX(in1-1) + 1  
         else
          k1= INDEX(in1-1) + 2  
        endif

        k2= INDEX(in2-1) + 1  

        AMAT(k1)= AMAT(k1) + EMAT(1,2)
        AMAT(k2)= AMAT(k2) + EMAT(2,1)

        QN= 0.50d0*QV*AREA*DL
        RHS(in1)= RHS(in1) + QN
        RHS(in2)= RHS(in2) + QN
      enddo
!C===

!C
!C +---------------------+
!C | BOUNDARY CONDITIONS |
!C +---------------------+
!C===

!C
!C-- X=Xmin
      i= 1
      jS= INDEX(i-1)

      AMAT(jS+1)= 0.d0
      DIAG(i)= 1.d0
      RHS (i)= 0.d0
      do k= 1, NPLU
        if (ITEM(k).eq.1) AMAT(k)= 0.d0
      enddo
!C===

!C
!C +---------------+
!C | CG iterations |
!C +---------------+
!C===
      R = 1
      Z = 2
      Q = 2
      P = 3
      DD= 4

      do i= 1, N
        W(i,DD)= 1.0D0 / DIAG(i)
      enddo

!C
!C-- {r0}= {b} - [A]{xini} |

      do i= 1, N
        W(i,R) = DIAG(i)*PHI(i)
        do j= INDEX(i-1)+1, INDEX(i)
          W(i,R) = W(i,R) + AMAT(j)*PHI(ITEM(j))
        enddo
      enddo

      BNRM2= 0.0D0
      do i= 1, N
        BNRM2 = BNRM2  + RHS(i) **2
        W(i,R)= RHS(i) - W(i,R)
      enddo

!C********************************************************************
      do iter= 1, ITERmax
!C
!C-- {z}= [Minv]{r}

      do i= 1, N
        W(i,Z)= W(i,DD) * W(i,R)
      enddo

!C
!C-- RHO= {r}{z}

      RHO= 0.d0
      do i= 1, N
        RHO= RHO + W(i,R)*W(i,Z)   
      enddo     

!C
!C-- {p} = {z} if      ITER=1  
!C   BETA= RHO / RHO1  otherwise 

      if ( iter.eq.1 ) then
        do i= 1, N
          W(i,P)= W(i,Z)
        enddo
       else
         BETA= RHO / RHO1
         do i= 1, N
           W(i,P)= W(i,Z) + BETA*W(i,P)
         enddo
      endif

!C
!C-- {q}= [A]{p}

      do i= 1, N
        W(i,Q) = DIAG(i)*W(i,P)
        do j= INDEX(i-1)+1, INDEX(i)
          W(i,Q) = W(i,Q) + AMAT(j)*W(ITEM(j),P)
        enddo
      enddo

!C
!C-- ALPHA= RHO / {p}{q}

      C1= 0.d0
      do i= 1, N
        C1= C1 + W(i,P)*W(i,Q)
      enddo
      ALPHA= RHO / C1

!C
!C-- {x}= {x} + ALPHA*{p}
!C   {r}= {r} - ALPHA*{q}

      do i= 1, N
        PHI(i)= PHI(i) + ALPHA * W(i,P)
        W(i,R)= W(i,R) - ALPHA * W(i,Q)
      enddo

      DNRM2 = 0.0
      do i= 1, N
        DNRM2= DNRM2 + W(i,R)**2
      enddo

        RESID= dsqrt(DNRM2/BNRM2)

        if (mod(iter,1000).eq.0) then
        write (*,'(i8, a, 1pe16.6, a, 1pe16.6)')                        &
     &         iter, ' iters, RESID=', RESID, ' PHI(N)= ', PHI(N)
        endif

        if ( RESID.le.EPS) goto 900
        RHO1 = RHO

      enddo
!C********************************************************************

      IER = 1

  900 continue

        write (*,'(i8, a, 1pe16.6, a, 1pe16.6)')                        &
     &         iter, ' iters, RESID=', RESID, ' PHI(N)= ', PHI(N)

!C===

!C
!C-- OUTPUT
      write (*,'(/a)') '### TEMPERATURE'
      XL= dfloat(NE)*dX
      C2= QV*XL
      do i= 1, N
        Xi= X(i)
        PHIa= (-0.50d0*QV*Xi*Xi + C2*Xi)/COND
        write (*,'(i8, 2(1pe16.6))') i, PHI(i), PHIa
      enddo

      end program heat1D
