#include "MVC.h"

void MVC::draw(){
  for(int i=0; i<MAX_REDRAW_AREAS; i++)
  {
    if(redraw_flags[i]){
      if(drawCallback[i]) drawCallback[i](screen, i); //do not call undefined callbacks
      redraw_flags[i] = false;
    }
  }
};

void MVC::cleanup(){
  int i;
  for(i=1; i<MAX_REDRAW_AREAS; i++) drawCallback[i] = NULL; //do not cleanup area 0 controller: it's a clock
  for(i=0; i<BUTTONS; i++) controller[i] = NULL;
}

