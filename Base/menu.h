/*
 * menu.h
 *
 *  Created on: Apr 21, 2022
 *      Author: LX
 */

#ifndef MENU_H_
#define MENU_H_

#include "main.h"

struct MenuProperty_t
{
	uint8_t MenuLen;//当前菜单页菜单个数
	uint8_t scrollBarLen;//滚动条长度,由于都是用的16SIZE的字符，所以一个菜单页最多四个菜单，五个菜单滚动条就为1
};

//菜单结构体
struct Menu_t{
	struct MenuProperty_t *MenuProperty;
	uint8_t displayString[15];//当前菜单的字符
	void (*func1) (void);//当前菜单的功能函数
	void (*func2) (void);//当前菜单的功能函数
	struct Menu_t *fatherMenu;//当前菜单的父级菜单
	struct Menu_t *childrenMenu;//当前菜单的子级菜单
};

typedef struct
{
    uint8_t  Hour;
    uint8_t  Minute;
    uint8_t  Second;
    uint16_t Year;
    uint8_t  Month;
    uint8_t  Day;
}mytime_t;

extern uint16_t count1,count2,count3,count4,count5;

void GuiInit(void);
void GuiControl(void);
void DisplayRefreash(struct Menu_t *nowMenu,uint8_t selectItem,uint8_t scrollBar);
void DisplayRefreashData(struct Menu_t *nowMenu,uint8_t selectItem,uint8_t scrollBar);
void GuiDataDisplayRefresh(void);

#endif /* MENU_H_ */
