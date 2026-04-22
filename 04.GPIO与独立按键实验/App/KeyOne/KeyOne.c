/*********************************************************************************************************
* 模块名称：KeyOne.c
* 摘    要：KeyOne模块，进行独立按键初始化，以及按键扫描函数实现
* 当前版本：1.0.0
* 作    者：SZLY(COPYRIGHT 2018 - 2020 SZLY. All rights reserved.)
* 完成日期：2020年01月01日  
* 内    容：
* 注    意：                                                                  
**********************************************************************************************************
* 取代版本：
* 作    者：
* 完成日期：
* 修改内容：
* 修改文件：
*********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "KeyOne.h"
#include "stm32f10x_conf.h"
#include "UART1.h"
/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/
 #define KEY1 (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1))
 #define KEY2 (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2))
 #define KEY3 (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0))
 static u8 s_arrKeyDownLevel[KEY_NAME_MAX];//在InitKeyOne函数中先初始化
/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/
static void ConfigKeyOneGPIO(void);
/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：
* 函数功能：
* 输入参数：
* 输出参数：
* 返 回 值：
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
static void ConfigKeyOneGPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
  
  //配置PC1
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
  GPIO_Init(GPIOC,&GPIO_InitStructure);
  
  //配置PC2
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
  GPIO_Init(GPIOC,&GPIO_InitStructure);
  
  //配置PA0
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
}
/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：
* 函数功能：
* 输入参数：
* 输出参数：
* 返 回 值：
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
void InitKeyOne(void)
{
  ConfigKeyOneGPIO();
  
  s_arrKeyDownLevel[KEY_NAME_KEY1]=KEY_DOWN_LEVEL_KEY1;
  s_arrKeyDownLevel[KEY_NAME_KEY2]=KEY_DOWN_LEVEL_KEY2;
  s_arrKeyDownLevel[KEY_NAME_KEY3]=KEY_DOWN_LEVEL_KEY3;
}
//按键扫描//每10ms调用一次
void ScanKeyOne(u8 keyName,void(*OnKeyOneUp)(void),void(*OnKeyOneDown)(void))
{
  static u8 s_arrKeyVal[KEY_NAME_MAX];
  static u8 s_arrKeyFlag[KEY_NAME_MAX];
  
  s_arrKeyVal[keyName]=s_arrKeyVal[keyName]<<1;
  
  switch(keyName)
  {
    case KEY_NAME_KEY1:s_arrKeyVal[keyName]=s_arrKeyVal[keyName]|KEY1;break;
    case KEY_NAME_KEY2:s_arrKeyVal[keyName]=s_arrKeyVal[keyName]|KEY2;break;
    case KEY_NAME_KEY3:s_arrKeyVal[keyName]=s_arrKeyVal[keyName]|KEY3;break;
    default:break;
  }

//按键为TRUE时判断是否有效按下
  if(s_arrKeyVal[keyName]==s_arrKeyDownLevel[keyName]&&s_arrKeyFlag[keyName]==TRUE)
  {
    (*OnKeyOneDown)();
    s_arrKeyFlag[keyName]=FALSE;
  }
//按键为FALSE时判断是否有效松开
  else if(s_arrKeyVal[keyName]==(u8)(~s_arrKeyDownLevel[keyName])&&s_arrKeyFlag[keyName]==FALSE)
  {
    (*OnKeyOneUp)();
    s_arrKeyFlag[keyName]=TRUE;
  }
}
