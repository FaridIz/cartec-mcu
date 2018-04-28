/*******************************************************************************
*
* (c) Copyright 2017 NXP Semiconductors
*
****************************************************************************//*!
*
* @file     edma.h
*
* @version  1.0.0.0
*
* @date     Aug-29-2017
*
* @brief    Enhanced Direct Memory Access module header file.
*
*******************************************************************************/
#ifndef EDMA_H
#define EDMA_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "device_registers.h"

/*******************************************************************************
* Typedefs
*******************************************************************************/
typedef enum {
    DMA_SRC_SIZE_8BIT = 0,
    DMA_SRC_SIZE_16BIT = 1,
    DMA_SRC_SIZE_32BIT = 2,
    DMA_SRC_SIZE_16BYTE = 4,
    DMA_SRC_SIZE_32BYTE = 5
} DMA_SrcSizeType;

typedef enum {
    DMA_DEST_SIZE_8BIT = 0,
    DMA_DEST_SIZE_16BIT = 1,
    DMA_DEST_SIZE_32BIT = 2,
    DMA_DEST_SIZE_16BYTE = 4,
    DMA_DEST_SIZE_32BYTE = 5
} DMA_DestSizeType;

/*******************************************************************************
* Function prototypes
*******************************************************************************/
void eDMA_TCDConfig
(
    uint8_t index,
    uint32_t saddr,
    uint16_t soff,
    uint8_t smod,
    uint8_t ssize,
    uint8_t dmod,
    uint8_t dsize,
    uint32_t nbytes,
    uint32_t slast,
    uint32_t daddr,
    uint16_t doff,
    uint16_t citer,
    uint32_t dlast,
    uint16_t csr
);
void eDMA_ChannelEnable(uint8_t index);
void eDMA_ChannelDisable(uint8_t index);

#endif
