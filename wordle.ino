#define SD_LENGTH 1637
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv_new.h>
MCUFRIEND_kbv_new tft;
#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000
#include <PetitFS.h>

String solution = "";

const int XP = 8, XM = A2, YP = A3, YM = 9;  //240x320 ID=0x8230
const int TS_LEFT = 929, TS_RT = 102, TS_TOP = 70, TS_BOT = 897;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button q, w, e, r, t, y, u, i, o, p, a, s, d, f, g, h, j, k, l, z, x, c, v, b, n, m, next, enter, del, clr, custom;

int pixel_x, pixel_y;
bool cw = false;
String lastKey;
String typeWord = "";
int guessN = 0;
bool Touch_getXY(void) {
  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT);  //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);  //because TFT control pins
  digitalWrite(XM, HIGH);
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
    pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width());
    pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
  }
  return pressed;
}

#define BLACK 0x0000
#define RED 0xF800
#define GREEN 0x07E0
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define DGREY 0x31A6
#define DRED 0x7840

void alert(String msg, int col) {
  tft.fillRect(0, 200, tft.width(), 30, BLACK);
  showmsgXY(0, 200, 1, NULL, msg, col);
}

void randomWord() {
  FATFS fs;
  alert("Setting word", YELLOW);
  randomSeed(analogRead(A5));
  int ranID = random(1, SD_LENGTH + 1);
  int upto = 0;
  if (pf_mount(&fs) != FR_OK) {
    alert("INSERT SD CARD", RED);
    while (true) {
      ;
    }
  }
  if (pf_open("WORDS.TXT") != FR_OK) {
    alert("OPEN FAIL", RED);
    while (true) {
      ;
    }
  }
  char buffer[8];
  WORD bytesRead;
  while (true) {
    upto++;
    FRESULT fr = pf_read(buffer, sizeof(buffer) - 1, &bytesRead);
    if (fr != FR_OK) {
      alert("READ FAIL", RED);
      while (true) {
        ;
      }
      break;
    }
    if (bytesRead == 0) {
      break;
    }
    if (upto == ranID) {
      solution = String(buffer[0]) + String(buffer[1]) + String(buffer[2]) + String(buffer[3]) + String(buffer[4]);
      break;
    } else {
      buffer[bytesRead] = '\0';
    }
  }
  if (solution == "") {
    alert("WORD FAIL", RED);
    while (true) {
      ;
    }
  }
}
void setup(void) {
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9486;
  tft.begin(ID);
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  randomWord();
  q.initButton(&tft, 15, 260, 15, 15, BLACK, WHITE, BLACK, "Q", 1);
  w.initButton(&tft, 35, 260, 15, 15, BLACK, WHITE, BLACK, "W", 1);
  e.initButton(&tft, 55, 260, 15, 15, BLACK, WHITE, BLACK, "E", 1);
  r.initButton(&tft, 75, 260, 15, 15, BLACK, WHITE, BLACK, "R", 1);
  t.initButton(&tft, 95, 260, 15, 15, BLACK, WHITE, BLACK, "T", 1);
  y.initButton(&tft, 115, 260, 15, 15, BLACK, WHITE, BLACK, "Y", 1);
  u.initButton(&tft, 135, 260, 15, 15, BLACK, WHITE, BLACK, "U", 1);
  i.initButton(&tft, 155, 260, 15, 15, BLACK, WHITE, BLACK, "I", 1);
  o.initButton(&tft, 175, 260, 15, 15, BLACK, WHITE, BLACK, "O", 1);
  p.initButton(&tft, 195, 260, 15, 15, BLACK, WHITE, BLACK, "P", 1);
  clr.initButton(&tft, 220, 260, 25, 15, WHITE, RED, WHITE, "CLR", 1);
  a.initButton(&tft, 15, 280, 15, 15, BLACK, WHITE, BLACK, "A", 1);
  s.initButton(&tft, 35, 280, 15, 15, BLACK, WHITE, BLACK, "S", 1);
  d.initButton(&tft, 55, 280, 15, 15, BLACK, WHITE, BLACK, "D", 1);
  f.initButton(&tft, 75, 280, 15, 15, BLACK, WHITE, BLACK, "F", 1);
  g.initButton(&tft, 95, 280, 15, 15, BLACK, WHITE, BLACK, "G", 1);
  h.initButton(&tft, 115, 280, 15, 15, BLACK, WHITE, BLACK, "H", 1);
  j.initButton(&tft, 135, 280, 15, 15, BLACK, WHITE, BLACK, "J", 1);
  k.initButton(&tft, 155, 280, 15, 15, BLACK, WHITE, BLACK, "K", 1);
  l.initButton(&tft, 175, 280, 15, 15, BLACK, WHITE, BLACK, "L", 1);
  del.initButton(&tft, 210, 280, 45, 15, WHITE, DRED, WHITE, "DELETE", 1);
  z.initButton(&tft, 15, 300, 15, 15, BLACK, WHITE, BLACK, "Z", 1);
  x.initButton(&tft, 35, 300, 15, 15, BLACK, WHITE, BLACK, "X", 1);
  c.initButton(&tft, 55, 300, 15, 15, BLACK, WHITE, BLACK, "C", 1);
  v.initButton(&tft, 75, 300, 15, 15, BLACK, WHITE, BLACK, "V", 1);
  b.initButton(&tft, 95, 300, 15, 15, BLACK, WHITE, BLACK, "B", 1);
  n.initButton(&tft, 115, 300, 15, 15, BLACK, WHITE, BLACK, "N", 1);
  m.initButton(&tft, 135, 300, 15, 15, BLACK, WHITE, BLACK, "M", 1);
  next.initButton(&tft, 165, 300, 35, 15, WHITE, MAGENTA, WHITE, "RPT", 1);
  enter.initButton(&tft, 210, 300, 40, 15, WHITE, MAGENTA, WHITE, "ENTER", 1);
  custom.initButton(&tft, 205, 20, 55, 15, WHITE, MAGENTA, WHITE, "CUSTOM", 1);
  drawKeyboard();
}

