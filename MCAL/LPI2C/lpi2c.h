/*******************************************************************************
*
* (c) Copyright 2017 NXP Semiconductors
*
****************************************************************************//*!
*
* @file     lpi2c.h
*
* @version  1.0.0.0
*
* @date     Aug-29-2017
*
* @brief    Inter-Integrated Circuit module header file.
*
*******************************************************************************/
#ifndef LPI2C_H
#define LPI2C_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "device_registers.h"

/*******************************************************************************
* Macros and definitions
*******************************************************************************/
#define I2C_CMD_RECEIVE     0x0100
#define I2C_CMD_STOP        0x0200
#define I2C_CMD_START       0x0400

/*******************************************************************************
* Typedefs
*******************************************************************************/
typedef enum {
    I2C_WRITE = 0,
    I2C_READ
} I2C_RwType;

typedef enum {
    I2C_BUFFER_UNINIT = 0,
    I2C_BUFFER_INIT
} I2C_BufferStatusType;

typedef enum {
    I2C_OK = 0,
    I2C_NOT_OK
} I2C_ReturnType;

typedef enum {
    I2C_TX_IDLE = 0,
    I2C_TX_BUSY,
    I2C_TX_DONE
} I2C_TxStatusType;

typedef enum {
    I2C_RX_IDLE = 0,
    I2C_RX_BUSY,
    I2C_RX_DONE
} I2C_RxStatusType;

/*******************************************************************************
* Public function prototypes
*******************************************************************************/
void LPI2C1_Init(void);
void LPI2C1_WriteBuffer(I2C_RwType rw, uint8_t *data, uint16_t length);
I2C_ReturnType LPI2C1_Transmit(void);
I2C_ReturnType LPI2C1_Receive(uint8_t *RxBuffer);

#endif
