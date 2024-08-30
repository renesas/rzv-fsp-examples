/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pll_init.h"

void pll_init_cm33boot (void)
{
    volatile uint32_t reg32;

    /*****************************************/
    /* Setting PLLCLN                        */
    /*****************************************/

    /* Setting PLL for normal mode */
    R_CPG->CPG_PLLCLN_STBY = R_CPG_CPG_PLLCLN_STBY_RESETB_WEN_Msk | R_CPG_CPG_PLLCLN_STBY_RESETB_Msk;

    /* Wait until the PLL is in locked-state */
    do
    {
        reg32 = R_CPG->CPG_PLLCLN_MON;
    } while ((reg32 & (R_CPG_CPG_PLLCLN_MON_PLLn_LOCK_Msk)) != (R_CPG_CPG_PLLCLN_MON_PLLn_LOCK_Msk));

    /*****************************************/
    /* Setting PLLDTY                        */
    /*****************************************/

    /* Setting PLL for normal mode */
    R_CPG->CPG_PLLDTY_STBY = R_CPG_CPG_PLLDTY_STBY_RESETB_WEN_Msk | R_CPG_CPG_PLLDTY_STBY_RESETB_Msk;

    /* Wait until the PLL is in locked-state */
    do
    {
        reg32 = R_CPG->CPG_PLLDTY_MON;
    } while ((reg32 & (R_CPG_CPG_PLLDTY_MON_PLLn_LOCK_Msk)) != (R_CPG_CPG_PLLDTY_MON_PLLn_LOCK_Msk));

    /*****************************************/
    /* Setting PLLCA55                       */
    /*****************************************/

    /* Output clock settings 1 */
    R_CPG->CPG_PLLCA55_CLK1 = CPG_PLLCA55_CLK1_INITVAL;

    /* Setting the output clock and SSC modulation value 2 */
    R_CPG->CPG_PLLCA55_CLK2 = CPG_PLLCA55_CLK2_INITVAL;

    /* Setting PLL for normal mode */
    R_CPG->CPG_PLLCA55_STBY = R_CPG_CPG_PLLCA55_STBY_RESETB_WEN_Msk | R_CPG_CPG_PLLCA55_STBY_RESETB_Msk;

    /* Wait until the PLL is in locked-state */
    do
    {
        reg32 = R_CPG->CPG_PLLCA55_MON;
    } while ((reg32 & (R_CPG_CPG_PLLCA55_MON_PLLn_LOCK_Msk)) != (R_CPG_CPG_PLLCA55_MON_PLLn_LOCK_Msk));

    /*****************************************/
    /* Setting PLLVDO                        */
    /*****************************************/

    /* Setting PLL for normal mode */
    R_CPG->CPG_PLLVDO_STBY = R_CPG_CPG_PLLVDO_STBY_RESETB_WEN_Msk | R_CPG_CPG_PLLVDO_STBY_RESETB_Msk;

    /* Wait until the PLL is in locked-state */
    do
    {
        reg32 = R_CPG->CPG_PLLVDO_MON;
    } while ((reg32 & (R_CPG_CPG_PLLVDO_MON_PLLn_LOCK_Msk)) != (R_CPG_CPG_PLLVDO_MON_PLLn_LOCK_Msk));

    /*****************************************/
    /* Setting PLLDRP                        */
    /*****************************************/

    /* Output clock settings 1 */
    R_CPG->CPG_PLLDRP_CLK1 = CPG_PLLDRP_CLK1_INITVAL;

    /* Setting the output clock and SSC modulation value 2 */
    R_CPG->CPG_PLLDRP_CLK2 = CPG_PLLDRP_CLK2_INITVAL;

    /* Setting PLL for normal mode */
    R_CPG->CPG_PLLDRP_STBY = R_CPG_CPG_PLLDRP_STBY_RESETB_WEN_Msk | R_CPG_CPG_PLLDRP_STBY_RESETB_Msk;

    /* Wait until the PLL is in locked-state */
    do
    {
        reg32 = R_CPG->CPG_PLLDRP_MON;
    } while ((reg32 & (R_CPG_CPG_PLLDRP_MON_PLLn_LOCK_Msk)) != (R_CPG_CPG_PLLDRP_MON_PLLn_LOCK_Msk));
}

