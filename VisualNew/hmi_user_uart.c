/************************************版权申明********************************************
**                             广州大彩光电科技有限公司
**                             http://www.gz-dc.com
**-----------------------------------文件信息--------------------------------------------
** 文件名称:   hmi_user_uart.c
** 修改时间:   2011-05-18
** 文件说明:   用户MCU串口驱动函数库
** 技术支持：  Tel: 020-82186683  Email: hmi@gz-dc.com Web:www.gz-dc.com
--------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------
                                  使用必读
   hmi_user_uart.c中的串口发送接收函数共3个函数：串口初始化Uartinti()、发送1个字节SendChar()、
   发送字符串SendStrings().若移植到其他平台，需要修改底层寄
   存器设置,但禁止修改函数名称，否则无法与HMI驱动库(hmi_driver.c)匹配。
--------------------------------------------------------------------------------------



----------------------------------------------------------------------------------------
                          1. 基于STM32平台串口驱动
----------------------------------------------------------------------------------------*/
#include "base.h"
#include "usart.h"
#include "hmi_user_uart.h"
#include "hmi_driver.h"
#include "cmd_queue.h"
#include "cmd_process.h"
/****************************************************************************
* 名    称： UartInit()
* 功    能： 串口初始化
* 入口参数： 无
* 出口参数： 无
****************************************************************************/

#define T_UART	huart1
#define R_UART	huart2
#define R_UART_	USART2

unsigned char buffer[1];

void TFT_Init(void)
{
    HAL_UART_Receive_IT(&R_UART, buffer,sizeof(buffer));//使能接收中断
    queue_reset();
}
void Param_Update(void) //获取当前新参数
{
    qsize size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE);
    if(size)
    {
        ProcessMessage((PCTRL_MSG)cmd_buffer, size);//指令处理
    }
}

void USART2_IRQHandler()
{
	HAL_UART_IRQHandler(&R_UART);
	HAL_UART_Receive_IT(&R_UART, buffer,sizeof(buffer));  //使能接受中断，因为调用中断回调函数后，会关闭串口中断
    
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)   //接受完数组长度后调用中断回调函数
{
	if(huart->Instance==R_UART_)
	{
		u8 rec=*((huart->pRxBuffPtr)-1); //给数组赋值
		queue_push(rec);//发送数据
        Param_Update();//中断里面处理完指令
	}
}

/*****************************************************************
* 名    称： SendChar()
* 功    能： 发送1个字节
* 入口参数： t  发送的字节
* 出口参数： 无
 *****************************************************************/
void  SendChar(uchar t)
{
    HAL_UART_Transmit(&T_UART,&t,1,1000);
}


