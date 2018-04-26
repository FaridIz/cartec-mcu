/*
 * PORT_config.h
 *
 *  Rev: 0.1
 *  Author: Isaac Avila
 *  Date: March 13th, 2017
 */

#ifndef PORT_CONFIG_H_
#define PORT_CONFIG_H_

#include "system.h"

typedef enum {
    eInvalidPort = -1,
    ePortA = 0,
    ePortB,
    ePortC,
    ePortD,
    ePortE
}ePort;

typedef enum {
    eMuxDisabled = 0,
    eMuxAsGPIO,
    eMux2,
    eMux3,
    eMux4,
    eMux5,
    eMux6,
    eMux7
}eMuxValue;

typedef enum {
    eInterrupt_DMA_Disabled = 0,
    eDMA_rising_edge,
    eDMA_falling_edge,
    eDMA_either_edge,
    eInterrupt_logic_zero = 8,
    eInterrupt_rising_edge,
    eInterrupt_falling_edge,
    eInterrupt_either_edge,
    eInterrupt_logic_one
}eInterruptDMAConfig;

typedef enum {
    ePullDisable = 0,
    ePullDownEnable = 2,
    ePullUpEnable = 3
}ePullConfiguration;

typedef enum {
    eLowDriveStrength = 0,
    eHighDriveStrength
}eDriveStrConfig;

typedef enum {
    ePassiveFilterDisabled = 0,
    ePassiveFilterEnabled
}ePassiveFilterConfig;

typedef enum {
    eInput = 0,
    eOutput
}eDirection;

typedef struct {
    eInterruptDMAConfig     int_dma;    /* Interrupt/DMA configuration */
    ePullConfiguration      pull;       /* Pull-up, pull-down, pull disable */
    eDriveStrConfig         driveStr;   /* DriveStr setting */
    ePassiveFilterConfig    filter;     /* Passive filter enable/disable */
}port_attributes_t;


/* PORT configuration structure: it contains configuration
 * settings for I/O pins such as multiplexing alternative,
 * interrupt/dma option, passive filter, pull-up/down, etc. */

typedef struct {
    ePort               port;   /* Port */
    uint8_t             pin;    /* Pin number */
    eMuxValue           mux;    /* MUX option */
    eDirection          dir;    /* GPIO direction */
    port_attributes_t   attrib; /* Attributes */
}PORT_config_t;

#endif /* PORT_CONFIG_H_ */

