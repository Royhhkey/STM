#include "cmd_process.h"
#include "hmi_driver.h"
#include "cmd_queue.h"
#include "main.h"
#include "dac.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "stm32f4xx_it.h"
#include "base.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

/*! 
 *  \brief  ��Ϣ��������
 *  \param msg ��������Ϣ
 *  \param size ��Ϣ����
 */

uint8 cmd_buffer[CMD_MAX_SIZE];//ָ���
unsigned char buffer[1];


extern u32 M;
//extern u16 k,f,a;
extern u16 mvpp;
extern u8  mode ;
extern int sin_table[1024];
u8 buttonId = 0;
int value ;
#define step_vpp  150
#define step_fre 1000
#define MAX_VPP  3000
#define feqWord 17189935

void TFT_Init(void)
{
    HAL_UART_Receive_IT(&huart3, buffer,sizeof(buffer));//ʹ�ܽ����ж�
    queue_reset();
}
void UI_Init(void){
	SetTextInt32(1,2,5,0,1);
	SetTextInt32(1,7,1,0,1);
}
void init_table(uint16_t mvpp){
//  float vpp = ((mvpp-60.0)/3300.0*4095.0)/2.0;
  float vpp = ((mvpp-15.0)/3300.0*4095.0)/2.0;

  float offset = vpp + 100;  
  if(offset < 150) offset = 150;  
  if(offset + vpp > 4095) {  
      offset = 4095 - vpp;
  }
  
  for(int i=0;i<1024;i++) {
      sin_table[i] = vpp*sin(2*3.1415926*i/1024) + offset;
  }
}

void USART3_IRQHandler()																//��stm32f4xx_it.c�еĸú���ע�͵�
{
	HAL_UART_IRQHandler(&huart3);
	HAL_UART_Receive_IT(&huart3, buffer,sizeof(buffer));  //ʹ�ܽ����жϣ���Ϊ�����жϻص������󣬻�رմ����ж�
    
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)   //���������鳤�Ⱥ�����жϻص�����
{
	u8 rec;
	if(huart->Instance==USART3)
	{
		rec=*((huart->pRxBuffPtr)-1); //�����鸳ֵ
		queue_push(rec);//��������
    Param_Update();//�ж����洦����ָ��
	}
}

void Param_Update(void) //��ȡ��ǰ�²���
{
    qsize size;
    size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE);
    if(size)
    {
        ProcessMessage((PCTRL_MSG)cmd_buffer, size);//ָ���
        //LED1_T;
    }
}
 
void ProcessMessage( PCTRL_MSG msg, uint16 size)
{
	uint8 cmd_type = msg->cmd_type;//ָ������
	uint8 ctrl_msg = msg->ctrl_msg;   //��Ϣ������
	uint8 control_type = msg->control_type;//�ؼ�����
	uint16 screen_id = PTR2U16(&msg->screen_id);//����ID
	uint16 control_id = PTR2U16(&msg->control_id);//�ؼ�ID
	uint32 value = PTR2U32(msg->param);//��ֵ

	switch(cmd_type)
	{		
	case NOTIFY_TOUCH_PRESS://����������
	case NOTIFY_TOUCH_RELEASE://�������ɿ�
		NotifyTouchXY(cmd_buffer[1],PTR2U16(cmd_buffer+2),PTR2U16(cmd_buffer+4),NULL);
		break;	
	case NOTIFY_WRITE_FLASH_OK://дFLASH�ɹ�
		NotifyWriteFlash(1,NULL);
		break;
	case NOTIFY_WRITE_FLASH_FAILD://дFLASHʧ��
		NotifyWriteFlash(0,NULL);
		break;
	case NOTIFY_READ_FLASH_OK://��ȡFLASH�ɹ�
		NotifyReadFlash(1,cmd_buffer+2,size-6,NULL);//ȥ��֡ͷ֡β
		break;
	case NOTIFY_READ_FLASH_FAILD://��ȡFLASHʧ��
		NotifyReadFlash(0,0,0,NULL);
		break;
	case NOTIFY_READ_RTC://��ȡRTCʱ��
		NotifyReadRTC(cmd_buffer[1],cmd_buffer[2],cmd_buffer[3],cmd_buffer[4],cmd_buffer[5],cmd_buffer[6],cmd_buffer[7],NULL);
		break;
	case NOTIFY_CONTROL:
		{
			if(ctrl_msg==MSG_GET_CURRENT_SCREEN)//����ID�仯֪ͨ
			{
				NotifyScreen(screen_id,NULL);
			}
			else
			{
				switch(control_type)
				{
				case kCtrlButton: //��ť�ؼ�
					NotifyButton(screen_id,control_id,msg->param[0],NULL);
					break;
				case kCtrlText://�ı��ؼ�
					NotifyText(screen_id,control_id,msg->param,NULL);
					break;
				case kCtrlProgress: //�������ؼ�
					NotifyProgress(screen_id,control_id,value,NULL);
					break;
				case kCtrlSlider: //�������ؼ�
					NotifySlider(screen_id,control_id,value,NULL);
					break;
				case kCtrlMeter: //�Ǳ�ؼ�
					NotifyMeter(screen_id,control_id,value,NULL);
					break;
				case kCtrlMenu://�˵��ؼ�
					NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1],NULL);
					break;
				case kCtrlSelector://ѡ��ؼ�
					NotifySelector(screen_id,control_id,msg->param[0],NULL);
					break;
				case kCtrlRTC://����ʱ�ؼ�
					NotifyTimer(screen_id,control_id,NULL);
					break;
				default:
					break;
				}
			}			
		}
		break;
	default:
		break;
	}
}

