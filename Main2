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
#include <math.h>
//#include "logo.h"

#define NONPROT 0xFFFFFFFF
#define CRP1  	0x12345678
#define CRP2  	0x87654321
/*If CRP3 is selected, no future factory testing can be performed on the device*/
#define CRP3  	0x43218765

#ifndef SDRAM_DEBUG
#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned crp = NONPROT;
#endif

#define LCD_VRAM_BASE_ADDR ((Int32U)&SDRAM_BASE_ADDR)
//copied
#define TIMER3_TICK_PER_SEC   0.5
#define TIMES_TO_GET_VALUE_PER_SEC 5

extern Int32U SDRAM_BASE_ADDR;
extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;
extern int power[28];
extern int reactive[28];
int new_device;



void LCD_GRAPH(int power[]);
void shift_power(int power[]);
int graph_switch(int xOffset, int yOffset);
void GLCD_SetW(int x1, int y1, int x2, int y2);
void readTouchScreen();
void touchReaction();
int newDevice();

//TouchScreen
ToushRes_t XY_Touch;
Boolean Touch = FALSE;
Int32U cursor_x = (C_GLCD_H_SIZE - CURSOR_H_SIZE)/2, cursor_y = (C_GLCD_V_SIZE - CURSOR_V_SIZE)/2;

int touchOK;
int touchP;
int touchQ;
bool flag1 = false;
/*
int bulbAuto;
int socketAuto;
int bulbOn;

int socketOn;
int bulbForceOn;
int socketForceOn;
int bulbForceOff;
int socketForceOff;
int relay1State,relay2State;
*/
//DisplayCount COPIED
int DisplayCount=0;

/*************************************************************************
 * Function Name: Timer0IntrHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
void Timer3IntrHandler (void)
{
flag1 = true;
  
  // clear interrupt
  T3IR_bit.MR0INT = 1;
  VICADDRESS = 0;
}

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
  //Int32U cursor_x = (C_GLCD_H_SIZE - CURSOR_H_SIZE)/2, cursor_y = (C_GLCD_V_SIZE - CURSOR_V_SIZE)/2;
  //ToushRes_t XY_Touch;
  //Boolean Touch = FALSE;
  //Boolean LKupdateDisplay = FALSE;
  //int LKbutton = 0;
  //int LKselectedDevice =0;
  char printstring[15];
  Int8U Buffer[70] = {0x00};
  struct timer periodic_timer;//copied
  
  
  int power[28]={0};
  int reactive[28]={0};
  
  
  Int32U Size;
  Boolean CdcConfigureStateHold;
  Device devices[3]={{0}};
  
  RS232 previous[3]={{0}};
  
  
  int DevicesRegistered=0,DeviceID[2]={0},i, trigger=1;// number of registered devices
  double TotalP = 0,TotalQ = 0;
  
  new_device=0;
  touchOK=0;

  
    GLCD_Ctrl (FALSE);
 
#if CDC_DEVICE_SUPPORT_LINE_CODING > 0
//CDC_LineCoding_t CDC_LineCoding;
UartLineCoding_t UartLineCoding;
#endif // CDC_DEVICE_SUPPORT_LINE_CODING > 0
#if CDC_DEVICE_SUPPORT_LINE_STATE > 0
UartLineEvents_t      UartLineEvents;
SerialState_t   SerialState;
#endif // CDC_DEVICE_SUPPORT_LINE_STATE > 0



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
  GLCD_Init (BackgroundPic.pPicStream, NULL);
  GLCD_Cursor_Dis(0);
  GLCD_Copy_Cursor ((Int32U *)Cursor, 0, sizeof(Cursor)/sizeof(Int32U));
  GLCD_Cursor_Cfg(CRSR_FRAME_SYNC | CRSR_PIX_32);
  GLCD_Move_Cursor(cursor_x, cursor_y);
  GLCD_Cursor_En(0);
  
    /*Modified for UI.TOUCH*/
  //For UIp
  // Sys timer init 1/100 sec tick
  clock_init(2);
  timer_set(&periodic_timer, CLOCK_SECOND / 2);
 // timer_set(&arp_timer, CLOCK_SECOND * 10);
  
  
    // Enable TIM0 clocks
  PCONP_bit.PCTIM3 = 1; // enable clock
  
    // Init Time0
  T3TCR_bit.CE = 0;     // counting  disable
  T3TCR_bit.CR = 1;     // set reset
  T3TCR_bit.CR = 0;     // release reset
  T3CTCR_bit.CTM = 0;   // Timer Mode: every rising PCLK edge
  T3MCR_bit.MR0I = 1;   // Enable Interrupt on MR0
  T3MCR_bit.MR0R = 1;   // Enable reset on MR0
  T3MCR_bit.MR0S = 0;   // Disable stop on MR0
  
  // set timer 0 period
  T3PR = 0;
  T3MR0 = SYS_GetFpclk(TIMER3_PCLK_OFFSET)/(TIMER3_TICK_PER_SEC);
  
  //init timer 0 interrupt
  T3IR_bit.MR0INT = 1;  // clear pending interrupt
  VIC_SetVectoredIRQ(Timer3IntrHandler,3,VIC_TIMER3);
  VICINTENABLE |= 1UL << VIC_TIMER3;
  T3TCR_bit.CE = 1;     // counting Enable
  //__enable_interrupt();
  GLCD_Ctrl (TRUE);
  

    //Init parameters
 /* bulbAuto =1;
  socketAuto =1;
  bulbOn =1;
  socketOn =1;
  bulbForceOn =0;
  socketForceOn =0;  
  bulbForceOff =0;
  socketForceOff =0;
  */
  // Init touch screen
  TouchScrInit();
  
  
    // Touched indication LED
  //USB_H_LINK_LED_SEL = 0; // GPIO
  //USB_H_LINK_LED_FSET = USB_H_LINK_LED_MASK;
  //USB_H_LINK_LED_FDIR |= USB_H_LINK_LED_MASK;

  
  // Init UART 0
  UartInit(UART_0,4,NORM);
  // Init USB
  UsbCdcInit();

  __enable_interrupt();

  // Soft connection enable
  USB_ConnectRes(TRUE);
  // Enable GLCD
  GLCD_Ctrl (TRUE);


  //CdcConfigureStateHold = !IsUsbCdcConfigure();
        previous[0].V = 0;
        previous[0].A = 0;
        previous[0].P = 0;
        previous[0].Q = 0;
        previous[0].PF = 0;
        
