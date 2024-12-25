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
#include "hal_data.h"
#include "common_utils.h"
#include "intc_tint_ep.h"
#define MODULE_NAME		"r_intc_tint"
FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event);

FSP_CPP_FOOTER

/* Board's user LED */
extern bsp_leds_t g_bsp_leds;
/* Boolean flag to determine switch is pressed or not.*/
extern volatile bool g_sw_press;

/*******************************************************************************************************************//**
 * main() is generated by the Configuration editor and is used to generate threads if an RTOS is used.  This function
 * is called by main() when no RTOS is used.
 **********************************************************************************************************************/
void hal_entry (void)
{

    fsp_err_t err                           = FSP_SUCCESS;
    bsp_io_level_t led_current_state        = (bsp_io_level_t) RESET_VALUE;
    fsp_pack_version_t version              = {RESET_VALUE};

    /* LED type structure */
    bsp_leds_t leds = g_bsp_leds;

    /* version get API for FLEX pack information */
    R_FSP_VersionGet(&version);

    /* Example Project information printed on the RTT */
    APP_PRINT(BANNER_INFO,EP_VERSION,version.version_id_b.major, version.version_id_b.minor, version.version_id_b.patch);
    APP_PRINT(EP_INFO);

    /* Initialize TINT driver*/
    err = intc_tint_init();
    /* Handle error */
    if(FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("** INTC TINT INIT FAILED ** \r\n");
        APP_ERR_TRAP(err);
    }

    /* Enable TINT driver*/
    err = intc_tint_enable();
    /* Handle error */
    if(FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("** INTC TINT ENABLE FAILED ** \r\n");
        /* Close TINT module.*/
        intc_tint_deinit();
        APP_ERR_TRAP(err);
    }

    /* If this board has no LEDs then trap here */
    if (RESET_VALUE == leds.led_count)
    {
        APP_PRINT("There are no LEDs on this board\r\n\r\n");
        /* Close TINT module.*/
        intc_tint_deinit();
        APP_ERR_TRAP(err);
    }

    /* Main loop */
	while (true)
	{
		/* Toggle user LED  when user pushbutton is pressed*/
		if(true == g_sw_press)
		{
			/* Clear user pushbutton flag */
			g_sw_press = false;

			/* Notify that user pushbutton is pressed */
			APP_PRINT("\r\nUser Pushbutton Pressed\r\n");

			/* Read user LED pin */
			err = R_IOPORT_PinRead(&g_ioport_ctrl,(bsp_io_port_pin_t)leds.p_leds[RESET_VALUE], &led_current_state);

			/* Handle error */
			if (FSP_SUCCESS != err)
			{
				APP_ERR_PRINT("** R_IOPORT_PinRead FAILED ** \r\n");
				/* Close External IRQ module.*/
				intc_tint_deinit();
				APP_ERR_TRAP(err);
			}

			/* Reverse LED pin state*/
			led_current_state ^= BSP_IO_LEVEL_HIGH;

			/* Toggle user LED */
			err = R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t)leds.p_leds[RESET_VALUE], led_current_state);

			/* Handle error */
			if (FSP_SUCCESS != err)
			{
				APP_ERR_PRINT("** R_IOPORT_PinWrite FAILED ** \r\n");
				/* Close External IRQ module.*/
				intc_tint_deinit();
				APP_ERR_TRAP(err);
			}
			if(BSP_IO_LEVEL_HIGH == led_current_state)

			{
				/* Print LED Pin state */
				APP_PRINT("LED State: High{ON}\r\n");
			}
			else
			{
				/* Print LED Pin state */
				APP_PRINT("LED State: Low{OFF}\r\n");
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