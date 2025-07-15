#include "key4.h"

void KEY_4x4_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    // 初始化行线为推挽输出
    GPIO_InitStruct.Pin = ROW1_PIN|ROW2_PIN|ROW3_PIN|ROW4_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(ROW_PORT, &GPIO_InitStruct);

    // 初始化列线为上拉输入
    GPIO_InitStruct.Pin = COL1_PIN|COL2_PIN|COL3_PIN|COL4_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(COL_PORT, &GPIO_InitStruct);

    // 行线默认输出高电平
    HAL_GPIO_WritePin(ROW_PORT, ROW1_PIN|ROW2_PIN|ROW3_PIN|ROW4_PIN, GPIO_PIN_SET);
}
void KEY_4x4_Scan(uint8_t *key)
{
    static uint8_t anxian = 0;
    
    // 第一行扫描
    HAL_GPIO_WritePin(ROW_PORT, ROW1_PIN|ROW2_PIN|ROW3_PIN|ROW4_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW_PORT, ROW1_PIN, GPIO_PIN_RESET);
    if(HAL_GPIO_ReadPin(COL_PORT, COL1_PIN) == GPIO_PIN_RESET)
    {
        HAL_Delay(10);
        if(HAL_GPIO_ReadPin(COL_PORT, COL1_PIN) == GPIO_PIN_RESET)
        {
            anxian = 1;
            *key = KEY_1;
            while(HAL_GPIO_ReadPin(COL_PORT, COL1_PIN) == GPIO_PIN_RESET);
        }
    }
    else if(HAL_GPIO_ReadPin(COL_PORT, COL2_PIN) == GPIO_PIN_RESET)
    {
        HAL_Delay(10);
        if(HAL_GPIO_ReadPin(COL_PORT, COL2_PIN) == GPIO_PIN_RESET)
        {
            anxian = 1;
            *key = KEY_2;
            while(HAL_GPIO_ReadPin(COL_PORT, COL2_PIN) == GPIO_PIN_RESET);
        }
    }
    else if(HAL_GPIO_ReadPin(COL_PORT, COL3_PIN) == GPIO_PIN_RESET)
    {
        HAL_Delay(10);
        if(HAL_GPIO_ReadPin(COL_PORT, COL3_PIN) == GPIO_PIN_RESET)
        {
            anxian = 1;
            *key = KEY_3;
            while(HAL_GPIO_ReadPin(COL_PORT, COL3_PIN) == GPIO_PIN_RESET);
        }
    }
    else if(HAL_GPIO_ReadPin(COL_PORT, COL4_PIN) == GPIO_PIN_RESET)
    {
        HAL_Delay(10);
        if(HAL_GPIO_ReadPin(COL_PORT, COL4_PIN) == GPIO_PIN_RESET)
        {
            anxian = 1;
//            *key = KEY_A;
					  *key = KEY_4;
            while(HAL_GPIO_ReadPin(COL_PORT, COL4_PIN) == GPIO_PIN_RESET);
        }
    }
    
    // 第二行扫描
    HAL_GPIO_WritePin(ROW_PORT, ROW1_PIN|ROW2_PIN|ROW3_PIN|ROW4_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW_PORT, ROW2_PIN, GPIO_PIN_RESET);
    if(HAL_GPIO_ReadPin(COL_PORT, COL1_PIN) == GPIO_PIN_RESET)
    {
        HAL_Delay(10);
        if(HAL_GPIO_ReadPin(COL_PORT, COL1_PIN) == GPIO_PIN_RESET)
        {
            anxian = 1;
//            *key = KEY_4;
            *key = KEY_5;
            while(HAL_GPIO_ReadPin(COL_PORT, COL1_PIN) == GPIO_PIN_RESET);
        }
    }
    else if(HAL_GPIO_ReadPin(COL_PORT, COL2_PIN) == GPIO_PIN_RESET)
    {
        HAL_Delay(10);
        if(HAL_GPIO_ReadPin(COL_PORT, COL2_PIN) == GPIO_PIN_RESET)
        {
            anxian = 1;
//            *key = KEY_5;
            *key = KEY_6;
            while(HAL_GPIO_ReadPin(COL_PORT, COL2_PIN) == GPIO_PIN_RESET);
        }
    }
    else if(HAL_GPIO_ReadPin(COL_PORT, COL3_PIN) == GPIO_PIN_RESET)
    {
        HAL_Delay(10);
        if(HAL_GPIO_ReadPin(COL_PORT, COL3_PIN) == GPIO_PIN_RESET)
        {
            anxian = 1;
//            *key = KEY_6;
				    *key = KEY_7;
            while(HAL_GPIO_ReadPin(COL_PORT, COL3_PIN) == GPIO_PIN_RESET);
        }
    }
    else if(HAL_GPIO_ReadPin(COL_PORT, COL4_PIN) == GPIO_PIN_RESET)
    {
        HAL_Delay(10);
        if(HAL_GPIO_ReadPin(COL_PORT, COL4_PIN) == GPIO_PIN_RESET)
        {
            anxian = 1;
//            *key = KEY_B;
            *key = KEY_8;
            while(HAL_GPIO_ReadPin(COL_PORT, COL4_PIN) == GPIO_PIN_RESET);
        }
    }
    
    // 第三行扫描
    HAL_GPIO_WritePin(ROW_PORT, ROW1_PIN|ROW2_PIN|ROW3_PIN|ROW4_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW_PORT, ROW3_PIN, GPIO_PIN_RESET);
    if(HAL_GPIO_ReadPin(COL_PORT, COL1_PIN) == GPIO_PIN_RESET)
    {
        HAL_Delay(10);
        if(HAL_GPIO_ReadPin(COL_PORT, COL1_PIN) == GPIO_PIN_RESET)
        {
            anxian = 1;
//            *key = KEY_7;
            *key = KEY_9;

            while(HAL_GPIO_ReadPin(COL_PORT, COL1_PIN) == GPIO_PIN_RESET);
        }
    }
    else if(HAL_GPIO_ReadPin(COL_PORT, COL2_PIN) == GPIO_PIN_RESET)
    {
        HAL_Delay(10);
        if(HAL_GPIO_ReadPin(COL_PORT, COL2_PIN) == GPIO_PIN_RESET)
        {
            anxian = 1;
//            *key = KEY_8;
            *key = KEY_0;
            while(HAL_GPIO_ReadPin(COL_PORT, COL2_PIN) == GPIO_PIN_RESET);
        }
    }
    else if(HAL_GPIO_ReadPin(COL_PORT, COL3_PIN) == GPIO_PIN_RESET)
    {
        HAL_Delay(10);
        if(HAL_GPIO_ReadPin(COL_PORT, COL3_PIN) == GPIO_PIN_RESET)
        {
            anxian = 1;
//            *key = KEY_9;
            *key = KEY_A;
            while(HAL_GPIO_ReadPin(COL_PORT, COL3_PIN) == GPIO_PIN_RESET);
        }
    }
    else if(HAL_GPIO_ReadPin(COL_PORT, COL4_PIN) == GPIO_PIN_RESET)
    {
        HAL_Delay(10);
        if(HAL_GPIO_ReadPin(COL_PORT, COL4_PIN) == GPIO_PIN_RESET)
        {
            anxian = 1;
            *key = KEY_B;
            while(HAL_GPIO_ReadPin(COL_PORT, COL4_PIN) == GPIO_PIN_RESET);
        }
    }
    
    // 第四行扫描
    HAL_GPIO_WritePin(ROW_PORT, ROW1_PIN|ROW2_PIN|ROW3_PIN|ROW4_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW_PORT, ROW4_PIN, GPIO_PIN_RESET);
    if(HAL_GPIO_ReadPin(COL_PORT, COL1_PIN) == GPIO_PIN_RESET)
    {
        HAL_Delay(10);
        if(HAL_GPIO_ReadPin(COL_PORT, COL1_PIN) == GPIO_PIN_RESET)
        {
            anxian = 1;
//            *key = KEY_STAR;
            *key = KEY_C;
            while(HAL_GPIO_ReadPin(COL_PORT, COL1_PIN) == GPIO_PIN_RESET);
        }
    }
    else if(HAL_GPIO_ReadPin(COL_PORT, COL2_PIN) == GPIO_PIN_RESET)
    {
        HAL_Delay(10);
        if(HAL_GPIO_ReadPin(COL_PORT, COL2_PIN) == GPIO_PIN_RESET)
        {
            anxian = 1;
//            *key = KEY_0;
            *key = KEY_D;
            while(HAL_GPIO_ReadPin(COL_PORT, COL2_PIN) == GPIO_PIN_RESET);
        }
    }
    else if(HAL_GPIO_ReadPin(COL_PORT, COL3_PIN) == GPIO_PIN_RESET)
    {
        HAL_Delay(10);
        if(HAL_GPIO_ReadPin(COL_PORT, COL3_PIN) == GPIO_PIN_RESET)
        {
            anxian = 1;
//            *key = KEY_HASH;
            *key = KEY_STAR;
            while(HAL_GPIO_ReadPin(COL_PORT, COL3_PIN) == GPIO_PIN_RESET);
        }
    }
    else if(HAL_GPIO_ReadPin(COL_PORT, COL4_PIN) == GPIO_PIN_RESET)
    {
        HAL_Delay(10);
        if(HAL_GPIO_ReadPin(COL_PORT, COL4_PIN) == GPIO_PIN_RESET)
        {
            anxian = 1;
//            *key = KEY_D;
					  *key = KEY_MODE;

            while(HAL_GPIO_ReadPin(COL_PORT, COL4_PIN) == GPIO_PIN_RESET);
        }
    }
    
    if(anxian == 0)
    {
        *key = KEY_NONE;
    }
}
