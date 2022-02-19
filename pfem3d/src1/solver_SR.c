#include <stdio.h>
#include <math.h>
#include "mpi.h"
#include "precision.h"
#include "allocate.h"
static MPI_Status  *sta1,*sta2;
static MPI_Request *req1,*req2;
static KINT NFLAG=0;
extern FILE *fp_log;
void SOLVER_SEND_RECV ( int N, int NEIBPETOT,
			int NEIBPE[], int IMPORT_INDEX[], int IMPORT_ITEM[],
			int EXPORT_INDEX[], int EXPORT_ITEM[],
			KREAL WS[], KREAL WR[], KREAL X[], int my_rank)
{

  int  ii,k,neib,istart,inum;
/***
    INIT.
***/
  if( NFLAG == 0 ){
    sta1=(MPI_Status*)allocate_vector(sizeof(MPI_Status),NEIBPETOT);
    sta2=(MPI_Status*)allocate_vector(sizeof(MPI_Status),NEIBPETOT);
    req1=(MPI_Request*)allocate_vector(sizeof(MPI_Request),NEIBPETOT);
    req2=(MPI_Request*)allocate_vector(sizeof(MPI_Request),NEIBPETOT);
    NFLAG=1;
  } 
/***
    SEND
***/
  for( neib=1;neib<=NEIBPETOT;neib++){
    istart=EXPORT_INDEX[neib-1];
    inum  =EXPORT_INDEX[neib]-istart;
#pragma omp parallel for private (k,ii)
    for( k=istart;k<istart+inum;k++){
      ii= EXPORT_ITEM[k];
      WS[k]= X[ii-1];
    }
    
    MPI_Isend(&WS[istart],inum,MPI_DOUBLE,
	      NEIBPE[neib-1],0,MPI_COMM_WORLD,&req1[neib-1]);
  }
/***
    RECEIVE
***/
  for( neib=1;neib<=NEIBPETOT;neib++){
    istart=IMPORT_INDEX[neib-1];
    inum  =IMPORT_INDEX[neib]-istart;
    MPI_Irecv(&WR[istart],inum,MPI_DOUBLE,
	      NEIBPE[neib-1],0,MPI_COMM_WORLD,&req2[neib-1]);
  }
  
  MPI_Waitall (NEIBPETOT, req2, sta2);
  
  for( neib=1;neib<=NEIBPETOT;neib++){
    istart=IMPORT_INDEX[neib-1];
    inum  =IMPORT_INDEX[neib]-istart;
#pragma omp parallel for private (k,ii)
    for( k=istart;k<istart+inum;k++){
      ii   = IMPORT_ITEM[k];
      X[ii-1]= WR[k];
    }
  }
  
  MPI_Waitall (NEIBPETOT, req1, sta1);

}
