/***********************************************************************************************************************
 * File Name    : hal_entry.c
 * Description  : Contains data structures and functions used in hal_entry.c.
 **********************************************************************************************************************/
/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "hal_data.h"
#include "pd_axi_on.h"
#include "i2c_pmic.h"
#include "pll_init.h"
#include "xspi_init.h"
#include "ca55_start.h"
#include "cr8_start.h"
#include "common_utils.h"
#include "can_fd_ep.h"
#define MODULE_NAME             "r_canfd"

/* Timeout value */
volatile uint32_t g_time_out = WAIT_TIME;
/** Structure with LED information for board. */

const bsp_leds_t g_leds =
{
    .led_count = (uint16_t) ((sizeof(g_prv_leds)) / (sizeof(g_prv_leds[0]))),
    .p_leds    = &g_prv_leds[0]
};

/* User defined function */
extern void led_update(led_state_t led_state);
static void led_pin_initialisation(void);

/* Flags to be set in Callback function */
bool b_canfd_ch0_tx_complete = false;
bool b_canfd_ch0_rx_complete = false;

bool b_canfd_ch1_tx_complete = false;
bool b_canfd_ch1_rx_complete = false;

bool b_canfd_err_status = false;

/* Acceptance filter array parameters */
const canfd_afl_entry_t p_canfd_ch0_afl[CANFD_CFG_AFL_CH0_RULE_NUM] =
{
 { /* Accept all messages with Extended ID 0x1000-0x1FFF */
   .id =
   {
    /* Specify the ID, ID type and frame type to accept. */
    .id         = CANFD_FILTER_ID,
    .frame_type = CAN_FRAME_TYPE_DATA,
    .id_mode    = CAN_ID_MODE_EXTENDED
   },

   .mask =
   {
    /* These values mask which ID/mode bits to compare when filtering messages. */
    .mask_id         = MASK_ID,
    .mask_frame_type = ZERO,
    .mask_id_mode    = MASK_ID_MODE
   },

   .destination =
   {
    /* If DLC checking is enabled any messages shorter than the below setting will be rejected. */
    .minimum_dlc = (canfd_minimum_dlc_t)ZERO,

    /* Optionally specify a Receive Message Buffer (RX MB) to store accepted frames. RX MBs do not have an
     * interrupt or overwrite protection and must be checked with R_CANFD_InfoGet and R_CANFD_Read. */
    .rx_buffer   = CANFD_RX_MB_0,

    /* Specify which FIFO(s) to send filtered messages to. Multiple FIFOs can be OR'd together. */
    .fifo_select_flags = CANFD_RX_FIFO_0,
   }
 },
};

const canfd_afl_entry_t p_canfd_ch1_afl[CANFD_CFG_AFL_CH1_RULE_NUM] =
{
 { /* Accept all messages with Extended ID 0x1000-0x1FFF */
   .id =
   {
    /* Specify the ID, ID type and frame type to accept. */
    .id         = CANFD_FILTER_ID,
    .frame_type = CAN_FRAME_TYPE_DATA,
    .id_mode    = CAN_ID_MODE_EXTENDED
   },

   .mask =
   {
    /* These values mask which ID/mode bits to compare when filtering messages. */
    .mask_id         = MASK_ID,
    .mask_frame_type = ZERO,
    .mask_id_mode    = MASK_ID_MODE
   },

   .destination =
   {
    /* If DLC checking is enabled any messages shorter than the below setting will be rejected. */
    .minimum_dlc = (canfd_minimum_dlc_t)ZERO,

    /* Optionally specify a Receive Message Buffer (RX MB) to store accepted frames. RX MBs do not have an
     * interrupt or overwrite protection and must be checked with R_CANFD_InfoGet and R_CANFD_Read. */
    .rx_buffer   = CANFD_RX_MB_0,

    /* Specify which FIFO(s) to send filtered messages to. Multiple FIFOs can be OR'd together. */
    .fifo_select_flags = CANFD_RX_FIFO_0,
   }
 },
};

FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event);

FSP_CPP_FOOTER

/*******************************************************************************************************************//**
 * main() is generated by the Configuration editor and is used to generate threads if an RTOS is used.  This function
 * is called by main() when no RTOS is used.
 **********************************************************************************************************************/
