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
#include "spi_ep.h"

void R_BSP_WarmStart(bsp_warm_start_event_t event);
/*******************************************************************************************************************//**
 * @addtogroup r_rspi_ep
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief The RZ Configuration tool generates main() and uses it to generate threads if an RTOS is used.  This function is
 *        called by main() when no RTOS is used.
 * @param[IN]   None
 * @retval      None
 **********************************************************************************************************************/
void hal_entry(void)
{
    fsp_err_t err = FSP_SUCCESS;
    fsp_pack_version_t version = {RESET_VALUE};
    unsigned char rByte[BUFFER_SIZE_DOWN] = {RESET_VALUE};
    uint8_t converted_rtt_input = RESET_VALUE;

    /* version get API for FLEX pack information */
    R_FSP_VersionGet(&version);

    /* Example Project information printed on the Console */
    APP_PRINT(BANNER_1);
    APP_PRINT(BANNER_2);
    APP_PRINT(BANNER_3,EP_VERSION);
    APP_PRINT(BANNER_4,version.major, version.minor, version.patch );
    APP_PRINT(BANNER_5);
    APP_PRINT(BANNER_6);
    APP_PRINT(BANNER_7);

    /* Initialize SPI driver */
    err = spi_init();

    if (FSP_SUCCESS != err)
    {   /* SPI module init failed */
        APP_PRINT("\r\n ** RSPI INIT FAILED ** \r\n");
        APP_ERR_TRAP(err);
    }
    else
    {
        APP_PRINT("\r\n ** RSPI INIT SUCCESSFUL ** \r\n");
    }

    /* Menu for User Selection */
    APP_PRINT("\r\nSelect from the below Menu options\r\n");
    APP_PRINT("\r\nPress 1 for Write() and Read()");
    APP_PRINT("\r\nPress 2 for WriteRead()");
    APP_PRINT("\r\nPress 3 to Exit\r\n");

    while (EXIT != converted_rtt_input)
    {
        if (APP_CHECK_DATA)
        {
            APP_READ(rByte);
            converted_rtt_input = (uint8_t)atoi((char *)rByte);

            switch (converted_rtt_input)
            {
                case WRITE_AND_READ:
                    err = spi_write_and_read ();
                    if (FSP_SUCCESS != err)
                    {
                        APP_PRINT("** RSPI WRITE AND READ Demo failed **  \r\n");
                        spi_clean_up();
                        APP_ERR_TRAP(err);
                    }
                    else
                    {
                        APP_PRINT("\r\n** RSPI WRITE AND READ Demo Successful**  \r\n");
                    }
                    break;
                case WRITE_READ:
                    err = spi_write_read ();
                    if (FSP_SUCCESS != err)
                    {
                        APP_PRINT("\r\n** RSPI WRITE Demo failed **  \r\n");
                        spi_clean_up();
                        APP_ERR_TRAP(err);
                    }
                    else
                    {
                        APP_PRINT("** RSPI WRITE_READ Demo Successful**  \r\n");
                    }
                    break;
                case EXIT:
                    err = spi_exit_demo();
                    if (FSP_SUCCESS != err)
                    {
                        APP_PRINT("** RSPI EXIT Demo failed ** \r\n");
                        APP_ERR_TRAP(err);
                    }
                    else
                    {
                        APP_PRINT("** RSPI EXIT Demo Successful ** \r\n");
                    }
                    break;
                default:
                    /* Menu for User Selection */
                    APP_PRINT("\r\nSelect from the below Menu options\r\n");
                    APP_PRINT("\r\nPress 1 for Write() and Read()");
                    APP_PRINT("\r\nPress 2 for WriteRead()");
                    APP_PRINT("\r\nPress 3 to Exit\r\n");
                    break;
            }
            if((WRITE_AND_READ <= converted_rtt_input) && (EXIT > converted_rtt_input))
            {
                APP_PRINT ("\r\nEnter any other key to go back to the main menu\r\n");
            }
        }
    }

    APP_PRINT(RTT_CTRL_TEXT_BRIGHT_CYAN "\r\nExiting RSPI demo....\r\n" RTT_CTRL_RESET);
}

/*******************************************************************************************************************//**
 * @brief This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  Event where at in the start up process the code is currently at
 * @retval      None
 **********************************************************************************************************************/
void R_BSP_WarmStart(bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open (&g_ioport_ctrl, &g_bsp_pin_cfg);
    }
}
/*******************************************************************************************************************//**
 * @} (end addtogroup r_rspi_ep)
 **********************************************************************************************************************/
