#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "histogram.h"

#define RANGE 16

int main (int argc, char* argv[]) {
	
	chart(argc,argv);
	
	float meancalc = mean(argc, argv);
	printf("Mean of %d values: %.2f\n",argc-1,meancalc);
	
	float sdcalc = sd(meancalc, argc, argv);
	printf("Standard deviation of %d values: %.2f\n",argc-1,sdcalc);
	
	return 0;
}

void print(int low, int high,int val, int total) {
	printf("%3d - %3d: ",low,high);
	int i;
	
	for (i = 0; i<val;i++)
		printf("#");
	printf(" (%d)\n",val);
}

int sortints(int in, int range) {

	int length = RANGE;
	int i;
	
	for (i = 0; i < length; i++) {
		if (in >= i*range && in < ((i+1)*range))
			return i;			
	}
	
	return 0;
}

void chart(int argc, char* argv[]) {
	
	int counts[RANGE] = {0};
	int i;
	printf("\n");
	
	for(i = 1; i < argc; i++) {
		int temp = atoi(argv[i]);
		int index = sortints(temp, 16);
		counts[index] = counts[index] + 1;

	}

	for (i = 0; i < 16; i++) {
		//printf("counts[%d]: %d\n",i,counts[i]);

		char in = counts[i];
		print(i*16, (i+1)*16-1 ,in , argc-1);
	}
	printf("\n");

}

float mean(int argc, char* argv[]) {
	float total = 0;
	int i;
	
	for (i = 1; i < argc; i++) {
		total += atoi(argv[i]);
	}
	return total/(float)(argc-1);
	
}

float sd(float mean, int argc, char* argv[]) {
	float total = 0.0;
	int i;
	
	for (i = 1; i < argc; i++) {
		// (val - mean)^2
		float deviations = (atoi(argv[i]) - mean);
		deviations = deviations * deviations;
		total = total + deviations;
		
	}
	
	total = total/(float)(argc-1);
	
	float sd = sqrt(total);
	
	return sd;
}


