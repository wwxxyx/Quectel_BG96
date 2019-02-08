/******************************************************************************
*@file    example_adc.h
*@brief   read value of ADC0 and ADC1
*  ---------------------------------------------------------------------------
*
*  Copyright (c) 2018 Quectel Technologies, Inc.
*  All Rights Reserved.
*  Confidential and Proprietary - Quectel Technologies, Inc.
*  ---------------------------------------------------------------------------
*******************************************************************************/
#ifndef __EXAMPLE_ADC_H__
#define __EXAMPLE_ADC_H__

#if defined(__EXAMPLE_ADC__)
void adc_open_handle(void);
void adc_get_properties(const char *Channel_Name_Ptr,qapi_Adc_Input_Properties_Type_t *Properties_Ptr);

#endif /*__EXAMPLE_ADC__*/

#endif /*__EXAMPLE_ADC_H__*/

