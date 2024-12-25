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
#include "timer_pwm.h"
#include "uart_ep.h"
#define MODULE_NAME		"r_sci_b_uart"

/*******************************************************************************************************************//**
 * @addtogroup r_sci_b_uart_ep
 * @{
 **********************************************************************************************************************/

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

    /* Version get API for FLEX pack information */
    R_FSP_VersionGet(&version);

    /* Example Project information printed on the Console */
    APP_PRINT(BANNER_INFO, EP_VERSION, version.version_id_b.major, version.version_id_b.minor, version.version_id_b.patch);
    APP_PRINT("\r\n\r\nThe project initializes the UART with baud rate of 115200 bps.");
    APP_PRINT("\r\nOpen Serial Terminal with this baud rate value and");
    APP_PRINT("\r\nProvide Input ranging from 1 - 2000 to set LED Intensity\r\n");

    /* Initializing GTM in Periodic mode */
    err = gtm_initialize();
    if (FSP_SUCCESS != err)
    {
        APP_PRINT ("\r\n ** GTM TIMER INIT FAILED ** \r\n");
        APP_ERR_TRAP(err);
    }

    /* Starting GTM */
    err = gtm_start();
    if (FSP_SUCCESS != err)
    {
        APP_PRINT ("\r\n ** GTM START FAILED ** \r\n");
        timer_gtm_deinit();
        APP_ERR_TRAP(err);
    }

    /* Initializing UART */
    err = uart_initialize();
    if (FSP_SUCCESS != err)
    {
        APP_PRINT ("\r\n ** UART INIT FAILED ** \r\n");
        timer_gtm_deinit();
        APP_ERR_TRAP(err);
    }

    /* User defined function to demonstrate UART functionality */
    err = uart_ep_demo();
    if (FSP_SUCCESS != err)
    {
        APP_PRINT ("\r\n ** UART EP Demo FAILED ** \r\n");
        timer_gtm_deinit();
        deinit_uart();
        APP_ERR_TRAP(err)
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
        R_IOPORT_Open (&IOPORT_CFG_CTRL, &IOPORT_CFG_NAME);

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

/*******************************************************************************************************************//**
 * @} (end addtogroup r_sci_b_uart_ep)
 **********************************************************************************************************************/