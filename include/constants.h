#ifndef CONSTANTS
#define CONSTANTS

#define N_SAMPLES 20// This implies 2^20 samples
#define ITER_SYNCHRONOUS 1<<14
#define ITER_ASYNCHRONOUS 5000
#define SYNCHRONOUS_CYCLE_MAX_LENGTH 4
#define LOWER_EXPRESSION -1 //Defines the lower expression of a node (-1 implies that state can be -1,1; if 0, state can be 0,1)
#if LOWER_EXPRESSION == -1
#define NORMAL_FLAG 0
#elif LOWER_EXPRESSION == 0
#define NORMAL_FLAG -1
#endif
#endif
