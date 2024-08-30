/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

1. Project Overview:
	The example projects demonstrates Message Queue and Semaphore between tasks and interrupt. Message Queue is 
	demonstrated between Tasks and between Task and interrupt. GTM timer periodically generates interrupt at 1000msec. 
	For first few seconds, messages are shared between Sender and Receiver Tasks and GTM ISR0. Receiver task pends on 
	Message Queue, receives and displays message received on RTTViewer periodically at 500msec. For next few seconds, 
	Semaphore Task waits for sempahore until it is released by GTM ISR1. GTM ISR1 releases semaphore periodically 
	at 1000msec. All the tasks run with equal priority level.

2. Software Requirement:
    Software          : JLink RTT Viewer v7.96e

3. Hardware Settings:
    Bootmode:
    CA55 coldboot (SCIF boot)

    Board:
    RZ/V2H EVK-Ver1

    Hardware Connection:

    (1) Set for DIP switches and jumpers as follow.
        - DSW1 : 1-ON,2-OFF,3-ON,4-OFF,5-ON,6-OFF,7-ON,8-OFF.
        - Others : Default.
    (2) Connect the J-Link Base and the connector on the SMARC Module Board with JTAG cable.
        When connecting the conversion adapter "J-Link 9-pin Cortex-M Adapter" made by SEGGER will be required.
  
NOTE:
1. Segger RTT block address is required to connect J-Link RTTViewer. 
   The RTT block address can be confirmed by searching "_SEGGER_RTT" in .map file
   which is located in Debug folder and generated when building.