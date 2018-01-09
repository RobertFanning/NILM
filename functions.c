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
    Vol[i] = Buffer[i + 7];
  }
  return (strtol(Vol,NULL,16) * 0.000023-2);
}

double convAmp(pInt8U Buffer)
{
  Int8U Amp[7];
  for (int i = 0; i < 6; i++){
    Amp[i] = Buffer[i + 19];
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
    Pow[i] = Buffer[i + 31];
  }

  if (strtol(Pow, NULL, 16) * 0.0014 - 1.4 < 0)
    return 0;
  else
    return strtol(Pow, NULL, 16) * 0.0014 - 1.4;
}

double convPF(pInt8U Buffer)
{
  Int8U PF[7];
  for(int i = 0; i < 6; i++){
    PF[i] = Buffer[i + 43];
  }
  return (strtol(PF,NULL,16) * 0.000000048-0.12);
}

double convPowR(pInt8U Buffer)
{
  Int8U PowR[7];
  for(int i = 0; i < 6; i++){
    PowR[i] = Buffer[i + 55];
  }
  
  if (strtol(PowR,NULL,16) * 0.0000018-0.00016 < 0)
    return 0;
  else
    return (strtol(PowR,NULL,16) * 0.0000018-0.00016);
}

void shiftPrevious(RS232 previous[3])
{
  int i=1;
  for (i=1; i>=0 ; i--){
   previous[i+1].V = previous[i].V;
   previous[i+1].A = previous[i].A;
   previous[i+1].P = previous[i].P;
   previous[i+1].Q = previous[i].Q;
   previous[i+1].PF = previous[i].PF;
  }
    return;
}

int EdgeDetect(RS232 previous[3])
{
  
  if ((previous[0].P-previous[1].P)>2)//Positive Edge Detected
    return (1);
  else if ((previous[0].P-previous[1].P)<-2)//Negative Edge Detected
    return (-1);
  else 
    return (0);
}

//function to determine device causing edge
int FindMatch(RS232 previous[3],Device devices[20], int DevicesRegistered)
{
  int i;
  for(i=0; i<DevicesRegistered; i++){
    
    //check if the edge is within 2 watts of registered devices power
    if (devices[i].P-abs(previous[0].P-previous[1].P)<2)
      return i;
    
  }
  
  return -1;

}
