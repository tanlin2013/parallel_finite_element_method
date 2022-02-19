!C
!C   1D Steady-State Heat Transfer 
!C   FEM with Piece-wise Linear Elements
!C   CG (Conjugate Gradient) Method 
!C
!C   d/dx(CdT/dx) + Q = 0
!C   T=0@x=0  
!C
      program heat1Dp
      implicit REAL*8 (A-H,O-Z)
      include 'mpif.h'

      integer :: N, NPLU, ITERmax
      integer :: R, Z, P, Q, DD

      real(kind=8) :: dX, RESID, EPS 
      real(kind=8) :: AREA, QV, COND
      real(kind=8), dimension(:), allocatable :: PHI, RHS
      real(kind=8), dimension(:  ), allocatable :: DIAG, AMAT
      real(kind=8), dimension(:,:), allocatable :: W

      real(kind=8), dimension(2,2) :: KMAT, EMAT

      integer, dimension(:), allocatable :: ICELNOD
      integer, dimension(:), allocatable :: INDEX, ITEM
      integer(kind=4) :: NEIBPETOT, BUFlength, PETOT
      integer(kind=4), dimension(2) :: NEIBPE

      integer(kind=4), dimension(0:2) :: import_index, export_index
      integer(kind=4), dimension(  2) :: import_item , export_item

      real(kind=8), dimension(2) :: SENDbuf, RECVbuf

      integer(kind=4), dimension(:,:), allocatable :: stat_send
      integer(kind=4), dimension(:,:), allocatable :: stat_recv
      integer(kind=4), dimension(:  ), allocatable :: request_send
      integer(kind=4), dimension(:  ), allocatable :: request_recv

!C
!C +-------+
!C | INIT. |
!C +-------+
!C===
!C
!C-- MPI init.
      call MPI_Init      (ierr)
      call MPI_Comm_size (MPI_COMM_WORLD, PETOT, ierr )
      call MPI_Comm_rank (MPI_COMM_WORLD, my_rank, ierr )

!C
!C-- CTRL data 
      if (my_rank.eq.0) then
        open  (11, file='input.dat', status='unknown')
         read (11,*) NEg
         read (11,*) dX, QV, AREA, COND
         read (11,*) ITERmax
         read (11,*) EPS
         write (*,'(i10)') NEg
        close (11)
      endif

      call MPI_Bcast (NEg    , 1, MPI_INTEGER, 0, MPI_COMM_WORLD, ierr)
      call MPI_Bcast (ITERmax, 1, MPI_INTEGER, 0, MPI_COMM_WORLD, ierr)
      call MPI_Bcast (dX     , 1, MPI_DOUBLE_PRECISION, 0,              &
     &                                            MPI_COMM_WORLD, ierr)
      call MPI_Bcast (QV     , 1, MPI_DOUBLE_PRECISION, 0,              &
     &                                            MPI_COMM_WORLD, ierr)
      call MPI_Bcast (AREA   , 1, MPI_DOUBLE_PRECISION, 0,              &
     &                                            MPI_COMM_WORLD, ierr)
      call MPI_Bcast (COND   , 1, MPI_DOUBLE_PRECISION, 0,              &
     &                                            MPI_COMM_WORLD, ierr)
      call MPI_Bcast (EPS,   1, MPI_DOUBLE_PRECISION, 0,                &
     &                                            MPI_COMM_WORLD, ierr)

!C
!C-- Local Mesh Size
      Ng= NEg + 1
      N = Ng / PETOT

      nr = Ng - N*PETOT
      if (my_rank.lt.nr) N= N+1
  
      NE= N - 1 + 2
      NP= N + 2

      if (my_rank.eq.0) NE= N - 1 + 1
      if (my_rank.eq.0) NP= N + 1
      if (my_rank.eq.PETOT-1) NE= N - 1 + 1
      if (my_rank.eq.PETOT-1) NP= N + 1

      if (PETOT.eq.1) NE= N-1
      if (PETOT.eq.1) NP= N  

