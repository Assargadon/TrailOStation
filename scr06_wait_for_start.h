extern int player_id;
extern void to_enter_id(MVC &mvc, int btn);
extern void draw_game_params(TFT &screen, int area);


void to_play(MVC &mvc, int btn){
  mvc.switch_to = SCR_PLAY;
}

void scr_wait_for_start_init(MVC &mvc){
  mvc.screen.background(0, 0, 0);
  draw_game_params(mvc.screen, -1); //Last parameter is meaningless in this context
  
  mvc.screen.stroke(255,255,255);
  mvc.screen.setTextSize(1);
  mvc.screen.text("0 enter other ID", 1, SCR_HEIGHT-25);

  mvc.screen.stroke(0,255,255);
  mvc.screen.text("start 8", SCR_WIDTH-(FONT_WIDTH*7), SCR_HEIGHT-25);

  mvc.controller[0]=to_enter_id; 
  mvc.controller[8]=to_play; 
}
