extern void default_timer_controller(MVC &self, unsigned long int seconds);
extern void to_enter_id(MVC &mvc, int btn);

extern unsigned long int seconds;
extern int player_id;

extern short int tasks; //number of tasks
extern short int selected_flags[MAX_TASKS]; // answers of a player
extern unsigned long int start_time;

unsigned long int end_time;
extern int current_task;

void fill_unfinished_tasks(){
  for(int task=current_task; task<tasks; task++)
    selected_flags[task] = MAX_FLAG+1; //no answer
}

void draw_seconds_spend(TFT &screen){
  screen.stroke(255,255,255);
  screen.fill(0,0,0);
  screen.setTextSize(3);
  screen.rect(40-4, 10-4, FONT_WIDTH*3*3+8, FONT_HEIGHT*3+8);
  screen.text(String((end_time-start_time)/10).c_str(), 40, 10);
}



int save_result()
{
  if(!SD.begin()) return -1;;
  file = SD.open("result.txt", FILE_WRITE);
  if(!file) return -2;
  
  if(!file.print(start_time)) return -5;
  if(!file.print('\t')) return -4;
  if(!file.print(player_id)) return -3;
  if(!file.print('\t')) return -6;
  if(!file.print(end_time-start_time)) return -7;
  
  for(int task=0; task<tasks; task++)
  {
    if(!file.print('\t')) return -8;
    if(!file.print(flags[selected_flags[task]])) return -9;
  }
  if(!file.print("\r\n")) return -10; //Windows-style end-of-line
  file.close();
  return 1;
}
  
  
void scr_save_result_init(MVC &mvc)
{
  end_time = seconds;
  mvc.timer_controller = default_timer_controller;
  draw_seconds_spend(mvc.screen);


  fill_unfinished_tasks();
  
  mvc.screen.noFill();
  int tmp=save_result();
  if(tmp>0)
    mvc.screen.stroke(0,255,0);
  else
  {
    Serial.print("writing error "); Serial.println(tmp);
    mvc.screen.stroke(0,0,255);
  }
  mvc.screen.rect(0,0,SCR_WIDTH-1,SCR_HEIGHT-1);
  
  mvc.screen.setTextSize(1);
  mvc.screen.stroke(255,255,255);
  mvc.screen.text("next 8", SCR_WIDTH-(FONT_WIDTH*6), SCR_HEIGHT-10);
  
  mvc.controller[8] = to_enter_id;
}
