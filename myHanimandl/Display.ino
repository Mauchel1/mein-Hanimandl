static int16_t Width_Header = 35;
static int16_t Height_Header = 33;
static int16_t Margin = 2;
static int16_t Margin_Item = 25;
static int16_t triangleSize = 13;

static int16_t SingleCharHeight = 8;
static int16_t SingleCharWidth = 6;

SetupMenu currentMenu;

void drawMsg(char msg[], int x, int y, int textsize=2)
{
  myScreen.setTextSize(textsize);
  myScreen.text(msg, x, y );
}

void SetupInitScreen(){

  currentMenu = Setup_Calibrate;
  myScreen.background(0,0,0);  // clear the screen with black
  myScreen.stroke(255,255,255);
  myScreen.setTextSize(3);

  myScreen.text("SETUP",Width_Header,Margin);

  myScreen.setTextSize(2);

  for(int item = Setup_Calibrate; item<=Setup_Parameter; item++) {
    myScreen.text(SetupMenuEntries[item], Margin_Item, (item * 20) + Height_Header );
  }
  setCursor(currentMenu, 0xFFFF);
}

void CalibScreen1(){

  myScreen.background(0,0,0);  // clear the screen with black
  myScreen.stroke(255,255,255);
  myScreen.setTextSize(3);

  myScreen.text("Kalib.",Width_Header,Margin);

  myScreen.setTextSize(2);

  myScreen.setCursor(Margin_Item, (0 * 20) + Height_Header);
  myScreen.println("Waage leer");
  myScreen.println("    machen!");

  myScreen.text("Ok", Margin_Item, (3 * 20) + Height_Header );

  setCursor(3, 0xFFFF);
}

void CalibScreen2(){
  myScreen.fillRect(0, Height_Header, myScreen.width(), 60, 0x0000); //delete all but Header

  myScreen.text("Waage mit", Margin_Item, (0 * 20) + Height_Header );
  myScreen.text(" beladen", Margin_Item, (2 * 20) + Height_Header );
}

void CalibScreenResult(){
  myScreen.fillRect(0, Height_Header, myScreen.width(), 60, 0x0000);

  myScreen.setCursor(Margin_Item, (0 * 20) + Height_Header);
  myScreen.println("Erfolgreich");
}

void ServoScreenStart(){
  currentMenu = 0;
  myScreen.background(0,0,0);  // clear the screen with black
  myScreen.stroke(255,255,255);
  myScreen.setTextSize(3);

  myScreen.text("SERVO",Width_Header,Margin);

  myScreen.setTextSize(2);

  myScreen.text("Minimal", Margin_Item, (0 * 20) + Height_Header );
  sprintf(charBuf, "%d", minAngle.getValue());
  drawMsg(charBuf, Margin_Item + SingleCharWidth*8*2, (0 * 20) + Height_Header, 2);
  myScreen.text("Maximal", Margin_Item, (1 * 20) + Height_Header );
  sprintf(charBuf, "%d", maxAngle.getValue());
  drawMsg(charBuf, Margin_Item + SingleCharWidth*8*2, (1 * 20) + Height_Header, 2);

  myScreen.text("       back", Margin_Item, (4 * 20) + Height_Header ); 

  setCursor(0, 0xFFFF);
}

void ServoScreenChoose(){
  
  myScreen.fillRect(0, Height_Header, myScreen.width(), 60, 0x0000); //delete all but Header

  if (currentMenu == 0)
  {
    myScreen.text("Minimal", Margin_Item, (0 * 20) + Height_Header ); 
    sprintf(charBuf, "%d", minAngle.getValue());
    drawMsg(charBuf, Margin_Item + SingleCharWidth*8*2, (0 * 20) + Height_Header, 2);
  }
  else
  {
    myScreen.text("Maximal", Margin_Item, (0 * 20) + Height_Header ); 
    sprintf(charBuf, "%d", maxAngle.getValue());
    drawMsg(charBuf, Margin_Item + SingleCharWidth*8*2, (0 * 20) + Height_Header, 2);
  }
  myScreen.text("live Angle:", Margin_Item, (2 * 20) + Height_Header ); 
  sprintf(charBuf, "%d", angle);
  drawMsg(charBuf, Margin_Item, (3 * 20) + Height_Header, 2);
  myScreen.text("change back", Margin_Item, (4 * 20) + Height_Header ); 

}

