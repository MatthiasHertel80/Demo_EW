/* Copyright (c) 2020 - 2021 ALIF SEMICONDUCTOR

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ALIF SEMICONDUCTOR nor the names of its contributors
     may be used to endorse or promote products derived from this software
     without specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/

/**************************************************************************//**
 * @file     Driver_PINMUX_AND_PINPAD.c
 * @author   Girish BN
 * @email    girish.bn@alifsemi.com
 * @version  V1.0.0
 * @date     21-Aug-2020
 * @brief    Driver for PINMUX and PINPAD.
 * @bug      None.
 * @Note     None.
 ******************************************************************************/

#include "PINMUX_AND_PINPAD_Internal.h"

/**
 \fn            int32_t PINMUX_Config (uint8_t port, uint8_t pin_no, uint8_t AF_number);
 \brief         Configuring the PINMUX.
 \param[in]     port: GPIO PORT.
 \param[in]     pin_no: GPIO Pin .
 \param[in]     AF_number: Alternate function number.
 \param[out]    int32_t: execution status.
 */

int32_t PINMUX_Config (uint8_t port, uint8_t pin_no, uint8_t AF_number) {

    if (port > PORT_NUMBER_3)                       { return DRIVER_ERROR; }
    if (pin_no > PIN_NUMBER_31)                     { return DRIVER_ERROR; }
    if (AF_number > PINMUX_ALTERNATE_FUNCTION_7)    { return DRIVER_ERROR; }

    uint32_t *ptr;
    uint32_t value = 0;

    port = (port *16) + 16;

    if(pin_no >= 24)
    {
        ptr = (uint32_t*) (PINMUX_BASE + port + 0x0C);
        value = pin_no-24;
    }
    else if(pin_no >= 16)
    {
        ptr = (uint32_t*) (PINMUX_BASE + port + 0x08);
        value = pin_no-16;
    }
    else if(pin_no >= 8)
    {
        ptr = (uint32_t*) (PINMUX_BASE + port + 0x04);
        value = pin_no-8;
    }
    else
    {
        ptr = (uint32_t*) (PINMUX_BASE + port);
        value = pin_no;
    }

    value *= 4;
    *ptr &= ~(0xF << value);
    *ptr |= (AF_number << value);
    return DRIVER_OK;
}

/**
 \fn            int32_t PINMUX_read (uint8_t port, uint8_t pin_no, uint32_t *AF_number);
 \brief         read the PINMUX value.
 \param[in]     port: GPIO PORT.
 \param[in]     pin_no: GPIO Pin .
 \param[in]     AF_number: pointer to Alternate function number.
 \param[out]    int32_t: execution status.
 */

int32_t PINMUX_read (uint8_t port, uint8_t pin_no, uint32_t *AF_number) {

    if (port > PORT_NUMBER_3)                       { return DRIVER_ERROR; }
    if (pin_no > PIN_NUMBER_31)                     { return DRIVER_ERROR; }
    if (AF_number == 0)                             { return DRIVER_ERROR; }

    uint32_t value = 0;
    uint32_t *ptr;

    port = (port *16) + 16;

    if(pin_no >= 24)
    {
        ptr = (uint32_t*) (PINMUX_BASE + port + 0x0C);
        value = pin_no-24;
    }
    else if(pin_no >= 16)
    {
        ptr = (uint32_t*) (PINMUX_BASE + port + 0x08);
        value = pin_no-16;
    }
    else if(pin_no >= 8)
    {
        ptr = (uint32_t*) (PINMUX_BASE + port + 0x04);
        value = pin_no-8;
    }
    else
    {
        ptr = (uint32_t*) (PINMUX_BASE + port);
        value = pin_no;
    }

    value *= 4;

    *AF_number = (((*ptr) >> value) & 0xF);

    return DRIVER_OK;
}

/**
 \fn            int32_t PINPAD_Config (uint8_t port, uint8_t pin_no, uint8_t function);
 \brief         Configuring the PINPAD.
 \param[in]     port: GPIO PORT.
 \param[in]     pin_no: GPIO Pin .
 \param[in]     function: function to be configure.
 \param[out]    int32_t: execution status.
 */
int32_t PINPAD_Config (uint8_t port, uint8_t pin_no, uint8_t function) {

    Pad_RegInfo *reg_ptr = (Pad_RegInfo*)(PADCTRL_BASE);

    if (port > PORT_NUMBER_4)                       { return DRIVER_ERROR; }
    if (pin_no > PIN_NUMBER_31)                     { return DRIVER_ERROR; }

    switch (port)
    {
        case PORT_NUMBER_1:
        {
            reg_ptr->port_1[pin_no].pad = function;
            break;
        }
        case PORT_NUMBER_2:
        {
            reg_ptr->port_2[pin_no].pad = function;
            break;
        }
        case PORT_NUMBER_3:
        {
            if (pin_no >= PORT3_MAX_PIN_NUMBER)
            {
                return DRIVER_ERROR;   // Port-3 have 0 to 23 pins, throwing error for other pin index
            }
            reg_ptr->port_3[pin_no].pad = function;
            break;
        }
        case PORT_NUMBER_4:
        {
            if (pin_no >= PORT4_MAX_PIN_NUMBER)
            {
                return DRIVER_ERROR;    // Port-4 have 0 to 7 pins, throwing error for other pin index
            }
            reg_ptr->port_4[pin_no].pad = function;
            break;
        }
    }
    return DRIVER_OK;
}

/**
 \fn            int32_t PINPAD_read (uint8_t port, uint8_t pin_no, uint32_t *function);
 \brief         reading the PINPAD value.
 \param[in]     port: GPIO PORT.
 \param[in]     pin_no: GPIO Pin.
 \param[in]     function: pointer to function configured.
 \param[out]    int32_t: execution status.
 */
int32_t PINPAD_read (uint8_t port, uint8_t pin_no, uint32_t *function) {

    Pad_RegInfo *reg_ptr = (Pad_RegInfo*)(PADCTRL_BASE);

    if (port > PORT_NUMBER_4)                       { return DRIVER_ERROR; }
    if (pin_no > PIN_NUMBER_31)                     { return DRIVER_ERROR; }
    if (function == 0)                              { return DRIVER_ERROR; }

    switch (port)
    {
        case PORT_NUMBER_1:
        {
            *function = reg_ptr->port_1[pin_no].pad;
            break;
        }
        case PORT_NUMBER_2:
        {
            *function = reg_ptr->port_2[pin_no].pad;
            break;
        }
        case PORT_NUMBER_3:
        {
            if (pin_no >= PORT3_MAX_PIN_NUMBER)
            {
                return DRIVER_ERROR;   // Port-3 have 0 to 23 pins, throwing error for other pin index
            }
            *function = reg_ptr->port_3[pin_no].pad;
            break;
        }
        case PORT_NUMBER_4:
        {
            if (pin_no >= PORT4_MAX_PIN_NUMBER)
            {
                return DRIVER_ERROR;    // Port-4 have 0 to 7 pins, throwing error for other pin index
            }
            *function = reg_ptr->port_4[pin_no].pad;
            break;
        }
    }
    return DRIVER_OK;
}