void loop(void) {
  bool down = Touch_getXY();
  q.press(down && q.contains(pixel_x, pixel_y));
  w.press(down && w.contains(pixel_x, pixel_y));
  e.press(down && e.contains(pixel_x, pixel_y));
  r.press(down && r.contains(pixel_x, pixel_y));
  t.press(down && t.contains(pixel_x, pixel_y));
  y.press(down && y.contains(pixel_x, pixel_y));
  u.press(down && u.contains(pixel_x, pixel_y));
  i.press(down && i.contains(pixel_x, pixel_y));
  o.press(down && o.contains(pixel_x, pixel_y));
  p.press(down && p.contains(pixel_x, pixel_y));
  a.press(down && a.contains(pixel_x, pixel_y));
  s.press(down && s.contains(pixel_x, pixel_y));
  d.press(down && d.contains(pixel_x, pixel_y));
  f.press(down && f.contains(pixel_x, pixel_y));
  g.press(down && g.contains(pixel_x, pixel_y));
  h.press(down && h.contains(pixel_x, pixel_y));
  j.press(down && j.contains(pixel_x, pixel_y));
  k.press(down && k.contains(pixel_x, pixel_y));
  l.press(down && l.contains(pixel_x, pixel_y));
  z.press(down && z.contains(pixel_x, pixel_y));
  x.press(down && x.contains(pixel_x, pixel_y));
  c.press(down && c.contains(pixel_x, pixel_y));
  v.press(down && v.contains(pixel_x, pixel_y));
  b.press(down && b.contains(pixel_x, pixel_y));
  n.press(down && n.contains(pixel_x, pixel_y));
  m.press(down && m.contains(pixel_x, pixel_y));
  enter.press(down && enter.contains(pixel_x, pixel_y));
  del.press(down && del.contains(pixel_x, pixel_y));
  next.press(down && next.contains(pixel_x, pixel_y));
  clr.press(down && clr.contains(pixel_x, pixel_y));
  custom.press(down && custom.contains(pixel_x, pixel_y));
  if (q.justPressed()) {
    processKeyPress("q");
  }
  if (w.justPressed()) {
    processKeyPress("w");
  }
  if (e.justPressed()) {
    processKeyPress("e");
  }
  if (r.justPressed()) {
    processKeyPress("r");
  }
  if (t.justPressed()) {
    processKeyPress("t");
  }
  if (y.justPressed()) {
    processKeyPress("y");
  }
  if (u.justPressed()) {
    processKeyPress("u");
  }
  if (i.justPressed()) {
    processKeyPress("i");
  }
  if (o.justPressed()) {
    processKeyPress("o");
  }
  if (p.justPressed()) {
    processKeyPress("p");
  }
  if (a.justPressed()) {
    processKeyPress("a");
  }
  if (s.justPressed()) {
    processKeyPress("s");
  }
  if (d.justPressed()) {
    processKeyPress("d");
  }
  if (f.justPressed()) {
    processKeyPress("f");
  }
  if (g.justPressed()) {
    processKeyPress("g");
  }
  if (h.justPressed()) {
    processKeyPress("h");
  }
  if (j.justPressed()) {
    processKeyPress("j");
  }
  if (k.justPressed()) {
    processKeyPress("k");
  }
  if (l.justPressed()) {
    processKeyPress("l");
  }
  if (z.justPressed()) {
    processKeyPress("z");
  }
  if (x.justPressed()) {
    processKeyPress("x");
  }
  if (c.justPressed()) {
    processKeyPress("c");
  }
  if (v.justPressed()) {
    processKeyPress("v");
  }
  if (b.justPressed()) {
    processKeyPress("b");
  }
  if (n.justPressed()) {
    processKeyPress("n");
  }
  if (m.justPressed()) {
    processKeyPress("m");
  }
  if (enter.justPressed()) {
    processKeyPress("en");
  }
  if (del.justPressed()) {
    processKeyPress("de");
  }
  if (next.justPressed()) {
    processKeyPress("ne");
  }
  if (clr.justPressed()) {
    processKeyPress("cl");
  }
  if (custom.justPressed()) {
    processKeyPress("cs");
  }
}

