/////////////////////////////////////////////////////////////////////////////////////////////////
// 	global variables
//	product: XDJ-X
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////
static DMA2D_HandleTypeDef hDma2dHand;
uint8_t PART_CODE = 0; //part of code when run in main()						
								
#define dkA		0
#define dkB		1
uint8_t masterdeck = 0xFF;

/* SDRAM linker ---------------------------------------------------------*/
uint16_t fbuf[0x369C0] 	__attribute__((at(0xC0000000)));  //END: 0xC006D380
uint16_t PCM[2][272][8192][2]   __attribute__((at(0xC006D380))); //SIZE:8912896  END: 0xC116D380

//				   CIRCLE BUFF        CUE   HCUEA  HCUEB  HCUEC  HCUED  HCUEE  HCUEF  HCUEG  HCUEH
//		|..........128..........|..16..|..16..|..16..|..16..|..16..|..16..|..16..|..16..|..16..|

//memory map:
//
//		PCM[dkA][0][0][0] - PCM[128][0][0]	circle buffer (23,777 sec)
//		PCM[dkB][0][0][0] - PCM[128][0][0]	circle buffer (23,777 sec)
//
//
//
//
//
//
//
//
//

/* Display and static information variables ---------------------------------------------------------*/
#define LCD_FRAME_BUFFER    ((uint32_t)0xC0000000)
#define LCD_FRAME_BUFFER_2	((uint32_t)0xC00369C0)
uint32_t i = 0;
uint32_t a = 0;
uint32_t errs = 0;
#define BROWSER 			0
#define WAVEFORM 			1
#define BROWSER_INFO 	2
#define TAG_LIST			3
#define TAG_LIST_INFO 4
#define UTILITY				5
#define BROWSER_NAVI	6
#define BR_NAVI_END		7
uint8_t dSHOW = 0xFF;
uint8_t need_redraw_memline[2] = {0};

/* Display INFO variables ---------------------------------------------------------*/
uint8_t BROWSER_INFO_enable = 0;
uint8_t TAG_LIST_INFO_enable = 0;
#define LCD_COLOR_PAPER           	((uint32_t)0xFFFFFCE3)
#define LCD_COLOR_SHADOW           	((uint32_t)0xFFD4D4D4)

/* Display Browse ANIMATION variables ---------------------------------------------------------*/														 
uint32_t animation_time = 0;
uint8_t animation_en = 0;				//0 - none ro end, 1 - foward, 2 - back
uint16_t animation_step = 0; 														 
uint16_t previous_animation_step = 0; 
#define LCD_COLOR_PAPER_TRANSP           	((uint32_t)0xAFFFFCE3)
uint8_t animation_finish = 1;														 
uint8_t info_animation_enable = 0;	
uint8_t BROWSE_LEVEL = 0;			//2					//0 - tracklist, 1 - playlists, 2 - menu playlist, filename 3 - SD card information														 
uint8_t PREVIOUS_BROWSE_LEVEL = 0;			//
uint8_t CURRENT_LAY;

/* Display Browser variables ---------------------------------------------------------*/
uint8_t B0CurrentCursorPosition = 0;			//0...8 position
uint8_t B1CurrentCursorPosition = 0;		//0...8 position
uint8_t B2CurrentCursorPosition = 0;		//0...8 position
uint16_t BCurrentPlaylistPosition = 1;			//1....TOTAL_TRACKLISTS-8
uint16_t BCurrentTrackPosition = 1;			//1....TOTAL_TRACKS-8
uint8_t ScrollLong = 161;								//5...161
uint8_t ScrollPosition = 0;							//0...161-ScrollLong
#define LCD_COLOR_DGREEN           	((uint16_t)0x8320)
#define LCD_COLOR_LIGHT_1           ((uint16_t)0xEF7B)				//color for line-cursor browser
#define LCD_COLOR_LIGHT_2           ((uint16_t)0xF7BD)				//color for line-cursor browser
#define LCD_COLOR_LIGHT_3						((uint32_t)0xFFCFCFCF)				//color for secondary line-cursor UTILITY
#define LCD_COLOR_LIGHT_4						((uint32_t)0xFFAFAFAF)				//color for secondary line-cursor UTILITY
#define LCD_COLOR_LIGHT_5						((uint32_t)0xFFBFBFBF)				//color for secondary line-cursor UTILITY
#define LCD_COLOR_DARK_1           	((uint16_t)0xB18C)
#define LCD_COLOR_DARK_2           	((uint16_t)0xA108)
#define BROWSER0_DOWN		0
#define BROWSER0_UP			1
#define TAGLIST_DOWN		2
#define TAGLIST_UP			3
#define UTILITY_DOWN		4
#define UTILITY_UP			5
#define BROWSER1_DOWN		6
#define BROWSER1_UP			7
#define BROWSER2_DOWN		8
#define BROWSER2_UP			9

