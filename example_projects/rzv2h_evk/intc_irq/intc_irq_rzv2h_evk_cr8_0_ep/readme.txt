/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

1. Project Overview:
     The example project demonstrates the typical use of the INTC IRQ module APIs.
     The project initializes the IRQ interrupt in Interrupt Controller 
     User is requested to press the push button to trigger the external IRQ and this then will start toggling of user LED.

2. Hardware and Software Requirement:
    External Hardware : Pmod BUTTON
    Software          : JLink RTT Viewer v7.96e

3. Hardware settings:
    Boot Mode:
    CA55 coldboot (SCIF boot)

    Board:
    RZ/V2H EVK-Ver1

    Hardware Connection:
    (1) Set for DIP switches and jumpers as follow.
        - DSW1 : 1-ON,2-OFF,3-ON,4-OFF,5-ON,6-OFF,7-ON,8-OFF.
        - Others : Default.
    (2) Connect the J-Link Base and the connector on the SMARC Module Board with JTAG cable.
        When connecting the conversion adapter "J-Link 9-pin Cortex-M Adapter" made by SEGGER will be required.
    (3) Connect PMOD Type1/1A PIN1-6 on Expansion Board with Pmod BUTTON.

Note:
1. Segger RTT block address is required to connect J-Link RTTViewer. 
   The RTT block address can be confirmed by searching "_SEGGER_RTT" in .map file 
   which is located in Debug folder and generated when building.