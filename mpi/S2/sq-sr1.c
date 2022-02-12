#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "mpi.h"
int main(int argc, char **argv){

	int n, np, NeibPeTot, BufLength;
	MPI_Status *StatSend, *StatRecv;
	MPI_Request *RequestSend, *RequestRecv;

	int MyRank, PeTot;
	int *val, *SendBuf, *RecvBuf, *NeibPe;
	int *ImportIndex, *ExportIndex, *ImportItem, *ExportItem;

	char FileName[80], line[80];
	int i, nn, neib;
	int iStart, iEnd;
	FILE *fp;

/*
!C
!C +-----------+
!C | INIT. MPI |
!C +-----------+
!C===*/
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &PeTot);
	MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
/*
!C
!C +-----------+
!C | DATA file |
!C +-----------+
!C===*/
	sprintf(FileName, "sqm.%d", MyRank);
	fp = fopen(FileName, "r");
	assert(fp != NULL);

	fscanf(fp, "%80s", line);
	assert(!strncmp(line, "#NEIBPEtot", 80));
	fscanf(fp, "%d", &NeibPeTot);
	NeibPe = calloc(NeibPeTot, sizeof(int));
	ImportIndex = calloc(1+NeibPeTot, sizeof(int));
	ExportIndex = calloc(1+NeibPeTot, sizeof(int));

	fscanf(fp, "%80s", line);
	assert(!strncmp(line, "#NEIBPE", 80));
	for(neib=0;neib<NeibPeTot;neib++){
		fscanf(fp, "%d", &NeibPe[neib]);
	}

	fscanf(fp, "%80s", line);
	assert(!strncmp(line, "#NODE", 80));
	fscanf(fp, "%d %d", &np, &n);

	fscanf(fp, "%80s", line);
	assert(!strncmp(line, "#IMPORTindex", 80));
	for(neib=1;neib<NeibPeTot+1;neib++){
		fscanf(fp, "%d", &ImportIndex[neib]);
	}
	nn = ImportIndex[NeibPeTot];
	ImportItem = malloc(nn * sizeof(int));
	fscanf(fp, "%80s", line);
	assert(!strncmp(line, "#IMPORTitems", 80));
	for(i=0;i<nn;i++){
		fscanf(fp, "%d", &ImportItem[i]);
		ImportItem[i]--;
	}

	fscanf(fp, "%80s", line);
	assert(!strncmp(line, "#EXPORTindex", 80));
	for(neib=1;neib<NeibPeTot+1;neib++){
		fscanf(fp, "%d", &ExportIndex[neib]);
	}
	nn = ExportIndex[NeibPeTot];
	ExportItem = malloc(nn * sizeof(int));
	fscanf(fp, "%80s", line);
	assert(!strncmp(line, "#EXPORTitems", 80));
	for(i=0;i<nn;i++){
		fscanf(fp, "%d", &ExportItem[i]);
		ExportItem[i]--;
	}

	sprintf(FileName, "sq.%d", MyRank);
	fp = fopen(FileName, "r");
	assert(fp != NULL);

	val = calloc(np, sizeof(*val));
	for(i=0;i<n;i++){
		fscanf(fp, "%d", &val[i]);
	}
/*
!C
!C +--------+
!C | BUFFER |
!C +--------+ 
!C===*/
	SendBuf = calloc(ExportIndex[NeibPeTot], sizeof(*SendBuf));
	RecvBuf = calloc(ImportIndex[NeibPeTot], sizeof(*RecvBuf));
	
	for(neib=0;neib<NeibPeTot;neib++){
		iStart = ExportIndex[neib];
		iEnd   = ExportIndex[neib+1];
		for(i=iStart;i<iEnd;i++){
			SendBuf[i] = val[ExportItem[i]];
		}
	}
		
/*
!C
!C +-----------+
!C | SEND-RECV |
!C +-----------+ 
!C===*/
	StatSend = malloc(sizeof(MPI_Status) * NeibPeTot);
	StatRecv = malloc(sizeof(MPI_Status) * NeibPeTot);
	RequestSend = malloc(sizeof(MPI_Request) * NeibPeTot);
	RequestRecv = malloc(sizeof(MPI_Request) * NeibPeTot);

	for(neib=0;neib<NeibPeTot;neib++){
		iStart = ExportIndex[neib];
		iEnd   = ExportIndex[neib+1];
		BufLength = iEnd - iStart;
		MPI_Isend(&SendBuf[iStart], BufLength, MPI_INT,
				NeibPe[neib], 0, MPI_COMM_WORLD, &RequestSend[neib]);
	}

	for(neib=0;neib<NeibPeTot;neib++){
		iStart = ImportIndex[neib];
		iEnd   = ImportIndex[neib+1];
		BufLength = iEnd - iStart;

		MPI_Irecv(&RecvBuf[iStart], BufLength, MPI_INT,
				NeibPe[neib], 0, MPI_COMM_WORLD, &RequestRecv[neib]);
	}

	MPI_Waitall(NeibPeTot, RequestRecv, StatRecv);

	for(neib=0;neib<NeibPeTot;neib++){
		iStart = ImportIndex[neib];
		iEnd   = ImportIndex[neib+1];
		for(i=iStart;i<iEnd;i++){
			val[ImportItem[i]] = RecvBuf[i];
		}
	}
	MPI_Waitall(NeibPeTot, RequestSend, StatSend);
/*
!C
!C +--------+
!C | OUTPUT |
!C +--------+
!C===*/
	for(neib=0;neib<NeibPeTot;neib++){
		iStart = ImportIndex[neib];
		iEnd   = ImportIndex[neib+1];
		for(i=iStart;i<iEnd;i++){
			int in = ImportItem[i];
			printf("RECVbuf%8d%8d%8d\n", MyRank, NeibPe[neib], val[in]);
		}
	}
	MPI_Finalize();

	return 0;
}


