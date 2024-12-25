/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

1. Project Overview:
     The example project demonstrates the typical use of the SPI_B HAL module APIs.The project configure 
     SPI_B channels (Channel 0 and Channel 2) in Master and Slave mode. Once the module is initialised and the 
     channels are configured, Master and Slave can transmit and receive data based on commands from user sent through 
     JLinkRTTViewer.
	 
     This project supports the DMAC feature. In cases where this feature is not used in the application,
     please invalid the DMAC feature by the following steps.
     - Set DMAC Support to Disabled on the main stack of the driver in FSP Configuration. 
     - Remove the DMAC sub stacks that linked to the main stack.
	 
2. Software Requirement:
    Software          : JLink RTT Viewer v7.96e

3. Hardware settings:
    Bootmode:
    CA55 coldboot (SCIF boot)

    Board:
    RZ/V2N EVK

    Hardware Connection:

    (1) Set for DIP switches and jumpers as follow.
        - DSW1 : 1-ON,2-OFF,3-ON,4-OFF,5-ON,6-OFF,7-ON,8-OFF.
        - Others : Default
    (2) Connect the J-Link Base and the connector on the SMARC Module Board with JTAG cable.
        When connecting the conversion adapter "J-Link 9-pin Cortex-M Adapter" made by SEGGER will be required.
    (3) Un-connect the Secure Evaluation Board with Expansion Board.
        On the J2 header GPIO connector (Secure Evaluation board): 
                    Master (SPI 0)       Slave (SPI 2)
        MISOA  ----> P91(Pin 8)    -     PB3 (Pin 44)
        MOSIA  ----> P90(Pin 6)    -     PB4 (Pin 46)
        RSPCKA ----> P92(Pin 10)   -     PB5 (Pin 48)
        SSLA0  ----> P93(Pin 12)   -     PA7 (Pin 36)

NOTE:
1. User is expected to enter data of size not exceeding 64 bytes.
   As SPI bit width is set to 32 bits, each 4 bytes of user data input will be transferred in single SPI_B transmission.
   The bytes sent for master should be bigger than the bytes sent for slave when using WriteRead operation.
   Operation is not guaranteed for any user input value other than integer, char (i.e. float, special char) through RTT.

2. Segger RTT block address is required to connect J-Link RTTViewer. 
   The RTT block address can be confirmed by searching "_SEGGER_RTT" in .map file 
   which is located in Debug folder and generated when building.