

// State Machine for Modes
void StateMachine() {

  switch (currentModus) {
    case Modus_Manuel:
      if (currentModus != lastModus) {
        lastModus = currentModus;
        Serial.println("enter Modus_Manuel");
        currentManuelState = ManuelStateStop;
        ManuelInitScreen();
      }
      ManuelStateMachine();
      break;
    case Modus_Automatic:
      if (currentModus != lastModus) {
        lastModus = currentModus;
        Serial.println("enter Modus_Automatic");
        AutomaticInitScreen();
      }
      AutomaticStateMachine();
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
        scaleEmpty = scale.get_offset();
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
  }
}

void ManuelStateMachine() {
  if (button_stop.pressed()) {
    angle = minAngle;
    currentManuelState = ManuelStateStop;

    myScreen.stroke(0,0,0);
    drawMsg("Live",myScreen.width()-SingleCharWidth * 2 * 6, myScreen.height()-(SingleCharHeight*2+Margin),2); 
    myScreen.stroke(255,0,255);
    drawMsg("Pause",myScreen.width()-SingleCharWidth * 2 * 6, myScreen.height()-(SingleCharHeight*2+Margin),2); 

    myScreen.stroke(0,0,0);
    sprintf(charBuf, "%d", oldDisplayedAngle);
    drawMsg(charBuf,myScreen.width()-(SingleCharWidth*2*4),Margin + SingleCharHeight*3,2);
    sprintf(charBuf, "%4d", weight);
    drawMsg(charBuf, Margin, myScreen.height()-(SingleCharHeight*8), 4);



  } else if (button_start.pressed()) {
    currentManuelState = ManuelStateStart;
    myScreen.stroke(0,0,0);
    drawMsg("Pause",myScreen.width()-SingleCharWidth * 2 * 6, myScreen.height()-(SingleCharHeight*2+Margin),2); 
    myScreen.stroke(255,0,255);
    drawMsg("Live",myScreen.width()-SingleCharWidth * 2 * 6, myScreen.height()-(SingleCharHeight*2+Margin),2); 

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