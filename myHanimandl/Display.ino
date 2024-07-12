
void SetupInitScreen(){

  myScreen.background(0,0,0);  // clear the screen with black
  myScreen.stroke(255,255,255);
  myScreen.setTextSize(3);

  myScreen.text("SETUP",40,2);

  myScreen.setTextSize(2);

for(int item = Setup_Calibrate; item<=Setup_Parameter; item++) {
  myScreen.text(SetupMenuEntries[item], 30, (item * 20) + 33 );
}
}