!C
!C-- ARRAYs
      allocate (PHI(NP), DIAG(NP), AMAT(2*NP-2), RHS(NP))
      allocate (ICELNOD(2*NE))
      allocate (INDEX(0:NP), ITEM(2*NP-2), W(NP,4))
       PHI= 0.d0
      AMAT= 0.d0
      DIAG= 0.d0
       RHS= 0.d0

      do icel= 1, NE
        ICELNOD(2*icel-1)= icel
        ICELNOD(2*icel  )= icel + 1
      enddo

      if (PETOT.gt.1) then
      if (my_rank.eq.0) then
	icel= NE
	ICELNOD(2*icel-1)= N
	ICELNOD(2*icel  )= N + 1
       else if (my_rank.eq.PETOT-1) then
	icel= NE
	ICELNOD(2*icel-1)= N + 1
	ICELNOD(2*icel  )= 1
       else
	icel= NE - 1
	ICELNOD(2*icel-1)= N + 1
	ICELNOD(2*icel  )= 1
	icel= NE
	ICELNOD(2*icel-1)= N
	ICELNOD(2*icel  )= N + 2
      endif
      endif

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

      INDEX(N+1)= 1
      INDEX(NP )= 1

      if (my_rank.eq.0)       INDEX(1)= 1 
      if (my_rank.eq.PETOT-1) INDEX(N)= 1

      do i= 1, NP
        INDEX(i)= INDEX(i) + INDEX(i-1)      
      enddo

      NPLU= INDEX(NP)
      ITEM= 0

      do i= 1, N
        jS= INDEX(i-1)
        if (my_rank.eq.0.and.i.eq.1) then
          ITEM(jS+1)= i+1
         else if (my_rank.eq.PETOT-1.and.i.eq.N) then
          ITEM(jS+1)= i-1
         else
          ITEM(jS+1)= i-1            
          ITEM(jS+2)= i+1     
          if (i.eq.1) ITEM(jS+1)= N + 1
          if (i.eq.N) ITEM(jS+2)= N + 2
          if (my_rank.eq.0.and.i.eq.N) ITEM(jS+2)= N + 1
        endif
      enddo



      i = N + 1
      jS= INDEX(i-1)
      if (my_rank.eq.0) then
	ITEM(jS+1)= N
       else
	ITEM(jS+1)= 1
      endif

      i = N + 2
      if (my_rank.ne.0.and.my_rank.ne.PETOT-1) then
        jS= INDEX(i-1)
	ITEM(jS+1)= N
      endif


!C
!C-- COMMUNICATION
      NEIBPETOT= 2
      if (my_rank.eq.0      ) NEIBPETOT= 1
      if (my_rank.eq.PETOT-1) NEIBPETOT= 1
      if (PETOT.eq.1)         NEIBPETOT= 0

      NEIBPE(1)= my_rank - 1
      NEIBPE(2)= my_rank + 1

      if (my_rank.eq.0      ) NEIBPE(1)= my_rank + 1
      if (my_rank.eq.PETOT-1) NEIBPE(1)= my_rank - 1

      BUFlength= 1

      import_index= 0
      export_index= 0
      import_item = 0
      export_item = 0

      import_index(1)= 1
      import_index(2)= 2
      import_item (1)= N+1
      import_item (2)= N+2

      export_index(1)= 1
      export_index(2)= 2
      export_item (1)= 1
      export_item (2)= N

      if (my_rank.eq.0) then
        import_item (1)= N+1
        export_item (1)= N
      endif
!C
!C-- INIT. arrays for MPI_Waitall

      allocate (stat_send(MPI_STATUS_SIZE,2*NEIBPETOT))
      allocate (request_send(2*NEIBPETOT))
!C===
      call MPI_Barrier (MPI_COMM_WORLD, ierr)
      S1Time = MPI_Wtime()
!C
!C +-----------------+
!C | MATRIX ASSEMBLE |
!C +-----------------+
!C===
      do icel= 1, NE
        in1= ICELNOD(2*icel-1)
        in2= ICELNOD(2*icel  )
        DL = dX

        cK= AREA*COND/DL
        EMAT(1,1)= Ck*KMAT(1,1)
        EMAT(1,2)= Ck*KMAT(1,2)
        EMAT(2,1)= Ck*KMAT(2,1)
        EMAT(2,2)= Ck*KMAT(2,2)

        DIAG(in1)= DIAG(in1) + EMAT(1,1)
        DIAG(in2)= DIAG(in2) + EMAT(2,2)


        if (my_rank.eq.0.and.icel.eq.1) then
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
      if (my_rank.eq.0) then
        i = 1
        jS= INDEX(i-1)

        AMAT(jS+1)= 0.d0
        DIAG(i)= 1.d0
        RHS (i)= 0.d0
        do k= 1, NPLU
          if (ITEM(k).eq.1) AMAT(k)= 0.d0
        enddo
      endif
!C===
      E1Time = MPI_Wtime()

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

!C-    init
        do neib= 1, NEIBPETOT
          do k= export_index(neib-1)+1, export_index(neib)
            kk= export_item(k)
            SENDbuf(k)= PHI(kk)
          enddo  
        enddo  

