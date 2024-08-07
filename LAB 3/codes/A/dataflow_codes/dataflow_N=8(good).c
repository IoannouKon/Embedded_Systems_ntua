#include <math.h>
#include "calcDist.h"

void compute_dist(float data_hw_tmp1[MOVIES_NUM/8][USERS_NUM],float dists_hw_tmp1[MOVIES_NUM/8],int start,int N)
{
	float movie_tmp1[USERS_NUM];
	LOAD_MOVIE_TMP:
		for (int i = 0; i < USERS_NUM ; i++){
	        #pragma HLS unroll
			movie_tmp1[i] = data_hw_tmp1[MOVIE_ID][i];
		}

	for (int i = start; i < MOVIES_NUM/N + start; i++) {
		float sum = 0.0, diff = 0.0;

		for (int j = 0; j < USERS_NUM; j++){
          #pragma HLS unroll
			diff = data_hw_tmp1[i][j] - movie_tmp1[j];
		}
			sum += diff * diff;
        	dists_hw_tmp1[i] = sqrt(sum);
	}
}



void calcDistancesHW(float* data_hw, float* dists_hw)
{
	#pragma HLS loop_merge

	float data_hw_tmp1[MOVIES_NUM/8][USERS_NUM],data_hw_tmp2[MOVIES_NUM/8][USERS_NUM];
	float data_hw_tmp3[MOVIES_NUM/8][USERS_NUM],data_hw_tmp4[MOVIES_NUM/8][USERS_NUM];
	float data_hw_tmp5[MOVIES_NUM/8][USERS_NUM],data_hw_tmp6[MOVIES_NUM/8][USERS_NUM];
	float data_hw_tmp8[MOVIES_NUM/8][USERS_NUM],data_hw_tmp7[MOVIES_NUM/8][USERS_NUM];

	float dists_hw_tmp1[MOVIES_NUM/8],dists_hw_tmp2[MOVIES_NUM/8];
	float dists_hw_tmp3[MOVIES_NUM/8],dists_hw_tmp4[MOVIES_NUM/8];
	float dists_hw_tmp5[MOVIES_NUM/8],dists_hw_tmp6[MOVIES_NUM/8];
	float dists_hw_tmp7[MOVIES_NUM/8],dists_hw_tmp8[MOVIES_NUM/8];

	LOAD_DATA_HW_TMP:
	for (int i = 0; i < MOVIES_NUM; i++) {
	    for (int j = 0; j < USERS_NUM; j++) {
	        #pragma HLS unroll
	        int dest_i = i % (MOVIES_NUM/8);

	        if (i < MOVIES_NUM/8)
	            data_hw_tmp1[dest_i][j] = data_hw[i * USERS_NUM + j];
	        else if (i < 2 * MOVIES_NUM/8)
	            data_hw_tmp2[dest_i][j] = data_hw[i * USERS_NUM + j];
	        else if (i < 3 * MOVIES_NUM/8)
	            data_hw_tmp3[dest_i][j] = data_hw[i * USERS_NUM + j];
	        else if (i < 4 * MOVIES_NUM/8)
	            data_hw_tmp4[dest_i][j] = data_hw[i * USERS_NUM + j];
	        else if (i < 5 * MOVIES_NUM/8)
	            data_hw_tmp5[dest_i][j] = data_hw[i * USERS_NUM + j];
	        else if (i < 6 * MOVIES_NUM/8)
	            data_hw_tmp6[dest_i][j] = data_hw[i * USERS_NUM + j];
	        else if (i < 7 * MOVIES_NUM/8)
	            data_hw_tmp7[dest_i][j] = data_hw[i * USERS_NUM + j];
	        else
	            data_hw_tmp8[dest_i][j] = data_hw[i * USERS_NUM + j];
	    }
	}


COMPUTE_DISTS:
#pragma HLS dataflow
{
 compute_dist( data_hw_tmp1,  dists_hw_tmp1,0,8);
 compute_dist( data_hw_tmp2,  dists_hw_tmp2,MOVIES_NUM/8,8);
 compute_dist( data_hw_tmp3,  dists_hw_tmp3,2*MOVIES_NUM/8,8);
 compute_dist( data_hw_tmp4,  dists_hw_tmp4,3*MOVIES_NUM/8,8);
 compute_dist( data_hw_tmp5,  dists_hw_tmp5,4*MOVIES_NUM/8,8);
 compute_dist( data_hw_tmp6,  dists_hw_tmp6,5*MOVIES_NUM/8,8);
 compute_dist( data_hw_tmp7,  dists_hw_tmp7,6*MOVIES_NUM/8,8);
 compute_dist( data_hw_tmp8,  dists_hw_tmp8,7*MOVIES_NUM/8,8);
}

WRITE_DISTS:
for (int i = 0; i < MOVIES_NUM; i++) {
    #pragma HLS unroll
    int dest_i = i % (MOVIES_NUM/8);

    if (i < MOVIES_NUM/8)
        dists_hw[i] = dists_hw_tmp1[dest_i];
    else if (i < 2 * MOVIES_NUM/8)
        dists_hw[i] = dists_hw_tmp2[dest_i];
    else if (i < 3 * MOVIES_NUM/8)
        dists_hw[i] = dists_hw_tmp3[dest_i];
    else if (i < 4 * MOVIES_NUM/8)
        dists_hw[i] = dists_hw_tmp4[dest_i];
    else if (i < 5 * MOVIES_NUM/8)
        dists_hw[i] = dists_hw_tmp5[dest_i];
    else if (i < 6 * MOVIES_NUM/8)
        dists_hw[i] = dists_hw_tmp6[dest_i];
    else if (i < 7 * MOVIES_NUM/8)
        dists_hw[i] = dists_hw_tmp7[dest_i];
    else
        dists_hw[i] = dists_hw_tmp8[dest_i];
}


}



