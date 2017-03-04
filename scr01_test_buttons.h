boolean buttons[10]={0};

void redraw_test_rects(TFT &screen, int area){
  screen.noFill();
  screen.stroke(0,0,255);
  for(int n=0; n<10; n++){
    if(buttons[n])
      screen.rect(
        (n%5)*30+10,
        (n<5)?15:45,
        20,25
      );
  }
}

void check_button_controller(MVC &mvc, int button){
  if(!buttons[button]){
    buttons[button]=true;
    mvc.redraw_flags[1]=true;
  };
  
  for(int btn=0; btn<BUTTONS; btn++)
    if(!buttons[btn]) return;
  mvc.switch_to=SCR_TEST_MEMBERS;
  
}


void scr_test_buttons_init(MVC &mvc){
  mvc.screen.stroke(255,255,255);
  mvc.screen.setTextSize(2);
  mvc.screen.text("keyboard test", (SCR_WIDTH-13*FONT_WIDTH*2)/2, 75);
  mvc.screen.text("press all", (SCR_WIDTH-9*FONT_WIDTH*2)/2, 75+FONT_HEIGHT*2+2);
  for(int n=0; n<10; n++){
    mvc.screen.text(
      String(n).c_str(),
      (n%5)*30+15,
      (n<5)?20:50
    );
    mvc.controller[n] = check_button_controller;
  }

  mvc.drawCallback[1] = redraw_test_rects;
}




