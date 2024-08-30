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
#include "timer_setup.h"
#include "ext_irq_setup.h"

/*
 * External Global Variables
 */
/* LED Structure used to blink on board LED */
extern bsp_leds_t g_bsp_leds;
extern volatile uint8_t g_timer_overflow_counter;
volatile bool system_reset_flag = false;

FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event);

FSP_CPP_FOOTER

/*
 * private function declarations
 */
/* function used to read RTT input and proceed */
static fsp_err_t read_Input_from_RTT(void);
/* function used to enable wdt in debug mode */
static void enable_wdt_count_in_debug_mode(void);
/* function used to check if reset is caused by wdt or not */
static void check_reset_status(void);
/* function used to Enable WDT System Reset */
static void procedure_system_reset(void);

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
    APP_PRINT(BANNER_INFO, EP_VERSION, version.major, version.minor, version.patch);
    APP_PRINT ("\r\nThis example project demonstrates the typical use of the WDT HAL module APIs\r\n");
    APP_PRINT ("User input initializes the WDT and start GTM timer\r\n");
    APP_PRINT ("WDT counter is refreshed periodically every 1 second when the GTM timer expires\r\n");
    APP_PRINT ("On pressing the Push button, WDT counter stops from refreshing\r\n");
    APP_PRINT ("WDT resets the MCU in 2 seconds\r\n");

    /* If this board has no LEDs then trap here */
    if (RESET_VALUE == g_bsp_leds.led_count)
    {
        APP_PRINT ("\r\nNo on-board LEDs Found");
        APP_ERR_TRAP (FSP_ERR_UNSUPPORTED);
    }

    check_reset_status();

    /* Initializing GTM in PWM Mode */
    err = init_gtm_module();
    if(FSP_SUCCESS != err)
    {
        /* Timer init failed */
        APP_ERR_PRINT ("\r\n ** GTM TIMER INIT FAILED ** \r\n");
        APP_ERR_TRAP (err);
    }

    /* Initializing External IRQ */
    err = init_irq_module();
    if(FSP_SUCCESS != err)
    {
        /* External IRQ init failed, cleanup the timer initialization */
        APP_ERR_PRINT ("\r\n ** EXTERNAL IRQ INIT FAILED ** \r\n");
        deinit_gtm_module();
        APP_ERR_TRAP (err);
    }

    /* Menu for User Selection */
    APP_PRINT ("\r\nEnter 1 to Enable WDT\r\n");
    APP_PRINT ("User Input:  ");

    while(true)
    {
        /* Process input only when User has provided one */
        if(APP_CHECK_DATA)
        {
            err = read_Input_from_RTT();
            if(FSP_SUCCESS != err)
            {
                /* Close timer and external IRQ module */
                deinit_gtm_module();
                deinit_irq_module();
                APP_ERR_TRAP (err);
            }
        }
        procedure_system_reset();
        /* For every 3Sec. RTT prints WDT refresh message.
         * This is done to avoid the continuous print message on RTT*/
        if (WDT_REFRESH_COUNTER_VALUE == g_timer_overflow_counter)
        {
            g_timer_overflow_counter = RESET_VALUE;
            APP_PRINT("WDT counter Refreshed.\r\n");
        }
    }
}

/*******************************************************************************************************************//**
 * This function is called to read user input from SEGGER_RTT Viewer.
 * @brief    Read user input from SEGGER_RTT Viewer. Handle the Error internally with Proper Message. Application handles the rest.
 * @param[IN]   None
 * @retval      Any Other Error code apart from FSP_SUCCESS  Unsuccessful to perform operation
 **********************************************************************************************************************/
fsp_err_t read_Input_from_RTT(void)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status

    /* Variable to store user input */
    uint8_t rByte[BUFFER_SIZE] = { RESET_VALUE };
    uint8_t user_data = RESET_VALUE;

    /* Read First byte of data provided by user */
    APP_READ(rByte);

    user_data = (uint8_t) atoi((char *)&rByte);

    switch (user_data)
    {
        case ENABLE_WDT:
        {
            /* Enable WDT to count and generate NMI or Reset when the debugger(JLink) is connected. */
            enable_wdt_count_in_debug_mode();

            /* Open WDT. For every GTM timeout, wdt will get refreshed. */
            err = R_WDT_Open (&g_wdt_ctrl, &g_wdt_cfg);
            if (FSP_SUCCESS != err)
            {
                APP_ERR_PRINT ("\r\n ** R_WDT_Open API Failed ** \r\n");
                return err;
            }

            /* Start GTM timer in Periodic mode */
            err = timer_start();
            if (FSP_SUCCESS != err)
            {
                APP_ERR_PRINT ("\r\n ** GTM TIMER START FAILED ** \r\n");
                return err;
            }

            /* Enable External IRQ */
            err = enable_irq_module();
            if(FSP_SUCCESS != err)
            {
                APP_ERR_PRINT ("\r\n ** EXTERNAL IRQ ENABLE FAILED ** \r\n");
                return err;
            }

            /* Print message to indicate user about application status. */
            APP_PRINT ("\r\nWDT initialized, GTM Timer Started");
            APP_PRINT("\r\nTo stop WDT counter from refreshing, press the push button\r\n");
        }
        break;

        default:
        {
            /* Menu for User Selection */
            APP_PRINT ("\r\nInvalid Input");
            APP_PRINT ("\r\nPlease provide input from below option");
            APP_PRINT ("\r\nEnter 1 to Enable WDT\r\n");
            APP_PRINT ("User Input:  ");
        }
        break;
    }
    user_data = RESET_VALUE;
    return err;
}

/*******************************************************************************************************************//**
 * This function is called to enable WDT counter in debug mode.
 * @brief    Enables WDT counter to count in debug mode.
 * @param[IN]   None
 * @retval      None
 **********************************************************************************************************************/
static void enable_wdt_count_in_debug_mode(void)
{
    /* Clear this bit to Stop WDT0 from counting in debug mode */
    R_SYSC->SYS_WDT0_CTRL_b.WDTSTOPMASK = RESET_VALUE;
}

/*******************************************************************************************************************//**
 * This function is called to check whether reset is caused by WDT.
 * @brief    Check if reset caused by WDT or not.
 * @param[IN]   None
 * @retval      None
 **********************************************************************************************************************/
static void check_reset_status(void)
{
    /* Check if reset was caused by the WDT? If it was, turn on LED to indicate WDT reset triggered. */
    if (R_CPG->CPG_ERROR_RST2_b.ERROR_RST0)
    {
        /* Clear the flag or the test won't work properly the second time. */
        R_CPG->CPG_ERROR_RST2_b.ERROR_RST0_WEN = 1;
        R_CPG->CPG_ERROR_RST2_b.ERROR_RST0 = 1;

        // Turn ON LED
        R_BSP_PinWrite((bsp_io_port_pin_t)g_bsp_leds.p_leds[0], BSP_IO_LEVEL_HIGH);

        APP_PRINT("\r\n************************ WDT Reset detected ************************\r\n");
    }
}
static void procedure_system_reset(void)
{
    R_BSP_WDT_SYSTEM_RESET_ENABLE(g_wdt_cfg_extend.channel, system_reset_flag);
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
        R_IOPORT_Open(&g_ioport_ctrl, &g_bsp_pin_cfg);

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