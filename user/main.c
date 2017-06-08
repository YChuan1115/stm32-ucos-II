#include "stm32f10x.h"  //mcu
#include "stm32f10x_it.h"
#include "bsp.h"		//bsp
#include "includes.h" //os 
#include "app.h"		//app


/* 起始任务相关设置 */
//任务优先级
#define Start_Task_PRIO          10  //优先级最低
 
//任务堆栈大小
#define Start_STK_Size            64
 
//任务堆栈空间大小
OS_STK Start_Task_STK[Start_STK_Size];
 
//任务接口函数
void start_task(void *pdata);
 
 
/* LED0任务0 */
//任务优先级
#define LED0_Task_PRIO          5 //优先级最低
 
//任务堆栈大小
#define LED0_STK_Size            64
 
//任务堆栈空间大小
OS_STK LED0_Task_STK[LED0_STK_Size];
 
//任务接口函数
void LED0_task(void *pdata);
 
 
/* LED1任务1 */
//任务优先级
#define LED1_Task_PRIO          6  //优先级最低
 
//任务堆栈大小
#define LED1_STK_Size            64
 
//任务堆栈空间大小
OS_STK LED1_Task_STK[LED1_STK_Size];
 
//任务接口函数
void LED1_task(void *pdata);

int main (void)
{
	
	USART1_Config();
	printf("usart init ok...\r\n");	
	
	LEDInit();
	printf("led init ok...\r\n");
	
	SysTickConfig();
	NVIC_Config();
	printf("os init ok...\r\n");
	
	OSInit();
	OSTaskCreate(start_task, (void *)0, (OS_STK *)&Start_Task_STK[Start_STK_Size-1], Start_Task_PRIO);  //创建起始任务
	OSStart();  //ucos启动
	
//	LEDON(2);
//	while(1)
//	{
//		
//	}
//	return 0;
}



/******************************************************************************
* Function Name --> 开始任务
* Description   --> none
* Input         --> *pdata: 任务指针
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void start_task(void * pdata)
{
OS_CPU_SR cpu_sr=0;
 
pdata = pdata;
OS_ENTER_CRITICAL();  //进入临界区，无法被中断打断

printf("\r\n start task\r\n");
	
OSTaskCreate(LED0_task, (void *)0, (OS_STK *)&LED0_Task_STK[LED0_STK_Size-1], LED0_Task_PRIO);
OSTaskCreate(LED1_task, (void *)0, (OS_STK *)&LED1_Task_STK[LED1_STK_Size-1], LED1_Task_PRIO);

OSTaskSuspend(Start_Task_PRIO);  //挂起起始任务
OS_EXIT_CRITICAL();  //退出临界区，可以被中断打断
}
/******************************************************************************
* Function Name --> 任务0
* Description   --> none
* Input         --> *pdata: 任务指针
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void LED0_task(void * pdata)
{
while(1)
{
	printf("this is task1 \r\n");
	LEDON(3);
	DelayMs(50);
  LEDOFF(3);
	DelayMs(1000);
}
}
/******************************************************************************
* Function Name --> 任务1
* Description   --> none
* Input         --> *pdata: 任务指针
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void LED1_task(void * pdata)
{
while(1)
{
	printf("this is task2 \r\n");
	LEDON(1);
	DelayMs(50);
  LEDOFF(1);
	DelayMs(2500);
}
}
