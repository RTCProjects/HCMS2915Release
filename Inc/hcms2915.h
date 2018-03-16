#ifndef _HCMS2915_H
#define _HCMS2915_H

#pragma anon_unions

#include "main.h"
#include "stm32l0xx_hal.h"
#include "font5x7.h"

#include <string.h>

#define		CHIP_CNT		2
#define		CHIP_POS		8
#define 	FONT_COLS		5
#define		SCR_SIZE		40

#define		CHIP_RAW_SIZE	(CHIP_CNT*CHIP_POS*FONT_COLS)

typedef enum{				// real pwm
	PWM_BRIGHT_000	= 0,	// 0.0%
	PWM_BRIGHT_002	= 1,	// 1.7%
	PWM_BRIGHT_003	= 2, 	// 2.2%
	PWM_BRIGHT_005	= 3, 	// 5.0%
	PWM_BRIGHT_007	= 4, 	// 6.7%
	PWM_BRIGHT_008	= 5, 	// 8.3%
	PWM_BRIGHT_012	= 6, 	// 11.7%
	PWM_BRIGHT_015	= 7, 	// 15%
	PWM_BRIGHT_018	= 8, 	// 18%
	PWM_BRIGHT_023	= 9, 	// 23%
	PWM_BRIGHT_030	= 10,	// 30%
	PWM_BRIGHT_037	= 11,	// 37%
	PWM_BRIGHT_047	= 12,	// 47%
	PWM_BRIGHT_060	= 13,	// 60%
	PWM_BRIGHT_080	= 14,	// 80%
	PWM_PRIGHT_100	= 15 	// 100%
} pwm_brightness_t;

#define MIN_BRIGHTNESS			PWM_BRIGHT_002
#define MAX_BRIGHTNESS			PWM_PRIGHT_100

#define DEFAULT_BRIGHTNESS		PWM_BRIGHT_047
#define DEFAULT_PEAK_CURRENT	PEAK_09_MA

typedef enum{				// real peak current
	PEAK_04_MA		= 2,	//  4.0 mA
	PEAK_06_MA		= 1,	//  6.4 mA
	PEAK_09_MA		= 0,	//  9.3 mA
	PEAK_13_MA		= 3		// 12.8 mA
} peak_current_t;

typedef enum{
	EFFECT_OFF,
	BLINK_R1,
	BLINK_R2,
	BLINK_R3
}eEffectType;

typedef union{
	struct{
		uint8_t	r1_blink : 1;
		uint8_t	r2_blink : 1;
		uint8_t	r3_blink : 1;
		
		uint8_t	reserved : 5;
	};
	uint8_t	mode_byte;
}tEffectMode;

typedef	union{
	struct{
		pwm_brightness_t	brightness				: 4;	
		peak_current_t		peak_current			: 2;	
		uint8_t	sleep_mode		: 1;	
		uint8_t reg						: 1;	
	};
	uint8_t		byte;					
} ctrl_reg0_t;

typedef	union{
	struct{
		uint8_t	data_out_ctrl	: 1;	
		uint8_t	edo_prescaler	: 1;	
		uint8_t	reserved			: 5;	
		uint8_t	reg						: 1;	
	};
	uint8_t		byte;					
} ctrl_reg1_t;

#if (CHIP_RAW_SIZE > UINT8_MAX)
#define buf_size_t	uint16_t
#else
#define buf_size_t	uint8_t
#endif

extern char hcms_screen[SCR_SIZE];
extern SPI_HandleTypeDef hspi1;


void HCMS_Init(void);
void HCMS_Reset(void);
void HCMS_Ctrl_Register(uint8_t data);
void HCMS_RawPixels(uint8_t *buf, int sz);

void HCMS_CtrlMode(void);
void HCMS_DataMode(void);
void HCMS_Put_Byte(uint8_t	outByte);

void HCMS_Enable(void);
void HCMS_Disable(void);

void HCMS_SetBrightness(pwm_brightness_t	brightness);

void HCMS_PutStr(char *);
void HCMS_Update(void);
void HCMS_Clear(void);
void HCMS_On(uint8_t On);

void HCMS_Effect(eEffectType eEffect);

void HCMS_Process(void const * argument);
#endif
