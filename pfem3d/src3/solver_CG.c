/**
 ** CG
 **/
#include <stdio.h>
#include <math.h>
#include "mpi.h"
#include "precision.h"
#include "allocate.h"
extern FILE *fp_log;
extern void SOLVER_SEND_RECV (); 
/***
    CG solves the linear system Ax = b using the Conjugate Gradient 
    iterative method with the following preconditioners
***/
void  CG  (
	   KINT N,KINT NP,KINT NPLU, KREAL D[],
	   KREAL AMAT[],KINT indexLU[], KINT itemLU[],
	   KREAL B[],KREAL X[],KREAL RESID,KINT ITER, KINT *ERROR,int my_rank,
	   int NEIBPETOT,int NEIBPE[],
	   int IMPORT_INDEX[], int IMPORT_ITEM[],
	   int EXPORT_INDEX[], int EXPORT_ITEM[])
{
  int i,j,k,PEsmpTOT,nth,nr,ip,ip0;
  int ieL,isL,ieU,isU;
  double WVAL;
  double BNRM20,BNRM2,DNRM20,DNRM2;
  double S1_TIME,E1_TIME;
  double ALPHA,BETA;
  double C1,C10,RHO,RHO0,RHO1;
  int    iterPRE,*SMPindex;
  
  KREAL *WS,*WR, *W_RHO0,*W_C10,*W_BNRM20,*W_DNRM20;
  KREAL **WW;
  
  KINT R=0,Z=1,Q=1,P=2,DD=3;
  KINT MAXIT;
  KREAL TOL;
  
  double COMPtime,COMMtime,R1;
  double START_TIME,END_TIME;
  
/**
   +-------+
   | INIT. |
   +-------+
**/
  PEsmpTOT= 0;

#pragma omp parallel
{
#pragma omp master
  {PEsmpTOT= omp_get_num_threads();}
}
  
  ERROR= 0;
  
  COMPtime=0.0;
  COMMtime=0.0;
  
  WW=(KREAL**) allocate_matrix(sizeof(KREAL),4,NP);
  WS=(KREAL* ) allocate_vector(sizeof(KREAL),  NP);
  WR=(KREAL* ) allocate_vector(sizeof(KREAL),  NP);
  W_RHO0=(KREAL* ) allocate_vector(sizeof(KREAL),  PEsmpTOT);
  W_C10 =(KREAL* ) allocate_vector(sizeof(KREAL),  PEsmpTOT);
  W_BNRM20=(KREAL* ) allocate_vector(sizeof(KREAL),  PEsmpTOT);
  W_DNRM20=(KREAL* ) allocate_vector(sizeof(KREAL),  PEsmpTOT);
  SMPindex=(KINT* )  allocate_vector(sizeof(KINT ),  PEsmpTOT+1);          
  
  SMPindex[0]=0;
  nth= N/PEsmpTOT;
  nr = N - nth*PEsmpTOT;
    for(ip=1;ip<PEsmpTOT+1;ip++) {	   
      SMPindex[ip]= nth;
      if (ip <= nr) SMPindex[ip]=nth+1;
    }
    for(ip=1;ip<PEsmpTOT+1;ip++) {	   
      SMPindex[ip]= SMPindex[ip-1] + SMPindex[ip];
    }

  MAXIT  = ITER;
  TOL   = RESID;          
  
#pragma omp parallel for private (i)
  for(i=0;i<NP;i++) {
    WW[0][i]=0.0;
    WW[1][i]=0.0;
    WW[2][i]=0.0;
    WW[3][i]=0.0;
    WS[i]= 0.0;
    WR[i]= 0.0;
    X [i]= 0.0;
  }


/**
   +-----------------------+
   | {r0}= {b} - [A]{xini} |
   +-----------------------+
**/
/**
 ** INTERFACE data EXCHANGE
**/
  SOLVER_SEND_RECV
    ( NP, NEIBPETOT, NEIBPE, IMPORT_INDEX, IMPORT_ITEM,
      EXPORT_INDEX, EXPORT_ITEM, WS, WR, X , my_rank);

#pragma omp parallel private (ip,j,i,k,WVAL) 
  {
    ip= omp_get_thread_num();
    for(j=SMPindex[ip];j<SMPindex[ip+1];j++){
      WW[DD][j]= 1.0/D[j];
      WVAL     = B[j] - D[j]*X[j];
      for( k=indexLU[j];k<indexLU[j+1];k++){
        i    = itemLU[k];
        WVAL+=  -AMAT[k]*X[i];
      }
    WW[R][j]= WVAL;
    }
  
    W_BNRM20[ip]=0.0;	
    for(i=SMPindex[ip];i<SMPindex[ip+1];i++){      
      W_BNRM20[ip]+= B[i]*B[i];
    }
  }
/** END PARALLEL **/
   
    BNRM20= 0.0;
    for(ip0=0;ip0<PEsmpTOT;ip0++){
      BNRM20+= W_BNRM20[ip0];
    }
  
  MPI_Allreduce (&BNRM20, &BNRM2, 1, MPI_DOUBLE,MPI_SUM, MPI_COMM_WORLD);
  
  if (BNRM2 == 0.e0) BNRM2= 1.e0;
  
  ITER = 0;
  
  S1_TIME= MPI_Wtime();
  for( ITER=1;ITER<= MAXIT;ITER++){
/**
************************************************* Conjugate Gradient Iteration
**/

/**
   +----------------+
   | {z}= [Minv]{r} |
   +----------------+
**/  
#pragma omp parallel private (ip,i,RHO0) 
  {
    ip= omp_get_thread_num();
    for(i=SMPindex[ip];i<SMPindex[ip+1];i++){
      WW[Z][i]= WW[DD][i]*WW[R][i];
    }
/**
   +---------------+
   | {RHO}= {r}{z} |
   +---------------+
**/
    W_RHO0[ip]=0.0;	
    for(i=SMPindex[ip];i<SMPindex[ip+1];i++){      
      W_RHO0[ip]+= WW[R][i]*WW[Z][i];
    }
  }
/** END PARALLEL **/
   
    RHO0= 0.0;
    for(ip0=0;ip0<PEsmpTOT;ip0++){
      RHO0+= W_RHO0[ip0];
    }
  
    MPI_Allreduce (&RHO0, &RHO, 1, MPI_DOUBLE,MPI_SUM, MPI_COMM_WORLD);
    
/**
   +-----------------------------+
   | {p} = {z} if      ITER=1    |
   | BETA= RHO / RHO1  otherwise |
   +-----------------------------+
**/
#pragma omp parallel private (ip,i) 
  {
    ip= omp_get_thread_num();
    if( ITER == 1 ){
      for(i=SMPindex[ip];i<SMPindex[ip+1];i++){      
	WW[P][i]=WW[Z][i];
      }
    }else{
      BETA= RHO / RHO1;
      for(i=SMPindex[ip];i<SMPindex[ip+1];i++){            
	WW[P][i]=WW[Z][i] + BETA*WW[P][i];
      }
    }
  }
/** END PARALLEL **/   
/**
   +-------------+
   | {q}= [A]{p} |
   +-------------+
**/      

    SOLVER_SEND_RECV
      ( NP, NEIBPETOT, NEIBPE, IMPORT_INDEX, IMPORT_ITEM,
	EXPORT_INDEX, EXPORT_ITEM, WS, WR, WW[P], my_rank);

#pragma omp parallel private (ip,j,i,k,WVAL)
  {
    ip= omp_get_thread_num();
    for(j=SMPindex[ip];j<SMPindex[ip+1];j++){          
      WVAL= D[j] * WW[P][j];
      for(k=indexLU[j];k<indexLU[j+1];k++){
	i=itemLU[k];
	WVAL+= AMAT[k] * WW[P][i];
      }
      WW[Q][j]=WVAL;
    }
/**
   +---------------------+
   | ALPHA= RHO / {p}{q} |
   +---------------------+
**/
    W_C10[ip]=0.0;	
    for(i=SMPindex[ip];i<SMPindex[ip+1];i++){      
      W_C10[ip]+= WW[P][i]*WW[Q][i];
    }
  }
/** END PARALLEL **/

    C10= 0.0;
    for(ip0=0;ip0<PEsmpTOT;ip0++){    
      C10+= W_C10[ip0];
    }
    
    MPI_Allreduce (&C10,&C1, 1, MPI_DOUBLE,MPI_SUM, MPI_COMM_WORLD);
    ALPHA= RHO / C1;
    
/**
   +----------------------+
   | {x}= {x} + ALPHA*{p} |
   | {r}= {r} - ALPHA*{q} |
   +----------------------+
**/
#pragma omp parallel private (ip,i) 
  {
    ip= omp_get_thread_num();
    for(i=SMPindex[ip];i<SMPindex[ip+1];i++){              
      X [i]   +=  ALPHA *WW[P][i];
      WW[R][i]+= -ALPHA *WW[Q][i];
    }

    W_DNRM20[ip]=0.0;	
    for(i=SMPindex[ip];i<SMPindex[ip+1];i++){      
      W_DNRM20[ip]+= WW[R][i]*WW[R][i];
    }
  }
/** END PARALLEL **/
   
    DNRM20= 0.e0;
    for(ip0=0;ip0<PEsmpTOT;ip0++){    
      DNRM20+= W_DNRM20[ip0];
    }
    MPI_Allreduce (&DNRM20,&DNRM2, 1, MPI_DOUBLE,MPI_SUM, MPI_COMM_WORLD);
    RESID= sqrt(DNRM2/BNRM2);

/** ##### ITERATION HISTORY ***/
    if( my_rank == 0 ) fprintf(stdout,"%d %e\n",ITER,RESID);
    if( my_rank == 0 ) fprintf(fp_log,"%d %e\n",ITER,RESID);

    if ( RESID <= TOL   ) break;
    if ( ITER  == MAXIT ) *ERROR= -300;
    
    RHO1 = RHO ;                                                           
}
  E1_TIME= MPI_Wtime();
  COMPtime= E1_TIME - S1_TIME;
  
  R1= 100.e0 * ( 1.e0 - COMMtime/COMPtime );

  if (my_rank == 0) {
    fprintf(stdout,"### elapsed    :%e\n",COMPtime);
    fprintf(stdout,"### comm.      :%e\n",COMMtime);
    fprintf(stdout,"### work ratio :%e\n",R1);
  }
  
  SOLVER_SEND_RECV
    ( NP, NEIBPETOT, NEIBPE, IMPORT_INDEX, IMPORT_ITEM,
      EXPORT_INDEX, EXPORT_ITEM, WS, WR, X, my_rank);

  free ( (KREAL**)WW);
  deallocate_vector ( (KREAL**)WR);
  deallocate_vector( (KREAL**)WS);
}
