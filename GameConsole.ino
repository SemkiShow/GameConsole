#include <GyverOLED.h>
#include <charMap.h>
#include <icons_7x7.h>
#include <icons_8x8.h>

// Joystick init
#define JXPin A2
#define JYPin A1
#define JSPin A3
int JX = 0; // -1 - left, 0 - none, 1 - right (-2, 2 - hold)
int JY = 0; // -1 - down, 0 - none, 1 - up (-2, 2 - hold)
int JS = 0; // 0 - none, 1 - click, 2 - hold
int JXLast = 0;
int JYLast = 0;
int JSLast = 0;
int JXHold = false;
int JYHold = false;
int JSHold = false;
int JHoldDelay = 500;
long JMillis = 0;

// Public variables
int cursorPosition = 0;
int gamesAmount = 6;
String menu = "Main";

// OLED init
GyverOLED<SSH1106_128x64, OLED_BUFFER> oled;

void FullBridgeRectifier()
{
  oled.clear();
  // oled.print(F("And you shall not be a dummy again! The middle finger is for B-field. The thumb is reserved for a force. The index finger is for current. You put the fingers in this pose. If two fingers match their vectors - you know where the third one goes!"));
  oled.home();
  // The main body
  oled.line(30, 0, 0, 30);
  oled.line(0, 30, 30, 60);
  oled.line(30, 60, 60, 30);
  oled.line(60, 30, 30, 0);
  // Upper left diode
  oled.line(5, 15, 15, 25);
  oled.line(5, 15, 20, 10);
  oled.line(15, 25, 20, 10);
  oled.line(15, 5, 25, 15);
  // Upper right diode
  oled.line(45, 5, 35, 15);
  oled.line(45, 5, 50, 20);
  oled.line(35, 15, 50, 20);
  oled.line(45, 25, 55, 15);
  // Lower left diode
  oled.line(15, 35, 5, 45);
  oled.line(15, 35, 20, 50);
  oled.line(5, 45, 20, 50);
  oled.line(25, 45, 15, 55);
  // Lower right diode
  oled.line(45, 55, 35, 45);
  oled.line(45, 55, 50, 40);
  oled.line(35, 45, 50, 40);
  oled.line(45, 35, 55, 45);
  // The full bridge rectifier
  oled.textMode(BUF_REPLACE);
  oled.setScale(1);
  oled.setCursorXY(65, 10);
  oled.print("FULL");
  oled.setCursorXY(65, 30);
  oled.print("Bridge");
  oled.setCursorXY(65, 50);
  oled.print("Rectifier");
  oled.update();
}

class Settings
{
  public:
    Settings(int test) {}
    int brightness = 128;
    void update()
    {
      menu = "Settings";

      oled.home();
      oled.clear();
      oled.invertText(true);
      oled.println(settings[cursor]);
      oled.invertText(false);
      for (int i = cursor + 1; i < settingsAmount; i++)
      {
        oled.println(settings[i]);
      }

      oled.rect(128/2, 0, 128/2+brightness/4, 64/8);

      oled.update();

      if (JX != 0)
      {
        if (cursor == 0) {brightness += JX; oled.setContrast(brightness); delay(2);}
        // if (cursor == 1) {}
        // if (cursor == 2) {}
        // if (cursor == 3) {}
      }
      if (JYLast > 0)
      {
        cursor--;
      }
      if (JYLast < 0)
      {
        cursor++;
      }
      if (cursor < 0)
      {
        cursor = settingsAmount - 1;
      }
      if (cursor >= settingsAmount)
      {
        cursor = 0;
      }
      if (brightness < 2)
      {
        brightness = 2;
      }
      if (brightness > 253)
      {
        brightness = 253;
      }

      if (JSLast != JS && JSLast > 1)
      {
        menu = "Main";
      }
    }
  private:
    int cursor = 0;
    int settingsAmount = 1;
    String settings[5] = {"Brightness"};
};

Settings settings(0);

