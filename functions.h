/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : uart.h
 *    Description : UARTs module header
 *
 *    History :
 *    1. Date        : July 6, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 4288 $
 **************************************************************************/
#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H
#include "includes.h"

/*************************************************************************
 * Function Name: Uart0Isr
 * Parameters: none
 *
 * Return: none
 *
 * Description: UART 0 interrupt routine
 *
 *************************************************************************/
double convVolt(pInt8U Buffer);
double convAmp(pInt8U Buffer);
double convPow(pInt8U Buffer);

#endif