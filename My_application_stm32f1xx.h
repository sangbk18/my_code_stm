#ifndef _MY_APPLICATION_STM32_
#define _MY_APPLICATION_STM32_

#include "my_diver_stm32f1xx.h"
#include "My_sensors_lib.h"
/*dung de ep kieu cac loai function pointer*/

/*dam bao do chinh xac cua type_typedef*/

typedef enum
{
	ID_MAIN_MENU = 0u,
	ID_GARDEN_1,
	ID_GARDEN_2,
	ID_GARDEN_3,
	ID_SENSORS_1,
	ID_ACTUATORS_1,
	ID_SETTING_1,
	ID_DHT11_1,
	ID_THRESOLD_1,
	ID_OUTHOUSE_1,
}ID_Typedef;
#define IS_ID_TYPEDEF(ID) (((ID) == ID_MAIN_MENU)    ||\
                            ((ID) == ID_GARDEN_1)    ||\
														((ID) == ID_GARDEN_2)    ||\
														((ID) == ID_GARDEN_3)    ||\
														((ID) == ID_SENSORS_1)   ||\
														((ID) == ID_ACTUATORS_1) ||\
														((ID) == ID_SETTING_1)  ||\
                            ((ID) == ID_OUTHOUSE_1))
/*=====================================define_fuction_pointer======================================*/
extern volatile DATA_DHT11_Typedef data_dht11;
struct _my_struct_pointer_dht11
{
	 TIM_TypeDef *time_dht11_used;
	 GPIO_TypeDef *gpio_dht11_used;
	 DHT11_PIN_Typedef pin_dht11_used;
   volatile DATA_DHT11_Typedef *data_dht11;
	 I2C_TypeDef* I2C;
};
typedef struct _my_struct_pointer_dht11 FUNC_DHT11_Typedef;
extern FUNC_DHT11_Typedef DHT11_INITIAL;
typedef enum
{
	_DHT11_FUNC_TYPEDEF = 0U,
}TYPE_Typedef;
extern void my_fuction_dht_implement(void *p,TYPE_Typedef type);
/*=====================================define_fuction_pointer======================================*/
struct my_menu
{
	volatile ID_Typedef ID;
	volatile struct my_menu *p_tail;
	const char *title_main;
	const char *title_node_1;
	volatile struct my_menu *p_menu_1;void (*my_active_1[3])(void *p,TYPE_Typedef type);
	const char *title_node_2;
	volatile struct my_menu *p_menu_2;void (*my_active_2[3])(void *p,TYPE_Typedef type);
	const char *title_node_3;
	volatile struct my_menu *p_menu_3;void (*my_active_3[3])(void *p,TYPE_Typedef type);
};
typedef struct my_menu MENU_Typedef;
extern MENU_Typedef MAIN_MENU,GARDEN_1,GARDEN_2,GARDEN_3,SENSORS_1,ACTUATORS_1,SETTING_1,DHT11_1,THRESOLD_1,OUTHOUSE_1;
extern volatile MENU_Typedef* MENU_ACTIVE;
typedef enum
{
	_LINE_1 = 2U,
	_LINE_2,
	_LINE_3,
}POSITION_Typedef;
#define _IS_POSITION_TYPEDEF_(vt) (((vt)==_LINE_1) ||\
                                   ((vt)==_LINE_2) ||\
                                   ((vt)==_LINE_3))
extern volatile uint32_t threshold_tem,thresold_hum;
extern char S_SAMPLE[20];
/*==========================================define_thresold================================================*/
typedef enum
{
	ID_THRESOLD_TEM = 0U,
	ID_THRESOLD_HUM,
}ID_THRESOLD_Typedef;
#define IS_ID_THRESOLD_(x) (((x) == ID_THRESOLD_TEM) ||\
                            ((x) == ID_THRESOLD_HUM))
struct _data_thresold
{
	volatile uint32_t THRESOLD_TEM_VAL;
	volatile uint32_t THRESOLD_HUM_VAL;
};
typedef struct _data_thresold DATA_THRESOLD_Typedef;
extern DATA_THRESOLD_Typedef data_thresold;
/*==========================================define_thresold================================================*/
void hienthi(I2C_TypeDef* I2C,volatile MENU_Typedef *menu,volatile uint8_t position);
#endif
