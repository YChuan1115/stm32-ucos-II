#include "bsp_systick.h"
#include "stm32f10x.h"
#include "includes.h"

static uint8_t  fac_us=0;	
static uint16_t fac_ms=0;	

void SysTickConfig(void)
{

	uint32_t RELOAD=0;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//HCLK / 8
	fac_us = SystemCoreClock / 8000000; 

	RELOAD = SystemCoreClock / 8000000;	// cnt/s
	RELOAD *= 1000000 / OS_TICKS_PER_SEC;//RELOAD max 16777216
	fac_ms = 1000 / OS_TICKS_PER_SEC;
	
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	SysTick->LOAD = RELOAD;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void DelayUs(uint32_t nus)
{		
	uint32_t temp=0;
	uint32_t VAL_Prev=0;
	uint32_t VAL_Now=0; 
	uint32_t VAL_cnt=0;   
	uint32_t Reload=SysTick->LOAD;

	temp = nus*fac_us; 
	VAL_Prev = SysTick->VAL;
	
	while(1)
	{
		VAL_Now = SysTick->VAL;
		if(VAL_Now != VAL_Prev)
		{
			if(VAL_Now < VAL_Prev) 
				{
					VAL_cnt += VAL_Prev-VAL_Now;  
				}
			else                     
				{
					VAL_cnt += Reload - VAL_Now + VAL_Prev;
				}
			VAL_Prev = VAL_Now; 
			if(VAL_cnt >= temp)  break;
		}
	};

}

void DelayMs(uint32_t nms)
{
	if(OSRunning == 1) // os is running
	{
		if(nms > fac_ms) 
		{
			OSTimeDly(nms/fac_ms);
		}
		nms %= fac_ms;
	}
	DelayUs((uint32_t)(nms*1000));
}





/*
void SysTickConfig(void)
{
	uint32_t OS_reload = 0;
	OS_reload=Reload_1ms*(1000/OS_TICKS_PER_SEC);
	if(SysTick_Config(OS_reload) ==1)
		{
			while(1);
		}
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;// SysTick_CTRL_ENABLE_Msk; 
}

// delay ms 
void DelayMs(uint32_t time)
{
	if(time>=OS_tick_ms)
		{
			OSTimeDly(time/(OS_tick_ms));
		}
	time=time%OS_tick_ms;
	DelayUs(time*1000);

}

//delay us  
void DelayUs(uint32_t time)
{
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;
	ticks = time*Reload_1us;
	told = SysTick->VAL;
	while(1)
	{
		tnow = SysTick->VAL;
		if(tnow < told)
			{
				tcnt = tcnt + told - tnow;
			}
		else
			{
				tcnt = tcnt + reload - tnow + told;
			}
		told = tnow;
		if(tcnt >= ticks) break;

	}
}
*/
