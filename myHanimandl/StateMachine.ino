enum SetupStates {
  SetupStateMain,
  SetupStateCalibrate1,
  SetupStateCalibrate2,
  SetupStateCalibrateFinal
};

char charBuf[8];
long weight = 0;
int newTaraweight;

SetupStates currentSetupState = SetupStateMain;

// State Machine for Modes
void StateMachine() {

  switch (currentModus) {
    case Modus_Manuel:
      if (currentModus != lastModus) {
        lastModus = currentModus;
        Serial.println("enter Modus_Manuel");
      }
      break;
    case Modus_Automatic:
      if (currentModus != lastModus) {
        lastModus = currentModus;
        Serial.println("enter Modus_Automatic");
      }
      break;
    case Modus_Setup:
      if (currentModus != lastModus) {
        lastModus = currentModus;
        Serial.println("enter Modus_Setup");
        currentSetupState = SetupStateMain;
        SetupInitScreen();
      }
      SetupStateMachine();
      break;
  }
}

void SetupStateMachine() {
  switch (currentSetupState) {
    case SetupStateMain:
      EncoderSelectMenuChanged(ReadEncoder(), currentMenu, Setup_Calibrate, Setup_Parameter);
      if (button_select.pressed()) {
        switch (currentMenu) {
          case Setup_Calibrate:
            CalibScreen1();
            currentSetupState = SetupStateCalibrate1;
            break;
          case Setup_Servo:
            break;
          case Setup_Glas:
            break;
          case Setup_Automatic:
            break;
          case Setup_Parameter:
            break;
        }
      }
      break;
    case SetupStateCalibrate1:
      if (button_select.pressed()) {
        currentSetupState = SetupStateCalibrate2;
        //Waage leer tara
        scale.set_scale();
        scale.tare(10);
        delay(500);
        CalibScreen2();
        sprintf(charBuf, "%d g", taraweight);
        drawMsg(charBuf, Margin_Item, (1 * 20) + Height_Header);
      } else if (button_stop.pressed()) {
        currentSetupState = SetupStateMain;
        SetupInitScreen();
      }
      break;
    case SetupStateCalibrate2:
      newTaraweight = taraweight + ReadEncoder();
      if (newTaraweight != taraweight) {
        myScreen.fillRect(Margin_Item, (1 * 20) + Height_Header, SingleCharWidth * 2 * 6, SingleCharHeight * 2, 0x0000);
        sprintf(charBuf, "%d g", newTaraweight);
        drawMsg(charBuf, Margin_Item, (1 * 20) + Height_Header);
        taraweight = newTaraweight;
      }
      if (button_select.pressed()) {
        currentSetupState = SetupStateCalibrateFinal;
        //Waage voll tara
        scale.set_scale(scale.get_units(10) / taraweight);
        charBuf[0] = '\0';
        CalibScreenResult();
        Serial.println("Scale Set");
      } else if (button_stop.pressed()) {
        currentSetupState = SetupStateMain;
        SetupInitScreen();
        charBuf[0] = '\0';
      }
      break;
    case SetupStateCalibrateFinal:
      weight = ReadScale();
      delay(500);
      myScreen.fillRect(Margin_Item, (1 * 30) + Height_Header, SingleCharWidth * 3 * 6, SingleCharHeight * 3, 0x0000);
      sprintf(charBuf, "%d g", weight);
      drawMsg(charBuf, Margin_Item, (1 * 30) + Height_Header, 3);
      Serial.print("Gewicht: ");
      Serial.println(weight);
      if (button_select.pressed() || button_stop.pressed()) {
        Serial.println("Waage offset " + scale.get_offset());
        currentSetupState = SetupStateMain;
        charBuf[0] = '\0';
        SetupInitScreen();
      }
      break;
  }
}