void ServoScreenChange(){
  myScreen.fillRect(0, (4 * 20) + Height_Header, myScreen.width(), 60, 0x0000); //delete all but Header
  myScreen.text("save   back", Margin_Item, (4 * 20) + Height_Header ); 
}

void GlassInitScreen(){

  currentMenu = 0;
  myScreen.background(0,0,0);  // clear the screen with black
  myScreen.stroke(255,255,255);
  myScreen.setTextSize(3);

  myScreen.text("GLAS",Width_Header,Margin);

  myScreen.setTextSize(2);

  for (int i = 0; i < 4; i++){
    sprintf(charBuf, "Glas %d", i + 1);
    myScreen.text(charBuf, Margin_Item, (i * 20) + Height_Header );
    sprintf(charBuf, "%d", glasses[i].getFillweight());
    drawMsg(charBuf, Margin_Item + SingleCharWidth*7*2, (i * 20) + Height_Header, 2);
  }
  
  myScreen.text("       back", Margin_Item, (4 * 20) + Height_Header ); 

  setCursor(currentMenu, 0xFFFF);
}

void GlassSelectedScreen(int i){
  myScreen.fillRect(0, Height_Header, myScreen.width(), 100, 0x0000); //delete all but Header

  sprintf(charBuf, "   Glas %d", i + 1);
  myScreen.text(charBuf, Margin_Item, (0 * 20) + Height_Header );
  drawMsg("Fill:     g", Margin_Item, (1 * 20) + Height_Header, 2);
  sprintf(charBuf, "      %4d", glasses[i].getFillweight()); 
  drawMsg(charBuf, Margin_Item  , (1 * 20) + Height_Header, 2);
  drawMsg("Leer:     g", Margin_Item, (2 * 20) + Height_Header, 2);
  sprintf(charBuf, "      %4d", glasses[i].getEmptyweight());
  drawMsg(charBuf, Margin_Item , (2 * 20) + Height_Header, 2);

  myScreen.text("edit   back", Margin_Item, (4 * 20) + Height_Header ); 
  setCursor(1, 0xFFFF);

}

void ManuelInitScreen(){
  myScreen.background(0,0,0);  // clear the screen with black
  drawMsg("Man.",Margin,Margin,3);
  
  drawMsg("min:",myScreen.width()-(SingleCharWidth*8),Margin,1);
  sprintf(charBuf, "%d", minAngle.getValue());
  drawMsg(charBuf,myScreen.width()-(SingleCharWidth*4),Margin,1);
  drawMsg("max:",myScreen.width()-(SingleCharWidth*8),Margin + SingleCharHeight,1);
  sprintf(charBuf, "%d", maxAngle.getValue());
  drawMsg(charBuf,myScreen.width()-(SingleCharWidth*4),Margin + SingleCharHeight,1);

  drawMsg("Ist:",myScreen.width()-(SingleCharWidth*2*8),Margin + SingleCharHeight*3,2);

  drawMsg("g",SingleCharWidth * 4 * 4 - Margin, myScreen.height()-(SingleCharHeight*8),4);

  myScreen.stroke(255,0,255);
  drawMsg("PAUSE",SingleCharWidth * 2 * 1, myScreen.height()-(SingleCharHeight*2*3+Margin),2); 
  myScreen.stroke(255,255,255);
  myScreen.setTextSize(2); 
  myScreen.text("start  stop", Margin_Item, (4 * 20) + Height_Header ); 

  // increase font size for text in loop()
  myScreen.setTextSize(4); 
}

