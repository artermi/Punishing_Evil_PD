#include "Evil_PD.h"
#include <unistd.h>
using namespace std;

int do_once(double T, double W, double g, double U, double S){
	printf("Now doing Evil_PD with (T,W,g,U,s) = (%f,%f,%f,%f,%f)\n",
		T,W,g,U,S);
	char file_n[100];
	sprintf(file_n,"T_%04d_W_%04d_G_%04d_U_%04d_S_%04d.dat", 
		(int)((T + 0.000001) * 100),
		(int)((W + 0.000001) * 100), (int)((g + 0.000001) * 100),
		(int)((U + 0.000001) * 100), (int)((-S + 0.000001) * 100));

	FILE *file;
  	file = fopen(file_n, "r");

	if (file) {
		fclose(file);
		printf("file:'%s' exists\n",file_n);
		usleep(100000);
		return 0;
	}

	file = fopen(file_n, "w");
	fclose(file);

	Evil_PD gameOBJ(T,W,g,U,S,false,false,1000000);
	gameOBJ.game(true);

	return 0;
}


int main(int argc, char** argv){
	srand(time(NULL));

	double W = 0.2;
	double g = 0.2;
	double T = 2;


	
	for(double U = 0.0; U < 1.01; U += 0.02){
		do_once(T,W,g,U,-0.1);	
	}


	return 0;
}
