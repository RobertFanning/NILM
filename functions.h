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
struct measurements {double V; double A; double P; double Q; double PF;};
typedef struct measurements RS232;
struct device {char name[10]; Boolean status; double P; double Q; double PF;};
typedef struct device Device;

double convVolt(pInt8U Buffer);
double convAmp(pInt8U Buffer);
double convPow(pInt8U Buffer);
double convPF(pInt8U Buffer);
double convPowR(pInt8U Buffer);
void shiftPrevious(RS232 previous[]);
int EdgeDetect(RS232 previous[]);
int FindMatch(RS232 previous[],Device devices[], int DevicesRegistered);


#endif
