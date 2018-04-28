/*
 * lpi2c.c
 *
 *  Created on: Jun 9, 2016
 *      Author: B46911
 */

#include "lpi2c.h"
#include "dmamux.h"
#include "edma.h"

//#if (defined(MCU_S32K142) || defined(MCU_S32K144) || defined(MCU_S32K146) || defined(MCU_S32K148))
//
//void LPI2C_init(uint8_t pcc_index, LPI2C_Type* base){
//    PCC->PCCn[pcc_index ] |= PCC_PCCn_PCS(1)        /* Clk src: System Oscillator*/
//                              | PCC_PCCn_CGC_MASK;   /* Enable clock for LPI2C0 */
//
//    base->MCFGR1 = LPI2C_MCFGR1_PRESCALE(0);	/*Prescaler selection*/
//
//    base->MCCR0 = LPI2C_MCCR0_CLKLO(0x03)		/*Clock period low*/
//                | LPI2C_MCCR0_CLKHI(0x04)		/*Clock period high*/
//                | LPI2C_MCCR0_SETHOLD(0x03)		/*Setup hold delay*/
//                | LPI2C_MCCR0_DATAVD(0x01);		/*Data valid delay*/
//
//    base->MFCR = LPI2C_MFCR_TXWATER(0)
//                  |LPI2C_MFCR_RXWATER(3);
//    base->MCR |= LPI2C_MCR_MEN_MASK /* Enbale LPI2C as master */
//            | LPI2C_MCR_DBGEN_MASK;
//}
//
//void LPI2C_Transmit (LPI2C_Type* base){
//    base->MTDR = (0x05<<8)|((0x1E<<1)|0);
//}
//
//void wait(volatile int cycles){
//    /* Delay function - do nothing for a number of cycles */
//    while(cycles--);
//}
//
//#endif

/*******************************************************************************
* Local variables
*******************************************************************************/
static uint16_t I2C_TxDataBuffer[128], I2C_DataLength;
static I2C_BufferStatusType I2C_TxBufferStatus;
static I2C_TxStatusType I2C_TxStatus;
static I2C_RxStatusType I2C_RxStatus;

/*******************************************************************************
* Public functions
*******************************************************************************/
/***************************************************************************//*!
@brief      LPI2C_Init()

@details    Initializes LPI2C module for 400kbps and DMA operation. Considers
            a peripheral clock of 8MHz.
*******************************************************************************/
void LPI2C1_Init(void)
{
	PCC->PCCn[PCC_DMAMUX_INDEX] |= PCC_PCCn_CGC_MASK;

    DMAMUX_Configure(0x0, DMAMUX_SRC_I2C1_TX);
    DMAMUX_Configure(0x1, DMAMUX_SRC_I2C1_RX);

    eDMA_ChannelDisable(0);
    eDMA_ChannelDisable(1);

	PCC->PCCn[PCC_LPI2C1_INDEX] &= ~PCC_PCCn_CGC_MASK;
	PCC->PCCn[PCC_LPI2C1_INDEX] |=  PCC_PCCn_PCS(1)|
									PCC_PCCn_CGC_MASK;

    LPI2C1->MCFGR0 = 0x00000000;
    /* 2-pin open-drain mode, prescaler divide by 1 */
    LPI2C1->MCFGR1 = LPI2C_MCFGR1_PINCFG(0) | LPI2C_MCFGR1_PRESCALE(0);
    /* Glitch filter for SDA & SCL disabled, bus idle timeout se to 5 cycles */
    LPI2C1->MCFGR2 = LPI2C_MCFGR2_FILTSDA(0) | LPI2C_MCFGR2_FILTSCL(0) | \
                     LPI2C_MCFGR2_BUSIDLE(5);
    /* Data valid delay: 3 clocks, setup hold delay: 3 clocks,
       clock period high: 7 clocks, clock period low: 12 clocks */
    LPI2C1->MCCR0 = LPI2C_MCCR0_DATAVD(0x02) | LPI2C_MCCR0_SETHOLD(0x04) | \
                    LPI2C_MCCR0_CLKHI(0x05) | LPI2C_MCCR0_CLKLO(0x0B);
    /* Enable Rx and Tx data DMA */
    LPI2C1->MDER = LPI2C_MDER_RDDE_MASK | LPI2C_MDER_TDDE_MASK;
    /* Set Rx and Tx data watermark to 0 */
    LPI2C1->MFCR = LPI2C_MFCR_RXWATER(0) | LPI2C_MFCR_TXWATER(0);
    /* Reset Rx and Tx FIFOs, enable master debug, enable master logic */
    LPI2C1->MCR = LPI2C_MCR_RRF_MASK | LPI2C_MCR_RTF_MASK | \
                  LPI2C_MCR_DBGEN_MASK | LPI2C_MCR_MEN_MASK;

    I2C_TxBufferStatus = I2C_BUFFER_UNINIT;
    I2C_TxStatus = I2C_TX_IDLE;
    I2C_RxStatus = I2C_RX_IDLE;
}

