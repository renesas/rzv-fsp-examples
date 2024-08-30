/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "xspi_init.h"

/* Parameter for "WRITE ENHANCED VOLATILE CONFIGURATION REGISTER" command. */
static spi_flash_direct_transfer_t g_qspi0_w =
{
    .address        = 0,
    .data           = 0xFF,
    .command        = 0x61,
    .dummy_cycles   = SPI_FLASH_DUMMY_CLOCKS_0,
    .command_length = 1,
    .address_length = 0,
    .data_length    = 1,
};

/* Parameter for "WRITE ENABLE REGISTER" command. */
static spi_flash_direct_transfer_t g_qspi0_we =
{
    .address        = 0,
    .data           = 0,
    .command        = 0x06,
    .dummy_cycles   = SPI_FLASH_DUMMY_CLOCKS_0,
    .command_length = 1,
    .address_length = 0,
    .data_length    = 0,
};

/*******************************************************************************************************************//**
 * @brief  Open r_xspi_qspi driver
 *
 **********************************************************************************************************************/
void xspi_open (void)
{
    /* Set xSPI clock: The frequency is set to 50MHz to issue the H'03(READ) command. */
    uint32_t reg = 0;
    reg             |= R_CPG_CPG_SSEL1_SELCTL3WEN_Msk | R_CPG_CPG_SSEL1_SELCTL2WEN_Msk; // write enable
    reg             |= 0 << R_CPG_CPG_SSEL1_SELCTL3_Pos;                                // SMUX2_XSPI_CLK1: 0b= SMUX2_XSPI_CLK0
    reg             |= 1 << R_CPG_CPG_SSEL1_SELCTL2_Pos;                                // SMUX2_XSPI_CLK0: 1b= CDIV4_CLK400_PLLCM33_FIX
    R_CPG->CPG_SSEL1 = reg;

    /* Divisor=1/4 */
    reg               = 0;
    reg              |= R_CPG_CPG_CSDIV0_DIVCTL3WEN_Msk;   // write enable
    reg              |= 1 << R_CPG_CPG_CSDIV0_DIVCTL3_Pos; // CSDIV_2to16_PLLCM33_XSPI: 01b= 1/4
    R_CPG->CPG_CSDIV0 = reg;

    /* Set PFC_OEN register to xSPI enable */
    R_BSP_XSPIOutputEnableCfg();

    /* Open xSPI driver */
    g_qspi0.p_api->open(g_qspi0.p_ctrl, g_qspi0.p_cfg);
}

/*******************************************************************************************************************//**
 * @brief  Pre-initialize function for r_xspi_qspi driver
 *
 * Specify this function to "Extra->Pre initialize function" of the SmartCconfigurator property of the r_xspi_qspi driver.
 *
 **********************************************************************************************************************/
fsp_err_t xspi_pre_init (spi_flash_api_t const * p_api, xspi_qspi_instance_ctrl_t * p_ctrl)
{
    FSP_PARAMETER_NOT_USED(p_api);

    /* Reset the device connected to CS0 */
    p_ctrl->p_reg->LIOCTL_b.RSTCS0 = 0;
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MICROSECONDS);
    p_ctrl->p_reg->LIOCTL_b.RSTCS0 = 1;

    /* Set the operating mode on the device side */
    if ((SPI_FLASH_PROTOCOL_2S_2S_2S == p_ctrl->p_cfg->spi_protocol) ||
        (SPI_FLASH_PROTOCOL_4S_4S_4S == p_ctrl->p_cfg->spi_protocol))
    {
        if (SPI_FLASH_PROTOCOL_2S_2S_2S == p_ctrl->p_cfg->spi_protocol)
        {
            g_qspi0_w.data = 0xBF;     /* Dual I/O protocol(bit6) Enable */
        }
        else
        {
            g_qspi0_w.data = 0x7F;     /* Quad I/O protocol(bit6) Enable */
        }

        /* Set SPI protocol to 1S-1S-1S */
        p_api->spiProtocolSet(p_ctrl, SPI_FLASH_PROTOCOL_1S_1S_1S);

        /* Issuing the WREN command */
        p_api->directTransfer(p_ctrl, &g_qspi0_we, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);

        /* Issuing the Write enhanced volatile configuraiton register command */
        p_api->directTransfer(p_ctrl, &g_qspi0_w, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);

        /* Set SPI protocol to spi_protocol */
        p_api->spiProtocolSet(p_ctrl, g_qspi0_cfg.spi_protocol);
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Post-initialize function for r_xspi_qspi driver
 *
 * Specify this function to "Extra->Post initialize function" of the SmartCconfigurator property of the r_xspi_qspi driver.
 *
 **********************************************************************************************************************/
fsp_err_t xspi_post_init (spi_flash_api_t const * p_api, xspi_qspi_instance_ctrl_t * p_ctrl)
{
    FSP_PARAMETER_NOT_USED(p_api);
    FSP_PARAMETER_NOT_USED(p_ctrl);

    return FSP_SUCCESS;
}


void xspi_close (void){

   xspi_qspi_instance_ctrl_t * p_instance_ctrl = (xspi_qspi_instance_ctrl_t *) g_qspi0.p_ctrl;

   /*Issue dummy read (4Byte INCR) for both channel Accept response of dummy read*/
   FSP_REGISTER_READ(*((uint32_t *)0x60000000));

   p_instance_ctrl->p_reg->BMCTL0 = 0x0;

   g_qspi0.p_api->close(g_qspi0.p_ctrl);

#ifdef BSP_CFG_MCU_LAUNCH_CA55
   uint32_t reg = 1;
   reg             |= R_CPG_CPG_SSEL1_SELCTL3WEN_Msk | R_CPG_CPG_SSEL1_SELCTL2WEN_Msk; // write enable
   reg             |= 1 << R_CPG_CPG_SSEL1_SELCTL3_Pos;                                // SMUX2_XSPI_CLK1: 1b= CDIV5_CLK320_PLLCM33_FIX
   reg             |= 0 << R_CPG_CPG_SSEL1_SELCTL2_Pos;                                // SMUX2_XSPI_CLK0: 0b= : CDIV3_CLK533_PLLCM33_FIX (default)
   R_CPG->CPG_SSEL1 = reg;

   /* Divisor=1/8 */
   reg               = 0;
   reg              |= R_CPG_CPG_CSDIV0_DIVCTL3WEN_Msk;   // write enable
   reg              |= 2 << R_CPG_CPG_CSDIV0_DIVCTL3_Pos; // CSDIV_2to16_PLLCM33_XSPI: 10b= 1/8
   R_CPG->CPG_CSDIV0 = reg;
#endif
   R_BSP_MODULE_RSTON(FSP_IP_XSPI, 0);

}
