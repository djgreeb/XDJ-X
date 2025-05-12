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
					
/* SDRAM linker ---------------------------------------------------------*/
uint16_t fbuf[0x369C0] 	__attribute__((at(0xC0000000)));  //END: 0xC006D380
uint16_t PCM[206][8192][2]   __attribute__((at(0xC006D380))); //END: 0xC06DD380

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
uint8_t dSHOW = WAVEFORM;

/* FatFS ---------------------------------------------------------*/
unsigned int nbytes;
FRESULT res;
FIL file;
FILINFO finfo;
FATFS FAT;
char path_pic[]="0:wall4.bmp";

#define SD_OPEN_DOOR			0
#define SD_SCAN_RB				1
#define SD_MOUNTED				2
#define SD_NEED_REPLACE		3
uint8_t SD_STATUS = SD_OPEN_DOOR;

uint8_t filling_step = 0;
uint16_t start_adr_valid_data = 0;							//filling adress in memory
uint16_t end_adr_valid_data = 0;								//filling adress in memory ()





/* temp variables ---------------------------------------------------------*/
uint8_t VisibleLayer = 0;
uint8_t forcibly_redraw = 0; 
uint8_t REMAIN_ENABLE = 1;
uint8_t DynamicWaveformZOOM = 4;	//zoom parametr 1-2-4-8-16

uint32_t PreviousPositionDW = 0;
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

#define CUE_COLOR						0xFA07

uint8_t Prev10m = 0xFF;
uint8_t Prev1m = 0xFF;
uint8_t Prev10s = 0xFF;
uint8_t Prev1s = 0xFF;
uint8_t Prev10f = 0xFF;
uint8_t Prev1f = 0xFF;
uint8_t PrevHf = 0xFF;
uint8_t Buf[64]={0};

uint32_t j = 0;


/* deck transfer ---------------------------------------------------------*/
uint8_t deckRbuf[16] __attribute__((at(0x30000000)));
uint8_t deckTbuf[8][16] __attribute__((at(0x30000010)));
uint8_t TXpage = 0;									

uint8_t load_animation_enable = 0;
uint8_t PLAY_BUTTON_pressed = 0;	
uint8_t CUE_BUTTON_pressed = 0;
uint8_t JOG_MODE_BUTTON_pressed = 0;
uint8_t TEMPO_BUTTON_pressed = 0;
uint8_t SLIP_BUTTON_pressed = 0;
uint8_t CALL_NEXT_BUTTON_pressed = 0;
uint8_t CALL_PREVIOUS_BUTTON_pressed = 0;
uint8_t REVERSE_BUTTON_pressed = 0;
uint8_t RELOOP_BUTTON_pressed = 0;


uint8_t keep_to_play = 0;
uint8_t inertial_rotation = 0;				//inertial rotation for jog
uint8_t change_speed = 0;							//flag for RELEASE/START or TOUCH/BREAKE 
#define NO_CHANGE	0
#define NEED_UP		1
#define NEED_DOWN	2
uint8_t keep_slip = 0;
uint8_t SLIPEN = 0;			//slip mode status replacing Tbuffer[19]&0x8
uint8_t REVEN = 0;			//reverse mode status replacing Tbuffer[17]&0x20
uint8_t need_call_to_cue = 0;
uint8_t JOG_PRESSED = 0;

uint8_t LED_SD_timer = 0;
uint8_t timer_time = 0;
uint16_t acceleration_UP = 100;				//acceleration_UP for vinyl RELEASE/START
uint16_t acceleration_DOWN = 100;			//acceleration_DOWN for vinyl TOUCH/BREAKE
uint16_t potenciometer_tempo = 0;
uint16_t previous_potenciometer_tempo = 0;
uint8_t tempo_need_update = 0;
uint16_t pot10b;		//data from pitch pot in 10 bits format
uint32_t potwd;
 
uint8_t CUE_OPERATION = 0;
#define CUE_NEED_SET 							1
#define CUE_NEED_CALL 						2
#define MEMORY_NEED_NEXT_SET 			3
#define MEMORY_NEED_PREVIOUS_SET 	4
#define MEMORY_NEED_SET_PART2 		5
uint8_t JJ;			//variable for internal calc
uint8_t REALTIME_CUE_LED_BLINK = 16;
uint8_t RED_CIRCLE_CUE_ADR = 85;
uint8_t TIM_CUE_LED = 0;
uint8_t TIM_PLAY_LED = 0;

uint8_t tempo_range = 1;					//10% default
uint8_t tempo_range_need_update = 0;
uint8_t track_need_load = 0;


/* jog ring ---------------------------------------------------------*/

#define JOG_WHITE 0xFFFFFFAF



/* mixer uart variables ---------------------------------------------------------*/
uint32_t mxr_usart_timeout = 0;
uint32_t mxr_current_time;
uint8_t mxr_usart_data_cnt = 0;
uint8_t mxr_new_data = 0;
uint8_t mxr_urx_buf[16];
uint8_t mxrURX;

