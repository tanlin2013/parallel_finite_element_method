/**
 ** MAT_ASS_MAIN
 **/
#include <stdio.h>
#include <math.h>
#include "pfem_util.h"
#include "allocate.h"
extern FILE *fp_log;
extern void JACOBI();
void MAT_ASS_MAIN()
{
  int i,k,kk;
  int ip,jp,kp;
  int ipn,jpn,kpn;
  int icel;
  int ie,je;
  int iiS,iiE;
  int in1,in2,in3,in4,in5,in6,in7,in8;
  double SHi;
  double QP1,QM1,EP1,EM1,TP1,TM1;
  double X1,X2,X3,X4,X5,X6,X7,X8;
  double Y1,Y2,Y3,Y4,Y5,Y6,Y7,Y8;
  double Z1,Z2,Z3,Z4,Z5,Z6,Z7,Z8;
  double PNXi,PNYi,PNZi,PNXj,PNYj,PNZj;
  double COND0, QV0, QVC, COEFij;
  double coef;

  KINT nodLOCAL[8];
  
  AMAT=(KREAL*) allocate_vector(sizeof(KREAL),NPLU);
  B =(KREAL*) allocate_vector(sizeof(KREAL),NP);
  D =(KREAL*) allocate_vector(sizeof(KREAL),NP);
  X =(KREAL*) allocate_vector(sizeof(KREAL),NP);
  
  for(i=0;i<NPLU;i++) AMAT[i]=0.0;
  for(i=0;i<NP;i++) B[i]=0.0;
  for(i=0;i<NP;i++) D[i]=0.0;
  for(i=0;i<NP;i++) X[i]=0.0;
  
  WEI[0]= 1.0000000000e0;
  WEI[1]= 1.0000000000e0;
  
  POS[0]= -0.5773502692e0;
  POS[1]=  0.5773502692e0;
  
/***
    INIT.
    PNQ   - 1st-order derivative of shape function by QSI
    PNE   - 1st-order derivative of shape function by ETA
    PNT   - 1st-order derivative of shape function by ZET
***/
  for(ip=0;ip<2;ip++){
    for(jp=0;jp<2;jp++){
      for(kp=0;kp<2;kp++){
	QP1= 1.e0 + POS[ip];
	QM1= 1.e0 - POS[ip];
	EP1= 1.e0 + POS[jp];
	EM1= 1.e0 - POS[jp];
	TP1= 1.e0 + POS[kp];
	TM1= 1.e0 - POS[kp];
	SHAPE[ip][jp][kp][0]= O8th * QM1 * EM1 * TM1;
	SHAPE[ip][jp][kp][1]= O8th * QP1 * EM1 * TM1;
	SHAPE[ip][jp][kp][2]= O8th * QP1 * EP1 * TM1;
	SHAPE[ip][jp][kp][3]= O8th * QM1 * EP1 * TM1;
	SHAPE[ip][jp][kp][4]= O8th * QM1 * EM1 * TP1;
	SHAPE[ip][jp][kp][5]= O8th * QP1 * EM1 * TP1;
	SHAPE[ip][jp][kp][6]= O8th * QP1 * EP1 * TP1;
	SHAPE[ip][jp][kp][7]= O8th * QM1 * EP1 * TP1;
	PNQ[jp][kp][0]= - O8th * EM1 * TM1;
	PNQ[jp][kp][1]= + O8th * EM1 * TM1;
	PNQ[jp][kp][2]= + O8th * EP1 * TM1;
	PNQ[jp][kp][3]= - O8th * EP1 * TM1;
	PNQ[jp][kp][4]= - O8th * EM1 * TP1;
	PNQ[jp][kp][5]= + O8th * EM1 * TP1;
	PNQ[jp][kp][6]= + O8th * EP1 * TP1;
	PNQ[jp][kp][7]= - O8th * EP1 * TP1;
	PNE[ip][kp][0]= - O8th * QM1 * TM1;
	PNE[ip][kp][1]= - O8th * QP1 * TM1;
	PNE[ip][kp][2]= + O8th * QP1 * TM1;
	PNE[ip][kp][3]= + O8th * QM1 * TM1;
	PNE[ip][kp][4]= - O8th * QM1 * TP1;
	PNE[ip][kp][5]= - O8th * QP1 * TP1;
	PNE[ip][kp][6]= + O8th * QP1 * TP1;
	PNE[ip][kp][7]= + O8th * QM1 * TP1;
	PNT[ip][jp][0]= - O8th * QM1 * EM1;
	PNT[ip][jp][1]= - O8th * QP1 * EM1;
	PNT[ip][jp][2]= - O8th * QP1 * EP1;
	PNT[ip][jp][3]= - O8th * QM1 * EP1;
	PNT[ip][jp][4]= + O8th * QM1 * EM1;
	PNT[ip][jp][5]= + O8th * QP1 * EM1;
	PNT[ip][jp][6]= + O8th * QP1 * EP1;
				PNT[ip][jp][7]= + O8th * QM1 * EP1;
      }
    }
  }

  for( icel=0;icel< ICELTOT;icel++){
    COND0= COND;
    
    in1=ICELNOD[icel][0];
    in2=ICELNOD[icel][1];
    in3=ICELNOD[icel][2];
    in4=ICELNOD[icel][3];
    in5=ICELNOD[icel][4];
    in6=ICELNOD[icel][5];
    in7=ICELNOD[icel][6];
    in8=ICELNOD[icel][7];
/**
 **
 ** JACOBIAN & INVERSE JACOBIAN
**/
    nodLOCAL[0]= in1;
    nodLOCAL[1]= in2;
    nodLOCAL[2]= in3;
    nodLOCAL[3]= in4;
    nodLOCAL[4]= in5;
    nodLOCAL[5]= in6;
    nodLOCAL[6]= in7;
    nodLOCAL[7]= in8;
    
    X1=XYZ[in1-1][0];
    X2=XYZ[in2-1][0];
    X3=XYZ[in3-1][0];
    X4=XYZ[in4-1][0];
    X5=XYZ[in5-1][0];
    X6=XYZ[in6-1][0];
    X7=XYZ[in7-1][0];
    X8=XYZ[in8-1][0];
    
    Y1=XYZ[in1-1][1];
    Y2=XYZ[in2-1][1];
    Y3=XYZ[in3-1][1];
    Y4=XYZ[in4-1][1];
    Y5=XYZ[in5-1][1];
    Y6=XYZ[in6-1][1];
    Y7=XYZ[in7-1][1];
    Y8=XYZ[in8-1][1];
    
    QVC= O8th*(X1+X2+X3+X4+X5+X6+X7+X8+Y1+Y2+Y3+Y4+Y5+Y6+Y7+Y8);
    
    Z1=XYZ[in1-1][2];
    Z2=XYZ[in2-1][2];
    Z3=XYZ[in3-1][2];
    Z4=XYZ[in4-1][2];
    Z5=XYZ[in5-1][2];
    Z6=XYZ[in6-1][2];
    Z7=XYZ[in7-1][2];
    Z8=XYZ[in8-1][2];
    
    JACOBI(DETJ, PNQ, PNE, PNT, PNX, PNY, PNZ,     
	   X1, X2, X3, X4, X5, X6, X7, X8,
	   Y1, Y2, Y3, Y4, Y5, Y6, Y7, Y8,
	   Z1, Z2, Z3, Z4, Z5, Z6, Z7, Z8);
    
/**
   CONSTRUCT the GLOBAL MATRIX
**/
    for(ie=0;ie<8;ie++){
      ip=nodLOCAL[ie];
      for(je=0;je<8;je++){
	jp=nodLOCAL[je];
	
	kk=-1;
	if( jp != ip ){
	  iiS=indexLU[ip-1];
	  iiE=indexLU[ip  ];
	  for( k=iiS;k<iiE;k++){
	    if( itemLU[k] == jp-1 ){
	      kk=k;
	      break;
	    }
	  }
	}
	QV0= 0.e0;
	COEFij= 0.e0;
	
	for(kpn=0;kpn<2;kpn++){
	  for(jpn=0;jpn<2;jpn++){
	    for(ipn=0;ipn<2;ipn++){
	      coef= WEI[ipn]*WEI[jpn]*WEI[kpn];
	      
	      PNXi= PNX[ipn][jpn][kpn][ie];
	      PNYi= PNY[ipn][jpn][kpn][ie];
	      PNZi= PNZ[ipn][jpn][kpn][ie];
	      
	      PNXj= PNX[ipn][jpn][kpn][je];
	      PNYj= PNY[ipn][jpn][kpn][je];
	      PNZj= PNZ[ipn][jpn][kpn][je];
	      
	      COEFij+= coef*COND0*(PNXi*PNXj+PNYi*PNYj+PNZi*PNZj)*fabs(DETJ[ipn][jpn][kpn]);

	      SHi= SHAPE[ipn][jpn][kpn][ie];
	      QV0+= SHi * QVOL * coef * fabs(DETJ[ipn][jpn][kpn]);
	    }
	  }
	}
	
	if (jp==ip) { 
	  D[ip-1]+= COEFij;
	  B[ip-1]+= QV0*QVC;
	}
	if (jp != ip) { 
	  AMAT[kk]+= COEFij;
	}
	
      }
    }
  }
}

