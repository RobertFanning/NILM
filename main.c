/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : August 5, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR LPC2478-SK board.
 *  It implements USB CDC (Communication Device Class) device and install
 * it like a Virtual COM port. The UART0 is used for physical implementation
 * of the RS232 port.
 *
 * Jumpers:
 *  EXT/JLINK  - depending of power source
 *  ISP_E      - unfilled
 *  RST_E      - unfilled
 *  BDS_E      - unfilled
 *  C/SC       - SC
 *
 * Note:
 *  After power-up the controller gets it's clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect, therefore adaptive clocking
 * should always be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *
 * The LCD shares pins with Trace port. If ETM is enabled the LCD will not work.
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"
//#include "logo.h"

#define NONPROT 0xFFFFFFFF
#define CRP1  	0x12345678
#define CRP2  	0x87654321
/*If CRP3 is selected, no future factory testing can be performed on the device*/
#define CRP3  	0x43218765

#ifndef DATA_LOGGING
#define DATA_LOGGING 1
#endif

#ifndef SDRAM_DEBUG
#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned crp = NONPROT;
#endif

#define LCD_VRAM_BASE_ADDR ((Int32U)&SDRAM_BASE_ADDR)

extern Int32U SDRAM_BASE_ADDR;
extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/
int main(void)
{
  Int8U Buffer[100] = {0x00};
//pInt8U pBuffer;
Int32U Size,TranSize;

Boolean CdcConfigureStateHold;

#if CDC_DEVICE_SUPPORT_LINE_CODING > 0
CDC_LineCoding_t CDC_LineCoding;
UartLineCoding_t UartLineCoding;
#endif // CDC_DEVICE_SUPPORT_LINE_CODING > 0

#if CDC_DEVICE_SUPPORT_LINE_STATE > 0
UartLineEvents_t      UartLineEvents;

SerialState_t   SerialState;
#endif // CDC_DEVICE_SUPPORT_LINE_STATE > 0

  GLCD_Ctrl (FALSE);
  // Init GPIO
  GpioInit();
#ifndef SDRAM_DEBUG
  // MAM init
  MAMCR_bit.MODECTRL = 0;
  MAMTIM_bit.CYCLES  = 3;   // FCLK > 40 MHz
  MAMCR_bit.MODECTRL = 2;   // MAM functions fully enabled
  // Init clock
  InitClock();
  // SDRAM Init
  SDRAM_Init();
#endif // SDRAM_DEBUG
  // Init VIC
  VIC_Init();
  // GLCD init
  GLCD_Init (0, NULL);
  // Disable Hardware cursor
  GLCD_Cursor_Dis(0);
  // Init UART 0
  UartInit(UART_0,4,NORM);
  // Init USB
  UsbCdcInit();

  __enable_interrupt();

  // Soft connection enable
  USB_ConnectRes(TRUE);
  // Enable GLCD
  GLCD_Ctrl (TRUE);

  GLCD_SetFont(&Terminal_18_24_12,0x00FFFF,0x00000000);
  GLCD_SetWindow(80,10,270,33);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fSerial Port Test");



  //CdcConfigureStateHold = !IsUsbCdcConfigure();
printf("hello");

  while(1)
  {
    
    printf("1");
    //if (IsUsbCdcConfigure())
    //{
     // printf("2");
      // Data from USB
      //Size = UsbCdcRead(Buffer,sizeof(Buffer)-1);
      /*if(Size)
      {
#ifdef DATA_LOGGING
        Buffer[Size] = 0;
        printf("> %s\n",Buffer);
#endif // DATA_LOGGING
        TranSize = 0;
        pBuffer = Buffer;
        do
        {
          Size -= TranSize;
          pBuffer += TranSize;
          TranSize = UartWrite(UART_0,pBuffer,Size);
        }
        while(Size != TranSize);
      }*/

      // Data from UART1
        for(int i = 0; i < 200000; i++);
      Size = UartRead(UART_0,Buffer,sizeof(Buffer)-1);
      if(Size)
      {
        printf("2");
#ifdef DATA_LOGGING
        Buffer[Size] = 0;
       // printf("< %s\n",Buffer);
        GLCD_SetFont(&Terminal_6_8_6,0xFFFFFF,0x00000000);
        GLCD_SetWindow(10,110,300,133);
        GLCD_TextSetPos(0,0);
        //if(Buffer[0] == 0x41 && Buffer[1] == 0x54)
          //GLCD_print("True");
        GLCD_print(Buffer);
       // printf("hell2");
#endif  // DATA_LOGGING
       // while(!UsbCdcWrite(Buffer,Size));
      }

      // Get line and modem events from UART
#if CDC_DEVICE_SUPPORT_LINE_STATE > 0
      // Get line events - BI, FE, PE, OE
      UartLineEvents = UartGetUartLineEvents(UART_0);
      if(UartLineEvents.Data)
      {
        SerialState.Data = 0;
        // Line events report BI, PE, FE and OE
        SerialState.bBreak = UartLineEvents.bBI;
        SerialState.bFraming = UartLineEvents.bFE;
        SerialState.bOverRun = UartLineEvents.bOE;
        SerialState.bParity = UartLineEvents.bPE;
        // Send events
        UsbCdcReportSerialCommState(SerialState);
      }
#endif // CDC_DEVICE_SUPPORT_LINE_STATE > 0
    //}
    //else
      //printf("5");
    //{
      if(CdcConfigureStateHold == TRUE)
      {
        CdcConfigureStateHold = FALSE;
      }
    

    // UART line coding - Baud rate, number of the stop bits,
    // number of bits of the data word and parity type
/*#if CDC_DEVICE_SUPPORT_LINE_CODING > 0
    if(UsbCdcIsNewLineCodingSettings())
    {
      CDC_LineCoding = UsbCdcGetLineCodingSettings();
      // Update the baud rate
      UartLineCoding.dwDTERate = CDC_LineCoding.dwDTERate;
      // Update the stop bits number
      UartLineCoding.bStopBitsFormat = CDC_LineCoding.bCharFormat?UART_TWO_STOP_BIT:UART_ONE_STOP_BIT;
      // Update the parity type
      UartLineCoding.bParityType = (CDC_LineCoding.bParityType == 0)?UART_NO_PARITY:(UartParity_t)(CDC_LineCoding.bParityType-1);
      // Update the word width
      UartLineCoding.bDataBits = (UartWordWidth_t)(CDC_LineCoding.bDataBits - 5);
      // Set UART line coding
      UartSetLineCoding(UART_0,UartLineCoding);
      printf("6");
    } 
#endif // CDC_DEVICE_SUPPORT_LINE_CODING > 0
*/

    // CDC_LineCoding = UsbCdcGetLineCodingSettings();
      // Update the baud rate
      UartLineCoding.dwDTERate = 115200;
      // Update the stop bits number
      UartLineCoding.bStopBitsFormat = UART_ONE_STOP_BIT;
      // Update the parity type
      UartLineCoding.bParityType = UART_NO_PARITY;
      // Update the word width
      UartLineCoding.bDataBits = UART_WORD_WIDTH_8;
      // Set UART line coding
      UartSetLineCoding(UART_0,UartLineCoding);
#if CDC_DEVICE_SUPPORT_BREAK > 0
    // Break event
    UartSetUartLineState(UART_0,UsbCdcGetBreakState());
#endif // CDC_DEVICE_SUPPORT_BREAK > 0
  }
}