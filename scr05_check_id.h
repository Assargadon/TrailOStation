extern int player_id;

extern File file;
extern String line;

extern void to_enter_id(MVC &mvc, int btn);

char* player_name;
int player_found;
#define PLAYER_FOUND 1
#define PLAYER_NOT_FOUND 0
#define NO_SD_CARD -1
#define NO_MEMBERS_FILE -2

void to_wait_for_start(MVC &mvc, int btn){
  mvc.switch_to = SCR_WAIT_FOR_START;
}


//Takes line, and parse it to player_id and player_name, putting them into variables provided.
//Returns 0 if it's not possible to detect player_id at the beginning of the line or 1 if everything ok
int parse_member_line(const String &line, int &member_id, char* &member_name){
  int chars, mid;
  if(sscanf(line.c_str(), "%d%n", &mid, &chars)<1) return 0;
  member_name=(char*)line.c_str()+chars+1;
  member_id=mid;
  return 1;
}


int find_player()
{
  if(!SD.begin()) return NO_SD_CARD;
  if(!(file = SD.open("members.txt"))) return NO_MEMBERS_FILE;
  
  while(file.available())
  {
    line = file.readStringUntil('\n');
    int id;
    parse_member_line(line, id, player_name);
    if(id==player_id) return PLAYER_FOUND;
  };
  file.close();
  return PLAYER_NOT_FOUND;
}

void reset_sd2(MVC &mvc, int btn){
  player_found = find_player();
  mvc.redraw_flags[1] = true;
}

void draw_player(TFT &screen, int area){
  screen.stroke(0,0,0);
  screen.fill(0,0,0);
  screen.rect(0,60,SCR_WIDTH,FONT_HEIGHT*2*2+2);
 
  screen.stroke(255,255,255);
  screen.setTextSize(2);
  if(player_found>0) screen.text(player_name, 0, 60);
  else
  {
    screen.stroke(0,0,255);
    switch(player_found)
    {
      case(PLAYER_NOT_FOUND): screen.text("NOT FOUND", 0, 60); break;
      case(NO_SD_CARD): screen.text("No SD card", 0, 60); break;
      case(NO_MEMBERS_FILE): screen.text("members.txt", 0, 60); screen.text("not found", 0, 60+FONT_HEIGHT*2+2); break;
    }  
 
  }  
}


void scr_check_id_init(MVC &mvc){
  mvc.screen.background(0, 0, 0);
  mvc.screen.stroke(255,255,255);
  mvc.screen.setTextSize(3);
  mvc.screen.text("#", 0, 20);
  mvc.screen.text(String(player_id).c_str(), FONT_WIDTH*3, 20);
  
  reset_sd2(mvc, 0); //looking for player
  
  mvc.screen.stroke(255,255,255);
  mvc.screen.setTextSize(1);
  mvc.screen.text("1 read SD again", 1, SCR_HEIGHT-25-FONT_HEIGHT-2);
  mvc.screen.text("0 back", 1, SCR_HEIGHT-25);
  mvc.screen.text("ok 9", SCR_WIDTH-(FONT_WIDTH*4), SCR_HEIGHT-25);

  mvc.drawCallback[1]=draw_player;
  mvc.controller[1]=reset_sd2;
  mvc.controller[0]=to_enter_id;
  mvc.controller[9]=to_wait_for_start;
}