!C
!C-- SEND & RECV.
      do neib= 1, NEIBPETOT
        is   = export_index(neib-1) + 1
        len_s= export_index(neib) - export_index(neib-1) 
        call MPI_Isend (SENDbuf(is), len_s, MPI_DOUBLE_PRECISION,       &
     &                  NEIBPE(neib), 0, MPI_COMM_WORLD,                &
     &                  request_send(neib), ierr)
      enddo

      do neib= 1, NEIBPETOT
        ir   = import_index(neib-1) + 1
        len_r= import_index(neib) - import_index(neib-1) 
        call MPI_Irecv (PHI(ir+N), len_r, MPI_DOUBLE_PRECISION,         &
     &                  NEIBPE(neib), 0, MPI_COMM_WORLD,                &
     &                  request_send(neib+NEIBPETOT), ierr)
      enddo
      call MPI_Waitall (2*NEIBPETOT, request_send, stat_send, ierr)

      do i= 1, N
        W(i,R) = DIAG(i)*PHI(i)
        do j= INDEX(i-1)+1, INDEX(i)
          W(i,R) = W(i,R) + AMAT(j)*PHI(ITEM(j))
        enddo
      enddo

      BNRM20= 0.0D0
      do i= 1, N
        BNRM20 = BNRM20  + RHS(i)  **2
        W(i,R) = RHS(i) - W(i,R)
      enddo
      call MPI_Allreduce (BNRM20, BNRM2, 1, MPI_DOUBLE_PRECISION,       &
     &                    MPI_SUM, MPI_COMM_WORLD, ierr)

!C********************************************************************
      do iter= 1, ITERmax
!C
!C-- {z}= [Minv]{r}

      do i= 1, N
        W(i,Z)= W(i,DD) * W(i,R)
      enddo

!C
!C-- RHO= {r}{z}

      RHO0= 0.d0
      do i= 1, N
        RHO0= RHO0 + W(i,R)*W(i,Z)   
      enddo     
      call MPI_Allreduce (RHO0, RHO, 1, MPI_DOUBLE_PRECISION,           &
     &                    MPI_SUM, MPI_COMM_WORLD, ierr)

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

!C-    init
        do neib= 1, NEIBPETOT
          do k= export_index(neib-1)+1, export_index(neib)
            kk= export_item(k)
            SENDbuf(k)= W(kk,P)
          enddo  
        enddo  

!C
!C-- SEND & RECV.
      do neib= 1, NEIBPETOT
        is   = export_index(neib-1) + 1
        len_s= export_index(neib) - export_index(neib-1) 
        call MPI_Isend (SENDbuf(is), len_s, MPI_DOUBLE_PRECISION,       &
     &                  NEIBPE(neib), 0, MPI_COMM_WORLD,                &
     &                  request_send(neib), ierr)
      enddo

      do neib= 1, NEIBPETOT
        ir   = import_index(neib-1) + 1
        len_r= import_index(neib) - import_index(neib-1) 
        call MPI_Irecv (W(ir+N,P), len_r, MPI_DOUBLE_PRECISION,         &
     &                  NEIBPE(neib), 0, MPI_COMM_WORLD,                &
     &                  request_send(neib+NEIBPETOT), ierr)
      enddo
      call MPI_Waitall (2*NEIBPETOT, request_send, stat_send, ierr)

      do i= 1, N
        W(i,Q) = DIAG(i)*W(i,P)
        do j= INDEX(i-1)+1, INDEX(i)
          W(i,Q) = W(i,Q) + AMAT(j)*W(ITEM(j),P)
        enddo
      enddo

!C
!C-- ALPHA= RHO / {p}{q}

      C10= 0.d0
      do i= 1, N
        C10= C10 + W(i,P)*W(i,Q)
      enddo

      call MPI_Allreduce (C10, C1, 1, MPI_DOUBLE_PRECISION,             &
     &                    MPI_SUM, MPI_COMM_WORLD, ierr)

      ALPHA= RHO / C1

!C
!C-- {x}= {x} + ALPHA*{p}
!C   {r}= {r} - ALPHA*{q}

      do i= 1, N
        PHI(i)= PHI(i) + ALPHA * W(i,P)
        W(i,R)= W(i,R) - ALPHA * W(i,Q)
      enddo

      DNRM20 = 0.0
      do i= 1, N
        DNRM20= DNRM20 + W(i,R)**2
      enddo

      call MPI_Allreduce (DNRM20, DNRM2, 1, MPI_DOUBLE_PRECISION,       &
     &                    MPI_SUM, MPI_COMM_WORLD, ierr)

        RESID= dsqrt(DNRM2/BNRM2)

        if (my_rank.eq.0.and.mod(iter,1000).eq.0) then
          write (*, '(i8,1pe16.6)') iter, RESID
        endif

        if ( RESID.le.EPS) goto 900
        RHO1 = RHO

      enddo
!C********************************************************************

      IER = 1

  900 continue

      E2Time = MPI_Wtime()

        if (my_rank.eq.0) then
          write (*, '(i8,1pe16.6)') iter, RESID
        endif
!C===

!C
!C-- OUTPUT
        if (my_rank.eq.0) then
          write (*,'(2(1pe16.6),a)') E1Time-S1Time, E2Time-E1Time, 
     &                               'sec.'
        endif


      if (my_rank.eq.PETOT-1) then
        write (*,'(/a)') '### TEMPERATURE'
        write (*,'(2i8, 1pe27.20 //)') my_rank, N, PHI(N)
      endif

      call MPI_FINALIZE (ierr)
      end program heat1Dp
