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
 *  \brief  消息处理流程
 *  \param msg 待处理消息
 *  \param size 消息长度
 */

uint8 cmd_buffer[CMD_MAX_SIZE];//指令缓存
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
    HAL_UART_Receive_IT(&huart3, buffer,sizeof(buffer));//使能接收中断
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

void USART3_IRQHandler()																//把stm32f4xx_it.c中的该函数注释掉
{
	HAL_UART_IRQHandler(&huart3);
	HAL_UART_Receive_IT(&huart3, buffer,sizeof(buffer));  //使能接受中断，因为调用中断回调函数后，会关闭串口中断
    
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)   //接受完数组长度后调用中断回调函数
{
	u8 rec;
	if(huart->Instance==USART3)
	{
		rec=*((huart->pRxBuffPtr)-1); //给数组赋值
		queue_push(rec);//发送数据
    Param_Update();//中断里面处理完指令
	}
}

void Param_Update(void) //获取当前新参数
{
    qsize size;
    size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE);
    if(size)
    {
        ProcessMessage((PCTRL_MSG)cmd_buffer, size);//指令处理
        //LED1_T;
    }
}
 
void ProcessMessage( PCTRL_MSG msg, uint16 size)
{
	uint8 cmd_type = msg->cmd_type;//指令类型
	uint8 ctrl_msg = msg->ctrl_msg;   //消息的类型
	uint8 control_type = msg->control_type;//控件类型
	uint16 screen_id = PTR2U16(&msg->screen_id);//画面ID
	uint16 control_id = PTR2U16(&msg->control_id);//控件ID
	uint32 value = PTR2U32(msg->param);//数值

	switch(cmd_type)
	{		
	case NOTIFY_TOUCH_PRESS://触摸屏按下
	case NOTIFY_TOUCH_RELEASE://触摸屏松开
		NotifyTouchXY(cmd_buffer[1],PTR2U16(cmd_buffer+2),PTR2U16(cmd_buffer+4),NULL);
		break;	
	case NOTIFY_WRITE_FLASH_OK://写FLASH成功
		NotifyWriteFlash(1,NULL);
		break;
	case NOTIFY_WRITE_FLASH_FAILD://写FLASH失败
		NotifyWriteFlash(0,NULL);
		break;
	case NOTIFY_READ_FLASH_OK://读取FLASH成功
		NotifyReadFlash(1,cmd_buffer+2,size-6,NULL);//去除帧头帧尾
		break;
	case NOTIFY_READ_FLASH_FAILD://读取FLASH失败
		NotifyReadFlash(0,0,0,NULL);
		break;
	case NOTIFY_READ_RTC://读取RTC时间
		NotifyReadRTC(cmd_buffer[1],cmd_buffer[2],cmd_buffer[3],cmd_buffer[4],cmd_buffer[5],cmd_buffer[6],cmd_buffer[7],NULL);
		break;
	case NOTIFY_CONTROL:
		{
			if(ctrl_msg==MSG_GET_CURRENT_SCREEN)//画面ID变化通知
			{
				NotifyScreen(screen_id,NULL);
			}
			else
			{
				switch(control_type)
				{
				case kCtrlButton: //按钮控件
					NotifyButton(screen_id,control_id,msg->param[0],NULL);
					break;
				case kCtrlText://文本控件
					NotifyText(screen_id,control_id,msg->param,NULL);
					break;
				case kCtrlProgress: //进度条控件
					NotifyProgress(screen_id,control_id,value,NULL);
					break;
				case kCtrlSlider: //滑动条控件
					NotifySlider(screen_id,control_id,value,NULL);
					break;
				case kCtrlMeter: //仪表控件
					NotifyMeter(screen_id,control_id,value,NULL);
					break;
				case kCtrlMenu://菜单控件
					NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1],NULL);
					break;
				case kCtrlSelector://选择控件
					NotifySelector(screen_id,control_id,msg->param[0],NULL);
					break;
				case kCtrlRTC://倒计时控件
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
 *  \brief  画面切换通知
 *  \details  当前画面改变时(或调用GetScreen)，执行此函数
 *  \param screen_id 当前画面ID
 */
void NotifyScreen(uint16 screen_id,void* userdata)
{
   //TODO: 添加用户代码
}

/*! 
 *  \brief  触摸坐标事件响应
 *  \param press 1按下触摸屏，3松开触摸屏
 *  \param x x坐标
 *  \param y y坐标
 */
void NotifyTouchXY(uint8 press,uint16 x,uint16 y,void* userdata)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  按钮控件通知
 *  \details  当按钮状态改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param state 按钮状态：0弹起，1按下
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
 *  \brief  文本控件通知
 *  \details  当文本通过键盘更新(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param str 文本控件内容
 */
void NotifyText(uint16 screen_id, uint16 control_id, uint8 *str,void* userdata)
{ 
    //TODO: 添加用户代码
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
 *  \brief  进度条控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyProgress(uint16 screen_id, uint16 control_id, uint32 value,void* userdata)
{
		//TODO: 添加用户代码
}
/*! 
 *  \brief  滑动条控件通知
 *  \details  当滑动条改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
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
 *  \brief  仪表控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyMeter(uint16 screen_id, uint16 control_id, uint32 value,void* userdata)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  菜单控件通知
 *  \details  当菜单项按下或松开时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 菜单项索引
 *  \param state 按钮状态：0松开，1按下
 */
void NotifyMenu(uint16 screen_id, uint16 control_id, uint8  item, uint8  state,void* userdata)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  选择控件通知
 *  \details  当选择控件变化时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 当前选项
 */

void NotifySelector(uint16 screen_id, uint16 control_id, uint8  item,void* userdata)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  定时器超时通知处理
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 */
void NotifyTimer(uint16 screen_id, uint16 control_id,void* userdata)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  读取用户FLASH状态返回
 *  \param status 0失败，1成功
 *  \param _data 返回数据
 *  \param length 数据长度
 */
void NotifyReadFlash(uint8 status,uint8 *_data,uint16 length,void* userdata)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  写用户FLASH状态返回
 *  \param status 0失败，1成功
 */
void NotifyWriteFlash(uint8 status,void* userdata)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  读取RTC时间，注意返回的是BCD码
 *  \param year 年（BCD）
 *  \param month 月（BCD）
 *  \param week 星期（BCD）
 *  \param day 日（BCD）
 *  \param hour 时（BCD）
 *  \param minute 分（BCD）
 *  \param second 秒（BCD）
 */
void NotifyReadRTC(uint8 year,uint8 month,uint8 week,uint8 day,uint8 hour,uint8 minute,uint8 second,void* userdata)
{
    
}
