/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "i2c_pmic.h"
#include "pll_init.h"

/*******************************************************************************************************************//**
 * @addtogroup pd_all_on
 * @{
 **********************************************************************************************************************/

#if defined(BSP_FEATURE_BSP_HAS_CM33BOOT_SUPPORT) && (BSP_FEATURE_BSP_HAS_CM33BOOT_SUPPORT == 1)

/*******************************************************************************************************************//**
 *  @brief
 *  @param[IN]   None
 *  @retval      FSP_SUCCESS
 *  @retval      Any Other Error code apart from FSP_SUCCESS is  Unsuccessful open or start
 **********************************************************************************************************************/
fsp_err_t pd_all_on_preproc (void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Release reset of all CLMAs */
    R_CPG->CPG_CLMA0_CTL  = 0x001F001F;
    R_CPG->CPG_CLMA1_CTL  = 0x001F001F;
    R_CPG->CPG_CLMA2_CTL  = 0x001F001F;
    R_CPG->CPG_CLMA3_CTL  = 0x001F001F;
    R_CPG->CPG_CLMA4_CTL  = 0x001F001F;
    R_CPG->CPG_CLMA5_CTL  = 0x001F001F;
    R_CPG->CPG_CLMA6_CTL  = 0x001F001F;
    R_CPG->CPG_CLMA7_CTL  = 0x001F001F;
    R_CPG->CPG_CLMA8_CTL  = 0x001F001F;
    R_CPG->CPG_CLMA9_CTL  = 0x001F001F;
    R_CPG->CPG_CLMA10_CTL = 0x001F001F;
    R_CPG->CPG_CLMA11_CTL = 0x001F001F;
    R_CPG->CPG_CLMA12_CTL = 0x001F001F;
    R_CPG->CPG_CLMA13_CTL = 0x001F001F;
    R_CPG->CPG_CLMA14_CTL = 0x001F001F;

    return err;
}

/*******************************************************************************************************************//**
 *  @brief
 *  @param[IN]   None
 *  @retval      FSP_SUCCESS
 *  @retval      Any Other Error code apart from FSP_SUCCESS is  Unsuccessful open or start
 **********************************************************************************************************************/
fsp_err_t pd_all_on (void)
{
    fsp_err_t         err = FSP_SUCCESS;
    volatile uint32_t reg32;

    /****** PD_AWO->PD_ALL ******/

    // Pre-Process
    // #1
 #if 0
    R_CPG->CPG_RST_0  = 0x3FFF0000;
    R_CPG->CPG_RST_1  = 0x00070000;
    R_CPG->CPG_RST_2  = 0x1FFF0000;
    R_CPG->CPG_RST_3  = 0xE7FE0000;
    R_CPG->CPG_RST_4  = 0x003F0000;
    R_CPG->CPG_RST_5  = 0xFF800000;
    R_CPG->CPG_RST_6  = 0x9FFF0000;
    R_CPG->CPG_RST_7  = 0xFFFF0000;
    R_CPG->CPG_RST_8  = 0xFFFF0000;
    R_CPG->CPG_RST_9  = 0xFFDF0000;
    R_CPG->CPG_RST_10 = 0xFF9E0000;
    R_CPG->CPG_RST_11 = 0xFFFF0000;
    R_CPG->CPG_RST_12 = 0xFFFF0000;
    R_CPG->CPG_RST_13 = 0xF9FF0000;
    R_CPG->CPG_RST_14 = 0xFFFF0000;
    R_CPG->CPG_RST_15 = 0x71FF0000;
    R_CPG->CPG_RST_16 = 0xFFFF0000;
    R_CPG->CPG_RST_17 = 0x03FF0000;
 #endif

    // #2
    R_CPG->CPG_OTHERS_INI = 0x00000001;

    // #3
    pmic_sleep_to_active();

    // #4
    // waiting for stabilization
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);

    // Entry
    // #1
    R_CPG->CPG_LP_PWC_CTL1 = 0x00000001;

    // #2
    do
    {
        reg32 = R_CPG->CPG_LP_PWC_CTL2;
    } while ((reg32 & 0x0000006) != 0x00000006);

    // #3
    R_CPG->CPG_LP_PWC_CTL1 = 0x00000000;

    // #4
    R_CPG->CPG_BUS_12_MSTOP = 0x06000600;

    // #5
    R_CPG->CPG_OTHERS_INI = 0x00000000;

    // #6, #7
    pll_init_cm33boot();

    // #8 Clock ON several IPs
    R_CPG->CPG_CLKON_0  = 0xFFDFFFDF;
    R_CPG->CPG_CLKON_1  = 0xFFFBFFFB;
    R_CPG->CPG_CLKON_2  = 0x80008000;
    R_CPG->CPG_CLKON_3  = 0xFFFEFFFE;
    R_CPG->CPG_CLKON_4  = 0x7FFF7FFF;
    R_CPG->CPG_CLKON_5  = 0xFFF0FFF0;
    R_CPG->CPG_CLKON_6  = 0xFFFFFFFF;
    R_CPG->CPG_CLKON_7  = 0xFFFFFFFF;
    R_CPG->CPG_CLKON_8  = 0x7FFF7FFF;
    R_CPG->CPG_CLKON_9  = 0x7FFF7FFF;
    R_CPG->CPG_CLKON_10 = 0xFFF8FFF8;
    R_CPG->CPG_CLKON_11 = 0xFFFFFFFF;
    R_CPG->CPG_CLKON_12 = 0xFFFFFFFF;
    R_CPG->CPG_CLKON_13 = 0xFFFFFFFF;
    R_CPG->CPG_CLKON_14 = 0xFFFFFFFF;
    R_CPG->CPG_CLKON_15 = 0xFFFFFFFF;
    R_CPG->CPG_CLKON_16 = 0x06010601;
    R_CPG->CPG_CLKON_17 = 0x0FFF0FFF;
    R_CPG->CPG_CLKON_18 = 0xFFFCFFFC;
    R_CPG->CPG_CLKON_19 = 0xFFFFFFFF;
    R_CPG->CPG_CLKON_20 = 0xFFFFFFFF;
    R_CPG->CPG_CLKON_21 = 0x000F000F;

    // #9,#10
    R_CPG->CPG_LP_PMU_CTL1 = 0x00000003;

    // #11
    do
    {
        reg32 = R_CPG->CPG_LP_PMU_CTL1;
    } while ((reg32 & 0x0000000C) != 0x00000008);

    // #12
    R_CPG->CPG_LP_PMU_CTL1 = 0x00000002;

    // #13
    do
    {
        reg32 = R_CPG->CPG_OTP_HANDSHAKE_MON;
    } while ((reg32 & 0x00000001) != 0x00000001);

    return err;
}