void showmsgXY(int x, int y, int sz, const GFXfont *f, String msg, int col) {
  tft.setFont(f);
  tft.setCursor(x, y);
  tft.setTextColor(col);
  tft.setTextSize(sz);
  tft.print(msg);
  tft.setFont(NULL);
}

void drawKeyboard() {
  tft.fillScreen(BLACK);
  q.drawButton(false);
  w.drawButton(false);
  e.drawButton(false);
  r.drawButton(false);
  t.drawButton(false);
  y.drawButton(false);
  u.drawButton(false);
  i.drawButton(false);
  o.drawButton(false);
  p.drawButton(false);
  a.drawButton(false);
  s.drawButton(false);
  d.drawButton(false);
  f.drawButton(false);
  g.drawButton(false);
  h.drawButton(false);
  j.drawButton(false);
  k.drawButton(false);
  l.drawButton(false);
  z.drawButton(false);
  x.drawButton(false);
  c.drawButton(false);
  v.drawButton(false);
  b.drawButton(false);
  n.drawButton(false);
  m.drawButton(false);
  enter.drawButton(false);
  del.drawButton(false);
  next.drawButton(false);
  clr.drawButton(false);
  custom.drawButton(false);
}
void clearTypeArea(int slot) {
  tft.fillRect(0, 10 + slot * 20, tft.width(), 20, BLACK);
  custom.drawButton(cw);
}
void processKeyPress(String key) {
  if (key != lastKey) {
    lastKey = key;
    if (key == "en") {
      if (typeWord.length() == 5) {
        if (guessN < 5) {
          processCurrentGuess();
          guessN++;
          typeWord = "";
        } else if (guessN == 5) {
          guessN++;
          processCurrentGuess();
        }
      }
    } else if (key == "de") {
      if (typeWord.length() > 0 && guessN != 6) {
        typeWord.remove(typeWord.length() - 1);
        renderTyping();
      }
    } else if (key == "ne") {

    } else if (key == "cl") {
      typeWord = "";
      renderTyping();
    } else if (key == "cs") {
      if (typeWord.length() == 5 && !cw && guessN == 0) {
        cw = true;
        solution = typeWord;
        typeWord = "";
        renderTyping();
      }
    } else {
      if (typeWord.length() < 5 && guessN < 6) {
        typeWord += key;
        renderTyping();
      }
    }
  }
}
void renderTyping() {
  clearTypeArea(guessN);
  typeWord.toUpperCase();
  showmsgXY(0, 10 + guessN * 20, 2, NULL, typeWord, WHITE);
}