/* Display TAG LIST variables ---------------------------------------------------------*/
uint8_t TOTAL_TRACKS_IN_TAG_LIST = 0;					//0....100
uint16_t TAG_LIST_BASE[101] = {0};				//base tag list [track number]
uint8_t TCurrentCursorPosition = 0;			//0...7 position
uint8_t TCurrentTrackPosition = 1;			//1....TOTAL_TRACKS_IN_TAG_LIST-7
uint8_t s = 0;													//counter for write-delete-add tag track

/* FatFS ---------------------------------------------------------*/
unsigned int nbytes, nbytesb;
FRESULT res;
FIL file, fileb;
FILINFO finfo;
FATFS FAT;

uint8_t fat_semaphore = 0;

char path_pic[]="0:wall4.bmp";



#define SD_OPEN_DOOR			0
#define SD_SCAN_RB				1
#define SD_MOUNTED				2
#define SD_NEED_REPLACE		3
uint8_t SD_STATUS = SD_OPEN_DOOR;

uint8_t fill_stp[2] = {0};
uint16_t start_adata[2] = {0};							//filling adress in memory
uint16_t end_adata[2] = {0};								//filling adress in memory ()

static const uint16_t LCDPWM_MAP[5] = {100,	
													300,
													450,
													490,
													600};



/* EEPROM UTILITY variables ---------------------------------------------------------*/
//[0]PLAY MODE
//[1]LOAD LOCK
//[2]AUTO CUE MODE 		//AUTO CUE LEVEL:  0 -36dB; 1 -42dB; 2 -48dB; 3 -54dB; 4 -60dB; 5 -66dB; 6 -72dB; 7 -78dB; 8 - MEMORY; 9 - FIRST BEAT*;
//[3]QUANTIZE
//[4]TEMPO RANGE DEFAULT
//[5]COLOR  WAVEFORM
//[6]WAVEFORM CURRENT POSITION (CENTER, LEFT)
//[7]VINYL START
//[8]VINYL BRAKE
//[9]JOG ON AIR
//[10]JOG COLOR ON AIR
//[11]JOG COLOR INACTIVE
//[12]JOG BRIGHTNESS
//[13]LCD BRIGHTNESS
//[14]PADS MODE
//[15]FADER CURVE
//[16]EQ frequency LOW-MID
//[17]EQ frequency MID-HIGH
//[18]HEADPHONES CUE TYPE
#define ACUE	 2
#define QUANTIZE	 3
uint8_t UT_SET[19];


/* temp variables ---------------------------------------------------------*/
uint8_t VisibleLayer = 0;
uint8_t forcibly_redraw[2] = {0}; 
uint8_t REMAIN_ENABLE[2] = {1, 1};
uint8_t DynamicWaveformZOOM = 8;	//zoom parametr 1-2-4-8-16
uint32_t PreviousPositionDW[2] = {0};
uint8_t need_updt_dwf[2] = 0;

//original from dump cdj
static const uint16_t COLOR_MAP[2][8] = 
{
0x8993,
0x8A17,
0x865F,
0x8AFD,
0xBF1C,
0xCB1F,
0xEBBF,
0xFFFF,
0xEC00,	
0xDC89,
0xF52B,
0xFA8C,
0xD324,
0xAAE7,
0x9F59,
0x9E1B	
};


uint8_t Prev1m[2] = {0xFF, 0xFF};
uint8_t Prev10s[2] = {0xFF, 0xFF};
uint8_t Prev1s[2] = {0xFF, 0xFF};
uint8_t Prev10f[2] = {0xFF, 0xFF};
uint8_t Prev1f[2] = {0xFF, 0xFF};
uint8_t PrevHf[2] = {0xFF, 0xFF};
uint8_t Buf[64]={0};

uint32_t j = 0;

/* Display UTILITY variables ---------------------------------------------------------*/
uint8_t	countUTILITY = 0;
uint8_t edit_parameter = 0;								//flag in settings menu - entering to edit parameter	

