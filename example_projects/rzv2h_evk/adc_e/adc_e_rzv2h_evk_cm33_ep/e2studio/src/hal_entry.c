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
#include "adc_ep.h"
#define MODULE_NAME             "r_adc_e"
extern volatile bool g_b_ready_to_read;
FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event);

FSP_CPP_FOOTER

/*******************************************************************************************************************//**
 * main() is generated by the Configuration editor and is used to generate threads if an RTOS is used.  This function
 * is called by main() when no RTOS is used.
 **********************************************************************************************************************/
void hal_entry (void)
{
    fsp_pack_version_t version = {RESET_VALUE};
    fsp_err_t err = FSP_SUCCESS;

    /* Version get API for FLEX pack information */
    R_FSP_VersionGet(&version);

    /* Example Project information printed on the Console */
    APP_PRINT(BANNER_INFO, EP_VERSION, version.version_id_b.major, version.version_id_b.minor, version.version_id_b.patch);
    APP_PRINT(BANNER);
    /* Menu for User Selection */
    APP_PRINT(MENUOPTION1);
    APP_PRINT(MENUOPTION2);
    APP_PRINT(MENUOPTION3);
    APP_PRINT(MENUOPTION4);
    while (true)
    {
        if (APP_CHECK_DATA)
        {
            err = read_process_input_from_RTT ();

            /* handle error */
            if (FSP_SUCCESS != err)
            { /* adc scan start or stop failed, cleanup the adc initialization */
                deinit_adc_module ();
                APP_ERR_TRAP(err);
            }
        }

        /* read the adc output data and status */
        if (true == g_b_ready_to_read)
        {
            err = adc_read_data();
            /* handle error */
            if (FSP_SUCCESS != err)
            { /* adc reading failed, cleanup the adc initialization */
                deinit_adc_module ();
                APP_ERR_TRAP(err);
            }
        }
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
