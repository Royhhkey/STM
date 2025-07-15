
#ifndef __OLED_H
#define __OLED_H			  	 
#include "main.h"  
#include "base.h"

//typedef unsigned char u8;
//typedef unsigned short u16;
//typedef unsigned long u32;
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    




//-----------------OLED�˿ڶ���----------------  					   


// // GPIO ���ƺ궨��
// #define OLED_SCLK_Pin       GPIO_PIN_8
// #define OLED_SCLK_GPIO_Port GPIOB
// #define OLED_SDIN_Pin       GPIO_PIN_9
// #define OLED_SDIN_GPIO_Port GPIOB
// #define OLED_DC_Pin         GPIO_PIN_4
// #define OLED_DC_GPIO_Port   GPIOB
// #define OLED_CS_Pin         GPIO_PIN_3
// #define OLED_CS_GPIO_Port   GPIOB
// #define OLED_RST_PIN    GPIO_PIN_7
// #define OLED_RST_GPIO_Port    GPIOB

// GPIO 引脚定义
#define OLED_SCLK_Pin       GPIO_PIN_8
#define OLED_SCLK_GPIO_Port GPIOB
#define OLED_SDIN_Pin       GPIO_PIN_9
#define OLED_SDIN_GPIO_Port GPIOB
#define OLED_DC_Pin         GPIO_PIN_13    // 修改为PB13
#define OLED_DC_GPIO_Port   GPIOB
#define OLED_CS_Pin         GPIO_PIN_14    // 修改为PB14
#define OLED_CS_GPIO_Port   GPIOB
#define OLED_RST_PIN        GPIO_PIN_12     // 修改为PB12    OLED上电不显示的原因可能是PB7作为RST引脚的问题
#define OLED_RST_GPIO_Port  GPIOB
// GPIO ���ƺ���
#define OLED_SCLK_Set() HAL_GPIO_WritePin(OLED_SCLK_GPIO_Port, OLED_SCLK_Pin, GPIO_PIN_SET)
#define OLED_SCLK_Clr() HAL_GPIO_WritePin(OLED_SCLK_GPIO_Port, OLED_SCLK_Pin, GPIO_PIN_RESET)
#define OLED_SDIN_Set() HAL_GPIO_WritePin(OLED_SDIN_GPIO_Port, OLED_SDIN_Pin, GPIO_PIN_SET)
#define OLED_SDIN_Clr() HAL_GPIO_WritePin(OLED_SDIN_GPIO_Port, OLED_SDIN_Pin, GPIO_PIN_RESET)
#define OLED_DC_Set()   HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET)
#define OLED_DC_Clr()   HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET)
#define OLED_CS_Set()   HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET)
#define OLED_CS_Clr()   HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET)

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

#define OLED_NORMAL 0   // 正常显示（白底黑字）
#define OLED_INVERSE 1  // 反显（黑底白字）


//OLED�����ú���
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);

// 修改函数声明
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size, u8 mode);  // 添加mode参数
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 mode);
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 mode);
 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

    // void OLED_Update_Row(uint8_t row, float data, const char* title, const char* unit, uint8_t mode);

#endif
	 



