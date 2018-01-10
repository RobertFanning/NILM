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
 *    1. Date        : 12, August 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR LPC2478-SK board. It shows basic use of I/O,
 * timers, interrupts, LCD controllers and LCD touch screen.
 *
 *  A cursor is shown and moves when the screen is touched.
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
#include <includes.h>

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
// ########## colors #################
#define WHITE 0xffffff
#define BLACK 0x000000
#define RED 0x0000ff
#define GREEN 0x00ff00
#define BLUE 0xff0000
// ###################################

extern Int32U SDRAM_BASE_ADDR;
extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

extern struct device_t{int devicenumber;float WA;float VAR;float VA;} ;

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
Int32U cursor_x = (C_GLCD_H_SIZE - CURSOR_H_SIZE)/2, cursor_y = (C_GLCD_V_SIZE - CURSOR_V_SIZE)/2;
ToushRes_t XY_Touch;
Boolean Touch = FALSE;
int V=235;
int A = 2;
int P = 400;
int Q = 80;
int PF = 45;
int H = 1;
// ###################LK variables ###########
Boolean LKupdateDisplay = FALSE;
int LKbutton = 0;
int LKselectedDevice =0;
struct device_t device[4];
device[0].devicenumber = 1;
device[0].VA = 80;
device[0].WA = 70;
device[0].VAR=15;
device[1].devicenumber = 2;
device[1].VA = 40;
device[1].WA = 20;
device[1].VAR=30;
device[2].devicenumber = 3;
device[2].VA = 120;
device[2].WA = 70;
device[2].VAR=90;
device[3].devicenumber = 4;
device[3].VA = 150;
device[3].WA = 70;
device[3].VAR=80;

// ###########################################


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
  GLCD_Init (IarLogoPic.pPicStream, NULL);
  GLCD_Cursor_Dis(0);
  GLCD_Copy_Cursor ((Int32U *)Cursor, 0, sizeof(Cursor)/sizeof(Int32U));
  GLCD_Cursor_Cfg(CRSR_FRAME_SYNC | CRSR_PIX_32);
  GLCD_Move_Cursor(cursor_x, cursor_y);
  GLCD_Cursor_En(0);
  // Init touch screen
  TouchScrInit();

  // Touched indication LED
  USB_H_LINK_LED_SEL = 0; // GPIO
  USB_H_LINK_LED_FSET = USB_H_LINK_LED_MASK;
  USB_H_LINK_LED_FDIR |= USB_H_LINK_LED_MASK;

  __enable_interrupt();
  GLCD_Ctrl (TRUE);
  
// Draw Opening Menu
  drawMenuStart();
 
  while(1)
  {
    if(TouchScrGetStatus(&XY_Touch)) // did any change occur?
    {
      cursor_x = XY_Touch.X;
      cursor_y = XY_Touch.Y;
      GLCD_Move_Cursor(cursor_x, cursor_y); // update cursor position
        if (FALSE == Touch) // not being touched : turn off touch and LED
          {
            Touch = TRUE;
            USB_H_LINK_LED_FCLR = USB_H_LINK_LED_MASK;
            USB_D_LINK_LED_FCLR = USB_D_LINK_LED_MASK;
          }
    }
  
    else if(Touch &&cursor_x < 77 && cursor_y >=190) // button 1
    {
      LKbutton=1;LKupdateDisplay = TRUE;
      Touch = FALSE;
    }
    else if(Touch && LKbutton==0) // touch when learn1
    {
      LKbutton = 5;
      LKupdateDisplay = TRUE;
      Touch = FALSE;
    }
      else if(Touch && LKbutton==5) // touch when learn1
    {
      LKbutton = 6;
      LKupdateDisplay = TRUE;
      Touch = FALSE;
    }
       else if(Touch && LKbutton==6) // touch when learn1
    {
      LKbutton = 7;
      LKupdateDisplay = TRUE;
      Touch = FALSE;
    }
       else if(Touch && LKbutton==7) // touch when learn1
    {
      LKbutton = 0;
      LKupdateDisplay = TRUE;
      Touch = FALSE;
    }
    if(LKupdateDisplay == TRUE){
            LKupdateDisplay = FALSE; // this prevents unnecessary updates == no flicker on screen
            switch (LKbutton){ // Prints message for selected button
              case 0 : drawMenuStatus(); break;
              case 1 : drawMenuAdd();LKbutton=0;break;
              case 2 : drawMenuEdgeDetected();LKbutton=0;break;
              case 3 : drawMenuEdgeDetectedDeviceFound();LKbutton=0;break;
              case 4 : drawMenuDeviceAdded(); drawButtonUpdate();break;
              case 5 : drawLearn1(); break;
              case 6 : drawLearn2(); break;
              case 7 : drawLearn3(); break;
              default :   break;
              }
            drawTopUpdate(V,A,P,Q,PF,H);
    }
  }
}