/* Buttons variables ---------------------------------------------------------*/
uint8_t LOAD_BUTTON_pressed[2] = {0};
uint8_t KEY_BROWSE_pressed = 0;
uint8_t KEY_TAG_TRACK_pressed = 0;
uint8_t KEY_TAG_LIST_pressed = 0;
uint8_t KEY_INFO_pressed = 0;
uint8_t KEY_MENU_pressed = 0;
uint8_t KEY_BACK_pressed = 0;



/* ENCODER ---------------------------------------------------------*/
uint8_t ENC_BUTTON_pressed = 0;
uint8_t ENC_LED_BLINK[2] = {8, 8};
uint8_t enc_prev_dir = 0;
uint32_t enc_prev_time = 0;
uint8_t enc_need_up = 0;
uint8_t enc_need_down = 0;


/* deck transfer ---------------------------------------------------------*/
uint8_t deckRbuf[16] __attribute__((at(0x30000000)));
uint8_t deckTbuf[8][16] __attribute__((at(0x30000010)));
uint8_t TXpage = 0;									

uint8_t LOOP_LEDS_BLINK = 0;
uint8_t load_animation_en[2] = {0};
uint8_t PLAY_BTN_pressed[2] = {0};	
uint8_t CUE_BTN_pressed[2] = {0};
uint8_t JOG_MODE_BTN_pressed[2] = {0};
uint8_t TEMPO_BTN_pressed[2] = {0};
uint8_t SLIP_BTN_pressed[2] = {0};
uint8_t CALL_NEXT_BTN_pressed[2] = {0};
uint8_t CALL_PREV_BTN_pressed[2] = {0};
uint8_t RVRS_BTN_pressed[2] = {0};
uint8_t RELOOP_BTN_pressed[2] = {0};
uint8_t LP4_BTN_pressed[2] = {0};
uint8_t LP8_BTN_pressed[2] = {0};

uint8_t keep_to_play[2] = {0};
uint8_t inertial_rotation[2] = {0};				//inertial rotation for jog
uint8_t change_speed[2] = {0};							//flag for RELEASE/START or TOUCH/BREAKE 
#define NO_CHANGE	0
#define NEED_UP		1
#define NEED_DOWN	2
uint8_t keep_slip[2] = {0};
uint8_t SLIPEN[2] = {0};			//slip mode status replacing Tbuffer[19]&0x8
uint8_t RVRSEN[2] = {0};			//reverse mode status replacing Tbuffer[17]&0x20
uint8_t need_call_to_cue[2] = {0};
uint8_t JOG_PRESSED[2] = {0};

uint8_t LED_SD_timer = 0;
uint8_t timer_time = 0;
uint16_t acceleration_UP = 50;				//acceleration_UP for vinyl RELEASE/START
uint16_t acceleration_DOWN = 50;			//acceleration_DOWN for vinyl TOUCH/BREAKE
uint16_t potenciometr_tempo[2] = {0};
uint16_t prev_potenciometr_tempo[2] = {0};
uint8_t tempo_need_update[2] = {0};
uint8_t time_mode_need_update[2] = {0};
uint16_t pot10b[2];		//data from pitch pot in 10 bits format
uint32_t potwd[2];
 
uint8_t CUE_OPERATION[2] = {0};
#define CUE_NEED_SET 							1
#define CUE_NEED_CALL 						2
#define MEMORY_NEED_NEXT_SET 			3
#define MEMORY_NEED_PREVIOUS_SET 	4
#define MEMORY_NEED_SET_PART2 		5
uint8_t JJ;			//variable for internal calc
uint8_t RED_CRCL_CUE_ADR[2] = {85, 85};
uint8_t TIM_CUE_LED[2] = {0};
uint8_t TIM_PLAY_LED[2] = {0};

uint8_t tempo_range[2] = {1, 1};					//10% default
uint8_t tempo_range_need_update[2] = {0};
uint8_t quantize_mode_need_update = 1;
#define ROTENdkA 	((deckRbuf[2]&0x08)!=0)
#define ROTDISdkA 	((deckRbuf[2]&0x08)==0)
#define ROTENdkB 	((deckRbuf[10]&0x08)!=0)
#define ROTDISdkB 	((deckRbuf[10]&0x08)==0)