void Debug()
{
  menu = "Debug";
  oled.clear();
  oled.autoPrintln(false);
  
  oled.setCursor(128/4*0, 0);
  oled.print("JX");
  oled.setCursor(128/4*1, 0);
  oled.print("JY");
  oled.setCursor(128/4*2, 0);
  oled.print("JS");
  oled.setCursor(128/4*3, 0);
  oled.print("Contrast");

  oled.setCursor(128/4*0, 1);
  oled.print(JX);
  oled.setCursor(128/4*1, 1);
  oled.print(JY);
  oled.setCursor(128/4*2, 1);
  oled.print(JS);
  oled.setCursor(128/4*3, 1);
  oled.print(settings.brightness);

  oled.setCursor(128/4*0, 2);
  oled.print("JXPin");
  oled.setCursor(128/4*1, 2);
  oled.print("JYPin");
  oled.setCursor(128/4*2, 2);
  oled.print("JSPin");
  oled.setCursor(128/4*3, 2);
  // oled.print("Test");

  oled.setCursor(128/4*0, 3);
  oled.print(analogRead(JXPin));
  oled.setCursor(128/4*1, 3);
  oled.print(analogRead(JYPin));
  oled.setCursor(128/4*2, 3);
  oled.print(analogRead(JSPin));
  oled.setCursor(128/4*3, 3);
  // oled.print("Test");

  oled.setCursor(128/4*0, 4);
  oled.print("JXLast");
  oled.setCursor(128/4*1, 4);
  oled.print("JYLast");
  oled.setCursor(128/4*2, 4);
  oled.print("JSLast");
  oled.setCursor(128/4*3, 4);
  // oled.print("Test");

  oled.setCursor(128/4*0, 5);
  oled.print(JXLast);
  oled.setCursor(128/4*1, 5);
  oled.print(JYLast);
  oled.setCursor(128/4*2, 5);
  oled.print(JSLast);
  oled.setCursor(128/4*3, 5);
  // oled.print("Test");
  
  oled.autoPrintln(true);
  oled.update();

  if (JSLast != JS && JSLast > 1)
  {
    menu = "Main";
  }
}

void MainMenu(bool update = false)
{
  menu = "Main";
  if (JYLast == JY && JSLast == JS && !update) {return;}
  if (JS > 0)
  {
    if (cursorPosition == 0) {}
    if (cursorPosition == 1) {}
    if (cursorPosition == 2) {}
    if (cursorPosition == 3) {}
    if (cursorPosition == 4) {settings.update();}
    if (cursorPosition == 5) {Debug();}
  }
  if (JYLast > 0)
  {
    cursorPosition--;
  }
  if (JYLast < 0)
  {
    cursorPosition++;
  }
  if (cursorPosition < 0)
  {
    cursorPosition = gamesAmount - 1;
  }
  if (cursorPosition >= gamesAmount)
  {
    cursorPosition = 0;
  }
  oled.home();
  oled.clear();
  // String games[] = {"Tetris", "Snake", "Race", "Tic Tac Toe", "Flappy Bird", "Battleships", "Chess", "Super Mario Bros.", "Platformer", "Pseudo-3D Shooter", "Quizzes", "Text", "Random numbers generator", "Reaction test", "Clicker", "Chrome Dino"};
  String games[] = {"Tetris", "Snake", "Flappy Bird", "Race", "Settings", "Debug"};
  // Serial.print(sizeof(games)/sizeof(games[0]));
  oled.invertText(true);
  oled.println(games[cursorPosition]);
  oled.invertText(false);
  for (int i = cursorPosition + 1; i < gamesAmount; i++)
  {
    oled.println(games[i]);
  }
  oled.update();
}