void UpdateUI()
{
	
}

/*! 
 *  \brief  �����л�֪ͨ
 *  \details  ��ǰ����ı�ʱ(�����GetScreen)��ִ�д˺���
 *  \param screen_id ��ǰ����ID
 */
void NotifyScreen(uint16 screen_id,void* userdata)
{
   //TODO: ����û�����
}

/*! 
 *  \brief  ���������¼���Ӧ
 *  \param press 1���´�������3�ɿ�������
 *  \param x x����
 *  \param y y����
 */
void NotifyTouchXY(uint8 press,uint16 x,uint16 y,void* userdata)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ť�ؼ�֪ͨ
 *  \details  ����ť״̬�ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param state ��ť״̬��0����1����
 */
void NotifyButton(uint16 screen_id, uint16 control_id, uint8  state,void* userdata)
{
//			buttonId = control_id ;
//	  GetControlValue(screen_id,control_id);
//	  index ++;
				
	  
//	     delay_ms(100);

				if(screen_id==1) {
						 if(control_id == 3){
							 if(mvpp>step_vpp){
								 mvpp-=  step_vpp;
							 }
							 init_table(mvpp);
						 }
						 else if(control_id ==4){
							 if(mvpp< MAX_VPP){
								 mvpp+= step_vpp;
							 }
							 init_table(mvpp);

						 }
						 else if(control_id ==5){
										if(M>feqWord)
												M-=feqWord;
						 }
						 else if(control_id ==6){
											if(M<feqWord*40)
													M+=feqWord;
						 }
						 else if(control_id ==8){
							 mode =1;
							 M = feqWord;
							 mvpp = 150;
							 init_table(mvpp);
							 UI_Init();
						 }
						 else if(control_id ==9){
							 mode =2;
						 }
						 
						
				 }
			
}

/*! 
 *  \brief  �ı��ؼ�֪ͨ
 *  \details  ���ı�ͨ�����̸���(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param str �ı��ؼ�����
 */
void NotifyText(uint16 screen_id, uint16 control_id, uint8 *str,void* userdata)
{ 
    //TODO: ����û�����
				if(screen_id==1) {
					    value = atoi((char*)str);
						 if(buttonId == 3){
							   mvpp = value;
//							 if(mvpp>step_vpp){
//								 mvpp-=  step_vpp;
//							 }
							 init_table(mvpp);
						 }
						 else if(buttonId ==4){
								mvpp = value;
							 init_table(mvpp);

						 }
						 else if(buttonId ==5){
//										if(M>feqWord)
//												M-=feqWord;
							 M = feqWord * value;
						 }
						 else if(buttonId ==6){
							 M = feqWord * value;

//											if(M<feqWord*40)
//													M+=feqWord;
						 }
						 else if(buttonId ==8){
							 mode =1;
						 }
						 else if(buttonId ==9){
							 mode =2;
						 }
						 
						
				 }
}

/*! 
 *  \brief  �������ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyProgress(uint16 screen_id, uint16 control_id, uint32 value,void* userdata)
{
		//TODO: ����û�����
}
/*! 
 *  \brief  �������ؼ�֪ͨ
 *  \details  ���������ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifySlider(uint16 screen_id, uint16 control_id, uint32 value,void* userdata)
{
//		if(screen_id==0 && control_id == 1) 
//		{
//        f=value;
//				SetTextInt32(0,15,value,0,1);
//    }
//    else if(screen_id==0 && control_id == 2)
//    {
//        a=value;
//				printf("a:%d\r\n",a);
//				SetTextInt32(0,16,value,0,1);
//    }
}

/*! 
 *  \brief  �Ǳ�ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyMeter(uint16 screen_id, uint16 control_id, uint32 value,void* userdata)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  �˵��ؼ�֪ͨ
 *  \details  ���˵���»��ɿ�ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item �˵�������
 *  \param state ��ť״̬��0�ɿ���1����
 */
void NotifyMenu(uint16 screen_id, uint16 control_id, uint8  item, uint8  state,void* userdata)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ѡ��ؼ�֪ͨ
 *  \details  ��ѡ��ؼ��仯ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item ��ǰѡ��
 */

void NotifySelector(uint16 screen_id, uint16 control_id, uint8  item,void* userdata)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ʱ����ʱ֪ͨ����
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 */
void NotifyTimer(uint16 screen_id, uint16 control_id,void* userdata)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ȡ�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 *  \param _data ��������
 *  \param length ���ݳ���
 */
void NotifyReadFlash(uint8 status,uint8 *_data,uint16 length,void* userdata)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  д�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 */
void NotifyWriteFlash(uint8 status,void* userdata)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ȡRTCʱ�䣬ע�ⷵ�ص���BCD��
 *  \param year �꣨BCD��
 *  \param month �£�BCD��
 *  \param week ���ڣ�BCD��
 *  \param day �գ�BCD��
 *  \param hour ʱ��BCD��
 *  \param minute �֣�BCD��
 *  \param second �루BCD��
 */
void NotifyReadRTC(uint8 year,uint8 month,uint8 week,uint8 day,uint8 hour,uint8 minute,uint8 second,void* userdata)
{
    
}
