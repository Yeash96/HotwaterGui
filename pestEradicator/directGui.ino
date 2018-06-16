
void directGui() {
  //Complete UI Text
  tft.textMode();
  tft.textSetCursor(100, 25);
  tft.textColor(RA8875_BLUE, RA8875_WHITE);
  tft.textEnlarge(1);
  tft.textWrite("Hot Water:", 10);
  tft.textSetCursor(100, 60);
  checkWaterLevel();
  String temp1 = "Level: " + String(waterLevel) + "%";
  int size1 = temp1.length();
  char char_array1[size1 + 1];
  strcpy(char_array1, temp1.c_str());
  tft.textWrite(char_array1, 11);
  tft.textSetCursor(100, 140);
  String temp2 = "Temperature";
  int size2 = temp2.length();
  char char_array2[size2 + 1];
  strcpy(char_array2, temp2.c_str());
  tft.textWrite(char_array2, 12);


  //Temperature arrows
  tft.fillRect(310, 55, 80, 80, RA8875_BLACK);
  tft.fillRect(310, 155, 80, 80, RA8875_BLACK);
  tft.fillTriangle(350, 65, 320, 125, 380, 125, RA8875_YELLOW);
  tft.fillTriangle(350, 225, 320, 165, 380, 165, RA8875_YELLOW);

  
}
