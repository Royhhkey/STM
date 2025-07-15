#ifndef HEAD_DEFINE_H
#define HEAD_DEFINE_H
#include "base.h"

//typedef enum {
//  BOW_READY = 0,   // 准备就绪
//  BOW_CHARGING,    // 正在充电
//  BOW_FIRED,        // 已发射
//	BOW_AUTO,					//自动
//	BOW_SCANF,        //搜索目标
//	BOW_AUTO_SCANF,   // 动态扫描
//	BOW_WaitSTART         //等待启动
//} BOW_StateTypeDef;


#define KEY_NONE    0   // 无按键按下
#define KEY_1       1   // 第1行第1列按键(通常为数字1)
#define KEY_2       2   // 第1行第2列按键
#define KEY_3       3   // 第1行第3列按键
#define KEY_4       4   // 第1行第4列按键
#define KEY_5       5   // 第2行第1列按键
#define KEY_6       6   // 第2行第2列按键
#define KEY_7       7   // 第2行第3列按键
#define KEY_8       8   // 第2行第4列按键
#define KEY_9       9   // 第3行第1列按键
#define KEY_0       10  // 第3行第2列按键
#define KEY_A       11  // 第3行第3列按键(通常为字母A)
#define KEY_B       12  // 第3行第4列按键
#define KEY_C       13  // 第4行第1列按键
#define KEY_D       14  // 第4行第2列按键
#define KEY_STAR    15  // 第4行第3列按键(通常为*键)
#define KEY_MODE    16  // 第4行第4列按键(模式键)

#define KEY_UP_PIN      GPIO_PIN_5
#define KEY_DOWN_PIN    GPIO_PIN_3
#define KEY_LEFT_PIN    GPIO_PIN_2
#define KEY_RIGHT_PIN   GPIO_PIN_4

#define FLAG_CAPTURE1  0x01
#define FLAG_CAPTURE2  0x02

extern volatile uint8_t capture_flag;

#endif
