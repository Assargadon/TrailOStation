// include the necessary libraries
#include <SPI.h>
#include <TFT.h>
#include <SD.h>
#include "hardware.h"
#include "MVC.h"
#include "screens.h"
#include "scr01_test_buttons.h"
#include "scr02_test_members.h"
#include "scr03_load_game.h"
#include "scr04_enter_id.h"
#include "scr05_check_id.h"
#include "scr06_wait_for_start.h"
#include "scr07_play.h"
#include "scr08_save_result.h"


MVC mvc;
File file;
String line;

unsigned long int seconds = 0; //global time in 10'th of seconds (so 7 seconds equal to 70 in this variable
int player_id;

//====game settings=======
boolean zero_accepted; //if true - "zero" answer accepted, if false - no
int time_to_complete; //in seconds, UNLIKE internal 'seconds' counter
short int tasks; //number of tasks
short int accepted_flags[MAX_TASKS]; // 1- only A accepted (and maybe zero). 2 - A and B accepted. 5 - A,B,C,D,E accepted.
short int selected_flags[MAX_TASKS]; // answers of a player



unsigned short int buttons_bounce[BUTTONS] = {0};
#define BOUNCE_CAP 100

void draw_time(TFT &screen, int area){
  screen.fill(0,0,0);
  screen.stroke(0,0,0);
  screen.rect(10,SCR_HEIGHT-10,7*FONT_WIDTH,FONT_HEIGHT);
  screen.stroke(0,160,255);
  screen.setTextSize(1);
  char time[12];
  snprintf(time, 12, "%03ld", seconds/10);
  screen.text(time,10,SCR_HEIGHT-10);
}

void default_timer_controller(MVC &self, unsigned long int seconds){
  if(!(seconds%10)) self.redraw_flags[0]=true;
}


void load_time_from_results_file()
{
  if(!SD.begin()) return;
  if(!(file = SD.open("result.txt"))) return;
  seconds=0;
  while(file.available())
  {
      line = file.readStringUntil('\n'); //Insecure! No limitation of line lenght
      int secs;
      if(sscanf(line.c_str(), "%d", &secs)>0)
        if (secs && (secs>seconds)) seconds = secs;
  }
}

void setup() {
  Serial.begin(9600);
  mvc.screen.begin();
  mvc.screen.background(0, 0, 0);

  for(int pin=22;pin<42;pin+=2)
    pinMode(pin, INPUT_PULLUP);
  pinMode(sd_cs, OUTPUT); //Need this for SD-card/SPI working.
  
  load_time_from_results_file();

  mvc.timer_controller = default_timer_controller;
  mvc.drawCallback[0] = draw_time;

  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = 59286;            // preload timer 65536-16MHz/256/10Hz
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts  

  mvc.switch_to = SCR_TEST_BUTTONS;
}



ISR(TIMER1_OVF_vect)        
{
  TCNT1 = 59286;            // preload timer
  seconds++;
  mvc.timer_controller(mvc, seconds);
}



void loop()
{
  if(mvc.switch_to){
    mvc.cleanup();
    switch(mvc.switch_to){
      case SCR_TEST_BUTTONS: scr_test_buttons_init(mvc); break;
      case SCR_TEST_MEMBERS: scr_test_members_init(mvc); break;
      case SCR_LOAD_GAME: scr_load_game_init(mvc); break;
      case SCR_ENTER_ID: scr_enter_id_init(mvc); break;
      case SCR_CHECK_ID: scr_check_id_init(mvc); break;
      case SCR_WAIT_FOR_START: scr_wait_for_start_init(mvc); break;
      case SCR_PLAY: scr_play_init(mvc); break;
      case SCR_SAVE_RESULT: scr_save_result_init(mvc); break;
    }
    mvc.switch_to = 0;
  }

// Debouncing alhorithm
  for(int pin=22;pin<42;pin+=2)
  {
    int btn = (pin-22)/2; //Decoding from physical pin number to virtual "button number"
    if(LOW==digitalRead(pin)) //We can speed-up reading of button state, of course - but WHY? NOT doing this is just as good, and simpler!
    { //Button pressed (OR bouncing)
      if(buttons_bounce[btn] == 0)
        if(mvc.controller[btn])
          mvc.controller[btn](mvc, btn);
          
      buttons_bounce[btn] = BOUNCE_CAP;
    }
    else
    { //Button UNpressed OR bouncing
      if(buttons_bounce[btn] > 0) buttons_bounce[btn]--;
    }
  }
 
  mvc.draw();
}