/* jog ring ---------------------------------------------------------*/
//WHITE 
//PINK 
//RED 
//ORANGE 
//YELLOW 
//GREEN 
//AQUA 
//BLUE 
//PURPLE 
static const uint32_t jog_color[2][9] = {
0xFFB2B27A,
0xFFB2003D,
0xFFB20000,
0xFFB23300,
0xFFB28800,
0xFF00B200,
0xFF00A7B2,
0xFF0000B2,
0xFF2800B2,
0xFFFFFFAF,
0xFFFF0057,
0xFFFF0000,
0xFFFF3700,
0xFFFFC300,
0xFF00FF00,
0xFF00C1FF,
0xFF0000FF,
0xFF3A00FF};

uint8_t curr_trck_color[2] = {0};
uint32_t jog_work_color[2] = {0};
uint8_t prev_inair[2] = {0xFF, 0xFF};

/* mixer uart variables ---------------------------------------------------------*/
uint32_t mxr_usart_timeout = 0;
uint32_t mxr_current_time;
uint8_t mxr_usart_data_cnt = 0;
uint8_t mxr_new_data = 0;
uint8_t mxr_urx_buf[8];
uint8_t mxrURX;

/* power manager uart variables ---------------------------------------------------------*/
uint32_t pm_usart_timeout = 0;
uint32_t pm_current_time;
uint8_t pm_usart_data_cnt = 0;
uint8_t pm_new_data = 0;
uint8_t pm_urx_buf[2];
uint8_t pmURX;

/* Batt icon ---------------------------------------------------------*/
uint8_t batt_status = 0xFF;
uint8_t batt_prcnt = 0xFF;
uint8_t batt_need_update = 0;


/* debug uart variables ---------------------------------------------------------*/
char U_TX_DATA[64] = {0};	
uint32_t dbg_usart_timeout = 0;
uint32_t dbg_current_time;
uint8_t dbg_usart_data_cnt = 0;
uint8_t dbg_new_data = 0;
uint8_t dbg_urx_buf[16];
uint8_t dbgURX;

/* REKORDBOX DATABASE variables ---------------------------------------------------------*/
uint32_t BEATGRID[2][2048];					// BEATGRID (0, 3, 7... ) in ms
uint16_t BPMGRID[2][2048];					// bpmgrid BPM*100 (204,8 bpm max for 10min track)
uint8_t GRID_OFFSET[2] = {1, 1};		//ofsset grid 1.2.3.4
#define WFD_SIZE	90000
uint8_t WFORMDYNAMIC[2][WFD_SIZE];


uint8_t artist_n[2][32];
uint8_t track_n[2][32];
uint8_t playlist[512][55]; 	//		//54byte - status byte
uint16_t original_tempo[512];
uint16_t rating[512];		//rating and COLOR tracks	
uint16_t duration[512];	//
uint8_t key_id[512];		//
uint32_t parcser_adress[512];
char SDCARD_NAME[20] = "SD CARD";
char SD_DATE[15] = "03-10-2025";
uint16_t TOTAL_TRACKS = 0;
uint16_t TOTAL_TRACKS_IN_CURRENT_PLAYLIST = 31;		//need add code for write value
uint16_t TRACK_PLAY_IN_CURRENT_PLAYLIST = 0;
uint16_t track_play_now[2] = {0};
uint8_t need_load_track[2] = {0};		
uint8_t TRACK_n_PLAY[2] = {0};


char path_export[]="0:/PIONEER/rekordbox/export.pdb";
char KEYS[25][4];				//strings
uint8_t KEYSmatchtable[25];  //matchisng strings to real KEY
const char KEY_MATCH[24][3] ={'A', 'b', 'm',
										 		 			'A', 'm', 0,
															'B', 'b', 'm',
															'B', 'm', 0,
															'C', 'm', 0,
															'D', 'b', 'm',
															'D', 'm', 0,
															'E', 'b', 'm',
															'E', 'm', 0,
															'F', 'm', 0,
															'F', '#', 'm',
															'G', 'm', 0,
															'B', 0, 0,
															'C', 0, 0,
															'D', 'b', 0,
															'D', 0, 0,
															'E', 'b', 0,
															'E', 0, 0,
															'F', 0, 0,
															'F', '#', 0,
															'G', 0, 0,
															'A', 'b', 0,
															'A', 0, 0,
															'B', 'b', 0};

const uint8_t KEY_FONT_W[25] = {213,			//Abm
																217,			//Am
																213,			//Bbm
																217,			//Bm
																217,			//Cm
																213,			//Dbm
																217,			//Dm
																214,			//Ebm
																217,			//Em
																217,			//Fm
																214,			//F#m
																217,			//Gm
																221,			//B
																221,			//C
																218,			//Db
																221,			//D
																218,			//Eb
																222,			//E
																222,			//F
																219,			//F#
																221,			//G
																218,			//Ab
																221,			//A
																218,			//Bb															
																213};			//another key

