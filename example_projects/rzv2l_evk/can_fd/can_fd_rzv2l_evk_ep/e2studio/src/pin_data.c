/***********************************************************************************************************************
 * File Name    : pin_data.c
 * Description  : Contains configuration pins for smarc boards.
 *********************************************************************************************************************/
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

#include "bsp_api.h"
#include "r_ioport_api.h"
#include "r_ioport.h"

const ioport_pin_cfg_t g_bsp_pin_cfg_data[] =
{
    { .pin = BSP_IO_PORT_10_PIN_01, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
          | (uint32_t) IOPORT_PERIPHERAL_MODE2 | (uint32_t) IOPORT_CFG_DRIVE_B11
          | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST)
    },
    { .pin = BSP_IO_PORT_11_PIN_00, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
          | (uint32_t) IOPORT_PERIPHERAL_MODE2 | (uint32_t) IOPORT_CFG_DRIVE_B11
          | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST)
    },
    { .pin = BSP_IO_PORT_12_PIN_01, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
          | (uint32_t) IOPORT_PERIPHERAL_MODE2 | (uint32_t) IOPORT_CFG_DRIVE_B11
          | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST)
    },
    { .pin = BSP_IO_PORT_13_PIN_00, .pin_cfg = ((uint32_t) IOPORT_CFG_PERIPHERAL_PIN
          | (uint32_t) IOPORT_PERIPHERAL_MODE2 | (uint32_t) IOPORT_CFG_DRIVE_B11
          | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST)
    },
    { .pin = BSP_IO_PORT_43_PIN_00, .pin_cfg = ((uint32_t) IOPORT_CFG_PORT_DIRECTION_OUTPUT
          | (uint32_t) IOPORT_CFG_PORT_OUTPUT_LOW)
    },
    { .pin = BSP_IO_PORT_43_PIN_02, .pin_cfg = ((uint32_t) IOPORT_CFG_PORT_DIRECTION_OUTPUT
          | (uint32_t) IOPORT_CFG_PORT_OUTPUT_LOW)
    },
    { .pin = BSP_IO_PORT_41_PIN_00, .pin_cfg = ((uint32_t) IOPORT_CFG_PORT_DIRECTION_OUTPUT
          | (uint32_t) IOPORT_CFG_PORT_OUTPUT_LOW)
    },
    { .pin = BSP_IO_PORT_43_PIN_01, .pin_cfg = ((uint32_t) IOPORT_CFG_PORT_DIRECTION_OUTPUT
          | (uint32_t) IOPORT_CFG_PORT_OUTPUT_LOW)
    },

};

const ioport_cfg_t g_bsp_pin_cfg =
{
    .number_of_pins = sizeof(g_bsp_pin_cfg_data) / sizeof(ioport_pin_cfg_t),
    .p_pin_cfg_data = &g_bsp_pin_cfg_data[0],
};
