#include <stdlib.h>
#include <ap_fixed.h>

#define MOVIES_NUM 1024
#define USERS_NUM 32

#define ITERATIONS 1024

#define MOVIE_ID 0

#define INT_BITS 10
#define DEC_BITS 2
typedef ap_ufixed<INT_BITS + DEC_BITS, INT_BITS, AP_RND> DTYPE1;

//#define INT_BITS_ 3 //16
//#define DEC_BITS_ 1//8
//typedef ap_ufixed<INT_BITS_ + DEC_BITS_, INT_BITS_, AP_RND> DTYPE2;

#pragma SDS data copy(data_hw[0:32768], dists_hw[0:32])
#pragma SDS data access_pattern(data_hw:SEQUENTIAL, dists_hw:SEQUENTIAL)
void calcDistancesHW(float* data_hw, float* dists_hw);
