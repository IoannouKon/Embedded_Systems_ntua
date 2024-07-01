#include <math.h>

#include "calcDist.h"

void compute_dist1(DTYPE1 data_hw_tmp1[MOVIES_NUM/2][USERS_NUM],float dists_hw_tmp1[MOVIES_NUM/2],int start,int N)
{
	DTYPE1  movie_tmp1[USERS_NUM];
	int i,j;
	LOAD_MOVIE_TMP:
		for ( i = 0; i < USERS_NUM; i++){
			#pragma HLS unroll
			movie_tmp1[i] = data_hw_tmp1[MOVIE_ID][i];
		}

		DTYPE1 sum, diff;
	COMPUTE_DISTS:
		for (i = start; i < MOVIES_NUM/N +start; i++) {
			sum = (DTYPE1)0.0;
			diff = (DTYPE1)0.0;
			for (j = 0; j < USERS_NUM; j++) {
				#pragma HLS unroll factor = 16
				diff = (data_hw_tmp1[i][j] > movie_tmp1[j]) ? data_hw_tmp1[i][j] - movie_tmp1[j] : movie_tmp1[j] - data_hw_tmp1[i][j];
				sum += diff * diff;
			}
			dists_hw_tmp1[i] = sqrt(sum.to_float());
		}
}

void calcDistancesHW(float* data_hw, float* dists_hw)
{
	#pragma HLS loop_merge
	DTYPE1 data_hw_tmp1[MOVIES_NUM/2][USERS_NUM],data_hw_tmp2[MOVIES_NUM/2][USERS_NUM];
	float dists_hw_tmp1[MOVIES_NUM/2],dists_hw_tmp2[MOVIES_NUM/2];

	int i, j;

LOAD_DATA_HW_TMP:
	for (i = 0; i < MOVIES_NUM; i++) {
		for (j = 0; j < USERS_NUM; j++) {
	   #pragma HLS unroll  factor= 16
			if(i<MOVIES_NUM/2)
				data_hw_tmp1[i][j] = data_hw[i * USERS_NUM + j];
			else
				data_hw_tmp2[i-(MOVIES_NUM/2)][j] = data_hw[i * USERS_NUM + j];
		}
	}

	COMPUTE_DISTS:
	#pragma HLS dataflow
	{
	 compute_dist1( data_hw_tmp1,  dists_hw_tmp1,0,2);
	 compute_dist1( data_hw_tmp2,  dists_hw_tmp2,MOVIES_NUM/2,2);
	}

	WRITE_DISTS:
		for (int i = 0; i < MOVIES_NUM; i++) {
	        #pragma HLS unroll
			if(i<MOVIES_NUM/2)
			dists_hw[i] = dists_hw_tmp1[i];
			else
			dists_hw[i] = dists_hw_tmp2[i -(MOVIES_NUM/2)];
		}

}
