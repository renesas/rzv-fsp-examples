/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pd_axi_on.h"

/*******************************************************************************************************************//**
 * @addtogroup pd_axi_on
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 *  @brief
 *  @param[IN]   None
 *  @retval      FSP_SUCCESS
 *  @retval      Any Other Error code apart from FSP_SUCCESS is  Unsuccessful open or start
 **********************************************************************************************************************/
fsp_err_t pd_all_on_postproc_axi (void)
{
    fsp_err_t         err = FSP_SUCCESS;
    volatile uint32_t reg32;

    /* Allow access "from MCPU-Bus to ACPU-Bus" and "from ACPU-Bus to MCPU-Bus" */
    R_CPG->CPG_BUS_12_MSTOP = 0x06000000;

    /* Release reset of AXIs */
    R_CPG->CPG_RST_15 = 0x40004000;    /* Release reset of RCPU_AXI_RESETN and MCPU_AXI_RESETN */
    R_CPG->CPG_RST_16 = 0xFFFFFFFF;    /* Release reset of ACPU_AXI* and VIDEO_AXI* and DRP_AXI* and COM_AXI* and TZC_AXI* */
    R_CPG->CPG_RST_17 = 0x03FF03FF;    /* Release reset of TZC_AXI* */

    do
    {
        reg32 = R_CPG->CPG_RSTMON_7;
    } while ((reg32 & 0xFFFF0000) != 0x00000000);

    do
    {
        reg32 = R_CPG->CPG_RSTMON_8;
    } while ((reg32 & 0x000003FF) != 0x00000000);

    return err;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup pd_axi_on)
 **********************************************************************************************************************/
