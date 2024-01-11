#include "Evil_PD.h"
using namespace std;

Evil_PD::Evil_PD(const double t, const double w, const double G, const double u,const double s,
	const bool prep,const bool Grid){

	T = t;
	W = w;
	g = G;
	U = u;
	S = s;

	grid = Grid;

	Strategy = new int[LL];
	payoff_matrix[2][2] = 1+W-g; payoff_matrix[2][1] = 1-g; payoff_matrix[2][0] = 0-g;
	payoff_matrix[1][2] = 1+W;   payoff_matrix[1][1] = 1;   payoff_matrix[1][0] = S;
	payoff_matrix[0][2] = 0-U;   payoff_matrix[0][1] = T;   payoff_matrix[0][0] = 0;

	// 0 for D, 1 for C, 2 for SC.
	for(int i = 0; i < 3; i++)
		Cate_Player[i] = 0;

	if(prep){
		for (int i = 0; i < LL; ++i){
			if (i / L < int (L * (2.0/3.0 - 1.0/(4.0 * sqrt(3) )) ) ){
				Strategy[i] = (i % L < L/2) ? 0: 1;
			}
			else if (i / L < int ( L * (2.0/3.0 + 1.0/(4.0 * sqrt(3))))){
				double horiz = L * (2.0/3.0 + 1.0/(4.0 * sqrt(3)));
				int row_leng =  int ( ( horiz - (double (i)) / L) * sqrt(3) );
				//printf("%d %d\n", int(horiz), row_leng);
				if(i % L <  row_leng )
					Strategy[i] = 0;
				else
					Strategy[i] = (i % L > L - row_leng) ? 1: 2;
			}
			else{
				Strategy[i] = 2;
			}
			Cate_Player[Strategy[i]] ++;
		}
	}
	else{
		for(int i = 0; i < LL; i++){
			Strategy[i] = rand() % 3;

			Cate_Player[Strategy[i]] ++;
		}
	}



	Neighbour = new int *[LL];
	for(int i = 0; i < LL; i++)
		Neighbour[i] = new int[4];

	for(int i = 0; i < LL; i++){
		Neighbour[i][0] = (i - L + LL ) % LL; //North
		Neighbour[i][1] = (i + L) % LL;
		Neighbour[i][2] = (i + 1) % LL;
		Neighbour[i][3] = (i - 1 + LL) % LL;
	}//initialise the neighbour



}

Evil_PD::~Evil_PD(){
	delete Strategy;
	for(int i = 0; i < LL; i++){
		delete [] Neighbour[i];
	}
	delete [] Neighbour;
}


double Evil_PD::centre_game(const int cent){
	double profit = 0;
	for(int i = 0; i <4; i++){
		int neig_c = Neighbour[cent][i];
		profit += payoff_matrix[Strategy[cent]][Strategy[neig_c]];
	}
	return profit;
}


int Evil_PD::game(bool ptf){
	char path[100];
	FILE *file;

	if(ptf){
		
		sprintf(path,"T_%04d_W_%04d_G_%04d_U_%04d_S_%04d.dat", 
			(int)((T + 0.000001) * 100),
			(int)((W + 0.000001) * 100), (int)((g + 0.000001) * 100),
			(int)((U + 0.000001) * 100), (int)((-S + 0.000001) * 100));

		file = fopen(path,"a+");
		printf("Now file:%s\n",path);
	}

	double rate[3] = {0.0, 0.0, 0.0};
	double previous[5][3];
	int iter = ITER;
	int gap = GAP;
	bool stop_all_0 = true;

	for(int i = 0; i < iter; i++){
		bool stop_all = true;

 
		if(i % gap == 0){
			for (int j = 0; j < 3; ++j)
				rate[j] = (double) Cate_Player[j] / LL;

			if(ptf){
				fprintf(file, "%06d %.4f %.4f %.4f\n", i, rate[0],rate[1],rate[2]);
			}
			printf( "%06d %.4f %.4f %.4f\n", i, rate[0],rate[1],rate[2]);

			double pert = PERT;
			for(int j = 1; j < 5; j++)
				for(int k = 0; k < 3; k ++)
					previous[j-1][k] = previous[j][k];
			for(int k = 0; k < 3; k++)
				previous[4][k] = rate[k];

			if(i > iter/2){
				stop_all_0 = true;

				for(int j = 0; j < 5; j++)
					for(int k = 0; k < 3; k ++)
						if(abs(rate[k] - previous[j][k]) > pert){
							stop_all_0 = false;
						}
			}
			else{
				stop_all_0 = false;
			}
		}

		for (int j = 0; j < 3; j++)
			if(rate[j] - 0.00000001 >= 0 && rate[j] + 0.00000001 <= 1)
				stop_all = false;

		if(stop_all || stop_all_0)
			continue;

		if(grid && i % gap == 0){
			char path2[100];

			sprintf(path2,"T_%04d_W_%04d_G_%04d_U_%04d_S_%04d_i_%05d.dat", 
				(int)((T + 0.000001) * 100),
				(int)((W + 0.000001) * 100), (int)((g + 0.000001) * 100),
				(int)((U + 0.000001) * 100), (int)((-S + 0.000001) * 100),
			i);


			FILE *gfile = fopen(path2,"a+");

			for(int j = 0; j < LL; j++){
				fprintf(gfile, "%d", Strategy[j]);
			}
			fclose(gfile);

		}

		for(int j = 0; j < LL; j++){
			int x = rand() % LL;

			int y = Neighbour[x][rand() % 4];
			int a_rand = rand() % 100;
			if (a_rand == 0){
				int rand_stg = rand() % 3;
				Cate_Player[ Strategy[y] ] --;
				Strategy[y] = rand_stg;
				Cate_Player[ Strategy[y] ] ++;
			}

			if (Strategy[x] == Strategy[y])
				continue;

			double x_earn = centre_game(x);
			double y_earn = centre_game(y);

			if ((double)rand()/(double)RAND_MAX < 1.0/( 1.0 + exp((y_earn - x_earn)/K) ) ){
				Cate_Player[ Strategy[y] ] --;
				Strategy[y] = Strategy[x];
				Cate_Player[ Strategy[y] ] ++;
			}

		}

	}
	if(ptf)
		fclose(file);

	return 0;
}

