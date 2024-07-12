static int16_t Width_Header = 35;
static int16_t Height_Header = 33;
static int16_t Margin = 2;
static int16_t Margin_Item = 25;
static int16_t triangleSize = 13;
SetupMenu currentMenu;

void SetupInitScreen(){

  currentMenu = Setup_Calibrate;
  myScreen.background(0,0,0);  // clear the screen with black
  myScreen.stroke(255,255,255);
  myScreen.setTextSize(3);

  myScreen.text("SETUP",Width_Header,Margin);

  myScreen.setTextSize(2);

  for(int item = Setup_Calibrate; item<=Setup_Parameter; item++) {
    myScreen.text(SetupMenuEntries[item], Margin_Item, (item * 20) + 33 );
  }
  setCursor(currentMenu, 0xFFFF);
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
  Serial.println(currentMenu);
}

void EncoderChanged(int change) 
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
}