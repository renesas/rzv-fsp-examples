/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

1. Project Overview:
    The example project demonstrates the typical use of the IIC master HAL module APIs.
    The project initializes IIC master module with fast mode and interfaces with PmodACL™ Board for ADXL345.

    On power up after establishing the connection of sensor with the board, it displays accelerometer axis data on 
    RTTviewer. Any API/event failure will be displayed on RTTviewer.

2. Hardware and Software Requirements:
    External Hardware : Pmod ACL
    Software          : JLink RTT Viewer v7.96e

3. Hardware Settings:
    Bootmode:
    CA55 coldboot (SCIF boot)

    Board:
    RZ/V2H EVK-Ver1

    Hardware Connection:

    (1) Set for DIP switches and jumpers as follow.
		- DSW1 : 1-ON,2-OFF,3-ON,4-OFF,5-ON,6-OFF,7-ON,8-OFF.
		- Others : Default

    (2) Connect the J-Link Base and the connector on the SMARC Module Board with JTAG cable.
        When connecting the conversion adapter "J-Link 9-pin Cortex-M Adapter" made by SEGGER will be required.
    (3) Connect Pmod ACL with  RZV2H_Expansion Board as follows.
        Pmod ACL has two on board connectors, J2 is used for I2C communication.

        J2 (Pmod ACL)          PMOD Type6/6A (RZV2H_Expansion_Board)
                                      -------
                                      |1  |
        ----                          |2  |
        |1  ---SCL  --->  RIICx_SCL---|3  |
        |2  ---SDA  --->  RIICx_SDA---|4  |
        |3  ---GND  --->  GND pin  ---|5  |
        |4  ---VCC  --->  VCC pin  ---|6  |
        ----                          -----

NOTE:
1. With the SDO/ALT ADDRESS pin high, the 7-bit I2C address for the device is 0x1D, followed by the R/W bit.
   An alternate I2C address of 0x53 (followed by the R/W bit) can be chosen by grounding the SDO/ALT ADDRESS pin.

2. Segger RTT block address is required to connect J-Link RTTViewer. 
   The RTT block address can be confirmed by searching "_SEGGER_RTT" in .map file 
   which is located in Debug folder and generated when building.