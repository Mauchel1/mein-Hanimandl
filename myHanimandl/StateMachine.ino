

// State Machine for Modes
void StateMachine() {

  switch (currentModus) {
    case Modus_Manuel:
      if (currentModus != lastModus) {
        lastModus = currentModus;
        Serial.println("enter Modus_Manuel");
        angle = minAngle.getValue();
        currentManuelState = ManuelStateStop;
        ManuelInitScreen();
      }
      ManuelStateMachine();
      break;
    case Modus_Automatic:
      if (currentModus != lastModus) {
        lastModus = currentModus;
        angle = minAngle.getValue();
        Serial.println("enter Modus_Automatic");
        AutomaticInitScreen();
      }
      AutomaticStateMachine();
      break;
    case Modus_Setup:
      if (currentModus != lastModus) {
        lastModus = currentModus;
        Serial.println("enter Modus_Setup");
        angle = minAngle.getValue();
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
            ServoScreenStart();
            currentSetupState = SetupStateServoStart;
            break;
          case Setup_Glas:
            GlassInitScreen();
            currentSetupState = SetupStateGlasInit;
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
        scaleEmpty.setValue(scale.get_offset()); 
        scaleEmpty.saveValueToEEPROM();
        factor.setValue(scale.get_scale());
        factor.saveValueToEEPROM();
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
        Serial.print("Waage offset ");
        Serial.println(scale.get_offset());
        currentSetupState = SetupStateMain;
        charBuf[0] = '\0';
        SetupInitScreen();
      }
      break;
    case SetupStateServoStart:
      EncoderSelectMenuChanged(ReadEncoder(), currentMenu, 0, 1);
      if (button_select.pressed()) 
      {
        ServoScreenChoose();
        currentSetupState = SetupStateServoChoose;
      } else if (button_stop.pressed()) 
      {
        angle = minAngle.getValue();
        currentSetupState = SetupStateMain;
        SetupInitScreen();
      }

      break;
    case SetupStateServoChoose:
      if (button_start.pressed()) 
      {
        ReadEncoder(); //clear Encoder 
        ServoScreenChange();
        currentSetupState = SetupStateServoChange;
      }
       else if (button_stop.pressed()) 
      {
        ServoScreenStart();
        currentSetupState = SetupStateServoStart;
      }
      break;
    case SetupStateServoChange:

      ChangeAngle(ReadEncoder(), true);
      if(angle != oldDisplayedAngle){
        myScreen.fillRect(Margin_Item, (3 * 20) + Height_Header, SingleCharWidth * 2 * 3, SingleCharHeight * 2, 0x0000);
        myScreen.stroke(255,0,255);
        sprintf(charBuf, "%d", angle);
        drawMsg(charBuf, Margin_Item, (3 * 20) + Height_Header, 2);
        oldDisplayedAngle = angle;
        myScreen.stroke(0,0,0);
      }
      if (button_start.pressed()) 
      {
        if (currentMenu == 0){
            minAngle.setValue(angle);
            minAngle.saveValueToEEPROM();
        }
        else {
            maxAngle.setValue(angle);
            maxAngle.saveValueToEEPROM();
        }
        ServoScreenStart();
        currentSetupState = SetupStateServoStart;
      }
      else if (button_stop.pressed()) 
      {
        ServoScreenStart();
        currentSetupState = SetupStateServoStart;
      }
      break;
    case SetupStateGlasInit:
      EncoderSelectMenuChanged(ReadEncoder(), currentMenu, 0, 3);
      if (button_select.pressed() ) {
        currentSetupState = SetupStateGlasSelected;
        currentGlass = currentMenu;
        Serial.println("change to SetupStateGlasSelected");
        GlassSelectedScreen(currentGlass);
        ReadEncoder();
        currentMenu = 1;
      } else if (button_stop.pressed()) {
        currentSetupState = SetupStateMain;
        Serial.println("change to SetupStateMain");
        SetupInitScreen();
      }
      break;
    case SetupStateGlasSelected:
      EncoderSelectMenuChanged(ReadEncoder(), currentMenu, 1, 2);
      if (button_stop.pressed()) {
        currentSetupState = SetupStateGlasInit;
        GlassInitScreen();
        Serial.println("change to SetupStateGlasInit");
      } else if (button_start.pressed()) 
      {
        if (currentMenu == 1) 
        {
          setupHelperValue = glasses[currentGlass].getFillweight();
          oldSetupHelperValue = 0;
          myScreen.stroke(0,0,0);
          sprintf(charBuf, "      %4d", setupHelperValue); 
          drawMsg(charBuf, Margin_Item  , (1 * 20) + Height_Header, 2);
          currentSetupState = SetupStateGlasFillChange;
          myScreen.text("edit", Margin_Item, (4 * 20) + Height_Header ); 
          myScreen.stroke(255,255,255);
          myScreen.text("save", Margin_Item, (4 * 20) + Height_Header ); 
        }
        else if (currentMenu == 2)
        {
          setupHelperValue = glasses[currentGlass].getEmptyweight();
          myScreen.stroke(0,0,0);
          sprintf(charBuf, "      %4d", setupHelperValue); 
          setupHelperValue = 0;
          oldSetupHelperValue = 0;
          drawMsg(charBuf, Margin_Item  , (2 * 20) + Height_Header, 2);
          myScreen.text("edit", Margin_Item, (4 * 20) + Height_Header ); 
          myScreen.stroke(255,255,255);
          myScreen.text("save", Margin_Item, (4 * 20) + Height_Header ); 
          currentSetupState = SetupStateGlasEmptyChange;
        }
      }
      break;
    case SetupStateGlasFillChange:
      setupHelperValue += ReadEncoder();
      if (setupHelperValue != oldSetupHelperValue) {
        myScreen.stroke(0,0,0);
        sprintf(charBuf, "      %4d", oldSetupHelperValue); 
        drawMsg(charBuf, Margin_Item  , (1 * 20) + Height_Header, 2);
        myScreen.stroke(255,0,255);
        sprintf(charBuf, "      %4d", setupHelperValue); 
        drawMsg(charBuf, Margin_Item  , (1 * 20) + Height_Header, 2);
        oldSetupHelperValue = setupHelperValue;
        myScreen.stroke(255,255,255);
      }
      if (button_stop.pressed()) {
        currentSetupState = SetupStateGlasSelected;
        GlassSelectedScreen(currentGlass);
        Serial.println("change to SetupStateGlasSelected");
      } else if (button_start.pressed()) {
        glasses[currentGlass].setFillweight(setupHelperValue);
        Serial.println("SAVE"); //TODO
        //glasses[currentGlass].saveFillweightToEEPROM();
        currentSetupState = SetupStateGlasSelected;
        GlassSelectedScreen(currentGlass);
        Serial.println("change to SetupStateGlasSelected");
      }
      break;
    case SetupStateGlasEmptyChange:
      newWeight = ReadScale();
      delay(100);
      if (button_stop.pressed()) {
        currentSetupState = SetupStateGlasSelected;
        GlassSelectedScreen(currentGlass);
        Serial.println("change to SetupStateGlasSelected");
      }
      if (newWeight != -999) { //unkalibriert
        //altes kalibrieren löschen
        if (oldSetupHelperValue == 1){
          myScreen.stroke(0,0,0);
          myScreen.text("KALIBRIEREN!", Margin_Item, (3 * 20) + Height_Header ); 
          myScreen.stroke(255,255,255);
          oldSetupHelperValue = 0;
        }
        if(button_select.pressed() && abs(newWeight) < 20) { // nur bei kleinem drift tara, sonst neu kalibrieren!
          scale.tare();
          Serial.println("TARA");
        } 
        if (abs(newWeight) > 20 ) {
          if (setupHelperValue == 0){
            myScreen.stroke(255,0,255);
            myScreen.text("LEER > TARA", Margin_Item, (3 * 20) + Height_Header ); 
            myScreen.stroke(255,255,255);
            Serial.println("LEEREN");
            setupHelperValue = 1;
          }
        } else if (setupHelperValue == 1){
          //altes leeren löschen
          myScreen.stroke(0,0,0);
          myScreen.text("LEER > TARA", Margin_Item, (3 * 20) + Height_Header ); 
          myScreen.stroke(255,255,255);
          setupHelperValue = 0;
        }
        if (newWeight != weight) { //neues Gewicht schreiben
          sprintf(charBuf, "      %4d", weight);
          myScreen.stroke(0,0,0);
          drawMsg(charBuf, Margin_Item  , (2 * 20) + Height_Header, 2);
          weight = newWeight;
          sprintf(charBuf, "      %4d", weight);
          myScreen.stroke(255,0,255);
          drawMsg(charBuf, Margin_Item  , (2 * 20) + Height_Header, 2);
          myScreen.stroke(255,255,255);
        }
        if (button_start.pressed()) {
          glasses[currentGlass].setEmptyweight(weight);
          //glasses[currentGlass].saveEmptyweightToEEPROM();
          Serial.println("Saved Emptyweight");
          currentSetupState = SetupStateGlasSelected;
          GlassSelectedScreen(currentGlass);
          Serial.println("change to SetupStateGlasSelected");
        }
      } else {
        myScreen.stroke(255,0,255);
        myScreen.text("KALIBRIEREN!", Margin_Item, (3 * 20) + Height_Header ); 
        myScreen.stroke(255,255,255);
        oldSetupHelperValue = 1;
      }
      break;

  }
}

