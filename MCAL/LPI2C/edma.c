/*******************************************************************************
*
* (c) Copyright 2017 NXP Semiconductors
*
****************************************************************************//*!
*
* @file     edma.c
*
* @version  1.0.0.0
*
* @date     Aug-29-2017
*
* @brief    Enhanced Direct Memory Access module control source file.
*
*******************************************************************************/
/*******************************************************************************
* Includes
*******************************************************************************/
#include "edma.h"

/*******************************************************************************
* Public functions
*******************************************************************************/
/***************************************************************************//*!
@brief      eDMA_TCDConfig()

@details    Configure eDMA Transfer Control Descriptor.
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
)
{
    DMA->TCD[index].SADDR = saddr;
    DMA->TCD[index].SOFF = soff;
    DMA->TCD[index].ATTR = DMA_TCD_ATTR_SMOD(smod) | \
                           DMA_TCD_ATTR_SSIZE(ssize) | \
                           DMA_TCD_ATTR_DMOD(dmod) | \
                           DMA_TCD_ATTR_DSIZE(dsize);
    DMA->TCD[index].NBYTES.MLNO = nbytes;
    DMA->TCD[index].SLAST = slast;
    DMA->TCD[index].DADDR = daddr;
    DMA->TCD[index].DOFF = doff;
    DMA->TCD[index].CITER.ELINKNO = citer;
    DMA->TCD[index].DLASTSGA = dlast;
    DMA->TCD[index].BITER.ELINKNO = citer;
    DMA->TCD[index].CSR = csr;
}

/***************************************************************************//*!
@brief      eDMA_ChannelEnable()

@param      index           eDMA channel index

@details    Enables eDMA channel requests.
*******************************************************************************/
void eDMA_ChannelEnable(uint8_t index)
{
    DMA->SERQ = index;
}
/***************************************************************************//*!
@brief      eDMA_ChannelDisable()

@param      index           eDMA channel index

@details    Disables eDMA channel requests.
*******************************************************************************/
void eDMA_ChannelDisable(uint8_t index)
{
    DMA->CERQ = index;
}