const uint32_t COLOR_MAP_RATING[9] = 
{
0xFF404040,
0xFFF870F8,
0xFFF80000,
0xFFF8A030,
0xFFF8E330,
0xFF00E000,
0xFF00C0F8,
0xFF0050F8,
0xFF9808F8};

uint16_t TRACKS_DATABASE[1024];		//database track ID [playlist 1[ID][ID][ID]][playlist 2[ID][ID][ID]][playlist 3[ID][ID][ID]]....
uint8_t TRACKLIST_NAME[20][21];		//20 tracklists max and 20 lengt
uint16_t TRACKLIST_OFFSET[20];		//offset for TRACKS_DATABASE
uint8_t TOTAL_TRACKLISTS = 0;			//maximum 20 tracklists
uint16_t originalBPM[2] = {0xFFFF, 0xFFFF};						//this original BPM*100 of track (pitch = 0.00%) 
uint16_t mixerBPM = 0xFFFF;
uint32_t beat_diff;
uint32_t beat_pitch;

uint32_t posa, posb, plaa, plab;

/* PADS variables ---------------------------------------------------------*/
uint8_t HCUEPCOLOR[2][8][3];  //pads color [deck][pad][rgb]
static const uint8_t HCUEdisableCOLOR[3] = {0x03, 0x03, 0x02};


/* HOT CUES and MEMORY variables ---------------------------------------------------------*/
#define NONE_MARK				0
#define MEMORY_MARK			1

uint32_t HCUE32COLOR[2][8] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
															0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
uint16_t HCUE16COLOR[2][8];
uint32_t HCUE_adr[2][2][8] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
															0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
															0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
															0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}; 
													 //[0] => HCUE in 0.5*frames [1] => loop end in 0.5*frames   150 on 1 sec;  				
uint32_t MEMORY_adr[2][2][10] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
																 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
																 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
																 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
//[0] => MEMORY CUE in 0.5*frames [1] =>  MEMORY loop end in 0.5*frames   150 on 1 sec;
															
uint8_t	HCUE_type[2][8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
													 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};	 			//b0 (0=cue / 1=loop); b1 (0=inactive / 1=active); 													
uint8_t	MEMORY_type[2][10] = {0};		//b0 (0=cue / 1=loop); b1 (0=inactive / 1=active); 
uint8_t number_of_hot_cue_points[2] = {0};
uint8_t number_of_memory_cue_points[2] = {0};


/* Static Waveform variables ---------------------------------------------------------*/
uint16_t prevTpos[2] = {0};
uint8_t WFORMSTATIC[2][203];
#define DRAW_NEW_STATIC_WAVEFORM		203
#define CLEAR_WAVEFORM_ARRAY				204
#define MS_NOT_LOADED								205
#define REDRAW_IN_NREMAIN_MODE			206
#define REDRAW_IN_REMAIN_MODE				207
#define MS_ERROR										210
uint8_t DRAWN_IN_REMAIN[2] = {0};
uint8_t need_DSW[2] = {0};						//flag for redraw progressbar remain or nrmain mode when track long 30s
uint8_t jog_light[2] = {1, 1};
#define PBAR_COLOR_1				((uint16_t)0xE318)
#define PBAR_COLOR_2				((uint16_t)0xB18D)
#define PBAR_COLOR_3				((uint16_t)0xA109)
const static uint16_t WS_COLOR_MAP[2] = 						//colors for static waveforms
{
0xADF4,
0xCAFC		
};
uint8_t RED_VERTICAL_LINE[2] = {0};

/* Dynamic Waveform variables ---------------------------------------------------------*/
uint16_t bars[2] = 0;	
uint16_t previous_bars[2] = {0};
uint8_t prev_phase_pos = 77;		//for sync phase bar

/////temp
uint32_t frametime;

#define TEST_H   	GPIOH->BSRR = 0x00000400	//HAL_GPIO_WritePin(GPIOH, LED_MENU_Pin, GPIO_PIN_SET)
#define TEST_L		GPIOH->BSRR = 0x04000000	//HAL_GPIO_WritePin(GPIOH, LED_MENU_Pin, GPIO_PIN_RESET)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				
																																									