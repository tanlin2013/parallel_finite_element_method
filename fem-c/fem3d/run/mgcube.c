#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

int main(){
  int NX, NY, NZ, INODTOT, ICELTOT, IBNODTOT;
  int NXP1, NYP1, NZP1;
  int i,j,k,icou,imat,igtot,ii;
  int in1,in2,in3,in4,in5,in6,in7,in8;
  int ibt1,ibt2,ibt3,ibt4;
  int *icelnod, *IW1, *IW2, *IW3, *IW4, *itype;
  char group_name[4][80]={"Xmin","Ymin","Zmin","Zmax"};
  double DX, XX, YY, ZZ;
  FILE *fp, *fp2;

  printf("%s\n", "NX,NY,NZ?");
	scanf("%d %d %d", &NX, &NY, &NZ);

        NXP1= NX+1;
        NYP1= NY+1;
        NZP1= NZ+1;
	INODTOT=  NXP1*NYP1*NZP1;
	ICELTOT=  NX  *NY  *NZ;  
        IBNODTOT= NXP1*NYP1;

        DX= 1.0;
        imat= 1;

	fp = fopen("cube.0", "w");
	fprintf(fp,"%10d\n", INODTOT);

	icou=0;
	for(k=0;k<NZP1;k++){
	  for(j=0;j<NYP1;j++){
	    for(i=0;i<NXP1;i++){
              XX= i*DX;
              YY= j*DX;
              ZZ= k*DX;
	      icou= icou + 1;
		fprintf(fp,"%10d%16.6e%16.6e%16.6e\n", icou, XX, YY, ZZ);
	    }
	  }
	}

	itype = calloc(ICELTOT, sizeof(int));
	for(i=0;i<ICELTOT;i++){
	  itype[i]=361;
	}
	fprintf(fp,"%10d\n", ICELTOT);
	for(i=0;i<ICELTOT;i++){
	  fprintf(fp,"%10d", itype[i]);
	}
	  fprintf(fp,"\n");

	icou=0;
	for(k=0;k<NZ;k++){
	  for(j=0;j<NY;j++){
	    for(i=0;i<NX;i++){
	      icou= icou + 1;
	      in1 = k*IBNODTOT + j*NXP1 + i+1;
	      in2 = in1 + 1;
              in3 = in2 + NXP1;
              in4 = in3 - 1;
              in5 = in1 + IBNODTOT;
              in6 = in2 + IBNODTOT;
              in7 = in3 + IBNODTOT;
              in8 = in4 + IBNODTOT;
	      fprintf(fp,"%10d%10d%10d%10d%10d%10d%10d%10d%10d%10d\n",icou,imat,in1,in2,in3,in4,in5,in6,in7,in8);
	    }
	  }
	}


        igtot= 4;
          ibt1= NYP1*NZP1;
	  ibt2= NXP1*NZP1 + ibt1;
	  ibt3= NXP1*NYP1 + ibt2;
	  ibt4= NXP1*NYP1 + ibt3;

	fprintf(fp,"%10d\n", igtot);
	fprintf(fp,"%10d%10d%10d%10d\n", ibt1,ibt2,ibt3,ibt4);

	IW1 = calloc(ICELTOT, sizeof(int));
	IW2 = calloc(ICELTOT, sizeof(int));
	IW3 = calloc(ICELTOT, sizeof(int));
	IW4 = calloc(ICELTOT, sizeof(int));

        icou= 0;
        i=1;
	for(k=0;k<NZP1;k++){
	  for(j=0;j<NYP1;j++){
	    ii= k*IBNODTOT + j*NXP1 + i;
		IW1[icou]=ii;
	    icou= icou + 1;
	  }
	}

        icou= 0;
        j=0;
	for(k=0;k<NZP1;k++){
	  for(i=0;i<NXP1;i++){
	    ii= k*IBNODTOT + j*NXP1 + i + 1;
		IW2[icou]=ii;
	    icou= icou + 1;
	  }
	}

        icou= 0;
        k=0;
	for(j=0;j<NYP1;j++){
	  for(i=0;i<NXP1;i++){
	    ii= k*IBNODTOT + j*NXP1 + i + 1;
		IW3[icou]=ii;
	    icou= icou + 1;
	  }
	}

        icou= 0;
        k=NZP1-1;
	for(j=0;j<NYP1;j++){
	  for(i=0;i<NXP1;i++){
	    ii= k*IBNODTOT + j*NXP1 + i + 1;
		IW4[icou]=ii;
	    icou= icou + 1;
	  }
	}

	fprintf(fp,"%s\n", group_name[0]);
	for(i=0;i<NYP1*NZP1;i++){
	  fprintf(fp,"%10d", IW1[i]);
	}
        fprintf(fp,"\n");

	fprintf(fp,"%s\n", group_name[1]);
	for(i=0;i<NXP1*NZP1;i++){
	  fprintf(fp,"%10d", IW2[i]);
	}
        fprintf(fp,"\n");

	fprintf(fp,"%s\n", group_name[2]);
	for(i=0;i<NXP1*NYP1;i++){
	  fprintf(fp,"%10d", IW3[i]);
	}
        fprintf(fp,"\n");

	fprintf(fp,"%s\n", group_name[3]);
	for(i=0;i<NXP1*NYP1;i++){
	  fprintf(fp,"%10d", IW4[i]);
	}
        fprintf(fp,"\n");

	fclose(fp);
}