void pll_init_other (void)
{
    volatile uint32_t reg32;

    /*****************************************/
    /* Setting PLLETH                        */
    /*****************************************/

    /* Wait until the PLL is in standby mode and unlocked-state */
    do
    {
        reg32 = R_CPG->CPG_PLLETH_MON;
    } while ((reg32 & (R_CPG_CPG_PLLETH_MON_PLLn_LOCK_Msk | R_CPG_CPG_PLLETH_MON_PLLn_RESETB_Msk)) !=
             0);

    /* Setting PLL for normal mode */
    R_CPG->CPG_PLLETH_STBY = R_CPG_CPG_PLLETH_STBY_RESETB_WEN_Msk | R_CPG_CPG_PLLETH_STBY_RESETB_Msk;

    /* Wait until the PLL is in normal mode and locked-state */
    do
    {
        reg32 = R_CPG->CPG_PLLETH_MON;
    } while ((reg32 & (R_CPG_CPG_PLLETH_MON_PLLn_LOCK_Msk | R_CPG_CPG_PLLETH_MON_PLLn_RESETB_Msk)) !=
             (R_CPG_CPG_PLLETH_MON_PLLn_LOCK_Msk | R_CPG_CPG_PLLETH_MON_PLLn_RESETB_Msk));

    /*****************************************/
    /* Setting PLLDSI                        */
    /*****************************************/

    /* Wait until the PLL is in standby mode and unlocked-state */
    do
    {
        reg32 = R_CPG->CPG_PLLDSI_MON;
    } while ((reg32 & (R_CPG_CPG_PLLDSI_MON_PLLn_LOCK_Msk | R_CPG_CPG_PLLDSI_MON_PLLn_RESETB_Msk)) !=
             0);

    /* Output clock settings 1 */
    R_CPG->CPG_PLLDSI_CLK1 = CPG_PLLDSI_CLK1_INITVAL;

    /* Setting the output clock and SSC modulation value 2 */
    R_CPG->CPG_PLLDSI_CLK2 = CPG_PLLDSI_CLK2_INITVAL;

    /* Setting PLL for normal mode */
    R_CPG->CPG_PLLDSI_STBY = R_CPG_CPG_PLLDSI_STBY_RESETB_WEN_Msk | R_CPG_CPG_PLLDSI_STBY_RESETB_Msk;

    /* Wait until the PLL is in normal mode and locked-state */
    do
    {
        reg32 = R_CPG->CPG_PLLDSI_MON;
    } while ((reg32 & (R_CPG_CPG_PLLDSI_MON_PLLn_LOCK_Msk | R_CPG_CPG_PLLDSI_MON_PLLn_RESETB_Msk)) !=
             (R_CPG_CPG_PLLDSI_MON_PLLn_LOCK_Msk | R_CPG_CPG_PLLDSI_MON_PLLn_RESETB_Msk));

    /*****************************************/
    /* Setting PLLDDR0                       */
    /*****************************************/

    /* Wait until the PLL is in standby mode and unlocked-state */
    do
    {
        reg32 = R_CPG->CPG_PLLDDR0_MON;
    } while ((reg32 & (R_CPG_CPG_PLLDDR0_MON_PLLn_LOCK_Msk | R_CPG_CPG_PLLDDR0_MON_PLLn_RESETB_Msk)) !=
             0);

    /* Setting PLL for normal mode */
    R_CPG->CPG_PLLDDR0_STBY = R_CPG_CPG_PLLDDR0_STBY_RESETB_WEN_Msk | R_CPG_CPG_PLLDDR0_STBY_RESETB_Msk;

    /* Wait until the PLL is in normal mode and locked-state */
    do
    {
        reg32 = R_CPG->CPG_PLLDDR0_MON;
    } while ((reg32 & (R_CPG_CPG_PLLDDR0_MON_PLLn_LOCK_Msk | R_CPG_CPG_PLLDDR0_MON_PLLn_RESETB_Msk)) !=
             (R_CPG_CPG_PLLDDR0_MON_PLLn_LOCK_Msk | R_CPG_CPG_PLLDDR0_MON_PLLn_RESETB_Msk));

    /*****************************************/
    /* Setting PLLDDR1                       */
    /*****************************************/

    /* Wait until the PLL is in standby mode and unlocked-state */
    do
    {
        reg32 = R_CPG->CPG_PLLDDR1_MON;
    } while ((reg32 & (R_CPG_CPG_PLLDDR1_MON_PLLn_LOCK_Msk | R_CPG_CPG_PLLDDR1_MON_PLLn_RESETB_Msk)) !=
             0);

    /* Setting PLL for normal mode */
    R_CPG->CPG_PLLDDR1_STBY = R_CPG_CPG_PLLDDR1_STBY_RESETB_WEN_Msk | R_CPG_CPG_PLLDDR1_STBY_RESETB_Msk;

    /* Wait until the PLL is in normal mode and locked-state */
    do
    {
        reg32 = R_CPG->CPG_PLLDDR1_MON;
    } while ((reg32 & (R_CPG_CPG_PLLDDR1_MON_PLLn_LOCK_Msk | R_CPG_CPG_PLLDDR1_MON_PLLn_RESETB_Msk)) !=
             (R_CPG_CPG_PLLDDR1_MON_PLLn_LOCK_Msk | R_CPG_CPG_PLLDDR1_MON_PLLn_RESETB_Msk));

    /*****************************************/
    /* Setting PLLGPU                        */
    /*****************************************/

    /* Wait until the PLL is in standby mode and unlocked-state */
    do
    {
        reg32 = R_CPG->CPG_PLLGPU_MON;
    } while ((reg32 & (R_CPG_CPG_PLLGPU_MON_PLLn_LOCK_Msk | R_CPG_CPG_PLLGPU_MON_PLLn_RESETB_Msk)) !=
             0);

    /* Output clock settings 1 */
    R_CPG->CPG_PLLGPU_CLK1 = CPG_PLLGPU_CLK1_INITVAL;

    /* Setting the output clock and SSC modulation value 2 */
    R_CPG->CPG_PLLGPU_CLK2 = CPG_PLLGPU_CLK2_INITVAL;

    /* Setting PLL for normal mode */
    R_CPG->CPG_PLLGPU_STBY = R_CPG_CPG_PLLGPU_STBY_RESETB_WEN_Msk | R_CPG_CPG_PLLGPU_STBY_RESETB_Msk;

    /* Wait until the PLL is in normal mode and locked-state */
    do
    {
        reg32 = R_CPG->CPG_PLLGPU_MON;
    } while ((reg32 & (R_CPG_CPG_PLLGPU_MON_PLLn_LOCK_Msk | R_CPG_CPG_PLLGPU_MON_PLLn_RESETB_Msk)) !=
             (R_CPG_CPG_PLLGPU_MON_PLLn_LOCK_Msk | R_CPG_CPG_PLLGPU_MON_PLLn_RESETB_Msk));
}