/* power manager uart variables ---------------------------------------------------------*/
uint32_t pm_usart_timeout = 0;
uint32_t pm_current_time;
uint8_t pm_usart_data_cnt = 0;
uint8_t pm_new_data = 0;
uint8_t pm_urx_buf[2];
uint8_t pmURX;

/* debug uart variables ---------------------------------------------------------*/
char U_TX_DATA[64] = {0};	
uint32_t dbg_usart_timeout = 0;
uint32_t dbg_current_time;
uint8_t dbg_usart_data_cnt = 0;
uint8_t dbg_new_data = 0;
uint8_t dbg_urx_buf[16];
uint8_t dbgURX;

/* REKORDBOX DATABASE variables ---------------------------------------------------------*/
uint32_t BEATGRID[2048];			// beatgrid (0, 3, 7... ) in ms
uint16_t BPMGRID[2048];				// bpmgrit BPM*100
uint8_t GRID_OFFSET = 1;			//ofsset grid 1.2.3.4
#define WFD_SIZE	90000
uint8_t WFORMDYNAMIC[WFD_SIZE];

uint8_t playlist[512][55]; 	//	__attribute__((at(0xD3FF0000)));		//54byte - status byte
uint16_t original_tempo[512];// 	__attribute__((at(0xD3FF6E00)));
uint16_t rating[512];// 	__attribute__((at(0xD3FF7200))); 				//rating and COLOR tracks	
uint16_t duration[512];// 	__attribute__((at(0xD3FF7600)));
uint8_t key_id[512];// 	__attribute__((at(0xD3FF7A00)));
uint32_t parcser_adress[512];	// 	__attribute__ ((section(".sdram")));;
char SDCARD_NAME[20] = "SD CARD";
char SD_DATE[15] = "23-01-2023";
uint16_t TOTAL_TRACKS = 0;
uint16_t TOTAL_TRACKS_IN_CURRENT_PLAYLIST = 0;
uint16_t TRACK_PLAY_IN_CURRENT_PLAYLIST = 0;
uint16_t track_play_now = 0;
uint16_t _TRACK_NUMBER = 0xFFFF;						//	
uint16_t _TRACK_IN_PLAYLIST = 0xFFFF;				//	for loading tracks
uint8_t need_load_track = 0;								//

char path_export[]="0:/PIONEER/rekordbox/export.pdb";
char KEYS[25][4];
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
uint16_t originalBPM = 0xFFFF;						//this original BPM*100 of track (pitch = 0.00%) 


/* HOT CUES and MEMORY variables ---------------------------------------------------------*/
#define NONE_MARK				0
#define MEMORY_MARK			1

uint32_t HCUE32COLOR[8] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
uint16_t HCUE16COLOR[8];
uint32_t HCUE_adr[2][8] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
													 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};  																		//[0] => HCUE in 0.5*frames [1] => loop end in 0.5*frames   150 on 1 sec;  
uint32_t MEMORY_adr[2][10] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
															0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}; 	//[0] => MEMORY CUE in 0.5*frames [1] =>  MEMORY loop end in 0.5*frames   150 on 1 sec;
uint8_t	HCUE_type[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};	 			//b0 (0=cue / 1=loop); b1 (0=inactive / 1=active); 													
uint8_t	MEMORY_type[10] = {0};		//b0 (0=cue / 1=loop); b1 (0=inactive / 1=active); 
uint8_t number_of_hot_cue_points = 0;
uint8_t number_of_memory_cue_points = 0;



/* Static Waveform variables ---------------------------------------------------------*/
uint16_t previousTpos = 0;
uint8_t WFORMSTATIC[203];
uint8_t MemoryCuePyramid_ENABLE = 0;
#define DRAW_NEW_STATIC_WAVEFORM		203
#define CLEAR_WAVEFORM_ARRAY				204
#define MS_NOT_LOADED								205
#define REDRAW_IN_NREMAIN_MODE			206
#define REDRAW_IN_REMAIN_MODE				207
#define MS_ERROR										210
uint8_t DRAWN_IN_REMAIN = 0;
uint8_t need_DSW = 0;						//flag for redraw progressbar remain or nrmain mode when track long 30s
#define PBAR_COLOR_1				((uint16_t)0xE318)
#define PBAR_COLOR_2				((uint16_t)0xB18D)
#define PBAR_COLOR_3				((uint16_t)0xA109)
const static uint16_t WS_COLOR_MAP[2] = 						//colors for static waveforms
{
0xADF4,
0xCAFC		
};
uint8_t RED_VERTICAL_LINE = 0;


/* Dynamic Waveform variables ---------------------------------------------------------*/
uint16_t bars = 0;	




/////temp
uint32_t frametime;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				
																																									