/******************************************************************************
*@file    example_gpio.h
*@brief   example of gpio operation
*
*  ---------------------------------------------------------------------------
*
*  Copyright (c) 2018 Quectel Technologies, Inc.
*  All Rights Reserved.
*  Confidential and Proprietary - Quectel Technologies, Inc.
*  ---------------------------------------------------------------------------
*******************************************************************************/
#ifndef __QUECTEL_GPIO_H__
#define __QUECTEL_GPIO_H__

#include "qapi_tlmm.h"

/*  !!! This Pin Enumeration Only Applicable BG96-OPEN Project !!!
 */
typedef enum{
	PIN_E_GPIO_01=0,
	PIN_E_GPIO_02,
	PIN_E_GPIO_03,
	PIN_E_GPIO_04,
	PIN_E_GPIO_05,
	PIN_E_GPIO_06,
	PIN_E_GPIO_07,
	PIN_E_GPIO_08,
	PIN_E_GPIO_09,
	PIN_E_GPIO_10,
	PIN_E_GPIO_11,
	PIN_E_GPIO_19,
	PIN_E_GPIO_20,
	PIN_E_GPIO_21,
	PIN_E_GPIO_MAX
}MODULE_PIN_ENUM;

typedef struct{
	uint32_t pin_num;
	char     *pin_name;
	uint32_t gpio_id;
	uint32_t gpio_func;
}GPIO_MAP_TBL;


#endif /*__QUECTEL_GPIO_H__*/

