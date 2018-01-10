#include "includes.h"
extern struct button {char name[5]; int upper_x;int upper_y;int lower_x;int lower_y;};
//extern struct device_t{int devicenumber;float WA;float VAR;float VA;} ;


void drawMenuStart(void);
void drawMenuAdd(void);
void drawMenuEdgeDetected(void);
void drawMenuDeviceAdded(void);
void drawButtonUpdate(void);
void drawMenuEdgeDetectedDeviceFound(void);
void drawButtonUpdate(void);
void drawTopUpdate(int V,int A, int P, int Q, int PF, int H);
void drawLearn1(void);
void drawLearn2(void);
void drawLearn3(void);
void drawMenuStatus(void);