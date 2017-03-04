extern boolean zero_accepted; //if true - "zero" answer accepted, if false - no
extern int time_to_complete; //in seconds, UNLIKE internal 'seconds' counter
extern short int tasks; //number of tasks
extern short int accepted_flags[]; // 1- only A accepted (and maybe zero). 2 - A and B accepted. 5 - A,B,C,D,E accepted.
extern short int selected_flags[]; // answers of a player

extern unsigned long int seconds;

extern void draw_zero_icon(TFT &screen, int x, int y, boolean zero);


unsigned long int start_time;
int current_task;

extern char* flags[]; // {"Z", "A", "B", "C", "D", "E", "F", "G", "H", "-"} - defined in scr03_load_game.h

void draw_timer(TFT &screen, int area){
  screen.stroke(255,255,255);
  screen.fill(0,0,0);
  screen.setTextSize(3);
  screen.rect(40-4, 10-4, FONT_WIDTH*3*3+8, FONT_HEIGHT*3+8);
  screen.text(String((time_to_complete-(seconds-start_time))/10).c_str(), 40, 10);
}

void draw_task(TFT &screen, int area){
  screen.setTextSize(2);

  screen.stroke(255,255,255);
  screen.text("Task:", 0, 45);
  screen.text("flags A-", 0, 65);

  screen.stroke(0,0,0); screen.fill(0,0,0);
  screen.text(String(current_task  ).c_str(), 0+6*2*FONT_WIDTH, 45);
  screen.rect(0+8*2*FONT_WIDTH, 65, FONT_WIDTH*2, FONT_HEIGHT*2);
  
  screen.stroke(255,255,255);
  screen.text(String(current_task+1).c_str(), 0+6*2*FONT_WIDTH, 45);
  screen.text(flags[accepted_flags[current_task]], 0+8*2*FONT_WIDTH, 65);
}

void draw_answers(TFT &screen, int area){
  screen.stroke(255,255,255);
  screen.setTextSize(2);
  screen.text(flags[selected_flags[current_task-1]], (FONT_WIDTH*2 + 3)*(current_task-1), SCR_HEIGHT-35);
}


void finalize_selection(MVC &mvc)
{
  if(current_task>=tasks)
    mvc.switch_to=SCR_SAVE_RESULT;
  else
    mvc.redraw_flags[2]=true;
  mvc.redraw_flags[3]=true;
}


void flag_selected(MVC &mvc, int button){
  int flag = button + 1;
  if(flag <= accepted_flags[current_task])
  {
    selected_flags[current_task++] = flag;
    finalize_selection(mvc);
  }
}

void zero_selected(MVC &mvc, int button){
  if(zero_accepted)
  {
    selected_flags[current_task++] = 0;
    finalize_selection(mvc);
  }
}


void play_timer_controller(MVC &mvc, unsigned long int seconds){
  if(!(seconds%10)) mvc.redraw_flags[0]=true;
  if(!((seconds-start_time)%10)) mvc.redraw_flags[1]=true;
  if((start_time+(unsigned long int)time_to_complete) < seconds) mvc.switch_to = SCR_SAVE_RESULT;
}


void scr_play_init(MVC &mvc){
  mvc.screen.background(0, 0, 0);
  draw_zero_icon(mvc.screen, SCR_WIDTH-35, 15, zero_accepted);
  for(int n=0; n<8; n++)
    mvc.controller[n] = flag_selected;
  mvc.controller[9] = zero_selected;

  start_time = seconds+2; //small time to display everything and for initial time to be displayed at least for a moment
  current_task = 0;
  mvc.timer_controller = play_timer_controller;
  
  mvc.drawCallback[1] = draw_timer;
  mvc.drawCallback[2] = draw_task;
  mvc.drawCallback[3] = draw_answers;
  
  mvc.redraw_flags[1]=true;
  mvc.redraw_flags[2]=true;
}