void AutomaticInitScreen(){
  myScreen.background(0,0,0);  // clear the screen with black

  myScreen.stroke(255,255,255);
  drawMsg("Auto", Margin + SingleCharWidth*2,Margin,3);

  drawMsg("min:",myScreen.width()-(SingleCharWidth*8),Margin,1);
  sprintf(charBuf, "%d", minAngle.getValue());
  drawMsg(charBuf,myScreen.width()-(SingleCharWidth*4),Margin,1);
  drawMsg("max:",myScreen.width()-(SingleCharWidth*8),Margin + SingleCharHeight,1);
  sprintf(charBuf, "%d", maxAngle.getValue());
  drawMsg(charBuf,myScreen.width()-(SingleCharWidth*4),Margin + SingleCharHeight,1);
  drawMsg("Ist:",myScreen.width()-(SingleCharWidth*8),Margin + SingleCharHeight*2,1);

  drawMsg("TARA",myScreen.width()-(SingleCharWidth*5),Margin + SingleCharHeight*3,1);

  ChangeDisplayedAutoparamGlass();

  drawMsg(" Ziel:",Margin,SingleCharHeight*5+Margin,2);

  drawMsg("g",myScreen.width()/2-SingleCharWidth*2*2,SingleCharHeight*7+Margin,2);
  drawMsg("  g",myScreen.width()/2-SingleCharWidth*8,SingleCharHeight*9+Margin,1);
  ChangeDisplayedAutoparamKulanz();
  
  drawMsg("Ist",myScreen.width()*0.6,SingleCharHeight*5+Margin,2);
  drawMsg("g",myScreen.width()-SingleCharWidth*3-Margin,SingleCharHeight*7+Margin,3);


  myScreen.drawRect(Margin, SingleCharHeight*11, myScreen.width()-Margin, SingleCharHeight*2, 0xF00F);

  drawMsg("Autostart: ",Margin,myScreen.height()-SingleCharHeight*2-Margin*2,1);
  if (autoStart.getValue()) {
    drawMsg("on",SingleCharWidth*10 + Margin,myScreen.height()-SingleCharHeight*2-Margin*2,1);
  }else {
    drawMsg("off",SingleCharWidth*10 + Margin,myScreen.height()-SingleCharHeight*2-Margin*2,1);
  }
  
  drawMsg("Nr. XXX ",myScreen.width()-(SingleCharWidth*7),myScreen.height()-SingleCharHeight*2-Margin*2,1);

  drawMsg("      START",Margin,myScreen.height()-SingleCharHeight-Margin,1);
  drawMsg("   BACK",myScreen.width()-(SingleCharWidth*9),myScreen.height()-SingleCharHeight-Margin,1);
  //drawMsg("NOTHALT!",myScreen.width()-(SingleCharWidth*9),myScreen.height()-SingleCharHeight-Margin,1);

  // increase font size for text in loop()
  myScreen.setTextSize(4); 
}

void ChangeDisplayedAutoparam()
{
  myScreen.stroke(255,255,255);
  ChangeDisplayedAutoparamOnOff(lastAutomaticParam);
  myScreen.stroke(255,255,0);
  ChangeDisplayedAutoparamOnOff(currentAutomaticParam);
  myScreen.stroke(255,255,255);
}

void ChangeDisplayedAutoparamOnOff(int state) {

  switch (state) {
  case Autoparam_Autostart:
      drawMsg("Autostart: ",Margin,myScreen.height()-SingleCharHeight*2-Margin*2,1);
    if (autoStart.getValue()) {
      drawMsg("on",SingleCharWidth*10 + Margin,myScreen.height()-SingleCharHeight*2-Margin*2,1);
    }else {
      drawMsg("off",SingleCharWidth*10 + Margin,myScreen.height()-SingleCharHeight*2-Margin*2,1);
    }
    break;
  case Autoparam_Glass:
    ChangeDisplayedAutoparamGlass();
    //drawMsg("Glastyp: ",Margin,SingleCharHeight*3+Margin,1);
    //sprintf(charBuf, "%d", currentGlass.getValue() + 1);
    //drawMsg(charBuf,SingleCharWidth*8*1 + Margin,SingleCharHeight*3+Margin,1);
    break;
  case Autoparam_Kulanz:
    drawMsg("  g",myScreen.width()/2-SingleCharWidth*8,SingleCharHeight*9+Margin,1);
    ChangeDisplayedAutoparamKulanz();
    break;
  case Autoparam_MaxWinkel:
    drawMsg("max:",myScreen.width()-(SingleCharWidth*8),Margin + SingleCharHeight,1);
    ChangeDisplayedAutoparamMaxAngle();
    break;
  case Autoparam_Nothing:
    break;
  case Autoparam_Tara:
    drawMsg("TARA",myScreen.width()-(SingleCharWidth*5),Margin + SingleCharHeight*3,1);
    break;  
  }
}