void hal_entry (void)
{

    fsp_err_t err = FSP_SUCCESS;
    fsp_pack_version_t version = {RESET_VALUE};
    unsigned char rtt_input_buf[BUFFER_SIZE_DOWN] = {NULL_CHAR};


    /* Initialization of LED Pins*/
    led_pin_initialisation();

    /* version get API for FLEX pack information */
    R_FSP_VersionGet(&version);

    /* Example Project information printed on the Console */
    APP_PRINT(BANNER_INFO, EP_VERSION, version.version_id_b.major, version.version_id_b.minor, version.version_id_b.patch);
    R_BSP_SoftwareDelay(200, BSP_DELAY_UNITS_MILLISECONDS);
    APP_PRINT(EP_INFO);
    R_BSP_SoftwareDelay(200, BSP_DELAY_UNITS_MILLISECONDS);

    /* Initialize canfd module */
    err = R_CANFD_Open(&g_canfd_ch0_ctrl, &g_canfd_ch0_cfg);
    /* Error trap */
    if(FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("\nCANFD Open API failed");
        led_update(error);
        APP_ERR_TRAP(err);
    }

    err = R_CANFD_Open(&g_canfd_ch1_ctrl, &g_canfd_ch1_cfg);
    /* Error trap */
    if(FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("\nCANFD Open API failed");
        led_update(error);
        APP_ERR_TRAP(err);
    }

    APP_PRINT("\nPlease enter any key on Terminal Emulator to initiate CAN transmission.\n");

    while(1)
    {
        /* Check for user input */
        if (APP_CHECK_DATA)
        {
            APP_READ(rtt_input_buf);
            APP_PRINT("\nUser input: %s" , (char *)rtt_input_buf);
            /* Update transmit frame and initiate transmission on CAN frame */
            canfd_operation();
        }

        /* Get the status of transmitted frame and read the data */
        can_read_operation();
        if(true == b_canfd_err_status)
        {
            b_canfd_err_status = false;
            APP_ERR_PRINT("\nCAN ERR status");
            led_update(error);
            APP_ERR_TRAP(true);
        }


        /* Re initializing time out value */
        g_time_out = WAIT_TIME;
    }
}

/*******************************************************************************************************************//**
 * @brief       This function is to initialize state of LED pins.
 * @param[in]   None
 * @return      None
 **********************************************************************************************************************/
static void led_pin_initialisation(void)
{
    /* Set the LED pin state low */
    R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) g_leds.p_leds[LED_LED1], BSP_IO_LEVEL_LOW);
    R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) g_leds.p_leds[LED_LED2], BSP_IO_LEVEL_LOW);
    R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) g_leds.p_leds[LED_LED3], BSP_IO_LEVEL_LOW);
    R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) g_leds.p_leds[LED_LED4], BSP_IO_LEVEL_LOW);

    for(int i = 0; i < 4; i++)
    {
        R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) g_leds.p_leds[i], BSP_IO_LEVEL_HIGH);
        /* Delay */
        R_BSP_SoftwareDelay(200, BSP_DELAY_UNITS_MILLISECONDS);
    }

    for(int j = 1; j >= 0; j--)
    {
        R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t) g_leds.p_leds[j], BSP_IO_LEVEL_LOW);
        /* Delay */
        R_BSP_SoftwareDelay(200, BSP_DELAY_UNITS_MILLISECONDS);
    }
}

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart (bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
    }

    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open(&IOPORT_CFG_CTRL, &IOPORT_CFG_NAME);

#if defined(BSP_FEATURE_BSP_HAS_CM33BOOT_SUPPORT) && (BSP_FEATURE_BSP_HAS_CM33BOOT_SUPPORT == 1)
        pd_all_on_preproc();

        /* Initialization for CM33 coldboot */
        if (R_SYSC->SYS_LSI_MODE_b.STAT_BOOTSELECTER == 0)
        {
            /* PD_AWO -> PD_ALL_ON */
            pd_all_on();
        }
#endif

        /* Allow access to IP beyond AXI */
        pd_all_on_postproc_axi();

#if defined(BSP_FEATURE_BSP_HAS_CM33BOOT_SUPPORT) && (BSP_FEATURE_BSP_HAS_CM33BOOT_SUPPORT == 1)
        pd_all_on_postproc();

 #if BSP_CFG_MCU_CLOCK_INIT
        pll_init_other();
 #endif

        xspi_open();

 #if BSP_CFG_MCU_LAUNCH_CR8

        /* Release reset of CR8(Core0, Core1) and load dummy program to SRAM(RCPU) */
        release_reset_cr8();
 #endif
 #if BSP_CFG_MCU_LAUNCH_CA55

        /* Load CA55 program to SRAM(ACPU) */
        load_ca55_prog();

        xspi_close();

        /* Release reset of CA55(Core0) */
        release_reset_ca55();
 #endif
#endif
    }
}
