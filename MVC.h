#ifndef MVC_CLASS
#define MVC_CLASS
#include <TFT.h>
#include "hardware.h"

#define BUTTONS 10
#define NULL 0
#define MAX_REDRAW_AREAS 8

class MVC   // название класса
{
public:
  TFT screen = TFT(lcd_cs, dc, rst);
  bool redraw_flags[MAX_REDRAW_AREAS]={false};
  int switch_to=0; //if set to non-zero, switch to other screen
  
  void *model;
  void (*drawCallback[MAX_REDRAW_AREAS])(TFT &screen, int area) = {NULL};
  void (*controller[BUTTONS])(MVC &self, int button) = {NULL}; //we pass button number as a parameter to have the possibility to use the same callback for different buttons
  void (*timer_controller)(MVC &self, unsigned long int seconds);
  
  void cleanup();
  void draw();
};

#endif
