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
  myScreen.fillRect(0, Height_Header, myScreen.width(), 60, 0x0000);

  myScreen.text("Waage mit", Margin_Item, (0 * 20) + Height_Header );
  myScreen.text(" beladen", Margin_Item, (2 * 20) + Height_Header );
}

void CalibScreenResult(){
  myScreen.fillRect(0, Height_Header, myScreen.width(), 60, 0x0000);

  myScreen.setCursor(Margin_Item, (0 * 20) + Height_Header);
  myScreen.println("Erfolgreich");
}

void ManuelInitScreen(){
  myScreen.background(0,0,0);  // clear the screen with black
  myScreen.setTextSize(4);
  drawMsg("Man.",Margin,Margin,3);
  
  //myScreen.setTextSize(1);

  drawMsg("min:",myScreen.width()-(SingleCharWidth*8),Margin,1);
  sprintf(charBuf, "%d", minAngle);
  drawMsg(charBuf,myScreen.width()-(SingleCharWidth*4),Margin,1);
  drawMsg("max:",myScreen.width()-(SingleCharWidth*8),Margin + SingleCharHeight,1);
  sprintf(charBuf, "%d", maxAngle);
  drawMsg(charBuf,myScreen.width()-(SingleCharWidth*4),Margin + SingleCharHeight,1);

  drawMsg("Ist:",myScreen.width()-(SingleCharWidth*2*8),Margin + SingleCharHeight*3,2);

  drawMsg("g",SingleCharWidth * 4 * 4 - Margin, myScreen.height()-(SingleCharHeight*8),4);

  drawMsg("Pause",myScreen.width()-SingleCharWidth * 2 * 6, myScreen.height()-(SingleCharHeight*2+Margin),2); 
  
  // increase font size for text in loop()
  myScreen.setTextSize(4); 
}

void AutomaticInitScreen(){
  myScreen.background(0,0,0);  // clear the screen with black

  myScreen.stroke(255,0,255);
  myScreen.text("Welcome Automatik",2,0);

  // increase font size for text in loop()
  myScreen.setTextSize(4); 
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
  Serial.println("NewMenu: " + currentMenu);
}

void EncoderSelectMenuChanged(int change, int menuItem, int min, int max)
{
  if (change == 0) return;
  if (change > 0) {
    if(menuItem == max)
    {
      changeCurrentMenu(min);
    } else {
      changeCurrentMenu(menuItem+1);
    }
  } else {
    if(menuItem == min)
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