extern unsigned long int seconds;

extern boolean zero_accepted; //if true - "zero" answer accepted, if false - no
extern int time_to_complete;
extern short int tasks; //number of tasks
extern short int accepted_flags[]; // 1- only A accepted (and maybe zero). 2 - A and B accepted. 5 - A,B,C,D,E accepted.

boolean default_time_to_complete;
boolean default_zero_accepted;
boolean default_tasks;

char* flags[] = {"Z", "A", "B", "C", "D", "E", "F", "G", "H", "-"};

void load_default_time_to_complete(){
  time_to_complete = 600;
  default_time_to_complete = true;
 }

void load_default_zero_accepted(){
  zero_accepted = false;
  default_zero_accepted = true;
}

void load_default_tasks(){
  tasks = 4;
  for(int i=0; i<tasks; i++)
    accepted_flags[i] = 6;
  default_tasks = true;
}

void load_game_defaults()
{
  load_default_time_to_complete();
  load_default_zero_accepted();
  load_default_tasks();
}

void load_game(){
  default_time_to_complete = false;
  default_zero_accepted = false;
  default_tasks = false;

  if(!SD.begin()) {load_game_defaults(); return;}
  if(!(file = SD.open("game.txt"))) {load_game_defaults(); return;}

  // Reading time_to_complete - number in first line
  line = file.readStringUntil('\n');
  if(sscanf(line.c_str(), "%d", &time_to_complete)<1) load_default_time_to_complete();

  // Reading zero_accepted - first char in second line should be Z or z to accept zero, or any other char to decline it.
  line = file.readStringUntil('\n');
  if(line.length()<1) load_default_zero_accepted();
  else
  {
    if (line[0]=='\r') load_default_zero_accepted(); //empty line in windows-style \r\n line finishing
    else zero_accepted = (line[0]=='Z') || (line[0]=='z');
  }
  
  // Reading tasks - up to MAX_TASKS lines, each with the first character shows the number of accepted flags - see switch-case below
  tasks = 0;
  while((tasks < MAX_TASKS) && (file.available())){
    line = file.readStringUntil('\n');
    if(line.length()<1) continue;
    switch(line[0]){ 
      case 'A': case 'a': case '1': accepted_flags[tasks++]=1; break;
      case 'B': case 'b': case '2': accepted_flags[tasks++]=2; break;
      case 'C': case 'c': case '3': accepted_flags[tasks++]=3; break;
      case 'D': case 'd': case '4': accepted_flags[tasks++]=4; break;
      case 'E': case 'e': case '5': accepted_flags[tasks++]=5; break;
      case 'F': case 'f': case '6': accepted_flags[tasks++]=6; break;
      case 'G': case 'g': case '7': accepted_flags[tasks++]=7; break;
      case 'H': case 'h': case '8': accepted_flags[tasks++]=8; break;
    }
  }
  if(tasks<1) load_default_tasks();
  file.close();
}


void draw_zero_icon(TFT &screen, int x, int y, boolean zero)
{
  screen.setTextSize(3);
  screen.stroke(255,255,255);
  screen.text("Z", x+4, y+4);
  
  if(zero)
  {
    screen.stroke(0, 255, 0);
  }
  else
  {
    screen.stroke(0, 0, 255);
    screen.line(x, y+2, x+22, y+22+2);
    screen.line(x, y+3, x+22, y+22+3);
    screen.line(x, y+4, x+22, y+22+4);
    screen.line(x, y+5, x+22, y+22+5);

  }
  screen.noFill();
  screen.circle(x + 11, y+14, 15);
  screen.circle(x + 11, y+14, 15 + 1);
  screen.circle(x + 11, y+14, 15 + 2);
}

void draw_game_params(TFT &screen, int area){
  screen.fill(0,0,0);
  screen.stroke(0,0,0);
  screen.rect(0,0,SCR_WIDTH,SCR_HEIGHT-60+2*FONT_HEIGHT);
  
  screen.setTextSize(2);

  if (default_tasks) screen.stroke(0,0,255); else screen.stroke(255,255,255);
  screen.text("Tasks:", 10, 10); screen.text(String(tasks).c_str(), 90 , 10);
  for(int task = 0; task<tasks; task++)   //display accepted flags
    screen.text(flags[accepted_flags[task]], (FONT_WIDTH*2 + 3)*task, SCR_HEIGHT-60);

  if (default_time_to_complete) screen.stroke(0,0,255); else screen.stroke(255,255,255);
  screen.text("Time:", 10, 10+1*2*(FONT_HEIGHT+6)); screen.text(String(time_to_complete/10).c_str(), 90 , 10+1*2*(FONT_HEIGHT+6));

  draw_zero_icon(screen, SCR_WIDTH-35, 15, zero_accepted);
  if(default_zero_accepted){
    screen.stroke(255,255,0);
    screen.setTextSize(1);
    screen.text("default", SCR_WIDTH-35-9, 15+10);
  };
  
}

void to_enter_id(MVC &mvc, int btn){
  mvc.switch_to = SCR_ENTER_ID;
}

void reset_sd1(MVC &mvc, int btn){
  load_game();
  mvc.redraw_flags[1] = true;
}

void scr_load_game_init(MVC &mvc){
  mvc.screen.background(0,0,0);
  mvc.screen.stroke(255,255,255);
  mvc.screen.setTextSize(1);

  reset_sd1(mvc, 0); //Loading game parameters;
  
  mvc.drawCallback[1]=draw_game_params;
  mvc.controller[0]=reset_sd1;
  mvc.controller[9]=to_enter_id;
  mvc.screen.text("0 read SD again", 1, SCR_HEIGHT-25);
  mvc.screen.text("ok 9", SCR_WIDTH-(FONT_WIDTH*4), SCR_HEIGHT-25);
}

