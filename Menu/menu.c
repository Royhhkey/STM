/*
 * menu.c
 *
 *  Created on: Apr 21, 2022
 *      Author: LX
 */


#include "menu.h"
#include "oled.h"
#include "stdio.h"
//#include "tim.h"
//#include "key_scan.h"

mytime_t myTime    ={14,18,55,2022,4,21};
mytime_t myTimeTemp={14,18,55,2022,4,21};

extern uint8_t 
							isKeyUp ,
							isKeyDown,
							isKeyLeft,
							isKeyRight,
							isKeyOk;


void MyTimeFun(void)
{
    if(myTime.Second>59)
    {
        myTime.Second=0;
        myTime.Minute++;
    }
    if(myTime.Minute>59)
    {
        myTime.Minute=0;
        myTime.Hour++;
    }
    if(myTime.Hour>23)
    {
        myTime.Hour=0;
    }
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	static uint16_t time = 0;
//	if(htim == &htim6)
//	{
//		time++;
//		if(time % 10 == 0)
//		{
//			KeyScan();
//			MyTimeFun();
//		}
//		if(time == 1000)
//		{
//			myTime.Second++;
//			time = 0;
//		}
//	}
//}

int8_t selectItem = 0;//当前菜单在当前菜单页的索引，最大为3，一页最多显示四个菜单
int8_t scrollBar  = 0;//滚动条
struct Menu_t *menuPoint;//当前菜单

//拿来做测试的数据
int8_t kp = 0;
int8_t ki = 0;
int8_t kd = 0;
int16_t count_test=0;
uint16_t count1=0,count2=2,count3=4,count4=6,count5=8;

extern 
		uint16_t offset_phase; 
extern 
		float M;
//一些菜单的功能函数
//void KpSub(void);
//void KiAdd(void);
//void KdAdd(void);
//void KpAdd(void);
void MSub(void);
void MAdd(void);
//void OffsetPhaseSub(void);
//void OffsetPhaseAdd(void);
void PhaseOffsetAdd(void);
void PhaseOffsetSub(void);
void HourAdd(void);
void MinuteAdd(void);
void SecondAdd(void);
void YearAdd(void);
void MonthAdd(void);
void DayAdd(void);
void TimeOK(void);
void TimeSetInit(void);

//主UI
struct MenuProperty_t MainUIProperty={1,0};
struct Menu_t MainUI={&MainUIProperty,"MainUI" ,NULL,NULL,NULL};

//主菜单
struct MenuProperty_t menuMainProperty={4,0};
struct Menu_t menuMain[4]=
{
	{&menuMainProperty,"last menu     ", NULL,NULL, &MainUI,NULL},
	{&menuMainProperty,"DataDisplay   ", NULL,NULL, &MainUI,NULL},
	{&menuMainProperty,"AdjustMent    ", NULL,NULL, &MainUI,NULL},
	{&menuMainProperty,"Time set      ", NULL,TimeSetInit, &MainUI,NULL}
};
//animal的子菜单
struct MenuProperty_t setMenu1Property={6,2};
struct Menu_t setMenu1[6]=
{
	{&setMenu1Property,"last menu     ",NULL,NULL,menuMain,NULL},
//	{&setMenu1Property,"bull          ",NULL,NULL,menuMain,NULL},
//	{&setMenu1Property,"bird          ",NULL,NULL,menuMain,NULL},
//	{&setMenu1Property,"dog           ",NULL,NULL,menuMain,NULL},
//	{&setMenu1Property,"bow           ",NULL,NULL,menuMain,NULL},
	{&setMenu1Property,"M             ",NULL,NULL,menuMain,NULL}
};
//Pid的子菜单
struct MenuProperty_t setMenu2Property={5,1};
struct Menu_t setMenu2[5]=
{
	{&setMenu2Property,"last menu      ",NULL, NULL, menuMain,NULL},
	{&setMenu2Property,"M    0         ",MSub,MAdd,menuMain,NULL},
	{&setMenu2Property,"Phase   	0    ",PhaseOffsetSub,PhaseOffsetAdd,menuMain,NULL}

//	{&setMenu2Property,"KI   0         ",NULL, KiAdd,menuMain,NULL},
//	{&setMenu2Property,"KD   0         ",NULL, KdAdd,menuMain,NULL},
//	{&setMenu2Property,"PWM  0         ",NULL, NULL, menuMain,NULL},
};
//Time set的子菜单
struct MenuProperty_t setMenu3Property={8,4};
struct Menu_t setMenu3[8]=
{
	{&setMenu3Property,"last menu      ",NULL, NULL,menuMain,NULL},
	{&setMenu3Property,"Hour   0       ",NULL, HourAdd,menuMain,NULL},
	{&setMenu3Property,"Minute 0       ",NULL, MinuteAdd,menuMain,NULL},
	{&setMenu3Property,"Second 0       ",NULL, SecondAdd,menuMain,NULL},
	{&setMenu3Property,"Year   0       ",NULL, YearAdd,menuMain,NULL},
	{&setMenu3Property,"Month  0       ",NULL, MonthAdd,menuMain,NULL},
	{&setMenu3Property,"Day    0       ",NULL, DayAdd,menuMain,NULL},
	{&setMenu3Property,"OK             ",NULL, TimeOK,menuMain,NULL},
};
void MSub(void){
	M-=0.1;
}
void MAdd(void){
	M+=0.1;
}
void PhaseOffsetAdd(void){
	offset_phase +=5;
}
void PhaseOffsetSub(void){
	offset_phase -=5;
}


//void KpSub()
//{
//	kp--;
//}

//void KpAdd()
//{
//	kp++;
//}

//void KiAdd()
//{
//	ki++;
//}

//void KdAdd()
//{
//	kd++;
//}

void HourAdd(void)
{
	myTimeTemp.Hour++;
	if(myTimeTemp.Hour>23)
		myTimeTemp.Hour=0;
}
void MinuteAdd(void)
{
	myTimeTemp.Minute++;
	if(myTimeTemp.Minute>59)
		myTimeTemp.Minute=0;
}
void SecondAdd(void)
{
	myTimeTemp.Second++;
	if(myTimeTemp.Second>59)
		myTimeTemp.Second=0;
}
void YearAdd(void)
{
	myTimeTemp.Year++;
}
void MonthAdd(void)
{
	myTimeTemp.Month++;
	if(myTimeTemp.Month>12)
		myTimeTemp.Month=1;
}
void DayAdd(void)
{
	myTimeTemp.Day++;
	if(myTimeTemp.Day>30)
		myTimeTemp.Day=1;
}
void TimeOK(void)
{
	myTime = myTimeTemp;
}

void TimeSetInit(void)
{
	myTimeTemp = myTime;
}


void MainUiSet(void)
{
	int i;
	uint8_t time_x = 18,time_y = 12,date_x=26,date_y=40;
	for(i=1;i<15;i++)
	{
		OLED_ShowChar(8*i,0, '-',16);
		OLED_ShowChar(8*i,48,'-',16);
	}

	OLED_ShowNum(time_x,time_y,myTime.Hour  ,2,24);
	OLED_ShowChar(time_x+24,time_y,':'   ,24);
	OLED_ShowNum(time_x+36, time_y,myTime.Minute,2,24);
	OLED_ShowChar(time_x+60,time_y,':'   ,24);
	OLED_ShowNum(time_x+72, time_y,myTime.Second,2,24);

	OLED_ShowNum(date_x,    date_y,myTime.Year  ,4,16);
	OLED_ShowChar(date_x+32,date_y,'/'   ,16);
	OLED_ShowNum(date_x+40, date_y,myTime.Month,2,16);
	OLED_ShowChar(date_x+56,date_y,'/'   ,16);
	OLED_ShowNum(date_x+64, date_y,myTime.Day,  2,16);
}
/**
  * @Name    DisplayRefreash
  * @brief   刷新当前菜单页的所有菜单项
  * @param   nowMenu: 当前菜单指针
**			 selectItem: 当前菜单在当前菜单页的索引，指小光标在第几行
**           scrollBar：滚动条所在位置
  */
void DisplayRefreash(struct Menu_t *nowMenu,uint8_t selectItem,uint8_t scrollBar)
{
	int i = 0;
	static uint8_t lastSelectItem=0;//记录上次索引
	if(nowMenu==&MainUI)//当回到主菜单时，由于没有全占屏，所以全部清屏，再画
	{
		OLED_Clear();
		MainUiSet();
	}else
	{
		OLED_ShowChar(0,lastSelectItem*16, ' ',16);//清除上次索引
		OLED_ShowChar(0,selectItem*16,     '>',16);//画出这次索引
		for(i=0;i<(nowMenu->MenuProperty->MenuLen-nowMenu->MenuProperty->scrollBarLen);i++)
		{
			OLED_ShowString(8,i*16,nowMenu[i+scrollBar].displayString,16);
		}
	}
	OLED_Refresh();
	lastSelectItem = selectItem;
}
/**
  * @Name    DisplayRefreashDate
  * @brief   刷新当前菜单页的所有菜单项,主要拿来更新数据
  * @param   nowMenu:    当前菜单指针
**			 selectItem: 当前菜单在当前菜单页的索引，指小光标在第几行
**           scrollBar： 滚动条所在位置
  */
void DisplayRefreashData(struct Menu_t *nowMenu,uint8_t selectItem,uint8_t scrollBar)
{
	int i = 0;
	for(i=0;i<(nowMenu->MenuProperty->MenuLen-nowMenu->MenuProperty->scrollBarLen);i++)
	{
		OLED_ShowString(8,i*16,nowMenu[i+scrollBar].displayString,16);
	}
	OLED_Refresh();
}
void GuiInit()
{
	MainUI.childrenMenu = menuMain;
	menuMain[1].childrenMenu = setMenu1;
	menuMain[2].childrenMenu = setMenu2;
	menuMain[3].childrenMenu = setMenu3;
	menuPoint = &MainUI;
	DisplayRefreash(menuPoint,selectItem,scrollBar);
}
/**
  * @name    GuiDataDisplayRefresh
  * @brief   菜单页的数据更新，记得一定要字符串占满15个字符
  * @param   :
  */
void GuiDataDisplayRefresh()
{
	if(menuPoint == setMenu1)
	{
		sprintf((char*)setMenu1[1].displayString,"M     %.1f     ",M);
//		sprintf((char*)setMenu1[2].displayString,"bird  %3d     ",count2);
//		sprintf((char*)setMenu1[3].displayString,"dog   %3d     ",count3);
//		sprintf((char*)setMenu1[4].displayString,"bow   %3d     ",count4);
//		sprintf((char*)setMenu1[5].displayString,"fish  %3d     ",count5);
		DisplayRefreashData(menuPoint,selectItem,scrollBar);
	}
	else if(menuPoint==setMenu2)
	{
		sprintf((char*)setMenu2[1].displayString,"M   %.1f      ",M);
		sprintf((char*)setMenu2[2].displayString,"Phase   %d    ",offset_phase);
//		sprintf((char*)setMenu2[2].displayString,"KI   %3d      ",ki);
//		sprintf((char*)setMenu2[3].displayString,"KD   %3d      ",kd);
//		sprintf((char*)setMenu2[4].displayString,"PWM  %3d      ",0);
		DisplayRefreashData(menuPoint,selectItem,scrollBar);
	}
	else if(menuPoint==setMenu3)
	{
		sprintf((char*)setMenu3[1].displayString,"Hour   %2d    ",myTimeTemp.Hour);
		sprintf((char*)setMenu3[2].displayString,"Minute %2d    ",myTimeTemp.Minute);
		sprintf((char*)setMenu3[3].displayString,"Second %2d    ",myTimeTemp.Second);
		sprintf((char*)setMenu3[4].displayString,"Year   %4d    ",myTimeTemp.Year);
		sprintf((char*)setMenu3[5].displayString,"Month  %2d    ",myTimeTemp.Month);
		sprintf((char*)setMenu3[6].displayString,"Day    %2d    ",myTimeTemp.Day);
		DisplayRefreashData(menuPoint,selectItem,scrollBar);
	}
	else if(menuPoint==&MainUI)
	{
		MainUiSet();
		OLED_Refresh();
	}
}
void GuiControl()
{
	if(isKeyUp==1)
	{
		isKeyUp=0;
		selectItem--;
		if(selectItem<0&&scrollBar!=0)
		{
			selectItem = 0;
			scrollBar--;
		}else if(selectItem<0&&scrollBar==0)
		{
			selectItem = menuPoint->MenuProperty->MenuLen-1-menuPoint->MenuProperty->scrollBarLen;
			scrollBar  = menuPoint->MenuProperty->scrollBarLen;
		}
		DisplayRefreash(menuPoint,selectItem,scrollBar);
	}
	else if(isKeyDown==1)
	{
		isKeyDown=0;
		selectItem++;
		if(selectItem>(menuPoint->MenuProperty->MenuLen-1-menuPoint->MenuProperty->scrollBarLen)&&scrollBar!=menuPoint->MenuProperty->scrollBarLen)
		{
			selectItem = menuPoint->MenuProperty->MenuLen-1-menuPoint->MenuProperty->scrollBarLen;
			scrollBar++;

		}
		else if(selectItem>(menuPoint->MenuProperty->MenuLen-1-menuPoint->MenuProperty->scrollBarLen)&&scrollBar==menuPoint->MenuProperty->scrollBarLen)
		{
			selectItem=0;
			scrollBar =0;
		}
		DisplayRefreash(menuPoint,selectItem,scrollBar);
	}
	else if(isKeyLeft==1)
	{
		if(menuPoint[selectItem].fatherMenu!=NULL){  //返回到父菜单
				menuPoint = menuPoint[selectItem].fatherMenu;

		}

		if(menuPoint[selectItem+scrollBar].func1!=NULL)
		{
			menuPoint[selectItem+scrollBar].func1();
		}
		isKeyLeft=0;
		DisplayRefreash(menuPoint,selectItem,scrollBar);
	}
	else if(isKeyRight==1)
	{
		if(selectItem==0 && scrollBar==0 && menuPoint[selectItem].fatherMenu!=NULL)//假如索引为零而且父菜单不为空，指向父指针
		{
			menuPoint = menuPoint[selectItem].fatherMenu;
		}
		else if(menuPoint[selectItem+scrollBar].childrenMenu!=NULL)//假如该索引子菜单页不为空，指向子菜单
		{
			if(menuPoint[selectItem+scrollBar].func2!=NULL)//假如当前菜单的func2不为空，执行相关函数
			{
				menuPoint[selectItem+scrollBar].func2();
			}
			menuPoint = menuPoint[selectItem+scrollBar].childrenMenu;
//			selectItem = 0;
		}
		else if(menuPoint[selectItem+scrollBar].func2!=NULL)//假如当前菜单的func2不为空，执行相关函数
		{
			menuPoint[selectItem+scrollBar].func2();
		}
		isKeyRight=0;
		DisplayRefreash(menuPoint,selectItem,scrollBar);
	}else if(isKeyOk==1)
	{
		isKeyOk=0;
		DisplayRefreash(menuPoint,selectItem,scrollBar);
	}
	GuiDataDisplayRefresh();
}
