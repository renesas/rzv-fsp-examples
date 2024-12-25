/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

1. Project Overview:
    The example project demonstrates the typical use of the ELC HAL module APIs.
    ELC Software Event, GPT0 and GPT1 events are linked using ELC. The start source for GPT0 and GPT1 is ELC
    Software Event and the stop source for GPT0 is GPT1 counter overflow. GPT0 runs in PWM mode and GPT1 runs
    in one-shot mode. On giving valid RTT input, an ELC Software Event is generated that triggers LED blinking.
    LED stops blinking after 5 sec when GPT0 expires.

2. Hardware and Software Requirement:
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
    (3) Un-Connect the Secure Evaluation Board with Expansion Board.
        Connect pins in GPIO Connector J1 RZV2H_Expansion Board as follows : 
        - Connect GPIO Connector J1-24 to GPIO Connector J1-8.

Note:
1. Segger RTT block address is required to connect J-Link RTTViewer. 
   The RTT block address can be confirmed by searching "_SEGGER_RTT" in .map file 
   which is located in Debug folder and generated when building.