/////////////////////////////////////////////////////////////////////////////////////////////////
// 	global variables
//	product: XDJ-X power manager
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

/* STATUS ---------------------------------------------------------*/
uint8_t XDJ_POWER_ON = 0;		
uint8_t AC_IN_EN = 0;
uint8_t CHARGE_IN_PROCESS = 0;
uint8_t BUTTON_pressed = 0;
uint16_t tim_button_press = 0xFFFF;
uint16_t timeout_power = 0xFFFF;
uint8_t quick_blink = 0;
uint8_t previous_status = 0xFF;
uint8_t timeout_unpress = 0;
uint8_t need_change_status_power = 0;



/* LEDS ---------------------------------------------------------*/
#define LED_W_ON			GPIOB->BRR = 0x00000008		//HAL_GPIO_WritePin(GPIOB, LED_WHITE_Pin, GPIO_PIN_RESET)
#define LED_W_OFF			GPIOB->BSRR = 0x00000008	//HAL_GPIO_WritePin(GPIOB, LED_WHITE_Pin, GPIO_PIN_SET)
#define LED_G_ON			GPIOB->BRR = 0x00000020		//HAL_GPIO_WritePin(GPIOB, LED_GREEN_Pin, GPIO_PIN_RESET)
#define LED_G_OFF			GPIOB->BSRR = 0x00000020	//HAL_GPIO_WritePin(GPIOB, LED_GREEN_Pin, GPIO_PIN_SET)
uint8_t blink_tim = 0;

/* UART ---------------------------------------------------------*/
uint8_t TX_DATA[2] = {0xA1, 0x00};	//[0] - status: A1 no ac, A2 - AC_EN, A3_FULL
																		//[1] - 0...100 charge precentage (after request?), 
#define B_PRESS			0xFE
#define B_UNPRESS		0xFD
#define T_OFF				0xFC
#define AC_DSBL			0xA1
#define AC_CHARGE		0xA2
#define AC_FULL			0xA3
#define BATT_RQ			0x55

uint8_t usart_data_cnt = 0;
char U_RX_DATA[4] = {0};
uint8_t usart_new_data = 0;					//usart new data flag
uint32_t usart_timeout = 0;

/* ADC batt precentage ---------------------------------------------------------*/
uint8_t batt_prcntg = 0;
uint8_t previous_batt_prcntg = 0xFF;
uint32_t ADC_TEMP = 0;
uint32_t DATAV_TEMP = 0;
uint32_t adc_timeout;

/* temporary ---------------------------------------------------------*/



												
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				
																																									