/*******************************************************************************
*
* (c) Copyright 2017 NXP Semiconductors
*
****************************************************************************//*!
*
* @file     dmamux.c
*
* @version  1.0.2.0
*
* @date     Aug-29-2017
*
* @brief    Direct Memory Access Multiplexer module control source file.
*
*******************************************************************************/
/*******************************************************************************
* Includes
*******************************************************************************/
#include "dmamux.h"

/*******************************************************************************
* Public functions
*******************************************************************************/
/***************************************************************************//*!
@brief      DMAMUX_Configure()

@param      channel     DMA_MUX channel index
            source      DMA_MUX trigger source index

@details    Configures selected DMAMUX channel trigger source.
*******************************************************************************/
void DMAMUX_Configure(uint8_t channel, uint8_t source)
{
    DMAMUX->CHCFG[channel] = DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(source);
}
