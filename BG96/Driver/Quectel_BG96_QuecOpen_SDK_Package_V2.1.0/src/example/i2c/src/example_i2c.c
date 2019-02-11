/******************************************************************************
*@file    example_i2c.c
*@brief   example of i2c
*
*  ---------------------------------------------------------------------------
*
*  Copyright (c) 2018 Quectel Technologies, Inc.
*  All Rights Reserved.
*  Confidential and Proprietary - Quectel Technologies, Inc.
*  ---------------------------------------------------------------------------
*******************************************************************************/
#if defined(__EXAMPLE_I2C__)
#include "txm_module.h"
#include "qapi_diag.h"
#include "qapi_timer.h"
#include "qapi_uart.h"
#include "quectel_utils.h"
#include "quectel_uart_apis.h"
#include "qapi_fs_types.h"
#include "qapi_fs.h"
#include "example_i2c.h"
#include "qapi_i2c_master.h"

/**************************************************************************
*                                 DEFINE
***************************************************************************/
#define QUEC_I2C_UART_DBG
#ifdef QUEC_I2C_UART_DBG
#define I2C_UART_DBG(...)	\
{\
	i2c_uart_debug_print(__VA_ARGS__);	\
}
#else
#define I2C_UART_DBG(...)
#endif

/**************************************************************************
*                                 GLOBAL
***************************************************************************/

void    *client_handle = NULL;
uint8   rbuf[2];

/*===========================================================================

                           Static & global Variable Declarations

===========================================================================*/

/* Uart Dbg */
qapi_UART_Handle_t i2c_dbg_uart_handler;
static char quec_dbg_buffer[128];

void i2c_uart_dbg_init(void)
{
	qapi_Status_t status;
	qapi_UART_Open_Config_t uart_cfg;

	uart_cfg.baud_Rate			= 115200;
	uart_cfg.enable_Flow_Ctrl	= QAPI_FCTL_OFF_E;
	uart_cfg.bits_Per_Char		= QAPI_UART_8_BITS_PER_CHAR_E;
	uart_cfg.enable_Loopback	= 0;
	uart_cfg.num_Stop_Bits		= QAPI_UART_1_0_STOP_BITS_E;
	uart_cfg.parity_Mode		= QAPI_UART_NO_PARITY_E;
	uart_cfg.rx_CB_ISR			= NULL;
	uart_cfg.tx_CB_ISR			= NULL;

	status = qapi_UART_Open(&i2c_dbg_uart_handler, QAPI_UART_PORT_002_E, &uart_cfg);
	if (QAPI_OK != status)
	{
		return;
	}
	status = qapi_UART_Power_On(i2c_dbg_uart_handler);
}

void i2c_uart_debug_print(const char* fmt, ...) 
{
	va_list arg_list;

	va_start(arg_list, fmt);
    vsnprintf((char *)(quec_dbg_buffer), sizeof(quec_dbg_buffer), (char *)fmt, arg_list);
    va_end(arg_list);
		
	qapi_UART_Transmit(i2c_dbg_uart_handler, quec_dbg_buffer, strlen(quec_dbg_buffer), NULL);
	qapi_UART_Transmit(i2c_dbg_uart_handler, "\r\n", strlen("\r\n"), NULL);
	qapi_Timer_Sleep(10, QAPI_TIMER_UNIT_MSEC, true);
}

// Transfer completed
int32 CB_Parameter;
void client_callback (const uint32 status, void *cb_para)
{

}

/******************************************** 
** <S>  - START bit
** <P>  - STOP  bit
** <Sr> - Repeat Start bit
** <A>  - Acknowledge bit
** <N>  - Not-Acknowledge bit
** <R>  - Read Transfer
** <W> - Write Transfer
********************************************/
qapi_Status_t i2c_read (uint8 slave_Address, uint8 reg)
{
   qapi_Status_t res = QAPI_OK;
   qapi_I2CM_Config_t config;
   qapi_I2CM_Descriptor_t desc[2];
   uint32  transferred1 = 0, transferred2 = 0;

   I2C_UART_DBG("i2c_read: slave_addr[0x%x], reg[0x%x]\r\n", slave_Address, reg);

   // Configure the bus speed and slave address
   config.bus_Frequency_KHz = 50; 
   config.slave_Address     = slave_Address;
   config.SMBUS_Mode        = 0;
   config.slave_Max_Clock_Stretch_Us = 100000;
   config.core_Configuration1 = 0;
   config.core_Configuration2 = 0;

   // Read N bytes from a register 0x01 on a sensor device
   // <S><slave_address><W><A><register><A><S><slave_address><R><A><data1><A><data2><A>...<dataN><N><P>
   desc[0].buffer      = &reg;
   desc[0].length      = 1;
   desc[0].transferred = (uint32)&transferred1;
   desc[0].flags       = QAPI_I2C_FLAG_START | QAPI_I2C_FLAG_WRITE | QAPI_I2C_FLAG_STOP;
   res = qapi_I2CM_Transfer (client_handle, &config, &desc[0], 1, client_callback, &CB_Parameter, 100); 

   desc[1].buffer      = rbuf;
   desc[1].length      = 2;
   desc[1].transferred = (uint32)&transferred2;
   desc[1].flags       = QAPI_I2C_FLAG_START | QAPI_I2C_FLAG_READ  | QAPI_I2C_FLAG_STOP;

   res = qapi_I2CM_Transfer (client_handle, &config, &desc[1], 1, client_callback, &CB_Parameter, 100);   

   return res;
}

