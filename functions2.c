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
    PF[i] = Buffer[i + 55];
  }
  return (strtol(PF,NULL,16) * 0.000000048-0.12);
}

double convPowR(pInt8U Buffer)
{
  Int8U PowR[7];
  for(int i = 0; i < 6; i++){
    PowR[i] = Buffer[i + 43];
  }
  
  if ((strtol(PowR,NULL,16) * 0.0012)-0.97 < 0)
    return 0;
  else
    return ((strtol(PowR,NULL,16)* 0.0012)-0.97);
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

int EdgeDetect(RS232 previous[3], double TotalP)
{
  
  if ((previous[0].P-TotalP)>6)//Positive Edge Detected
    return (1);
  else if ((previous[0].P-TotalP)<-6)//Negative Edge Detected
    return (-1);
  else 
    return (0);
}

//function to determine device causing edge
void FindMatch(RS232 previous[3],Device devices[5], int DevicesRegistered, int match[2])
{
  int i,j;
  double TotalP=0;//, TotalQ=0;
  for(i=0; i<DevicesRegistered; i++){
    if (devices[i].status==TRUE)
      TotalP+=devices[i].P;
      //TotalQ+=devices[i].Q;
  } 
  //Check for 2 devices
  for (j=0; j<DevicesRegistered; j++){
    for(i=0; i<DevicesRegistered; i++){
      //check if the edge is within 2 watts of registered devices power
      if(i==j)
        i++;
    
      if(i==DevicesRegistered)
        break;
    
      if (abs(abs(TotalP-previous[0].P)-(devices[i].P+devices[j].P))<2){
        match[0]=i;
        match[1]=j;
        return;
      }
    }
  }
  //Check for 1 device
  for(i=0; i<DevicesRegistered; i++){
    //check if the edge is within 2 watts of registered devices power
    if (abs(abs(TotalP-previous[0].P)-devices[i].P)<2){
      match[0]=i;
      match[1]=-1;
      return;
    }
  }
  match[0]=-1;
  match[1]=-1;
  return;
}


Boolean StabilityCheck(RS232 previous[3]){
  
  if((abs(previous[0].P-previous[1].P)<0.1&&abs(previous[1].P-previous[2].P)<0.1&&abs(previous[0].P-previous[2].P)<0.1))
    return TRUE;
  else
    return FALSE;

}

void shift_power(int power[28]){
  int i=1;
  for (i=26; i>=0 ; i--){
   power[i+1] = power[i];
  }
    return;
}

