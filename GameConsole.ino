#include <EEPROM.h>
#include <GyverOLED.h>
#include <charMap.h>
#include <icons_7x7.h>
#include <icons_8x8.h>
#define MU_PRINT 1
#include <MicroUART.h>

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
unsigned long JMillis = 0;

// Public variables
MicroUART uart;
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

int PrintMenu(int _cursorPosition, String _menu[], int _menuLength)
{
  if (JYLast > 0)
  {
    _cursorPosition--;
  }
  if (JYLast < 0)
  {
    _cursorPosition++;
  }
  if (_cursorPosition < 0)
  {
    _cursorPosition = _menuLength - 1;
  }
  if (_cursorPosition >= _menuLength)
  {
    _cursorPosition = 0;
  }
  oled.clear();
  oled.home();
  oled.invertText(true);
  oled.println(_menu[_cursorPosition]);
  oled.invertText(false);
  for (int i = _cursorPosition + 1; i < _menuLength; i++)
  {
    oled.println(_menu[i]);
  }
  // oled.update();
  return _cursorPosition;
}

class FlappyBird
{
  public:
    FlappyBird(int test) {}
    void update()
    {
      if (_menu = "Main")
      {
        oled.home();
        oled.clear();
        oled.println("Flappy Bird");
        // oled.println("Play");
        // oled.println("Settings");
        // oled.println("Score");
        _cursorPosition = PrintMenu(_cursorPosition, _mainMenu, _mainMenues);
        oled.update();


      }
    }
  private:
    String _menu = "Main";
    int _mainMenues = 4;
    String _mainMenu[4] = {"Play", "Difficulty", "Top Score", "Exit"};
    int _cursorPosition = 0;
};

FlappyBird flappyBird(0);

class Settings
{
  public:
    Settings(int test) {brightness = EEPROM.read(0);}
    int brightness = 128;
    void update()
    {
      // oled.home();
      // oled.clear();
      // oled.invertText(true);
      // oled.println(_settings[_cursorPosition]);
      // oled.invertText(false);
      // for (int i = _cursorPosition + 1; i < _settingsAmount; i++)
      // {
      //   oled.println(_settings[i]);
      // }

      // oled.rect(128/2, 0, 128/2+map(brightness, 2, 253, 0, 255)/4, 64/8);
      // oled.setCursor(128/4*2, 0);
      // oled.invertText(true);
      // oled.print(map(brightness, 2, 253, 0, 255));
      // oled.invertText(false);
      _cursorPosition = PrintMenu(_cursorPosition, _settings, _settingsAmount);
      oled.update();

      if (JX != 0)
      {
        if (_cursorPosition == 0) {brightness += JX; oled.setContrast(brightness); delay(2);}
        // if (_cursorPosition == 1) {}
        // if (_cursorPosition == 2) {}
        // if (_cursorPosition == 3) {}
      }
      // if (JYLast > 0)
      // {
      //   _cursorPosition--;
      // }
      // if (JYLast < 0)
      // {
      //   _cursorPosition++;
      // }
      // if (_cursorPosition < 0)
      // {
      //   _cursorPosition = _settingsAmount - 1;
      // }
      // if (_cursorPosition >= _settingsAmount)
      // {
      //   _cursorPosition = 0;
      // }
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
        EEPROM.put(sizeof(int) * 0, brightness);
        menu = "Main";
      }
    }
  private:
    int _cursorPosition = 0;
    int _settingsAmount = 1;
    String _settings[5] = {"Brightness"};
};

Settings settings(0);

void Debug()
{
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

class MainMenu
{
  public:
    MainMenu(int test) {}
    void update()
    {
      // if (JYLast == JY && JSLast == JS && !update) {return;}
      if (JS > 0)
      {
        if (_cursorPosition == 0) {}
        if (_cursorPosition == 1) {}
        if (_cursorPosition == 2) {menu = "FlappyBird";}
        if (_cursorPosition == 3) {}
        if (_cursorPosition == 4) {menu = "Settings";}
        if (_cursorPosition == 5) {menu = "Debug";}
      }
      // if (JYLast > 0)
      // {
      //   _cursorPosition--;
      // }
      // if (JYLast < 0)
      // {
      //   _cursorPosition++;
      // }
      // if (_cursorPosition < 0)
      // {
      //   _cursorPosition = _gamesAmount - 1;
      // }
      // if (_cursorPosition >= _gamesAmount)
      // {
      //   _cursorPosition = 0;
      // }
      // oled.clear();
      // oled.home();
      // // String _games[] = {"Tetris", "Snake", "Race", "Tic Tac Toe", "Flappy Bird", "Battleships", "Chess", "Super Mario Bros.", "Platformer", "Pseudo-3D Shooter", "Quizzes", "Text", "Random numbers generator", "Reaction test", "Clicker", "Chrome Dino"};
      // // uart.print(sizeof(_games)/sizeof(_games[0]));
      // oled.invertText(true);
      // oled.println(_games[_cursorPosition]);
      // oled.invertText(false);
      // for (int i = _cursorPosition + 1; i < _gamesAmount; i++)
      // {
      //   oled.println(_games[i]);
      // }
      _cursorPosition = PrintMenu(_cursorPosition, _games, _gamesAmount);
      oled.update();
    }
  private:
    int _cursorPosition = 0;
    int _gamesAmount = 6;
    String _games[6] = {"Tetris", "Snake", "Flappy Bird", "Race", "Settings", "Debug"};
};

MainMenu mainMenu(0);

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
  // uart.begin(9600);

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

  // General init
  menu = "Main";

  // Logo
  FullBridgeRectifier();
  int delta = 8;
  for (int i = 0; i < 128; i+=delta)
  {
    // oled.rect(127 - i, 0, 127 - i + delta, 63);
    // oled.update(127 - i, 0, 127 - i + delta, 63);
    oled.rect(i, 63, i + delta, 63);
    oled.update();
    // uart.print(i);
    // delay(1000/delta);
  }
  oled.clear();
  oled.update();
  // MainMenu.update();
}

void loop() 
{
  JoystickCheck();
  if (menu == "Main") {mainMenu.update();}
  // if (menu == "") {}
  if (menu == "FlappyBird") {flappyBird.update();}
  // if (menu == "") {}
  if (menu == "Settings") {settings.update();}
  if (menu == "Debug") {Debug();}
  // uart.print(JX);
  // uart.print(" ");
  // uart.print(JY);
  // uart.print(" ");
  // uart.print(JS);
  // uart.print("    ");
  // uart.print(JXHold);
  // uart.print(" ");
  // uart.print(JYHold);
  // uart.print(" ");
  // uart.print(JSHold);
  // uart.print("    ");
  // uart.print(analogRead(JXPin));
  // uart.print(" ");
  // uart.print(analogRead(JYPin));
  // uart.print(" ");
  // uart.print(analogRead(JSPin));
  // uart.print("    ");
  // uart.print(millis() - JMillis);
  // uart.print(menu);
  // uart.print("    ");
  // uart.println();
  // delay(100);
}
