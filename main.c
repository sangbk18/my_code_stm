#include "MY_RTOS.h"
#include "My_application_stm32f1xx.h"
#include "My_sensors_lib.h"
typedef enum
{
	_CASE_0 = 0U,
	_CASE_1,
	_CASE_2,
	_CASE_3,
	_CASE_4,
	_CASE_5,
}TYPE_FUNCTION_Typedef;
void gpio_config(void);
__attribute__((noreturn)) void my_task_ide(void);
__attribute__((noreturn)) void my_task_1(void);
__attribute__((noreturn)) void my_task_2(void);
__attribute__((noreturn)) void my_task_3(void);
my_func_tasks ARR_MY_FUNCTION[MAX_TASKS] = {my_task_ide,my_task_1,my_task_2,my_task_3};
void SysTick_Handler(void);
void PendSV_Handler(void);

volatile MENU_Typedef* MENU_ACTIVE = &MAIN_MENU;
static volatile uint8_t position_current = 2U;
volatile DATA_DHT11_Typedef data_dht11 = {10U,10U,10U,10U,10U};
FUNC_DHT11_Typedef DHT11_INITIAL = {(TIM_TypeDef*)TIM1_ADRESS,(GPIO_TypeDef*)GPIOA_ADRESS,(DHT11_PIN_Typedef)GPIO_0,&data_dht11,(I2C_TypeDef*)_I2C1_ADRESS};
DATA_THRESOLD_Typedef data_thresold;
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
/*task4*/
void EXTI0_IRQHandler(void)
{
	if(EXTI->PR&(1U<<0))
	{
	//	value_ext[0] += 1U;
		/*----------------*/
		if(MENU_ACTIVE->p_tail != NULL)
		{
			MENU_ACTIVE = MENU_ACTIVE->p_tail;
		}
		hienthi((I2C_TypeDef*)_I2C1_ADRESS,MENU_ACTIVE,position_current);
		EXTI->PR |= (1U<<0);
	}
}
void EXTI1_IRQHandler(void)
{
	if(EXTI->PR&(1U<<1))
	{
	//	value_ext[1] += 1U;
		/*----------------*/
		if(MENU_ACTIVE->ID == ID_THRESOLD_1)
		{
	   FLash_read_half_word(adress_flash_start,(volatile uint32_t*)&data_thresold,2U);
			switch(position_current)
			{
				case 2:
					(data_thresold.THRESOLD_TEM_VAL < 40U) ? (data_thresold.THRESOLD_TEM_VAL += 1U) : (data_thresold.THRESOLD_TEM_VAL = 20);
					break;
				case 3:
					(data_thresold.THRESOLD_HUM_VAL < 60U) ? (data_thresold.THRESOLD_HUM_VAL += 1U) : (data_thresold.THRESOLD_HUM_VAL = 40);
					break;
				default:
					break;
			}
			FLash_erase(adress_flash_start);
			volatile uint16_t data_tmp[2] = {(volatile uint16_t)data_thresold.THRESOLD_TEM_VAL,(volatile uint16_t)data_thresold.THRESOLD_HUM_VAL};
			Flash_write(adress_flash_start,&data_tmp[0],4U);
			hienthi((I2C_TypeDef*)_I2C1_ADRESS,MENU_ACTIVE,position_current);
		}
		EXTI->PR |= (1U<<1);
	}
}
void EXTI2_IRQHandler(void)
{
	if(EXTI->PR&(1U<<2))
	{
	//	value_ext[2] += 1U;
		/*----------------*/
		if(position_current < 3U)
		{
			position_current = 4U;
		}
		else position_current -= 1U;
		hienthi((I2C_TypeDef*)_I2C1_ADRESS,MENU_ACTIVE,position_current);
		/*----------------*/
		EXTI->PR |= (1U<<2);
	}
}
void EXTI3_IRQHandler(void)
{
	if(EXTI->PR&(1U<<3))
	{
	//	value_ext[3] += 1U;
		/*----------------*/
		switch(position_current)
		{
			case 2:
				if(MENU_ACTIVE->p_menu_1 != NULL)
				{
					MENU_ACTIVE = MENU_ACTIVE->p_menu_1;
				}
				break;
			case 3:
				if(MENU_ACTIVE->p_menu_2 != NULL)
				{
					MENU_ACTIVE = MENU_ACTIVE->p_menu_2;
				}
				break;
			case 4:
				if(MENU_ACTIVE->p_menu_3 != NULL)
				{
					MENU_ACTIVE = MENU_ACTIVE->p_menu_3;
				}
				break;
			default:
				break;
		}
		hienthi((I2C_TypeDef*)_I2C1_ADRESS,MENU_ACTIVE,position_current);
		EXTI->PR |= (1U<<3);
	}
}
void EXTI4_IRQHandler(void)
{
	if(EXTI->PR&(1U<<4))
	{
		//value_ext[4] += 1U;
		/*----------------*/
		if(position_current > 3U)
		{
			position_current = 2U;
		}
		else position_current += 1U;
		hienthi((I2C_TypeDef*)_I2C1_ADRESS,MENU_ACTIVE,position_current);
		EXTI->PR |= (1U<<4);
	}
}
/*task4*/
//static Offset_interrupt total_interrupt[5U] = {_EXTI0_IQR,_EXTI1_IQR,_EXTI2_IQR,_EXTI3_IQR,_EXTI4_IQR};
//static my_func total_func_interrupt[5U] = {ngat_ext_0,ngat_ext_1,ngat_ext_2,ngat_ext_3,ngat_ext_4};
int main(void)
{
  enable_faults_exceptions();
	initial_stack_handler_mode(MSP_STACK);
	initial_stacks();
	thread_mode_using_psp();
	gpio_config();
	systick_config();
	NVIC_SetPriority(SysTick_IRQn,0U);
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(PendSV_IRQn,10U);
	my_task_1();
	
}
void SysTick_Handler(void)
{
	g_count++;
	uint8_t dem = 0U;
	for(dem = 1U; dem < MAX_TASKS; dem++)
	{
		if(TASKS[dem].STATUS != STATE_READY)
		{
       if(TASKS[dem].TIME_BLOCK == g_count)
			 {
				 TASKS[dem].STATUS = STATE_READY;
			 }
		}
	}
	SCB->ICSR |= (1U<<28);
}
void gpio_config(void)
{
	RCC->APB2ENR |= (1U<<2);
	GPIOA->CRH &= ~(15U<<0);
	GPIOA->CRH &= ~(15U<<12);
	GPIOA->CRH |= (3U<<0);
	GPIOA->CRH |= (3U<<12);
}
void my_task_ide(void)
{
	while(1)
	{
		
	}
}
void my_task_1(void)
{
	while(1)
	{
		GPIOA->ODR |= (1U<<8);
		task_delay(500);
		GPIOA->ODR &= ~(1U<<8);
		task_delay(500);
	}	
}
void my_task_2(void)
{
	while(1)
	{
		GPIOA->ODR |= (1U<<11);
		task_delay(1000);
		GPIOA->ODR &= ~(1U<<11);
		task_delay(1000);
	}
}
void my_task_3(void)
{
	i2c_init((I2C_TypeDef*)_I2C1_ADRESS);
	lcd_init((I2C_TypeDef*)_I2C1_ADRESS);
	hienthi((I2C_TypeDef*)_I2C1_ADRESS,MENU_ACTIVE,position_current);
	EXT_config();
	NVIC_SetPriority(EXTI0_IRQn,1U);
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_SetPriority(EXTI1_IRQn,2U);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_SetPriority(EXTI2_IRQn,2U);
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_SetPriority(EXTI3_IRQn,2U);
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_SetPriority(EXTI4_IRQn,2U);
	NVIC_EnableIRQ(EXTI4_IRQn);
	while(1)
	{
		if(MENU_ACTIVE->my_active_1[0] != NULL)
		{
		   ((void (*)(void *,TYPE_Typedef))(MENU_ACTIVE->my_active_1[0]))(&DHT11_INITIAL,_DHT11_FUNC_TYPEDEF);	
        task_delay(5000);			
		}
		if(MENU_ACTIVE->ID == ID_THRESOLD_1)
		{
			FLash_read_half_word(adress_flash_start,(volatile uint32_t*)&data_thresold,2U);/*avoid overflow flash*/
		}
		if(data_dht11.nhiet_do_in > data_thresold.THRESOLD_TEM_VAL)		
		{
			/*implement task */
		}				 
		else if(data_dht11.nhiet_do_in < data_thresold.THRESOLD_TEM_VAL)	
		{
      /*perform task*/
		}
	}
}
