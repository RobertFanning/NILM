/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : uart.c
 *    Description : UARTs module
 *
 *    History :
 *    1. Date        : August 5, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 **************************************************************************/
 
#include "functions.h"

/*************************************************************************
 * Function Name: FifoPush
 * Parameters: pUartFifo_t Fifo, Int8U Data
 *
 * Return: Boolean
 *
 * Description: Push a char in a FIFO. Return TRUE when push is successful
 *  or FALSE when the FIFO is full.
 *
 *************************************************************************/
double convVolt(pInt8U Buffer)
 //  double convVolt(pInt8U Vol)
{
  Int8U Vol[7];
  for(int i = 0; i < 6; i++){
    Vol[i] = Buffer[i + 7];
  }
  Vol[6] = 0x00;
  return (strtol(Vol,NULL,16) * 0.000023-2);
}

double convAmp(pInt8U Buffer)
//double convAmp(pInt8U Amp)
{
  Int8U Amp[7];
  for (int i = 0; i < 6; i++){
    Amp[i] = Buffer[i + 19];
  }
  Amp[6] = 0x00;
  if (strtol(Amp,NULL,16) <= 3512)
     return 0;
  else
     return strtol(Amp,NULL,16) * 0.0000016 - 0.00044;
}

double convPow(pInt8U Buffer)
//double convPow(pInt8U Pow)
{
  Int8U Pow[7];
  for (int i = 0; i < 6; i++){
    Pow[i] = Buffer[i + 31];
  }
  Pow[6] = 0x00;
  if (strtol(Pow, NULL, 16) * 0.0014 - 1.4 < 0)
    return 0;
  else
    return strtol(Pow, NULL, 16) * 0.0014 - 1.4;
}

