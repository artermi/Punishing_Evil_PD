#include <stdio.h>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <string>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>


#define K 1.2
#define L 1000
#define LL (L*L)
#define ITER 8001
#define GAP 100
#define PERT 0.02

class Evil_PD{
public:

	double T;
	double W;
	double g;
	double U;
	double S;
	int prob_rand;

	bool grid;

	char dir_name[100];

	int Cate_Player[3];
	double payoff_matrix[3][3];
	
	int *Strategy;
	int **Neighbour;

	Evil_PD(const double t, const double w, const double G, const double u,const double s,
	const bool prep = false,const bool Grid = false, const int pp = RAND_MAX);

	~Evil_PD();
	//double unit_game(const int cent,const int target);
	double centre_game(const int cent);
	int game(bool ptf, bool avg = false);
	int game2(bool ptf);
	int calc_avg();
};