// Draw Opening Menu
  //drawMenuStart();        
    
  while(1)
  {
 
    if(flag1){
      
      Size = UartRead(UART_0,Buffer,sizeof(Buffer)-1);
      if(Size)
      {

        shiftPrevious(previous);
        previous[0].V = convVolt(Buffer);
        previous[0].A = convAmp(Buffer);
        previous[0].P = convPow(Buffer);
        previous[0].Q = convPowR(Buffer);
        previous[0].PF = convPF(Buffer);
        Buffer[Size] = 0;
        
        shift_power(power);
        shift_power(reactive);
        power[0]=round(log(previous[0].P)*10);
        reactive[0]=round(log(previous[0].Q)*10);
        //for(i=0;i<28;i++)
          //power[i]=i*2.1;

        for(i=0; i<DevicesRegistered; i++){
          if (devices[i].status==TRUE){
                TotalP += devices[i].P;
                TotalQ += devices[i].Q;
          }
        }
        
        if (EdgeDetect(previous,TotalP)==1&&StabilityCheck(previous)){
          FindMatch(previous, devices, DevicesRegistered, DeviceID);
          
          if(DeviceID[0] != -1){
            devices[DeviceID[0]].status=TRUE;
            TotalP += devices[DeviceID[0]].P;
            TotalQ += devices[DeviceID[0]].Q;
            if(DeviceID[1] != -1){
              devices[DeviceID[1]].status=TRUE;
              TotalP += devices[DeviceID[1]].P;
              TotalQ += devices[DeviceID[1]].Q;
            }
          }
          else{//no match found, new device activated
            devices[DevicesRegistered].P = previous[0].P-TotalP;
            devices[DevicesRegistered].Q = previous[0].Q-TotalQ;
            devices[DevicesRegistered].status = TRUE;
            TotalP += devices[DevicesRegistered].P;
            TotalQ += devices[DevicesRegistered].Q;
            DevicesRegistered+=1;  
            new_device=1;
          }
        }
        
        //Negative Edge Detected
        if (EdgeDetect(previous, TotalP)==-1 && StabilityCheck(previous) ){
          //Let the negative edge stabilise
          // If the power is ~ 0
          if (previous[0].P<0.01){
             for(i=0; i<DevicesRegistered; i++){
               if (devices[i].status==TRUE){
                  devices[i].status=FALSE;
                  TotalP = 0;
                  TotalQ = 0;
               }
             }
          }
          else{
            FindMatch(previous, devices, DevicesRegistered, DeviceID);

            if(DeviceID[0] != -1){
              devices[DeviceID[0]].status=FALSE;
              TotalP -= devices[DeviceID[0]].P;
              TotalQ -= devices[DeviceID[0]].Q;
              if(DeviceID[1] != -1){
                devices[DeviceID[1]].status=FALSE;
                TotalP -= devices[DeviceID[1]].P;
                TotalQ -= devices[DeviceID[1]].Q;
              }
            }
          }
        }
        
       GLCD_SetFont(&Terminal_6_8_6,0x000000,0xffffff);
       GLCD_SetW(4, 56, 102, 72);
       GLCD_TextSetPos(0,0);
       //GLCD_print("devices ");
       sprintf(printstring, "Devices: %d",DevicesRegistered);
       GLCD_print(printstring);
       GLCD_SetFont(&Terminal_6_8_6,0x00ff00,0xffffff);
  GLCD_SetWindow(148,33, 318, 166);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f                                ");
      // GLCD_print("Total power: %d\n", (int)TotalPower);
       
       if(DevicesRegistered > 0){
         int offset=73;
         for(i = 0; i < DevicesRegistered; i++){
           //int d,f;
           float temp = devices[i].P;
           GLCD_SetFont(&Terminal_6_8_6,0x000000,0xffffff);
           GLCD_SetW(4, offset, 102, offset+16);
           GLCD_TextSetPos(0,0);
       //GLCD_print("devices ");
           sprintf(printstring, "%d  power: %d %s", i+1, (int)temp, devices[i].status ? "ON " : "OFF");
           GLCD_print(printstring);
           offset+=17;
         }
       }

      }
      
      
      
      flag1 = false;
    }
        
       
       touchReaction(); 

       if (new_device==1){
         if (newDevice())
            new_device=0;
       }
       else{
        trigger=graph_switch(217, 171);
          if (trigger==1)
            LCD_GRAPH(power);
          else
            LCD_GRAPH(reactive);
       }
       //DisplayCount=0;
       

      //}

      

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

      if(CdcConfigureStateHold == TRUE)
      {
        CdcConfigureStateHold = FALSE;
      }

    // UART line coding - Baud rate, number of the stop bits,
    // number of bits of the data word and parity type
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

void LCD_GRAPH(int power[28]){     

/* GLCD_SetFont(&Terminal_6_8_6,0x00ff00,0xffffff);
  GLCD_SetWindow(148,33, 318, 166);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f                                "); */
  
  for (int i = 0; i < 28; i++){
       
     if(power[i]>0){
        GLCD_SetFont(&Terminal_6_8_6,0x00ff00,0x00ff00);
        GLCD_SetWindow(310 - (6*i),(167-power[i]), 314 - (i*6), 167);
        GLCD_TextSetPos(0,0);
       GLCD_print("\f                                ");
     }
  }

  
  return;
}



int graph_switch(int xOffset, int yOffset) 
{
  GLCD_SetFont(&Terminal_18_24_12,0x000000,0xF7F7F7);
  GLCD_SetW(xOffset,yOffset,xOffset+20,yOffset+20);
  GLCD_print("P");
  GLCD_SetW(xOffset+48,yOffset,xOffset+56,yOffset+20);
  GLCD_print("Q");
 
  GLCD_SetFont(&Terminal_18_24_12,0x000FFF,0xF7F7F7);
  
  if (touchQ==1){
    GLCD_SetW(xOffset+48,yOffset,xOffset+56,yOffset+20);
    GLCD_print("Q");
    return 0;
  }
    
  else if (touchP == 1){
    GLCD_SetW(xOffset,yOffset,xOffset+20,yOffset+20);
    GLCD_print("P");
    return 1;
  }
  else
    return 1;
  
   
}


int newDevice(void) 
{
  GLCD_SetFont(&Terminal_18_24_12,0x000000,0xffffff);
  GLCD_SetWindow(148,33, 318, 166);
  GLCD_print("\n\r New Device \n\r  Registered");

  GLCD_SetFont(&Terminal_18_24_12,0x000000,0xF7F7F7);
  GLCD_SetW(220,130,246,150);
  GLCD_print("OK");
  
  GLCD_SetFont(&Terminal_18_24_12,0x000FFF,0xF7F7F7);
  int button;
  button = touchOK;
  if(button==1){
  GLCD_SetW(220,130,246,150);
  GLCD_print("OK");
  return 1;
  }
  else
    return 0;
}

void GLCD_SetW(int x1, int y1, int x2, int y2)
{
  GLCD_SetWindow(x1,y1,x2,y2);
  GLCD_TextSetPos(0,0);
}

void readTouchScreen()
{ 

  if( 205 < cursor_x  && cursor_x < 239 && 169 < cursor_y && cursor_y < 194){
    // button P
    touchOK=0;
    touchP=1;
    touchQ=0;
  }
  else if( 251 < cursor_x  && cursor_x < 284  && 169 < cursor_y  && cursor_y < 194 ){
    // button Q
    touchOK=0;
    touchP=0;
    touchQ=1;
  }

  else if( 220 < cursor_x  && cursor_x < 246  && 130 < cursor_y  && cursor_y < 150){
    // button OK
    touchOK=1;
    //touchP=0;
    //touchQ=0;
  }

}

void touchReaction()
{
    /****************************************
    //TouchScreen logic
    ****************************************/
    if(TouchScrGetStatus(&XY_Touch))
    {
      //Move the cursor
      cursor_x = XY_Touch.X;
      cursor_y = XY_Touch.Y;
      GLCD_Move_Cursor(cursor_x, cursor_y);
      if (FALSE == Touch)
      {
        Touch = TRUE;
        
      readTouchScreen();
    //    USB_H_LINK_LED_FCLR = USB_H_LINK_LED_MASK;
        
      }
    }
    else if(Touch)
    {
     // USB_H_LINK_LED_FSET = USB_H_LINK_LED_MASK;
      Touch = FALSE;
    }
}
