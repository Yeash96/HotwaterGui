void pump_button_on() {
  //Complete UI Text
  tft.fillRect(100, 255, 130, 80, RA8875_RED);
  tft.textMode();
  tft.textSetCursor(100, 265);
  tft.textColor(RA8875_BLACK, RA8875_RED);
  tft.textEnlarge(1);
  tft.textWrite("Pump:", 6);
  tft.textSetCursor(100, 295);
  tft.textWrite("ON",3);
    Serial.println("PUMP_BUTTON_OFF");
}
