#include <stdio.h>
int main(){
	int i,N;
	double VECp[20], VECs[20]; 
	double sum;

        N=20;
	for(i=0;i<N;i++){
            VECp[i] = 2.0;
            VECs[i] = 3.0;
                printf("%5d %15.0F %15.0F\n", i, VECp[i], VECs[i]);
	}

	sum = 0.0;
	for(i=0;i<N;i++){
		sum += VECp[i] * VECs[i];
	}

        printf("dot product %15.0F\n", sum);
	return 0;
}
