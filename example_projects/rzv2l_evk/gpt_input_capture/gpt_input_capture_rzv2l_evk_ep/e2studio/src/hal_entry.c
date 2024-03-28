/***********************************************************************************************************************
 * File Name    : hal_entry.c
 * Description  : Contains data structures and functions used in hal_entry.c.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Copyright 2024 Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics Corporation and/or its affiliates and may only
 * be used with products of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.
 * Renesas products are sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for
 * the selection and use of Renesas products and Renesas assumes no liability.  No license, express or implied, to any
 * intellectual property right is granted by Renesas.  This software is protected under all applicable laws, including
 * copyright laws. Renesas reserves the right to change or discontinue this software and/or this documentation.
 * THE SOFTWARE AND DOCUMENTATION IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND
 * TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY,
 * INCLUDING WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE
 * SOFTWARE OR DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR
 * DOCUMENTATION (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY
 * LOST PROFITS, OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

#include "common_utils.h"
#include "stdbool.h"

void R_BSP_WarmStart(bsp_warm_start_event_t event);
/*******************************************************************************************************************//**
 * @addtogroup r_gpt_ep
 * @{
 **********************************************************************************************************************/

/* Macro definition */
#define BUFF_SIZE       (30U)
#define EP_INFO         "\r\nThe Example Project demonstrates the functionality of GPT Input\r\n"\
                        "capture module.\r\n"\
                        "GPT4 is used to generate periodic pulses of 500msec duration and\r\n"\
                        "provided as input to GPT Input capture(GPT3).GPT3 counts the event\r\n"\
                        "pulse received at its input. Based on the period and capture event,\r\n"\
                        "the time period of pulse is calculated and displayed on RTTViewer.\r\n"

/* Private function declaration */
static void gpt_deinit(timer_ctrl_t * p_ctrl);

/* Global variables */
volatile bool b_start_measurement   = false;
uint64_t g_capture_count            = RESET_VALUE;
uint32_t g_capture_overflow         = RESET_VALUE;


/*******************************************************************************************************************//**
 * main() is generated by the Configuration editor and is used to generate threads if an RTOS is used.
 * This function is called by main() when no RTOS is used.
 **********************************************************************************************************************/
void hal_entry(void)
{
    fsp_err_t err                   = FSP_SUCCESS;
    fsp_pack_version_t version      = {RESET_VALUE};
    char timer_buffer[BUFF_SIZE]    = {RESET_VALUE};
    timer_info_t info               = {.clock_frequency = RESET_VALUE, .count_direction = RESET_VALUE,
                                       .period_counts =  RESET_VALUE};
    float pulse_time                = 0.0f;

    /* Version get API for FLEX pack information */
    R_FSP_VersionGet(&version);

    /* Banner information */
    APP_PRINT(BANNER_INFO, EP_VERSION, version.major, version.minor, version.patch);
    APP_PRINT(EP_INFO);

    /* Open GPT instance as a periodic timer */
    err = R_GPT_Open(&g_timer_ctrl, &g_timer_cfg);
    /* Handle error */
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("\r\n ** R_GPT_Open API failed ** \r\n");
        APP_ERR_TRAP(err);
    }

    /* Open GPT instance as input capture */
    err = R_GPT_Open(&g_input_capture_ctrl, &g_input_capture_cfg);
    /* Handle error */
    if (FSP_SUCCESS != err)
    {
        gpt_deinit(&g_timer_ctrl);
        APP_ERR_PRINT("\r\n ** R_GPT_Open API failed ** \r\n");
        APP_ERR_TRAP(err);
    }

    /* Enable GPT input capture */
    err = R_GPT_Enable(&g_input_capture_ctrl);
    /* Handle error */
    if (FSP_SUCCESS != err)
    {
        gpt_deinit(&g_timer_ctrl);
        gpt_deinit(&g_input_capture_ctrl);
        APP_ERR_PRINT("\r\n ** R_GPT_Enable API failed ** \r\n");
        APP_ERR_TRAP(err);
    }

    /* Start GPT timer in periodic mode */
    err = R_GPT_Start(&g_timer_ctrl);
    /* Handle error */
    if (FSP_SUCCESS != err)
    {
        gpt_deinit(&g_timer_ctrl);
        gpt_deinit(&g_input_capture_ctrl);
        APP_ERR_PRINT("\r\n ** R_GPT_Start API failed ** \r\n");
        APP_ERR_TRAP(err);
    }

    while(true)
    {
        /* Check for the flag from ISR callback */
        if (true == b_start_measurement)
        {
            /* Reset the flag */
            b_start_measurement = false;
            /* Get the period count and clock frequency */
            err =  R_GPT_InfoGet(&g_timer_ctrl, &info);
            /* Handle error */
            if (FSP_SUCCESS != err)
            {
                gpt_deinit(&g_timer_ctrl);
                gpt_deinit(&g_input_capture_ctrl);
                APP_ERR_PRINT("\r\n ** R_GPT_InfoGet API failed ** \r\n");
                APP_ERR_TRAP(err);
            }

            g_capture_count = (info.period_counts * g_capture_overflow) + g_capture_count;

            /* Calculate the pulse time */
            pulse_time =(float)(((float)g_capture_count)/((float)info.clock_frequency));

            /* Reset the variables */
            g_capture_count = RESET_VALUE;
            g_capture_overflow  = RESET_VALUE;

            sprintf (timer_buffer, "%.05f", pulse_time);
            APP_PRINT("\r\nPulse width measurement value(in second) - %s\r\n", timer_buffer);
        }
    }
}

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart (bsp_warm_start_event_t event) {
    if (BSP_WARM_START_RESET == event)
    {
    }

    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open(&g_ioport_ctrl, &g_bsp_pin_cfg);
    }
}

/*******************************************************************************************************************//**
 * @brief       User defined callback
 * @param[in]   p_args
 * @retval      None
 **********************************************************************************************************************/
void input_capture_user_callback(timer_callback_args_t *p_args)
{
    /* Check for the event */
    switch(p_args->event)
    {
        case TIMER_EVENT_CAPTURE_A :
        {
            /* Capture the count in a variable */
            g_capture_count     = p_args->capture;
            /* Set start measurement */
            b_start_measurement = true;
            break;
        }
        case TIMER_EVENT_CYCLE_END:
        {
            /* An overflow occurred during capture. */
            g_capture_overflow++;
            break;
        }
        default:
        {
            break;
        }
    }
}

/*******************************************************************************************************************//**
 * @brief       De-initializes GPT module
 * @param[in]   p_ctrl  control instances of GPT
 * @retval      None
 **********************************************************************************************************************/
static void gpt_deinit(timer_ctrl_t * p_ctrl)
{
    fsp_err_t err = FSP_SUCCESS;
    /* De-initialize GPT instances */
    err = R_GPT_Close(p_ctrl);
    if (FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("\r\n ** R_GPT_Close API failed ** \r\n");
    }
}
/*******************************************************************************************************************//**
 * @} (end addtogroup r_gpt_ep)
 **********************************************************************************************************************/
