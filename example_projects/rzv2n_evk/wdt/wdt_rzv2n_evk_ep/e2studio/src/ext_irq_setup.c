/***********************************************************************************************************************
 * File Name    : ext_irq_setup.c
 * Description  : Contains data structures and functions used in ext_irq_setup.c
 **********************************************************************************************************************/
/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */
/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

#include "common_utils.h"
#include "ext_irq_setup.h"

/*******************************************************************************************************************//**
 * @addtogroup r_wdt_ep
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief     Initialize the IRQ module.
 * @param[IN]   None
 * @retval FSP_SUCCESS                  Upon successful open
 * @retval Any Other Error code apart from FSP_SUCCES  Unsuccessful open
 ***********************************************************************************************************************/
fsp_err_t init_irq_module(void)
{
    /* variable to track error and return values */
    fsp_err_t err = FSP_SUCCESS;

    /* Open IRQ module */
    err = R_INTC_IRQ_ExternalIrqOpen(&g_external_irq_ctrl, &g_external_irq_cfg);
    if (FSP_SUCCESS != err)
    {
        /* Print Error on RTT console */
        APP_ERR_PRINT ("\r\n ** R_INTC_IRQ_ExternalIrqOpen API Failed ** \r\n");
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief    Enable the User Push Button and return the error to the Application.
 * @param[IN]   None
 * @retval FSP_SUCCESS                 Push Button enabled successfully.
 * @retval Any Other Error code apart from FSP_SUCCES  Unsuccessful to enable push button
 ***********************************************************************************************************************/
fsp_err_t enable_irq_module(void)
{
    /* variable to track error and return values */
    fsp_err_t err = FSP_SUCCESS;

    /* Enable external irq */
    err = R_INTC_IRQ_ExternalIrqEnable(&g_external_irq_ctrl);
    if (FSP_SUCCESS != err)
    {
        /* Print Error on RTT console */
        APP_ERR_PRINT ("\r\n ** R_IRQ_ExternalIrqEnable API Failed ** \r\n");
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief    Close the IRQ HAL driver and Handle the return closing API error, to the Application.
 * @param[IN]   None
 * @retval None
 ***********************************************************************************************************************/
void deinit_irq_module(void)
{
    /* variable to track error and return values */
    fsp_err_t err = FSP_SUCCESS;

    /* Close the ICU module */
    err=  R_INTC_IRQ_ExternalIrqClose(&g_external_irq_ctrl);
    /* handle error */
    if (FSP_SUCCESS != err)
    {
        /* Print Error on RTT console */
        APP_ERR_PRINT("\r\n ** R_INTC_IRQ_ExternalIrqClose API failed ** \r\n");
        return;
    }
}

/*******************************************************************************************************************//**
 * This function is called when user presses the push button.
 * @brief This functions is used to stop the gtm timer.
 * @param[IN] p_args    external irq callback argument
 * @retval    None
 **********************************************************************************************************************/
void ext_irq_callback(external_irq_callback_args_t *p_args)
{
    /* variable to track error and return values */
    fsp_err_t err = FSP_SUCCESS;

    FSP_PARAMETER_NOT_USED(p_args);
    system_reset_flag = true;
    /* By pressing push button, gtm timer will stops running */
    err = R_GTM_Stop(&g_timer_ctrl);

    if (FSP_SUCCESS != err)
    {
        /* Print Error on RTT console */
        APP_ERR_PRINT("\r\n ** R_GTM_Stop API failed ** \r\n");
        return;
    }
    APP_PRINT ("Push button is pressed.\r\nGTM timer stopped.\r\n");
    R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MILLISECONDS);


}

/*******************************************************************************************************************//**
 * @} (end addtogroup r_wdt_ep)
 **********************************************************************************************************************/