void processCurrentGuess() {
  // algorithm sourced from https://github.com/adolfintel/TinyTinyWordle
  int length = solution.length();
  int colors[length];
  for (int i = 0; i < length; i++) {
    if (typeWord[i] == solution[i]) {
      colors[i] = 2;
    } else {
      colors[i] = 0;
    }
  }
  for (int i = 0; i < length; i++) {
    if (colors[i] == 0) {
      for (int j = 0; j < length; j++) {
        if (i == j) continue;
        if (typeWord[i] == solution[j]) {
          int ctr = 0;
          for (int k = 0; k < length; k++) {
            if (solution[k] == solution[j]) ctr++;
            if (typeWord[k] == solution[j] && colors[k] != 0) ctr--;
          }
          if (ctr > 0) colors[i] = 1;
        }
      }
    }
  }
  int gn = guessN;
  if (guessN == 6) {
    gn = 5;
  }
  clearTypeArea(gn);
  for (int i = 0; i < 5; i++) {
    if (colors[i] == 1) {
      showmsgXY(i * 15, 10 + gn * 20, 2, NULL, String(typeWord[i]), YELLOW);
      //changeButtonCol(typeWord[i], YELLOW); not enough space
    } else if (colors[i] == 2) {
      showmsgXY(i * 15, 10 + gn * 20, 2, NULL, String(typeWord[i]), GREEN);
      //changeButtonCol(typeWord[i], GREEN);  not enough space
    } else {
      showmsgXY(i * 15, 10 + gn * 20, 2, NULL, String(typeWord[i]), DGREY);
      //changeButtonCol(typeWord[i], DGREY);  not enough space
    }
  }
  if (colors[0] == 2 && colors[1] == 2 && colors[2] == 2 && colors[3] == 2 && colors[4] == 2) {
    guessN = 6;
    alert("CONGRATS!", GREEN);
  } else if (guessN == 6) {
    alert(solution, RED);
  }
}
void changeButtonCol(char word, int col) {
  if (word == "Q") {
    q.initButton(&tft, 15, 260, 15, 15, WHITE, col, WHITE, "Q", 1);
  } else if (word == "W") {
    w.initButton(&tft, 35, 260, 15, 15, WHITE, col, WHITE, "W", 1);
  } else if (word == "E") {
    e.initButton(&tft, 55, 260, 15, 15, WHITE, col, WHITE, "E", 1);
  } else if (word == "R") {
    r.initButton(&tft, 75, 260, 15, 15, WHITE, col, WHITE, "R", 1);
  } else if (word == "T") {
    t.initButton(&tft, 95, 260, 15, 15, WHITE, col, WHITE, "T", 1);
  } else if (word == "Y") {
    y.initButton(&tft, 115, 260, 15, 15, WHITE, col, WHITE, "Y", 1);
  } else if (word == "U") {
    u.initButton(&tft, 135, 260, 15, 15, WHITE, col, WHITE, "U", 1);
  } else if (word == "I") {
    i.initButton(&tft, 155, 260, 15, 15, WHITE, col, WHITE, "I", 1);
  } else if (word == "O") {
    o.initButton(&tft, 175, 260, 15, 15, WHITE, col, WHITE, "O", 1);
  } else if (word == "P") {
    p.initButton(&tft, 195, 260, 15, 15, WHITE, col, WHITE, "P", 1);
  } else if (word == "A") {
    a.initButton(&tft, 15, 280, 15, 15, WHITE, col, WHITE, "A", 1);
  } else if (word == "S") {
    s.initButton(&tft, 35, 280, 15, 15, WHITE, col, WHITE, "S", 1);
  } else if (word == "D") {
    d.initButton(&tft, 55, 280, 15, 15, WHITE, col, WHITE, "D", 1);
  } else if (word == "F") {
    f.initButton(&tft, 75, 280, 15, 15, WHITE, col, WHITE, "F", 1);
  } else if (word == "G") {
    g.initButton(&tft, 95, 280, 15, 15, WHITE, col, WHITE, "G", 1);
  } else if (word == "H") {
    h.initButton(&tft, 115, 280, 15, 15, WHITE, col, WHITE, "H", 1);
  } else if (word == "J") {
    j.initButton(&tft, 135, 280, 15, 15, WHITE, col, WHITE, "J", 1);
  } else if (word == "K") {
    k.initButton(&tft, 155, 280, 15, 15, WHITE, col, WHITE, "K", 1);
  } else if (word == "L") {
    l.initButton(&tft, 175, 280, 15, 15, WHITE, col, WHITE, "L", 1);
  } else if (word == "Z") {
    z.initButton(&tft, 15, 300, 15, 15, WHITE, col, WHITE, "Z", 1);
  } else if (word == "X") {
    x.initButton(&tft, 35, 300, 15, 15, WHITE, col, WHITE, "X", 1);
  } else if (word == "C") {
    c.initButton(&tft, 55, 300, 15, 15, WHITE, col, WHITE, "C", 1);
  } else if (word == "V") {
    v.initButton(&tft, 75, 300, 15, 15, WHITE, col, WHITE, "V", 1);
  } else if (word == "B") {
    b.initButton(&tft, 95, 300, 15, 15, WHITE, col, WHITE, "B", 1);
  } else if (word == "N") {
    n.initButton(&tft, 115, 300, 15, 15, WHITE, col, WHITE, "N", 1);
  } else if (word == "M") {
    m.initButton(&tft, 135, 300, 15, 15, WHITE, col, WHITE, "M", 1);
  }
}