extern int player_id;

short int id_str[3];
short int id_cursor;
char* nums[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

void num_controller(MVC &mvc, int button){
  id_str[id_cursor++] = button;
  if(id_cursor>2) 
  {
    player_id = id_str[0]*100 + id_str[1]*10 + id_str[2];
    mvc.switch_to = SCR_CHECK_ID;  
  }
  else  mvc.redraw_flags[id_cursor] = true;
}

void redraw_id_char(TFT &screen, int area){
  screen.stroke(255,255,255);
  screen.setTextSize(2);
  Serial.print("Area "); Serial.println(area);
  Serial.print("id_str[area] "); Serial.println(id_str[area-1]);
  screen.text(nums[id_str[area-1]], 15+30*area, 45);
}


void scr_enter_id_init(MVC &mvc){
  mvc.screen.background(0, 0, 0);
  mvc.screen.stroke(255,255,255);
  mvc.screen.rect(40, 40, 20, 25);
  mvc.screen.rect(70, 40, 20, 25);
  mvc.screen.rect(100, 40, 20, 25);

  for(int n=0; n<10; n++) mvc.controller[n] = num_controller;
  for(int n=1; n<=3; n++) mvc.drawCallback[n] = redraw_id_char;
  id_cursor = 0;
  
  mvc.screen.stroke(255, 255, 255);
  mvc.screen.setTextSize(2);
  mvc.screen.text("member #", (SCR_WIDTH - FONT_WIDTH*2*8)/2, 85);
  
}

