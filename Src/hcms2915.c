#include "hcms2915.h"
#include "cmsis_os.h"

char hcms_screen[SCR_SIZE];

static ctrl_reg0_t def_ctrl_r0;
static ctrl_reg1_t def_ctrl_r1;
static tEffectMode	effectMode;

int8_t	scroll_x = 0;
int8_t	scroll_y = 0;
int8_t	blink = 0; 
 
osThreadId hcms2915TaskHandle;
 
void	HCMS_Init()
{	
	effectMode.r1_blink = 0;
	effectMode.r2_blink = 0;
	effectMode.r3_blink = 0;
	

	def_ctrl_r0.brightness = DEFAULT_BRIGHTNESS;
	def_ctrl_r0.peak_current = DEFAULT_PEAK_CURRENT;
	def_ctrl_r0.sleep_mode = 1;
	def_ctrl_r0.reg = 0;

	def_ctrl_r1.data_out_ctrl = 1;
	def_ctrl_r1.edo_prescaler = 0;
	def_ctrl_r1.reserved = 0;
	def_ctrl_r1.reg = 1;
	
	blink = 0;
	
	HCMS_Reset();
	HCMS_CtrlMode();
	
	for(uint8_t i=0; i < CHIP_CNT+1; i++)
		HCMS_Ctrl_Register(def_ctrl_r1.byte);
	HCMS_Ctrl_Register(def_ctrl_r0.byte);
	
	osThreadDef(hcmsTask, HCMS_Process, osPriorityNormal, 0, configMINIMAL_STACK_SIZE + 0x100);
  hcms2915TaskHandle = osThreadCreate(osThread(hcmsTask), NULL);
}

void HCMS_Reset()
{
	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port,LCD_RESET_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port,LCD_RESET_Pin,GPIO_PIN_SET);
	HAL_Delay(100);
}

void HCMS_CtrlMode()
{
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port,LCD_RS_Pin,GPIO_PIN_SET);
}

void HCMS_DataMode()
{
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port,LCD_RS_Pin,GPIO_PIN_RESET);
}
void HCMS_Enable()
{
	HAL_GPIO_WritePin(LCD_EN_GPIO_Port,LCD_EN_Pin,GPIO_PIN_RESET);
}
void HCMS_Disable()
{
	HAL_GPIO_WritePin(LCD_EN_GPIO_Port,LCD_EN_Pin,GPIO_PIN_SET);
}

void HCMS_Ctrl_Register(uint8_t data)
{
	HCMS_CtrlMode();
	HCMS_Enable();
	HCMS_Put_Byte(data);
	HCMS_Disable();
}
void HCMS_Put_Byte(uint8_t	outByte)
{
	__IO HAL_StatusTypeDef  hStatus;
	
	hStatus = HAL_SPI_Transmit(&hspi1,(uint8_t*)&outByte,sizeof(uint8_t),100);
}

void HCMS_RawPixels(uint8_t *buf, int sz)
{
	blink++;
	if(blink > 6)blink = -2;
	//scroll_x++;
	if(scroll_x > 7)scroll_x = -7;
	
	HCMS_DataMode();
	HCMS_Enable();
	for(int i=0; i < sz; i++)
	{
		/*
		if(i>=30){
			if(scroll_x < 0)
				HCMS_Put_Byte(buf[i]<< (-scroll_x));
			else
				HCMS_Put_Byte(buf[i]>>scroll_x);
		}
		else
			HCMS_Put_Byte(buf[i]);*/
		if(effectMode.r1_blink){
			if(i<=10){
				if(blink > 0)HCMS_Put_Byte(buf[i]);
				else HCMS_Put_Byte(0);				
			}
			else{
				HCMS_Put_Byte(buf[i]);
			}
		}
		else if(effectMode.r2_blink){
			if(i>=15 && i<=25){
				if(blink > 0)HCMS_Put_Byte(buf[i]);
				else HCMS_Put_Byte(0);				
			}
			else{
				HCMS_Put_Byte(buf[i]);
			}			
		}
		else if(effectMode.r3_blink){
			if(i>=30){
				if(blink > 0)HCMS_Put_Byte(buf[i]);
				else HCMS_Put_Byte(0);				
			}
			else{
				HCMS_Put_Byte(buf[i]);
			}			
		}
		else
			HCMS_Put_Byte(buf[i]);
	}
	HCMS_Disable();
}

void HCMS_Clear()
{
	memset(hcms_screen,0,sizeof(uint8_t) * SCR_SIZE);
	
}

void HCMS_PutStr(char *str)
{

	//HCMS_DataMode();
	//HCMS_Enable();
	 uint16_t ind = 0;
	 uint16_t	bufPos = 0;
	for(int i = 0;i<strlen(str);i++){
		__IO char curCh = str[i];
	
		if(curCh < 0x7F)
			bufPos = (curCh - 0x20) * 5;
		else
			bufPos = (curCh - 0x62) * 5;
		
		for(int j = bufPos;j<bufPos+5;j++){
			   //HCMS_Put_Byte( font5x7[j]);
			hcms_screen[ind++] = font5x7[j];
		}
	}
	
	//HCMS_Disable();
}

/*void HCMS_Update()
{
	HCMS_RawPixels(((uint8_t*)&hcms_screen),SCR_SIZE);
}
*/
void HCMS_On(uint8_t On)
{
	  def_ctrl_r0.sleep_mode = !On;
		HCMS_Ctrl_Register(def_ctrl_r0.byte);
}
void HCMS_SetBrightness(pwm_brightness_t	brightness)
{
	def_ctrl_r0.brightness = brightness;
	HCMS_Ctrl_Register(def_ctrl_r0.byte);
}
void HCMS_Effect(eEffectType eEffect)
{
	switch(eEffect)
	{
		case EFFECT_OFF:{
			effectMode.mode_byte = 0;
		}break;
		
		case BLINK_R1:{
			effectMode.mode_byte = 0;
			effectMode.r1_blink = 1;
		}break;
		case BLINK_R2:{
			effectMode.mode_byte = 0;
			effectMode.r2_blink = 1;
		}break;
		case BLINK_R3:{
			effectMode.mode_byte = 0;
			effectMode.r3_blink = 1;
		}break;
	}
}
void HCMS_Process(void const * argument)
{	
	while(1)
	{
		HCMS_RawPixels(((uint8_t*)&hcms_screen),SCR_SIZE);
		osDelay(50);
	}
}