/*******************************************************************************************************************//**
 *  @brief
 *  @param[IN]   None
 *  @retval      FSP_SUCCESS
 *  @retval      Any Other Error code apart from FSP_SUCCESS is  Unsuccessful open or start
 **********************************************************************************************************************/
fsp_err_t pd_all_on_postproc (void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Allow access to RCPU-Bus(Initialize TZC400-RCPU) */
    R_TZC_AXI_RCPU->GATE_KEEPER        = 0x00000000; /* Close filter before initialization */
    R_TZC_AXI_RCPU->REGION_ID_ACCESS_0 = 0x000F000F; /* Permit Secure/Nonsecure access */
    R_TZC_AXI_RCPU->GATE_KEEPER        = 0x00000001; /* Open filter #0 */

    /* Allow access to CR8(TCM) */
    R_CPG->CPG_BUS_10_MSTOP = 0x04000000;

    /* Allow access to ACPU SRAM */
    R_TZC_SRAMA->GATE_KEEPER        = 0x00000000; /* Close filter before initialization */
    R_TZC_SRAMA->REGION_ID_ACCESS_0 = 0x000F000F; /* Permit Secure/Nonsecure access */
    R_TZC_SRAMA->GATE_KEEPER        = 0x00000001; /* Open filter #0 */

    /* Release Reset of GIC and ACPU SRAMs */
    R_CPG->CPG_RST_3 = 0xFF00FF00;
    R_CPG->CPG_RST_4 = 0x003F003F;

    /* Allow access to xSPI(Initialize TZC400_XSPI) */
    R_TZC_XSPI->GATE_KEEPER         = 0x00000000; /* Close filter before initialization */
    R_TZC_XSPI->REGION_ID_ACCESS_0  = 0x000F000F; /* Permit Secure/Nonsecure access */
    R_TZC_XSPI->REGION_ATTRIBUTES_0 = 0x0000000F; /* Disable secure read/write access */
    R_TZC_XSPI->SPECULATION_CTRL    = 0x00000003; /* Disable read/write speculation */
    R_TZC_XSPI->GATE_KEEPER         = 0x0000000F; /* Open filter #0-#3 */

    return err;
}

#endif                                 /*defined(BSP_FEATURE_BSP_HAS_CM33BOOT_SUPPORT) && (BSP_FEATURE_BSP_HAS_CM33BOOT_SUPPORT == 1)*/

/*******************************************************************************************************************//**
 * @} (end addtogroup pd_all_on)
 **********************************************************************************************************************/
