/////////////////////////////////////////////////////////////////////////////////////////////////
// 	global variables
//	product: XDJ-X mixer assy
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

					

/* BLUETOOTH ---------------------------------------------------------*/
#define BT_POW_ON			GPIOE->BSRR = 0x00001000		//HAL_GPIO_WritePin(GPIOE, BT_PON_Pin, GPIO_PIN_SET)
#define BT_POW_OFF		GPIOE->BSRR = 0x10000000		//HAL_GPIO_WritePin(GPIOE, BT_PON_Pin, GPIO_PIN_RESET)
#define BT_LED_ON			GPIOE->BSRR = 0x00000100		//HAL_GPIO_WritePin(GPIOE, BT_bLED_Pin, GPIO_PIN_SET)
#define BT_LED_OFF		GPIOE->BSRR = 0x01000000		//HAL_GPIO_WritePin(GPIOE, BT_bLED_Pin, GPIO_PIN_RESET)
#define BT_UNPRESS 		GPIOA->BSRR = 0x00010000		//HAL_GPIO_WritePin(BT_BUTT_GPIO_Port, BT_BUTT_Pin, GPIO_PIN_RESET);
#define BT_PRESS 			GPIOA->BSRR = 0x00000001		//HAL_GPIO_WritePin(BT_BUTT_GPIO_Port, BT_BUTT_Pin, GPIO_PIN_SET);
uint8_t BT_ENABLE = 0;
uint8_t BT_ACTIVATION_PRC = 0;
uint8_t need_draw_bticon = 0;
uint8_t need_draw_chsel = 0;
uint8_t need_draw_fxsel = 0;


/* MAX7219 ---------------------------------------------------------*/
uint8_t MAX7219_DATA[8] = {0};
uint8_t MAX7219_need_update = 0;
uint8_t prev_lvl[4] = {0xFF, 0xFF, 0xFF, 0xFF};
uint8_t prev_pklvl[4] = {0xFF, 0xFF, 0xFF, 0xFF};

const static uint16_t dBConv[12]={
32766,		//over(+12)		0				100.00%				
23195,		//+9					-3			70.79%
16422,		//+6					-6			50.12%
11625,		//+3					-9			35.48%
8230,			//0						-12			25.12%
5826,			//-3					-15			17.78%
4125,			//-6					-18			12.59%	
2919,			//-9					-21			8.91%
2067,			//-12					-24			6.31%
1464,			//-15					-27			4.47%
1035,			//-18					-30			3.16%
518				//-24					-36			1.58%
};


/* TFT ---------------------------------------------------------*/
uint8_t previous_ch = 0xFF;
uint16_t BPM_link = 0;				//BPM*100
uint16_t prev_BPM_link = 0;		//BPM*100
uint8_t prev_rngdiv = 0xFF;		//for gui func
uint8_t need_draw_ms = 0;

/* BUTTONS ---------------------------------------------------------*/
#define MUX_ADDR0		GPIOC->BSRR = 0x00070000	//000	
#define MUX_ADDR1		GPIOC->BSRR = 0x00030004	//100
#define MUX_ADDR2		GPIOC->BSRR = 0x00050002	//010
#define MUX_ADDR3		GPIOC->BSRR = 0x00010006	//110
#define MUX_ADDR4		GPIOC->BSRR = 0x00060001	//001
#define MUX_ADDR5		GPIOC->BSRR = 0x00020005	//101
#define MUX_ADDR6		GPIOC->BSRR = 0x00040003	//011
#define MUX_ADDR7		GPIOC->BSRR = 0x00000007	//111

uint8_t CFX1_prsd = 0;
uint8_t CFX2_prsd = 0;
uint8_t CFX3_prsd = 0;
uint8_t CFX4_prsd = 0;
uint8_t CFX5_prsd = 0;
uint8_t CFX6_prsd = 0;
uint8_t CFX7_prsd = 0;
uint8_t CFX8_prsd = 0;
uint8_t BFXn_previous = 0xFF;
uint8_t BFXn = 0xFF;
uint8_t CHSEL_previous = 0xFF;
uint8_t CHSEL = 0xFF;
uint8_t EQCURVE_prsd = 0;
uint8_t CRSFCURVE_scan = 0;
uint8_t CRSFCURVE = 0;		
uint8_t BEATm_prsd = 0;
uint8_t BEATp_prsd = 0;
uint8_t BFXON_prsd = 0;
uint8_t CUE1_prsd = 0;
uint8_t CUE2_prsd = 0;
uint8_t BT_bBUTT_prsd = 0; 
uint8_t need_beat_change = 0;   //1++; 2--


const static uint8_t CFXmask[8] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};
uint8_t CFXON = 0;				//0 - disable, 1...8 - run effect
uint8_t prev_CFXON = 0xFF;
uint8_t BFXON = 0;		 		//0 - disable, 1 - run effect
uint8_t CUE1ON = 0;
uint8_t CUE2ON = 0;

/* Timer 2 (main) ---------------------------------------------------------*/
uint8_t tim_dv = 0;
uint8_t tim_dv2 = 0;		//internal divider for slow blinking
uint8_t tim_dv3 = 0;		//internal divider for very slow friquency blinking
uint8_t blink_sync = 0;	//internal divider for very slow friquency blinking

/* ADCs ---------------------------------------------------------*/
#define FDR0	13		
#define FDR1	12
#define MID0	11	
#define MID1	10
#define TRM0	15
#define TRM1	14
#define PRMT	0
#define HPHN	1
#define HI0		3
#define HI1		2
#define LOW0	7	
#define LOW1	6
#define CFX0	5
#define CFX1	4
#define MIXN	9
#define DEPT	8

uint16_t pot_ADC[16][8] = {0};
uint32_t pot_SUM[16] = {0};
uint16_t pot_out[16];					//potenciometer value after gysteresis 0..4095 approx
uint8_t pot_8b[16] = {0};		//potenciometer value (after conversion 4095 to 255) 0..255 approx
uint8_t prev_pot_8b_CFX0;
uint8_t prev_pot_8b_CFX1;
uint8_t prev_pot_8b_PRMT;
uint8_t prev_pot_8b_DEPT;
uint32_t ADC_TMP;
uint8_t cnt_ad = 0;
uint16_t ADC_prev;		//temp
#define adc_hysteresis	6			//7 stable for mediannyj filter; 7.5 for slip aver;
#define adc_hysteresis2	12

#define POTL	90						//for conversion 4095 to 255
#define POTCL	1989					//
#define POTCH	2189					//
#define POTH	4057					//	
#define POTxLN	264				//
#define POTxCD1	275				//				
#define POTxCD2	279				//

/* CROSSFADER ---------------------------------------------------------*/
uint8_t CRSF_midi = 63;

/* LINK UART5 ---------------------------------------------------------*/
uint32_t link_usart_timeout = 0;
uint32_t link_current_time;
uint8_t link_usart_data_cnt = 0;
uint8_t link_new_data = 0;
uint8_t link_urx_buf[16];
uint8_t linkURX;
uint8_t link_utx_buf[8];
uint8_t decka_inair = 0xFF;
uint8_t deckb_inair = 0xFF;
uint8_t need_send_inair = 0;




/* TEMPORARY ---------------------------------------------------------*/
uint32_t temp_time;
uint8_t divdr;
uint8_t lay = 0;
uint8_t STR_BUFF[64] = {0};

uint8_t B = 0;
uint8_t last_bit_time = 0xFF;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
								