qapi_Status_t i2c_write (uint8 slave_Address, uint8 reg, uint8 *wbuf, uint8 len)
{
   qapi_Status_t res = QAPI_OK;
   qapi_I2CM_Config_t config;
   qapi_I2CM_Descriptor_t desc[2];
   uint8   buffer[4], i;
   uint32  transferred1 = 0;

   buffer[0] = reg; // first byte is register addr
   for (i = 0; i < len; ++i)
   {
       buffer[i + 1] = wbuf[i];
   }

   I2C_UART_DBG("i2c_write: slave_addr[0x%x], reg[0x%x], byte1[0x%x], byte2[0x%x]\r\n", slave_Address, buffer[0], buffer[1], buffer[2]);

   // Configure the bus speed and slave address
   config.bus_Frequency_KHz = 50; 
   config.slave_Address     = slave_Address;
   config.SMBUS_Mode        = 0;
   config.slave_Max_Clock_Stretch_Us = 100000;
   config.core_Configuration1 = 0;
   config.core_Configuration2 = 0;

   desc[0].buffer      = buffer;
   desc[0].length      = 3;
   desc[0].transferred = (uint32)&transferred1;
   desc[0].flags       = QAPI_I2C_FLAG_START | QAPI_I2C_FLAG_WRITE | QAPI_I2C_FLAG_STOP;

   
   res = qapi_I2CM_Transfer (client_handle, &config, &desc[0], 1, client_callback, &CB_Parameter, 100);
   
   return res;
}


int quectel_task_entry(void)
{
    qapi_Status_t res = QAPI_OK;
    uint8   slave_Address = 0x1B;  // address of CODEC ALC5616
    uint8   reg = 0x19;
    uint8   wbuf[2] = {0x03, 0x04};    

    qapi_Timer_Sleep(2000, QAPI_TIMER_UNIT_MSEC, true);
    
    /* Initialize debug UART and syslog */
	i2c_uart_dbg_init();    
    I2C_UART_DBG("\r\n===quectel i2c task entry Start!!!===\r\n");

    // Obtain a client specific connection handle to the i2c bus instance 4, I2C
    res = qapi_I2CM_Open(QAPI_I2CM_INSTANCE_004_E, &client_handle);    
    I2C_UART_DBG("qapi_i2cm_open: res=%d, hdl=%x\r\n", res, client_handle);

    res = qapi_I2CM_Power_On(client_handle);
    I2C_UART_DBG("qapi_I2CM_Power_On: res=%d\r\n", res);
    
    // Reads the value of the register
    res = i2c_read(slave_Address, reg);  
    
    qapi_Timer_Sleep(500, QAPI_TIMER_UNIT_MSEC, true);
    I2C_UART_DBG("i2c_read:byte1[0x%x], byte2[0x%x]\r\n", rbuf[0], rbuf[1]);

    // write the register
    res = i2c_write(slave_Address, reg, wbuf, 2); 
    
    qapi_Timer_Sleep(500, QAPI_TIMER_UNIT_MSEC, true);
    I2C_UART_DBG("i2c_write:res=%d\r\n", res);
    
    // read the register again
    res = i2c_read(slave_Address, reg);   
    
    qapi_Timer_Sleep(500, QAPI_TIMER_UNIT_MSEC, true);
    I2C_UART_DBG("i2c_read:byte1[0x%x], byte2[0x%x]\r\n", rbuf[0], rbuf[1]);


    // Close the connection handle to the i2c bus instance
    res = qapi_I2CM_Close (client_handle);    
    I2C_UART_DBG("qapi_i2cm_close: res=%d\r\n", res);
   
	I2C_UART_DBG("\r\n===quectel i2c task entry Exit!!!===\r\n");
    return 0;
}

#endif /*__EXAMPLE_I2C__*/

