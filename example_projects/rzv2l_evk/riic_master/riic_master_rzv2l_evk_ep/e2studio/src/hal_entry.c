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
#include "common_utils.h"
#include "i2c_sensor.h"
#include "stdbool.h"
#include "stdint.h"
#define MODULE_NAME		"r_riic_master"
FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event);
void SCL_Toggle(void);
/*******************************************************************************************************************//**
 * @addtogroup r_riic_master_ep
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief The RZ Configuration tool generates main() and uses it to generate threads if an RTOS is used.  This function is
 *        called by main() when no RTOS is used.
 * @param[IN]   None
 * @retval      None
 **********************************************************************************************************************/
FSP_CPP_FOOTER


void hal_entry(void)
{
    fsp_err_t err = FSP_SUCCESS;
    fsp_pack_version_t version = {RESET_VALUE};
    uint8_t xyz_axis[6]      = {RESET_VALUE};
    char flt_str[64]        = {RESET_VALUE};
    float x_axis            = 0.0f;
    float y_axis            = 0.0f;
    float z_axis            = 0.0f;

    /* version get API for FLEX pack information */
    R_FSP_VersionGet(&version);

    /* Example Project information printed on the Console */
    APP_PRINT(BANNER_INFO,EP_VERSION,version.version_id_b.major, version.version_id_b.minor, version.version_id_b.patch );
    APP_PRINT("\nThis project utilizes PMOD ACL sensor as iic slave device\n");
    APP_PRINT("Upon successful initialization, MPU displays sensor axis data\n");
    APP_PRINT("\nIf SDA line is kept in LOW by any error \n");
    APP_PRINT("Please input 1 on J-Link RTT Viewer to Open bus.\n\n\n");

    /* Opening IIC master module and Initializing PMOD ACL sensor */
    err = init_sensor();

    if(FSP_SUCCESS != err)
    {
        /* Sensor init failed, so cleanup the sensor specific initialization */
        APP_ERR_PRINT("** SENSOR INIT FAILED ** \r\n");
        deinit_sensor();
        APP_ERR_TRAP(err);
    }

    /* Stay in forever loop and read the sensor data every 3 Seconds
     * If the Sensor data reading returns error, cleanup and break with TRAP.
     * Note: For Demonstration Purpose the failure ends up with TRAP.
     * Note: This can be handled in many ways as per the Application needs
     */
    while (1)
    {
        /* Read PMOD ACL sensor data */
        err =  read_sensor_data(xyz_axis);
        if(FSP_SUCCESS != err)
        {
            APP_ERR_PRINT("\r\n ** SENSOR READ DATA FAILED ** \r\n");
            deinit_sensor();
            APP_ERR_TRAP(err);
        }
        else
        {
             /*
              * X,Y,Z - axis data has to be evaluated so accessing particular array value,
              * shifting it by 8 bits obtains data value to float variable
              */
            x_axis = (float) (xyz_axis[0] | (xyz_axis[1] << BIT_SHIFT_8));   // X-axis value
            y_axis = (float) (xyz_axis[2] | (xyz_axis[3] << BIT_SHIFT_8));   // Y-axis value
            z_axis = (float) (xyz_axis[4] | (xyz_axis[5] << BIT_SHIFT_8));   // Z-axis value

            snprintf(flt_str,SIZE_64,"X-axis = %.02f, Y-axis = %.02f, Z-axis = %.02f",x_axis,y_axis,z_axis);
            APP_PRINT("%s \r\n",flt_str);

            /*Check SDA level*/
            if (!((g_i2c_master_ctrl.p_reg)->ICCR1 & R_RIIC0_ICCR1_SDAI_Msk))
            {
                APP_PRINT("\nSlave keep the SDA line in low level.\r");

                /*Check whether any user input */
                while(true)
                {
                    if (APP_CHECK_DATA)
                    {
                        unsigned char buf[16]       = {0x00};
                        APP_READ(buf);

                        /*Check the input */
                        if ((uint32_t) (atoi((char *)buf)) == 1)
                        {
                            SCL_Toggle();
                        }

                        /* Clear the buffer */
                        memset (buf,'\0',16U);
                        break;
                    }
                }
            }

            /* 3 seconds wait time between successive readings */
            R_BSP_SoftwareDelay(SENSOR_READ_DELAY, BSP_DELAY_UNITS_SECONDS);
        }
    }
}
/*******************************************************************************************************************//**
 * This function create the extra SCL Clock cycle by controlling SCLO bit
 **********************************************************************************************************************/
void SCL_Toggle(void)
{
        uint32_t temp_iccr1 = 0 ;
        APP_PRINT("\nToggle the SCL bus.\r");

        for(uint8_t i = 24; i  > 0 ; i--)
        {
        /*Change the SDAO bit to 1 and SOWP to 0 */
        temp_iccr1 = (g_i2c_master_ctrl.p_reg)->ICCR1;
        temp_iccr1 &= ~R_RIIC0_ICCR1_SOWP_Msk;
        temp_iccr1 ^= R_RIIC0_ICCR1_SCLO_Msk;
        temp_iccr1 ^= R_RIIC0_ICCR1_SDAO_Msk;
        (g_i2c_master_ctrl.p_reg)->ICCR1 = temp_iccr1;
        /* 2 microseconds delay*/
        R_BSP_SoftwareDelay(2, BSP_DELAY_UNITS_MICROSECONDS);
        }
}
/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart(bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
    }

    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open(&IOPORT_CFG_CTRL, &IOPORT_CFG_NAME);
    }
}
/*******************************************************************************************************************//**
 * @} (end addtogroup r_riic_master_ep)
 **********************************************************************************************************************/

