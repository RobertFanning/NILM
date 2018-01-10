#include "includes.h"
#include "functions.h"
extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;
extern struct device_t{int devicenumber;float WA;float VAR;float VA;} ;


struct button b1={"F1",0,190,77,239};
struct button b2={"F2",83,190,157,239};
struct button b3={"F3",163,190,237,239};
struct button b4={"F4",243,190,319,239};

void drawMenuStart(void){
   GLCD_SetFont(&Terminal_18_24_12,0x000000,0x000000); // top black line 5px.
  GLCD_SetWindow(0,0,319,4);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f                      ");
  GLCD_SetFont(&Terminal_18_24_12,0xffffff,0x00ff00); // top bar 
  GLCD_SetWindow(0,5,319,40);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f     Ready to operate");
    GLCD_SetFont(&Terminal_18_24_12,0x000000,0x000000); // black bar 4px
  GLCD_SetWindow(0,40,319,46);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f                                ");
  
  GLCD_SetFont(&Terminal_18_24_12,0xffffff,0x000000); 
  /*
 // Button 1 
  GLCD_SetWindow(b1.upper_x,b1.upper_y,b1.lower_x,b1.lower_y);
   GLCD_TextSetPos(0,10);
  GLCD_print("\f  %s ",b1.name);
 // Button 2 
  GLCD_SetWindow(b2.upper_x,b2.upper_y,b2.lower_x,b2.lower_y);
   GLCD_TextSetPos(0,10);
  GLCD_print("\f  %s ",b2.name);
 // Button 3 
  GLCD_SetWindow(b3.upper_x,b3.upper_y,b3.lower_x,b3.lower_y);
   GLCD_TextSetPos(0,10);
  GLCD_print("\f  %s ",b3.name);
 // Button 4 
   GLCD_SetWindow(b4.upper_x,b4.upper_y,b4.lower_x,b4.lower_y);
   GLCD_TextSetPos(0,10);
  GLCD_print("\f  %s ",b4.name);
*/
 // Middle bar LEFT
    GLCD_SetFont(&Terminal_18_24_12,0x000000,0xffffff);
  GLCD_SetWindow(0,46,180,189);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f MIDDLE\n\r LEFT ");
   // Middle bar RIGHT
    GLCD_SetFont(&Terminal_18_24_12,0x000000,0xffffff);
  GLCD_SetWindow(181,46,319,189);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f MIDDLE\n\r RIGHT ");
  
  /*
  // ########### Dividers  
  GLCD_SetFont(&Terminal_18_24_12,0xffffff,0xffffff); 
  // Divider 1
  GLCD_SetWindow(78,190,82,239);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f ");
 // Divider 2
  GLCD_SetWindow(158,190,162,239);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f ");
 // Divider 3
  GLCD_SetWindow(238,190,242,239);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f ");
*/
return;
}
void drawMenuAdd(void){
 // Middle bar LEFT
    GLCD_SetFont(&Terminal_18_24_12,0x000000,0xffffff);
  GLCD_SetWindow(0,46,180,189);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f \n\r Add new device? ");
  // Middle Bar RIGHT
  GLCD_SetFont(&Terminal_18_24_12,0xffffff,0x000000);
  GLCD_SetWindow(181,46,319,189);
  GLCD_TextSetPos(0,10);
  GLCD_print("\f \n\r  ADD ");
    GLCD_SetFont(&Terminal_18_24_12,0xffffff,0x000000);
  GLCD_SetWindow(b2.upper_x,b2.upper_y,b2.lower_x,b2.lower_y);
   GLCD_TextSetPos(0,10);
  GLCD_print("\fSKIP");
    GLCD_SetWindow(b3.upper_x,b3.upper_y,b3.lower_x,b3.lower_y);
   GLCD_TextSetPos(0,10);
  GLCD_print("\f ADD");
return;
}
void drawMenuEdgeDetected(void){
   // Middle bar 
    GLCD_SetFont(&Terminal_18_24_12,0x000000,0xffffff);
  GLCD_SetWindow(0,46,319,189);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f there has been a\n\r shift  in \n\r the Force! \n\r Edge detected ");
  return;
}
void drawMenuEdgeDetectedDeviceFound(void){
   // Middle bar 
    GLCD_SetFont(&Terminal_18_24_12,0x000000,0xffffff);
  GLCD_SetWindow(0,46,319,189);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f there has been a\n\r shift  in \n\r the Force! \n\r Device Found ");
  return;
}
void drawMenuDeviceAdded(void){
   // Middle bar 
    GLCD_SetFont(&Terminal_18_24_12,0x000000,0xffffff);
  GLCD_SetWindow(0,46,319,189);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fYou have \n\r succesfully added \n\r a new device!\n\r \n\r");
  
  return;
}
void drawButtonUpdate(void){
  GLCD_SetFont(&Terminal_18_24_12,0xffffff,0x000000);
  GLCD_SetWindow(b1.upper_x,b1.upper_y,b1.lower_x,b1.lower_y);
   GLCD_TextSetPos(0,10);
  GLCD_print("\f  %s ",b1.name);
 // Button 2 
  GLCD_SetWindow(b2.upper_x,b2.upper_y,b2.lower_x,b2.lower_y);
   GLCD_TextSetPos(0,10);
  GLCD_print("\f  %s ",b2.name);
 // Button 3 
  GLCD_SetWindow(b3.upper_x,b3.upper_y,b3.lower_x,b3.lower_y);
   GLCD_TextSetPos(0,10);
  GLCD_print("\f  %s ",b3.name);
 // Button 4 
   GLCD_SetWindow(b4.upper_x,b4.upper_y,b4.lower_x,b4.lower_y);
   GLCD_TextSetPos(0,10);
  GLCD_print("\f  %s ",b4.name);
  
  return;
  
}
void drawTopUpdate(int V,int A, int P, int Q, int PF, int H){
  GLCD_SetFont(&Terminal_9_12_6,0x000000,0x00ff00); // Status OK
  GLCD_SetWindow(0,5,319,41);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f                                          ");
  GLCD_SetFont(&Terminal_9_12_6,0x000000,0x00ff00); // Status OK
  GLCD_SetWindow(5,5,60,41);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f \n\rV:%d",V);
   // GLCD_SetFont(&Terminal_9_12_6,0xffff00,0x00ff00); // Status OK
  GLCD_SetWindow(63,5,105,41);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f \n\rA:%d",A);
   // GLCD_SetFont(&Terminal_9_12_6,0xffff00,0x00ff00); // Status OK
  GLCD_SetWindow(107,5,150,41);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f \n\rP:%d",P);
   // GLCD_SetFont(&Terminal_9_12_6,0xffff00,0x00ff00); // Status OK
  GLCD_SetWindow(153,5,200,41);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f \n\rQ:%d",Q);
  //  GLCD_SetFont(&Terminal_9_12_6,0xffff00,0x00ff00); // Status OK
  GLCD_SetWindow(205,5,249,41);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f \n\rPF:%d",PF);
   // GLCD_SetFont(&Terminal_9_12_6,0xffff00,0x00ff00); // Status OK
  GLCD_SetWindow(260,5,319,41);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f \n\rH:%d",H);
    GLCD_SetFont(&Terminal_9_12_6,0x000000,0x000000); // Status OK
  GLCD_SetWindow(0,42,319,46);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f                                          ");
  return;
}
void drawLearn1(void){
     // Middle bar RIGHT
    GLCD_SetFont(&Terminal_18_24_12,0x000000,0xffffff);
  GLCD_SetWindow(181,46,319,189);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f\n\r UNPLUG\n\r DEVICE ");
  return;
  
}
void drawLearn2(void){
     // Middle bar RIGHT
    GLCD_SetFont(&Terminal_18_24_12,0x000000,0xffffff);
  GLCD_SetWindow(181,46,319,189);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f \n\r PLUG\n\r DEVICE ");
  return;
}
void drawLearn3(void){
     // Middle bar RIGHT
    GLCD_SetFont(&Terminal_18_24_12,0xffffff,0x00ff00);
  GLCD_SetWindow(181,46,319,189);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f \n\r DEVICE\n\r ADDED\n\r  ");
  return;
}
//void drawMenuStatus(Device devices[5]){// preparing for merger
/*
for i=1to5;devices[i].name   devices[i].V etc.
*/


  void drawMenuStatus(void){
   // Middle 
    GLCD_SetFont(&Terminal_9_12_6,0x000000,0xffffff);
  GLCD_SetWindow(0,46,319,239);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f\n\r \n\r device 1\t P:50 Q:20 \n\r device 2\t P:90 Q:10 \n\r Lamp001 \t P:25 Q:00");
  
  return;
}
