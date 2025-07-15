#ifndef HEAD_DEFINE_H
#define HEAD_DEFINE_H
#include "base.h"

//typedef enum {
//  BOW_READY = 0,   // ׼������
//  BOW_CHARGING,    // ���ڳ��
//  BOW_FIRED,        // �ѷ���
//	BOW_AUTO,					//�Զ�
//	BOW_SCANF,        //����Ŀ��
//	BOW_AUTO_SCANF,   // ��̬ɨ��
//	BOW_WaitSTART         //�ȴ�����
//} BOW_StateTypeDef;


#define KEY_NONE    0   // �ް�������
#define KEY_1       1   // ��1�е�1�а���(ͨ��Ϊ����1)
#define KEY_2       2   // ��1�е�2�а���
#define KEY_3       3   // ��1�е�3�а���
#define KEY_4       4   // ��1�е�4�а���
#define KEY_5       5   // ��2�е�1�а���
#define KEY_6       6   // ��2�е�2�а���
#define KEY_7       7   // ��2�е�3�а���
#define KEY_8       8   // ��2�е�4�а���
#define KEY_9       9   // ��3�е�1�а���
#define KEY_0       10  // ��3�е�2�а���
#define KEY_A       11  // ��3�е�3�а���(ͨ��Ϊ��ĸA)
#define KEY_B       12  // ��3�е�4�а���
#define KEY_C       13  // ��4�е�1�а���
#define KEY_D       14  // ��4�е�2�а���
#define KEY_STAR    15  // ��4�е�3�а���(ͨ��Ϊ*��)
#define KEY_MODE    16  // ��4�е�4�а���(ģʽ��)

#define KEY_UP_PIN      GPIO_PIN_5
#define KEY_DOWN_PIN    GPIO_PIN_3
#define KEY_LEFT_PIN    GPIO_PIN_2
#define KEY_RIGHT_PIN   GPIO_PIN_4

#define FLAG_CAPTURE1  0x01
#define FLAG_CAPTURE2  0x02

extern volatile uint8_t capture_flag;

#endif