void ChangeDisplayedAutoparamKulanz() {
  myScreen.fillRect(myScreen.width()/2-SingleCharWidth*9,SingleCharHeight*9+Margin, SingleCharWidth*3, SingleCharHeight, 0x0000); 
  sprintf(charBuf, "%3d", kulanz.getValue());
  drawMsg(charBuf,myScreen.width()/2-SingleCharWidth*9,SingleCharHeight*9+Margin,1);
}

void ChangeDisplayedAutoparamMaxAngle() {
  myScreen.fillRect(myScreen.width()-(SingleCharWidth*4),Margin + SingleCharHeight, SingleCharWidth*3 + 1, SingleCharHeight, 0x0000); 
  sprintf(charBuf, "%d", maxAngle.getValue());
  drawMsg(charBuf,myScreen.width()-(SingleCharWidth*4),Margin + SingleCharHeight,1);
}

void ChangeDisplayedAutoparamGlass() {
  //beide felder schwarz übermalen
  myScreen.fillRect(SingleCharWidth*8, SingleCharHeight*3+Margin, SingleCharWidth + 1, SingleCharHeight, 0x0000); 
  myScreen.fillRect(myScreen.width()/2-SingleCharWidth*2*6, SingleCharHeight*7, SingleCharWidth*2*4, SingleCharHeight*2, 0x0000); 

  drawMsg("Glastyp: ",Margin,SingleCharHeight*3+Margin,1);
  sprintf(charBuf, "%d", currentGlass.getValue() + 1);
  drawMsg(charBuf,SingleCharWidth*8*1 + Margin,SingleCharHeight*3+Margin,1);

  sprintf(charBuf, "%4d", glasses[currentGlass.getValue()].getFillweight());
  drawMsg(charBuf,myScreen.width()/2-SingleCharWidth*2*6,SingleCharHeight*7+Margin,2);
}

void setCursor(SetupMenu item, color color)
{
  //myScreen.drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  myScreen.fillTriangle(Margin, Height_Header + (item * 20), Margin, Height_Header + triangleSize + (item * 20), Margin + (triangleSize/2), Height_Header + (triangleSize/2) + (item * 20), color);
}

void changeCurrentMenu (SetupMenu newMenu)
{
  setCursor(currentMenu, 0x0000);
  currentMenu = newMenu;
  setCursor(currentMenu, 0xFFFF);
  Serial.print("NewMenu: " );
  Serial.println(currentMenu);
}

void UpdateProgressbar(int percent) 
{
  if (currentProgress != percent) {
      Serial.print("percent: " );
      Serial.println(percent);

    if (currentProgress < percent) {//kleiner geworden, hinten schwarz machen
      //myScreen.stroke(0,0,0);
      myScreen.fillRect(Margin + 1, SingleCharHeight*11 +1, myScreen.width()-Margin -2, SingleCharHeight*2 -2, 0x0000); 
    }
    currentProgress = percent;
    //myScreen.stroke(0,0,0);
    myScreen.fillRect(Margin + 1, SingleCharHeight*11 +1, (percent * (myScreen.width()-Margin -2)) / 100, SingleCharHeight*2 -2, 0x5225); 
  }
}

void EncoderSelectMenuChanged(int change, int menuItem, int min, int max)
{
  if (change == 0) return;
  if (change > 0) {
    if(menuItem >= max)
    {
      changeCurrentMenu(min);
    } else {
      changeCurrentMenu(menuItem+1);
    }
  } else {
    if(menuItem <= min)
    {
      changeCurrentMenu(max);
    } else {
      changeCurrentMenu(menuItem-1);
    }
  }
}

/*void EncoderChanged(int change) 
{
  if (change > 0) {
    if(currentMenu == Setup_Parameter)
    {
      changeCurrentMenu(Setup_Calibrate);
    } else {
      changeCurrentMenu(currentMenu+1);
    }
  } else {
    if(currentMenu == Setup_Calibrate)
    {
      changeCurrentMenu(Setup_Parameter);
    } else {
      changeCurrentMenu(currentMenu-1);
    }
  }
}*/