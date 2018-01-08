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
{
  Int8U Vol[7];
  for(int i = 0; i < 6; i++){
    Vol[i] = Buffer[i + 8];
  }
  return (strtol(Vol,NULL,16) * 0.000023-2);
}

double convAmp(pInt8U Buffer)
{
  Int8U Amp[7];
  for (int i = 0; i < 6; i++){
    Amp[i] = Buffer[i + 20];
  }
  if (strtol(Amp,NULL,16) <= 3512)
     return 0;
  else
     return strtol(Amp,NULL,16) * 0.0000016 - 0.00044;
}

double convPow(pInt8U Buffer)
{
  Int8U Pow[7];
  for (int i = 0; i < 6; i++){
    Pow[i] = Buffer[i + 32];
  }

  if (strtol(Pow, NULL, 16) * 0.0014 - 1.4 < 0)
    return 0;
  else
    return strtol(Pow, NULL, 16) * 0.0014 - 1.4;
}