void ManuelStateMachine() {
  if (button_stop.pressed()) {
    angle = minAngle.getValue();
    currentManuelState = ManuelStateStop;

    myScreen.stroke(0,0,0);
    sprintf(charBuf, "%d", oldDisplayedAngle);
    drawMsg(charBuf,myScreen.width()-(SingleCharWidth*2*4),Margin + SingleCharHeight*3,2);
    sprintf(charBuf, "%4d", weight);
    drawMsg(charBuf, Margin, myScreen.height()-(SingleCharHeight*8), 4);    


    myScreen.stroke(255,0,255);
    drawMsg("PAUSE",SingleCharWidth * 2 * 1, myScreen.height()-(SingleCharHeight*2*3+Margin),2); 

  } else if (button_start.pressed()) {
    currentManuelState = ManuelStateStart;
    myScreen.stroke(0,0,0);
    drawMsg("PAUSE",SingleCharWidth * 2 * 1, myScreen.height()-(SingleCharHeight*2*3+Margin),2); 

  } 
  if (button_select.pressed()) {
    scale.tare(5);
  }

  if (currentManuelState == ManuelStateStart) {

    ChangeAngle(ReadEncoder());
    if(angle != oldDisplayedAngle){
      myScreen.stroke(0,0,0);

      sprintf(charBuf, "%d", oldDisplayedAngle);
      drawMsg(charBuf,myScreen.width()-(SingleCharWidth*2*4),Margin + SingleCharHeight*3,2);

      oldDisplayedAngle = angle;
      myScreen.stroke(17,222,88);

      sprintf(charBuf, "%d", angle);
      drawMsg(charBuf,myScreen.width()-(SingleCharWidth*2*4),Margin + SingleCharHeight*3,2);
    }

    

    newWeight = ReadScale();
    if (newWeight != -999 && newWeight != weight) {
      sprintf(charBuf, "%4d", weight);
      myScreen.stroke(0,0,0);
      drawMsg(charBuf, Margin, myScreen.height()-(SingleCharHeight*8), 4);

      weight = newWeight;
      sprintf(charBuf, "%4d", weight);
      myScreen.stroke(17,88,222);
      drawMsg(charBuf, Margin, myScreen.height()-(SingleCharHeight*8), 4);
    }
  }


}

