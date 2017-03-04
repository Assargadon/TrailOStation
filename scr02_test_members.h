extern File file;
extern String line;


int members_error=0; //if 0 - no errors. If positive - it's a number of (first) line with error. If negative - SD card or file not found.
#define NO_SD_CARD -1
#define NO_MEMBERS_FILE -2
int members;

extern int parse_member_line(const String &line, int &member_id, char* &member_name);

void draw_members_status(TFT &screen, int area){
  Serial.println("Draw members status enter");

  screen.noStroke();
  screen.fill(0,0,0);
  screen.rect(10,20,150,15);

  screen.stroke(0,0,255);
  screen.setTextSize(1);
  if(members_error==NO_SD_CARD) {screen.text("No SD card inserted!", 10, 20); return;}
  if(members_error==NO_MEMBERS_FILE) {screen.text("No members.txt file found!", 10, 20); return;}
/*  if(members_error>0)
  {
    screen.text("Error in members file!", 10, 20);
    screen.text("Line: ", 10, 20+FONT_HEIGHT);
    screen.text(String(members_error).c_str(), 10+FONT_WIDTH*6, 20+FONT_HEIGHT); return;
  }
  if(members_error) {screen.text("Unknown file/SD error!", 10, 20); return;}
*/  
  screen.stroke(0,255,0);
  screen.setTextSize(2);
  screen.text("Members:", 10, 20);
  screen.text(String(members).c_str(), 115 , 20);
}


void count_members(){
  Serial.println("Count members enter");
  members = 0;
  if(!SD.begin()){members_error=NO_SD_CARD; return;};
  if(!(file = SD.open("members.txt"))){members_error=NO_MEMBERS_FILE; return;};
  while(file.available()){
    line = file.readStringUntil('\n'); //Insecure! No limitation of line lenght
    int id;
    char*tmp;
    if(parse_member_line(line, id, tmp))
      if(id>0) members++;
  }
  file.close();
}

void reset_sd(MVC &mvc, int btn){
  count_members();
  mvc.redraw_flags[1]=true;
}


void to_load_game(MVC &mvc, int btn){
  mvc.switch_to = SCR_LOAD_GAME;
}

void scr_test_members_init(MVC &mvc){
  Serial.println("Init config test");
  mvc.screen.background(0,0,0);
  mvc.screen.stroke(255,255,255);
  mvc.screen.setTextSize(1);
  
  reset_sd(mvc, 0);
  
  mvc.drawCallback[1]=draw_members_status;
  mvc.controller[0]=reset_sd;
  mvc.controller[9]=to_load_game;
  mvc.screen.text("0 read SD again", 1, SCR_HEIGHT-25);
  mvc.screen.text("ok 9", SCR_WIDTH-(FONT_WIDTH*4), SCR_HEIGHT-25);
}

