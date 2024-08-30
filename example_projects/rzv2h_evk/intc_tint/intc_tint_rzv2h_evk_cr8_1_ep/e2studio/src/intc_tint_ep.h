/***********************************************************************************************************************
 * File Name    : intc_tint_ep.h
 * Description  : Contains Macros and function declarations.
 **********************************************************************************************************************/
/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef INTC_TINT_EP_H_
#define INTC_TINT_EP_H_

/* TINT channel */
#define USER_SW_TINT_NUMBER        (0x18)        /* Channel 24 */
#define EP_INFO     "This Example Project demonstrates the functionality of INTC_TINT \r\n" \
		            "driver. On pressing the user push button, an external TINT is \r\n"\
					"triggered, which toggles on-board LED.\r\n\r\n"


/* Function declaration */
fsp_err_t intc_tint_init(void);
fsp_err_t intc_tint_enable(void);
void intc_tint_deinit(void);

#endif /* INTC_TINT_EP_H_ */
