/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

1. Project Overview:
    The example project demonstrates typical use of the RIIC slave HAL module APIs.
    The project initializes RIIC slave and IIC master module with standard rate and is made interfaced with loop-back 
    mechanism. It performs Slave read and write operation continuously once initialization is successful. On successful
    I2C transaction (6 bytes), Data transceived is compared. Led blinks on data match else it is turned ON as sign of 
    failure. Output message for both corresponding slave operations is displayed on RTT Viewer. Any API/event failure 
    message is also displayed.

2. Hardware and Software Requirements:
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
    (3) Un-Connect the Secure Evaluation Board with Expansion Board.
        Connect pins in GPIO Connector J1 RZV2H_Expansion Board as follows.
        For Master and Slave : 
          - Connect GPIO Connector J1-41 to GPIO Connector J1-37
          - Connect GPIO Connector J1-39 to GPIO Connector J1-35.
        For leds on board :
          - Connect GPIO Connector J1-7 to GPIO Connector J1-8
          - Connect GPIO Connector J1-9 to GPIO Connector J1-10.

NOTE:
1. Segger RTT block address is required to connect J-Link RTTViewer. 
   The RTT block address can be confirmed by searching "_SEGGER_RTT" in .map file 
   which is located in Debug folder and generated when building.