void AutomaticStateMachine() {
/*
  switch (currentAutomaticStates) {
    case AutomaticStateIdle:
      if (button_start.pressed()) {

        if (Waage kalibriert?){
          currentAutomaticStates = AutomaticStateinProgress;
          startTimer();
        } else {
          Text anzeigen (nicht kalibriert)
        }
      //aUswahlmenü + quicktara
      }
    case AutomaticStateRunning:
      if (button_stop.pressed()) {
          currentAutomaticStates = AutomaticStateIdle;
          ChangeAngle(minAngle);
          UpdateProgressbar(0);
          break;
      }
      if (button_start.pressed()) {
        currentAutomaticStates = AutomaticStateinProgress;
        startTimer();
      }
      readyForNext aktualisieren
      evtl bei leerer Waage einmal tara gegen drift?
      if (autoStart && readyForNext) {
        currentAutomaticStates = AutomaticStateinProgress;
        startTimer();
        readyForNext = false;
      }

    case AutomaticStateinProgress:
      if (button_stop.pressed() || timer abgelaufen) {
          currentAutomaticStates = AutomaticStateIdle;
          ChangeAngle(minAngle);
          UpdateProgressbar(0);
          break;
      }

      if(tara not set) idle und break;

      if (Regelung()) { // fertig abgefüllt
        Serial.println("Glas fertig");
        //TODO signal geben;
        if (autoStart) {
          currentAutomaticStates = AutomaticStateRunning;
          Pausetimer starten?
        } else {
          currentAutomaticStates = AutomaticStateIdle;
        }
        
        ChangeAngle(minAngle);

      } 
      UpdateProgressbar(128.0 * ((float)gewicht / (float)zielgewicht));
     
  }
*/
}