/***************************************************************************//*!
@brief      LPI2C_Init()

@param      rw          Transfer type (read / write).
            *data       Pointer to data buffer.
            length      Number data bytes (including address byte).

@details    Prepared the data buffer for slave read or write data access.
*******************************************************************************/
void LPI2C1_WriteBuffer(I2C_RwType rw, uint8_t *data, uint16_t length)
{
    /* Leave first buffer element for address */
    uint16_t *pDataBuffer = &I2C_TxDataBuffer[0];
    uint8_t index;

    I2C_DataLength = length;    /* Address byte + data bytes */

    /* Write command */
    *pDataBuffer = I2C_CMD_START;
    /* Append address including read/write access specifier */
    *pDataBuffer++ |= ((uint16_t)*data++ | rw);

    if(rw == I2C_READ)
    {
        for(index=0; index<(length-1); index++)
        {
            /* Write receive command */
            *pDataBuffer++ = I2C_CMD_RECEIVE;
        }
    }
    else
    {
        for(index=0; index<(length-1); index++)
        {
            /* Write data */
            *pDataBuffer++ = (uint16_t)*data++;
        }
    }

    /* Write STOP command */
    *pDataBuffer = I2C_CMD_STOP;

    I2C_TxStatus = I2C_TX_IDLE;
    I2C_RxStatus = I2C_RX_IDLE;
    I2C_TxBufferStatus = I2C_BUFFER_INIT;
}

/***************************************************************************//*!
@brief      LPI2C1_Transmit()

@return     I2C_ReturnType      Return value.

@details    Transmits the data for slave write access.
*******************************************************************************/
I2C_ReturnType LPI2C1_Transmit(void)
{
    uint8_t ret;

    if((I2C_TxBufferStatus == I2C_BUFFER_UNINIT) || (I2C_TxStatus == I2C_TX_BUSY) || (I2C_RxStatus == I2C_RX_BUSY))
    {
        ret = I2C_NOT_OK;
    }
    else
    {
        /* Config Tx DMA TCD */
        eDMA_TCDConfig
        (
            0,                              /* eDMA channel 0 */
            (uint32_t)&I2C_TxDataBuffer[0], /* Source address */
            2,                              /* Source address offset */
            0,                              /* Source address modulo disabled */
            DMA_SRC_SIZE_16BIT,             /* Source size */
            0,                              /* Destination address modulo disabled */
            DMA_DEST_SIZE_16BIT,            /* Destination size */
            2,                              /* Nbytes */
            0,                              /* Source address adjustment */
            (uint32_t)&LPI2C1->MTDR,        /* Destination address */
            0,                              /* Destination address offset */
            (I2C_DataLength + 1),           /* Major iteration count (address + data + stop) */
            0,                              /* Destination address adjustment */
            DMA_TCD_CSR_DREQ_MASK           /* TCD_CSR: disable channel requests after major loop */
        );

        I2C_TxStatus = I2C_TX_BUSY;
        eDMA_ChannelEnable(0);

        /* MAKE NON-BLOCKING */
        /* ADD CHECK FOR ERRORS */
        /* Wait for transmit data transfer to complete */
        while((DMA->TCD[0].CSR & DMA_TCD_CSR_DONE_MASK) == 0)
        {
        }

        eDMA_ChannelDisable(0);

        I2C_TxStatus = I2C_TX_DONE;
        I2C_TxBufferStatus = I2C_BUFFER_UNINIT;

        ret = I2C_OK;
    }

    return(ret);
}

