      subroutine PARASET
      use partitioner

      N2= max (IELMTOT, N)
      allocate (IGROUP(N2))

!C
!C-- RCB/METIS

      do 
        write (*,'(/,"# select PARTITIONING METHOD")')
        write (*,'(  "  RCB                  (1)")')
        write (*,'(  "  K-METIS              (2)")')
        write (*,'(  "  P-METIS              (3)")')
        write (*,*) ' '
        write (*,*) 'Please TYPE 1-3 !!'
        write (*,'(/,">>>")')
        read  (*,*)  NTYP
        if (NTYP.ge.1 .and. NTYP.le.3) exit
      enddo

      if (NTYP.eq.1) then
        do 
          write (*,'(/,"*** RECURSIVE COORDINATE BISECTION (RCB)")')
          write (*,*) 'How many partitions (2**n)?'
          write (*,'(/,">>>")')
          read  (*,*)  NPOWER
          if (NPOWER.ge.0) exit
        enddo
        NP= 2**NPOWER
      endif

      if (NTYP.ge.2) then
        do
          write (*,'(/,"*** K-METIS/P-METIS")')
          write (*,*      ) 'NUMBER of regions ?'
          write (*,'(/,">>>")')
          read  (*,*)  NP
          if (NP.ge.1) exit
        enddo
      endif

      write (*,'(//,"*** ",i3," REGIONS",//)') NP

      if (NP.gt.N) call ERROR_EXIT(32,N)
!C
!C-- allocation
      allocate (STACK_EXPORT(0:NP,NP))
      allocate (STACK_IMPORT(0:NP,NP))
      allocate (NPN(NP))
      allocate (NPC(NP))
      allocate (ISTACKN(0:NP))
      allocate (ISTACKC(0:NP))
      allocate (NEIBPE   (NP,NP))
      allocate (NEIBPETOT(NP))
      allocate (   NODTOT(NP))
      allocate (INTNODTOT(NP))

      allocate (IWORK(0:N2))
      allocate (IMASK(-N2:+N2))
      allocate (IDEAD (N2))
      allocate (ISTACK(N2))

      allocate (ICOND1(N2))
      allocate (ICOND2(N2))

      allocate (INODLOCAL(N2))

      allocate (NOD_IMPORT(  N2))

!C
!C-- FILE NAME
      do
        write (*,'(a)') '# HEADER of the OUTPUT file ?'
        write (*,'(a)') '  HEADER should not be <work>'
        write (*,'(/,">>>")')
        read  (*,'(a80)') HEADER
        if (HEADER.ne.'work') exit
      enddo

      HEADW= 'work'
      allocate (FILNAME(NP), WORKFIL(NP))

      do my_rank= 0, NP-1
        call DEFINE_FILE_NAME (HEADER, my_rank, FILNAME(my_rank+1))
        call DEFINE_FILE_NAME (HEADW , my_rank, WORKFIL(my_rank+1))
      enddo

!C
!C-- PARAMETER SET
 
      inum   = N / NP
      idev   = N - inum * NP

      do ip= 1, NP
        NPN(ip)= inum
      enddo

      do ip= 1, idev
        NPN(ip)= NPN(ip) + 1
      enddo

      do i= 1, N
        IGROUP(i)= 0
        RHO   (i)= 0
        IMASK (i)= 0
        IDEAD (i)= 0
      enddo

      return

 998  continue
        call ERROR_EXIT (21,0)
 999  continue
        call ERROR_EXIT (22,0)

      end
