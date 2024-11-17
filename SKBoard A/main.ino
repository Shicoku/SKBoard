#include <Keyboard.h>

#define KEY_SPACE 0x20
#define WIN_LED 20
#define NUM_LED 19

bool WinLock = false;
bool NumLock = false;

int outNum = 9;
int inNum = 10;

int outPin[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8}
int inPin[10] = {9, 10, 11, 12, 13, 14, 15, 16, 17, 18}

char key_0[10,9] = {
  {KEY_ESC, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8},
  {KEY_F9, KEY_F10, KEY_F11, KEY_F12, KEY_KP_7, KEY_KP_8, KEY_K9, "`", "1"},
  {"2","3","4","5","6","7","8","9","0"},
  {"-","=",KEY_BACKSPACE,KEY_KP_4,KEY_KP_5,KEY_KP_6,KEY_TAB,"q","w"},
  {"e","r","t","y","u","i","o","p","["},
  {"]","\\",KEY_KP_1,KEY_KP_2,KEY_KP_3,""/*Caps*/,"a","s","d"},
  {"f","g","h","j","k","l",";","'",KEY_RETURN},
  {KEY_NUM_LOCK,KEY_KP_0,KEY_KP_DOT,KEY_LEFT_SHIFT,"z","x","c","v","b"},
  {"n","m",",",".","/",KEY_RIGHT_SHIFT,KEY_UP_ARout,KEY_LEFT_CTRL,""/*KEY_LEFT_GUI*/},
  {KEY_LEFT_ALT,KEY_SPACE,KEY_RIGHT_ALT,""/*Fn*/,KEY_RIGHT_CTRL,KEY_LEFT_ARout,KEY_DOWN_ARout,KEY_LEFT_ARout,""/*None*/},
}

const int FN_ROW = 3, FN_COL = 9;
const int WIN_ROW = 8, WIN_COL = 8;
const int NUM_ROW = 0, NUM_COL = 7;

bool currentState[outNum][inNum];
bool beforeState[outNum][inNum];

void setup() {
  for(auto &&pin: outPin)
    pinMode(pin, OUTPUT);
  for(auto &&pin: inPin)
    pinMode(pin, INPUT_PULLUP);

  pinMode(WIN_LED, OUTPUT);
  pinMode(NUM_LED, OUTPUT);

  for (i = 0; i < outNum; i++) {
    for (j = 0; j < inNum; j++) {
      currentState[i][j] = HIGH;
      beforeState[i][j] = HIGH;
    }
    digitalWrite(outPin[i], HIGH);
  }

  Keyboard.begin();
}

void loop() {
  for (i = 0; i < outNum; i++) {
    digitalWrite(outPin[i], LOW);

    for (j = 0; j < inNum; j++) {
      currentState[i][j] = digitalRead(inPin[j]);

      if (currentState[i][j] != beforeState[i][j]) {

        if (currentState[FN_ROW][FN_COL] == LOW && currentState[WIN_ROW][WIN_COL] == LOW) {
          WinLock = !WinLock;
          digitalWrite(WIN_LED, WIN_LED ? HIGH : LOW);
        }

        if(currentState[NUM_ROW][NUM_COL] == LOW) {
          NumLock = !NumLock;
          digitalWrite(NUM_LED, NumLock ? HIGH : LOW);
        }
        
        if(currentState[WIN_ROW][WIN_COL] == LOW && WinLock == false) {
          Keyboard.press(KEY_LEFT_GUI);
        } else if(currentState[WIN_ROW][WIN_COL] == LOW && WinLock == true) {
          Keyboard.press('');
        } else if (currentState[i][j] == LOW) {
          Keyboard.press(keyMap[i][j]);
        } else {
          Keyboard.releaseAll();
        }

        beforeState[i][j] = currentState[i][j];
      }
    }
    digitalWrite(outPin[i], HIGH);
  }
}