void JoystickCheck()
{
  // X check
  if (analogRead(JXPin) > 250 && analogRead(JXPin) < 750)
  {
    JXLast = JX;
    if (JX == 2 || JX == -2)
    {
      JX = 0;
    }
    else
    {
      JX = JXHold;
    }
    JXHold = 0;
  }
  if (analogRead(JXPin) <= 250)
  {
    if (millis() - JMillis >= JHoldDelay && JXHold == -1)
    {
      JXLast = JX;
      JX = -2;
      JMillis = millis();
    }
    JXHold = -1;
  }
  if (analogRead(JXPin) >= 750)
  {
    if (millis() - JMillis >= JHoldDelay && JXHold == 1)
    {
      JXLast = JX;
      JX = 2;
      JMillis = millis();
    }
    JXHold = 1;
  }

  // Y check
  if (analogRead(JYPin) > 250 && analogRead(JYPin) < 750)
  {
    JYLast = JY;
    if (JY == 2 || JY == -2)
    {
      JY = 0;
    }
    else
    {
      JY = JYHold;
    }
    JYHold = 0;
  }
  if (analogRead(JYPin) <= 250)
  {
    if (millis() - JMillis >= JHoldDelay && JYHold == -1)
    {
      JYLast = JY;
      JY = -2;
      JMillis = millis();
    }
    JYHold = -1;
  }
  if (analogRead(JYPin) >= 750)
  {
    if (millis() - JMillis >= JHoldDelay && JYHold == 1)
    {
      JYLast = JY;
      JY = 2;
      JMillis = millis();
    }
    JYHold = 1;
  }

  // Switch check
  if (analogRead(JSPin) > 250)
  {
    JSLast = JS;
    if (JS == 2)
    {
      JS = 0;
    }
    else
    {
      JS = JSHold;
    }
    JSHold = 0;
  }
  if (analogRead(JSPin) <= 250)
  {
    if (millis() - JMillis >= JHoldDelay && JSHold == 1)
    {
      JSLast = JS;
      JS = 2;
      JMillis = millis();
    }
    JSHold = 1;
  }

  // Reset millis
  if (analogRead(JXPin) > 250 && analogRead(JXPin) < 750 && analogRead(JYPin) > 250 && analogRead(JYPin) < 750 && analogRead(JSPin) > 250)
  {
    JMillis = millis();
  }
}

void setup()
{
  Serial.begin(9600);

  // Pin init
  pinMode(JXPin, INPUT_PULLUP);
  pinMode(JYPin, INPUT_PULLUP);
  pinMode(JSPin, INPUT_PULLUP);

  // Joystick init
  JMillis = millis();

  // OLED init
  oled.init();
  oled.clear();
  oled.setContrast(127);
  oled.setScale(1);
  oled.autoPrintln(true);

  // Classes init
  // settings = new Settings();

  // Logo
  FullBridgeRectifier();
  int delta = 8;
  for (int i = 0; i < 128; i+=delta)
  {
    // oled.rect(127 - i, 0, 127 - i + delta, 63);
    // oled.update(127 - i, 0, 127 - i + delta, 63);
    oled.rect(i, 63, i + delta, 63);
    oled.update();
    // Serial.print(i);
    // delay(1000/delta);
  }
  oled.clear();
  oled.update();
  MainMenu(true);
}

void loop() 
{
  JoystickCheck();
  if (menu == "Main") {MainMenu();}
  if (menu == "") {}
  if (menu == "") {}
  if (menu == "") {}
  if (menu == "Settings") {settings.update();}
  if (menu == "Debug") {Debug();}
  // Serial.print(JX);
  // Serial.print(" ");
  // Serial.print(JY);
  // Serial.print(" ");
  // Serial.print(JS);
  // Serial.print("    ");
  // Serial.print(JXHold);
  // Serial.print(" ");
  // Serial.print(JYHold);
  // Serial.print(" ");
  // Serial.print(JSHold);
  // Serial.print("    ");
  // Serial.print(analogRead(JXPin));
  // Serial.print(" ");
  // Serial.print(analogRead(JYPin));
  // Serial.print(" ");
  // Serial.print(analogRead(JSPin));
  // Serial.print("    ");
  // Serial.print(millis() - JMillis);
  // Serial.print("    ");
  // Serial.println();
  // delay(100);
}