#ifndef __KEY4_H
#define __KEY4_H
#include "main.h"

// ���߶��壨�����
#define ROW1_PIN GPIO_PIN_8
#define ROW2_PIN GPIO_PIN_7
#define ROW3_PIN GPIO_PIN_6
#define ROW4_PIN GPIO_PIN_5
#define ROW_PORT GPIOG

// ���߶��壨���룩
#define COL1_PIN GPIO_PIN_6
#define COL2_PIN GPIO_PIN_4
#define COL3_PIN GPIO_PIN_3
#define COL4_PIN GPIO_PIN_5
#define COL_PORT GPIOD

// ����ֵ����
#define KEY_NONE    0
#define KEY_1       1
#define KEY_2       2
#define KEY_3       3
#define KEY_4       4
#define KEY_5       5
#define KEY_6       6
#define KEY_7       7
#define KEY_8       8
#define KEY_9       9
#define KEY_0       10
#define KEY_A       11
#define KEY_B       12
#define KEY_C       13
#define KEY_D       14
#define KEY_STAR    15
#define KEY_MODE    16

void KEY_4x4_Init(void);
//uint8_t KEY_4x4_Scan(void);
void KEY_4x4_Scan(uint8_t *key);


#endif
