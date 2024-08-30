/***********************************************************************************************************************
 * File Name    : ext_irq_setup.h
 * Description  : Contains data structures and functions used in ext_irq_setup.c
 **********************************************************************************************************************/
/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef EXT_IRQ_SETUP_H_
#define EXT_IRQ_SETUP_H_


/*
 * function declarations
 */
fsp_err_t init_irq_module(void);
fsp_err_t enable_irq_module(void);
void deinit_irq_module(void);
extern volatile bool system_reset_flag;
#endif /* EXT_IRQ_SETUP_H_ */
