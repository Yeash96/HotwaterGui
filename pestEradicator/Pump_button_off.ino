void pump_button_off() {
  //Complete UI Text
  tft.fillRect(100, 255, 130, 80, RA8875_BLUE);
  tft.textMode();
  tft.textSetCursor(100, 265);
  tft.textColor(RA8875_BLACK, RA8875_BLUE);
  tft.textEnlarge(1);
  tft.textWrite("Pump:", 6);
  tft.textSetCursor(100, 295);
  tft.textWrite("OFF",4);
  Serial.println("PUMP_BUTTON_ON");
}
