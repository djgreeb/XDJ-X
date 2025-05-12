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

uint32_t play_adr = 0;						//Playing adress in samples (44100 per second)
uint32_t slip_play_adr = 0;				//Playing adress for SLIP MODE in samples (44100 per second)
uint32_t all_long = 0;						//all long of Track in 0.5*frames   150 on 1 sec
uint8_t reverse = 0;
uint16_t pitch = 10000;	// 10000 = 100% step 0,01%			
uint16_t previous_pitch = 0;
uint32_t position = 0;
uint32_t slip_position = 0;
uint16_t pitch_for_slip = 10000;	// 10000 = 100% step 0,01%	
uint8_t step_position = 0;
float SAMPLE_BUFFER;
float T;
uint8_t QUANTIZE = 1;					//QUANTIZE ENABLE
uint8_t	end_of_track = 0;			//end track flag
uint8_t loop_active = 0;			//loop flag
uint32_t LOOP_OUT = 0;				//adr LOOP OUT in frames 150
uint8_t lock_control = 1;			//lock buttons, when track not loading or in process

int16_t LR[2][4];
float c0r, c1r, c2r, c3r, r0r, r1r, r2r, r3r;
int32_t even1r, even2r, odd1r, odd2r;
float c0l, c1l, c2l, c3l, r0l, r1l, r2l, r3l;
int32_t even1l, even2l, odd1l, odd2l;
float PRETRIM, TRIM;

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
//Cut Frequency: 7.900000 KHz
static float FIRCoef[2] =
	{
	0.724370,
	-0.275630	
	};
static float firR[2]; //input samples	
static float firL[2]; //input samples

static float deckAout[2];	
	
uint32_t CUE_ADR = 0;					//REAL CUE adr in frames 150
uint8_t offset_adress = 0;			//address offset for calling CUE audio data (for work)
uint8_t mem_offset_adress = 0;	//address offset for calling CUE audio data (for memory)
uint32_t sdram_adr = 0;
uint8_t slip_play_enable = 0;
uint8_t play_enable = 0;						//allow [pitch] variable to change


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