/***************************************************************************//*!
@brief      LPI2C1_Receive()

@param      *RxBuffer           Recive data buffer pointer.

@return     I2C_ReturnType      Return value.

@details    Transmits the data for slave read access and stores received data
            into the memory location specified by RxBuffer pointer.
*******************************************************************************/
I2C_ReturnType LPI2C1_Receive(uint8_t *RxBuffer)
{
    uint8_t ret;

    if((I2C_TxBufferStatus == I2C_BUFFER_UNINIT) || (I2C_RxStatus == I2C_RX_BUSY) || (I2C_TxStatus == I2C_TX_BUSY))
    {
        ret = I2C_NOT_OK;
    }
    else
    {
        /* Config Tx DMA TCD */
        eDMA_TCDConfig
        (
            0,                              /* eDMA channel 0 */
            (uint32_t)&I2C_TxDataBuffer,    /* Source address */
            2,                              /* Source address offset */
            0,                              /* Source address modulo disabled */
            DMA_SRC_SIZE_16BIT,             /* Source size */
            0,                              /* Destination address modulo disabled */
            DMA_DEST_SIZE_16BIT,            /* Destination size */
            2,                              /* Nbytes */
            0,                              /* Source address adjustment */
            (uint32_t)&LPI2C1->MTDR,        /* Destination address */
            0,                              /* Destination address offset */
            (I2C_DataLength + 1),           /* Major iteration count (address + cmd. bytes + stop cmd.) */
            0,                              /* Destination address adjustment */
            DMA_TCD_CSR_DREQ_MASK           /* TCD_CSR: disable channel requests after major loop */
        );

        /* Config Rx DMA TCD */
        eDMA_TCDConfig
        (
            1,                              /* eDMA channel 1 */
            (uint32_t)&LPI2C1->MRDR,        /* Source address */
            0,                              /* Source offset */
            0,                              /* Source address modulo disabled */
            DMA_SRC_SIZE_8BIT,              /* Source size */
            0,                              /* Destination address modulo disabled */
            DMA_DEST_SIZE_8BIT,             /* Destination size */
            1,                              /* Nbytes */
            0,                              /* Source address adjustment */
            (uint32_t)RxBuffer,             /* Destination address */
            1,                              /* Destination address offset */
            (I2C_DataLength - 1),           /* data bytes only */
            0,                              /* Destination address adjustment */
            DMA_TCD_CSR_DREQ_MASK           /* TCD_CSR: disable channel requests after major loop */
        );

        I2C_RxStatus = I2C_RX_BUSY;
        eDMA_ChannelEnable(1);
        eDMA_ChannelEnable(0);

        /* MAKE NON-BLOCKING */
        /* ADD CHECK FOR ERRORS */
        /* Wait for transmit data transfer to complete */
        while((DMA->TCD[0].CSR & DMA_TCD_CSR_DONE_MASK) == 0)
        {
        }

        /* MAKE NON-BLOCKING */
        /* ADD CHECK FOR ERRORS */
        /* Wait for receive data transfer to complete */
        while((DMA->TCD[1].CSR & DMA_TCD_CSR_DONE_MASK) == 0)
        {
        }

        eDMA_ChannelDisable(0);
        eDMA_ChannelDisable(1);

        I2C_RxStatus = I2C_RX_DONE;
        I2C_TxBufferStatus = I2C_BUFFER_UNINIT;

        ret = I2C_OK;
    }

    return(ret);
}

