/***
	program heat3Dp
***/
/** PARALLEL VERSION **/
#include <stdio.h>
#include <stdlib.h>
FILE* fp_log;
#define GLOBAL_VALUE_DEFINE
#include "pfem_util.h"
#include "ppohVIS_PFEM3D_Util.h"
extern void PFEM_INIT(int,char**);
extern void INPUT_CNTL();
extern void INPUT_GRID();
extern void MAT_CON0();
extern void MAT_CON1();
extern void MAT_ASS_MAIN();
extern void MAT_ASS_BC();
extern void SOLVE11();
extern void OUTPUT_UCD();
extern void PFEM_FINALIZE();
int main(int argc,char* argv[])
{ int i;
  double START_TIME,END_TIME;
  struct ppohVIS_BASE_stContrl *pControl=NULL;
  struct ppohVIS_BASE_stResultCollection *pNodeResult=NULL;

/**
   +-------+
   | INIT. |
   +-------+
**/ 
  PFEM_INIT(argc,argv);
  ppohVIS_PFEM3D_Init(MPI_COMM_WORLD);

/** Logfile for debug **/
  if( my_rank == 0 ){
    if( (fp_log=fopen("log.log","w")) == NULL){
      fprintf(stdout,"input file cannot be opened!\n");
      exit(1);
    }
  }

  INPUT_CNTL();
  pControl = ppohVIS_PFEM3D_GetControl("vis.cnt");

  INPUT_GRID();
  if(ppohVIS_PFEM3D_SetMeshEx(
      NP,N,NODE_ID,XYZ,ICELTOT,ICELTOT_INT,ELEM_ID,ICELNOD,
      NEIBPETOT,NEIBPE,IMPORT_INDEX,IMPORT_ITEM,EXPORT_INDEX,EXPORT_ITEM)) {
    ppohVIS_BASE_PrintError(stderr);
    MPI_Abort(MPI_COMM_WORLD,errno);
  }

/**
   +---------------------+
   | matrix connectivity |
   +---------------------+
**/
  START_TIME= MPI_Wtime();

  MAT_CON0();
  MAT_CON1();

  END_TIME= MPI_Wtime();

  if (my_rank == 0) {
    fprintf(stdout,"*** matrix conn. %e sec.\n",END_TIME-START_TIME);
    fprintf(fp_log,"*** matrix conn. %e sec.\n",END_TIME-START_TIME);
  }
/**
   +-----------------+
   | MATRIX assemble |
   +-----------------+
**/
  START_TIME= MPI_Wtime();

  MAT_ASS_MAIN();
  MAT_ASS_BC()  ;
  
  END_TIME= MPI_Wtime();
  
  if (my_rank == 0) {
    fprintf(stdout,"*** matrix ass. %e sec.\n",END_TIME-START_TIME);
    fprintf(fp_log,"*** matrix ass. %e sec.\n",END_TIME-START_TIME);
  }
/**
   +--------+
   | SOLVER |
   +--------+
**/
  START_TIME= MPI_Wtime();

  SOLVE11();
  END_TIME= MPI_Wtime();

  if (my_rank == 0) {
    fprintf(stdout,"*** real COMP. %e sec.\n",END_TIME-START_TIME);
    fprintf(fp_log,"*** real COMP. %e sec.\n",END_TIME-START_TIME);
  }
/**
   +--------+
   | OUTPUT |
   +--------+
**/
  START_TIME= MPI_Wtime();
  pNodeResult=ppohVIS_BASE_AllocateResultCollection();
  if(pNodeResult == NULL) {
    ppohVIS_BASE_PrintError(stderr);
    MPI_Abort(MPI_COMM_WORLD,errno);
  }

  if(ppohVIS_BASE_InitResultCollection(pNodeResult,1)) {
    ppohVIS_BASE_PrintError(stderr);
    MPI_Abort(MPI_COMM_WORLD,errno);
  }

  pNodeResult->Results[0] = ppohVIS_PFEM3D_ConvResultNodeItem1N(NP,"temp",X);

  if(ppohVIS_PFEM3D_Visualize(pNodeResult,NULL,pControl,"vis",1)) {
    ppohVIS_BASE_PrintError(stderr);
    MPI_Abort(MPI_COMM_WORLD,errno);
  }

  ppohVIS_PFEM3D_Finalize();
  END_TIME= MPI_Wtime();

  if (my_rank == 0) {
    fprintf(stdout,"*** Para-Vis.  %e sec.\n",END_TIME-START_TIME);
    fprintf(fp_log,"*** Para-Vis.  %e sec.\n",END_TIME-START_TIME);
  }
  
  for(i=0;i<N;i++){
    if (XYZ[i][0]==0.e0) {
    if (XYZ[i][1]==0.e0) {
    if (XYZ[i][2]==0.e0) {
      printf("%6d%8d%16.6e\n\n\n", my_rank, i+1, X[i]);}
    }}}

  PFEM_FINALIZE() ;
}
/**
   ERROR_EXIT
**/
void  ERROR_EXIT (int IFLAG, int my_rank)
{
  if (my_rank == 0) {
    
    if( IFLAG == 101 ){
      fprintf(stdout,"#### PFEM-SOL-E0101: PEsmpTOT must be >0\n");
      errno=MPI_Finalize ();
      exit(1);
    }
    
    if( IFLAG == 102 ){
      fprintf(stdout,"#### PFEM-SOL-E0102: METHOD must be 1 or 2\n");
      errno=MPI_Finalize ();
      exit(1);
    }
    
    if( IFLAG == 103 ){
      fprintf(stdout,"#### PFEM-SOL-E0103: PRECOND must be 0 or 1\n");
      errno=MPI_Finalize ();
      exit(1);
    }
    
    if( IFLAG == 104 ){
      fprintf(stdout,"#### PFEM-SOL-E0104: ITER must be >0\n");
      errno=MPI_Finalize ();
      exit(1);
    }
    
    if( IFLAG == 111 ){
      fprintf(stdout,"#### PFEM-SOL-W0111: iterPREmax must be >=1\n");
      fprintf(stdout,"                     iterPREmax set to   =1\n");
      return;
    }
    
    if( IFLAG == 112 ){
      fprintf(stdout,"#### PFEM-SOL-W0112: iterPREmax must be =<4\n");
      fprintf(stdout,"                     iterPREmax set to   =4\n");
      return;
    }
    
    if( IFLAG == 201 ){
      fprintf(stdout,"#### PFEM-SOL-E0201: too many PEs specified\n");
      fprintf(stdout,"                                 in MPIRUN.\n");
      MPI_Abort (MPI_COMM_WORLD,errno);
      exit(1);
    }
    
    if( IFLAG == 202 ){
      fprintf(stdout,"#### PFEM-SOL-E0202: invalid mesh data %d\n",my_rank);
      fprintf(stdout,"                                 in MPIRUN.\n");
      errno=MPI_Finalize ();
      exit(1);
    }
  }
  return;
}


