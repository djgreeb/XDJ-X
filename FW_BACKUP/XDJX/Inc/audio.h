/////////////////////////////////////////////////////////////////////////////////////////////////
// Audio processing variables
//	product: XDJ-X
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdint.h"

uint32_t play_adr[2] = {0};						//Playing adress in samples (44100 per second)
uint32_t slip_pl_adr[2] = {0};				//Playing adress for SLIP MODE in samples (44100 per second)
uint32_t all_long[2] = {0};						//all long of Track in 0.5*frames   1/150 sec
uint8_t rvrs[2] = {0};
uint16_t pitch[2] = {10000, 10000};	// 10000 = 100% step 0,01%			
uint16_t prev_pitch[2] = {0};
uint32_t postn[2] = {0};
uint32_t slip_postn[2] = {0};
uint16_t slip_pitch[2] = {10000, 10000};	// 10000 = 100% step 0,01%	
uint8_t step_postn[2] = {0};
float SAMPLE_BUFFER;
float T[2];
uint8_t	end_track[2] = {0};				//end track flag
uint8_t loop_active[2] = {0};			//loop flag
uint8_t loop_act_gui[2] = {0};			//loop flag for gui icon
uint32_t LOOP_OUT[2] = {0};				//adr LOOP OUT in frames 150
uint8_t loop_lenght[2] = {0xFF, 0xFF};			//loop divider 1/64....64  9=>8 beats (0xFF = none)
uint8_t loopmode_need_update[2] = {0};
uint8_t	need_jump_addr[2] = {0};						//for jupmping between loop mem buf and cue buff

static const uint32_t loop150[13] = {	14062,			//1/64
																			28125,			//1/32
																			56250,			//1/16
																			112500,			//1/8
																			225000,			//1/4
																			450000,			//1/2
																			900000,			//1
																			1800000,		//2
																			3600000,		//4
																			7200000,		//8
																			14400000,		//16
																			28800000,		//32
																			57600000		//64
																				};

uint8_t lock_control[2] = {1, 1};			//lock buttons, when track not loading or in process

int16_t LR[2][2][4];

int32_t even1, even2, odd1, odd2;
float c0r[2], c1r[2], c2r[2], c3r[2], r0r[2], r1r[2], r2r[2], r3r[2];
float c0l[2], c1l[2], c2l[2], c3l[2], r0l[2], r1l[2], r2l[2], r3l[2];
float PRETRIM[2], TRIM[2];

static float COEF[8] = {			//////optimal 32x, 4-point, 3rd-order optimal
0.464656,
0.035344,
0.537268,
0.154244,
-0.249982, 
0.249982,
-0.493696,
0.164559
};

////////////////////////////////////////////////
//Filter type: High Pass
//Filter model: Butterworth
//Filter order: 4
//Sampling Frequency: 44 KHz
//Cut Frequency: 8.100000 KHz

static float FIRCoef[2] =
	{
	0.71889,
	-0.28111	
	};
	
static float firR[2][2]; //input samples	
static float firL[2][2]; //input samples
static float deckout[2][2];	
	
uint32_t CUE_ADR[2] = {0};						//REAL CUE adr in frames 150
uint32_t offset_adressBIG[2] = {0};		//address offset for calling CUE audio data (for work)	in samples
uint8_t offset_adress[2] = {0};				//address offset for calling CUE audio data (for work)
uint8_t mem_offset_adress[2] = {0};		//address offset for calling CUE audio data (for memory)
uint32_t sdram_adr[2] = {0};
uint8_t slip_pl_enable[2] = {0};
uint8_t play_enable[2] = {0};							//allow [pitch] variable to change


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
