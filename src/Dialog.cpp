#ifdef REAL_M5STACK
#include "M5Core2.h"
#else /* REAL_M5STACK */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif /* REAL_M5STACK */
#include "wwrapper.h"
#include "sdlcompat.h"

#define map16(r,g,b) ((((r)>>3)<<11) | (((g)>>2)<<5) | ((b)>>3))

void DisplayFunction(const char *buttonA, const char *buttonB, const char *buttonC)
{
#ifdef REAL_M5STACK
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(map16(255,255,255));
  M5.Lcd.fillRect(50, 220, 60, 19, map16(0,0,0));
  M5.Lcd.setCursor(50, 220);
  M5.Lcd.printf(buttonA);
  M5.Lcd.fillRect(145, 220, 60, 19, map16(0,0,0));
  M5.Lcd.setCursor(145, 220);
  M5.Lcd.printf(buttonB);
  M5.Lcd.fillRect(240, 220, 60, 19, map16(0,0,0));
  M5.Lcd.setCursor(240, 220);
  M5.Lcd.println(buttonC);
#else /* REAL_M5STACK */
  printf("[%s] [%s] [%s]\n", buttonA, buttonB, buttonC);
#endif /* REAL_M5STACK */
}

/*
 * VideoDisplayLogo
 */

void VideoDisplayLogo()
{
#ifdef REAL_M5STACK
  M5.Lcd.clearDisplay();
  M5.Lcd.setCursor(25,5);
  M5.Lcd.setTextSize(5);
  M5.Lcd.setTextColor(map16(255,255,255));
  M5.Lcd.printf("M5");
  M5.Lcd.setTextColor(map16(0,255,0));
  M5.Lcd.printf("A");
  M5.Lcd.setTextColor(map16(255,255,0));
  M5.Lcd.printf("p");
  M5.Lcd.setTextColor(map16(220,220,0));
  M5.Lcd.printf("p");
  M5.Lcd.setTextColor(map16(255,0,0));
  M5.Lcd.printf("l");
  M5.Lcd.setTextColor(map16(220,0,0));
  M5.Lcd.printf("e");
  M5.Lcd.setTextColor(map16(0,0,255));
  M5.Lcd.printf("][");
  M5.Lcd.setCursor(5,50);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(map16(255,255,255));
  M5.Lcd.printf("based on LinApple");
  M5.Lcd.setTextColor(0xFFFF);
  M5.Lcd.setTextSize(0);
  SuppressMessages();
#else /* REAL_M5STACK */
  printf("M5Apple][\n");
  printf("base on LinApple\n");
#endif /* REAL_M5STACK */
  DisplayFunction("BTN A", "Menu", "BTN B");
}

void DisplayBackground()
{
#ifdef REAL_M5STACK
  M5.Lcd.clearDisplay();
  M5.Lcd.setCursor(0,0);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(map16(255,255,255));
  M5.Lcd.printf("M5");
  M5.Lcd.setTextColor(map16(0,255,0));
  M5.Lcd.printf("A");
  M5.Lcd.setTextColor(map16(255,255,0));
  M5.Lcd.printf("p");
  M5.Lcd.setTextColor(map16(220,220,0));
  M5.Lcd.printf("p");
  M5.Lcd.setTextColor(map16(255,0,0));
  M5.Lcd.printf("l");
  M5.Lcd.setTextColor(map16(220,0,0));
  M5.Lcd.printf("e");
  M5.Lcd.setTextColor(map16(0,0,255));
  M5.Lcd.printf("][");
  M5.Lcd.println("");
  M5.Lcd.setTextColor(0xFFFF);
  M5.Lcd.setTextSize(0);
#endif /* REAL_M5STACK */
  DisplayFunction("BTN A", "Menu", "BTN B");
}

static void DisplayFrame(int x, int y, int w, int h)
{
#ifdef REAL_M5STACK
  M5.Lcd.drawRect(x, y, w, h, map16(255,255,255));
  M5.Lcd.fillRect(x+1, y+1, w-2, h-2, map16(0,0,0));
#endif /* REAL_M5STACK */
}

/*
 * StringList
 */
class StringList {
private:
  char *buffer;
  int bufsize;
  int next;
  int *linebuf;
  int *valuebuf;
  int linesize;
  int count;
  int iterateIndex;

public:
  StringList();
  ~StringList();

  void Setup(int total, int lines);
  void Add(const char *str, int value);
  void Delete();
  int Length() const { return count; }
  char *operator[](int index);
  void Rewind();
  bool Iterate(char *&str, int &value);
  int value(int index) const { return valuebuf[index]; }
};

StringList::StringList()
{
  buffer = NULL;
  bufsize = 0;
  next = 0;
  linebuf = NULL;
  linesize = 0;
  count = 0;
  valuebuf = NULL;
}

void StringList::Setup(int total, int lines)
{
  if (buffer == NULL) {
    buffer = (char*) VirtualAlloc(NULL, total, MEM_COMMIT, PAGE_READWRITE);
    bufsize = total;
  }
  if (linebuf == NULL) {
    linebuf = (int*) VirtualAlloc(NULL, lines * sizeof (int), MEM_COMMIT, PAGE_READWRITE);
    linesize = lines;
    valuebuf = (int*) VirtualAlloc(NULL, lines * sizeof (int), MEM_COMMIT, PAGE_READWRITE);
  }
  next = 0;
  count = 0;
}

StringList::~StringList()
{
  VirtualFree(valuebuf, 0, MEM_RELEASE);
  VirtualFree(linebuf, 0, MEM_RELEASE);
  VirtualFree(buffer, 0, MEM_RELEASE);
}

void StringList::Add(const char *str, int value)
{
  int len = strlen(str);
  if (next + len + 1 > bufsize) {
    error("StringList::Add buffer overflow");
  }
  if (count + 1 >= linesize) {
    error("StringList::Add lines overflow");
  }
  valuebuf[count] = value;
  linebuf[count++] = next;
  strcpy(&buffer[next], str);
  next += len + 1;
}

void StringList::Delete()
{
  next = 0;
  count = 0;
}

char *StringList::operator[](int index)
{
  if (index < 0 || index >= count) {
    error("StringList[] invalid index");
  }
  return &buffer[linebuf[index]];
}

void StringList::Rewind()
{
  iterateIndex = 0;
}

bool StringList::Iterate(char *&str, int &value)
{
  if (iterateIndex >= count) {
    return false;
  }
  value = valuebuf[iterateIndex];
  str = &buffer[linebuf[iterateIndex++]];
  return true;
}

/*
 * ChooseAnImage
 */

StringList Files;
const int PageLines = 8;
const int LineHeight = 10;
int CurFilesIndex = 0;
int TopFilesIndex = 0;
const int cs = 4;

static void ListupFiles(const char *dir)
{
  Files.Setup(1024, 100);
  Files.Add("<Cancel>", 0);
#ifdef REAL_M5STACK
  SD.begin(cs, SPI, 40000000, "/sd");
  delay(500);
  File root = SD.open("/m5apple2");
  String listTemp = "";
  while (true) {
    File entry = root.openNextFile();
    if (!entry) {
      break;
    }
    if (!entry.isDirectory()) {
      Files.Add(strdup(entry.name()), 1);
    }
    entry.close();
  }
  root.close();
#else /* REAL_M5STACK */
  Files.Add("../images/Choplifter.dsk", 1);
  Files.Add("../images/Master.dsk", 1);
  Files.Add("../images/PacMan.dsk", 1);
  Files.Add("../images/Zaxxon.dsk", 1);
  Files.Add("../images/AE - Back.dsk", 1);
  Files.Add("../images/AE - Front.dsk", 1);
  Files.Add("../images/LeatherGoddessesOfPhobos.dsk", 1);
  Files.Add("../images/Lode Runner.dsk", 1);
  Files.Add("../images/Zenith.dsk", 1);
#endif /* REAL_M5STACK */
}

static void DisplayFiles(int x, int y, int w, int h)
{
  info("DisplayFiles");
  char *name;
  int value;
  Files.Rewind();
#ifdef REAL_M5STACK
  DisplayFrame(x, y, w, h);
  M5.Lcd.setTextSize(1);
  for (int i = 0; Files.Iterate(name, value); i ++) {
    if (i < TopFilesIndex || i >= TopFilesIndex + PageLines) {
      continue;
    }
    M5.Lcd.setCursor(x+2, y+2);
    if (i == CurFilesIndex) {
      M5.Lcd.setTextColor(map16(0,0,0), map16(255,255,0));
    } else {
      M5.Lcd.setTextColor(map16(255,255,255), map16(0,0,0));
    }
    M5.Lcd.printf("%s", name);
    y += LineHeight;
  }
#else /* REAL_M5STACK */
  for (int i = 0; Files.Iterate(name, value); i ++) {
    if (i < TopFilesIndex || i >= TopFilesIndex + PageLines) {
      continue;
    }
    printf("%s%s", i == CurFilesIndex ? "*" : " ", name);
  }
#endif /* REAL_M5STACK */
}

////////////////////////////////////////////////////////////////////////////////////////
bool ChooseAnImage(int x,int y, char *incoming_dir, int slot, char **filename, bool *isdir, int *index_file)
{
  info("ChooseAnImage");
  ListupFiles(incoming_dir);
  CurFilesIndex = *index_file;
  if (CurFilesIndex >= Files.Length()) {
    CurFilesIndex = 0;
  }
  TopFilesIndex = CurFilesIndex - (PageLines / 2);
  if (TopFilesIndex < 0) {
    TopFilesIndex = 0;
  }
  while (true) {
    DisplayFiles(25, 110, 270, 90);
    SDL_Event event;
    event.type = SDL_QUIT;
    while(event.type != SDL_KEYDOWN) {  // wait for key pressed
      SDL_Delay(10);
      SDL_PollEvent(&event);
    }
    switch (event.key.keysym.sym) {
    case SDLK_BUTTONA:
    case SDLK_UP:
      if (CurFilesIndex > 0) {
        CurFilesIndex --;
      }
      if (CurFilesIndex < TopFilesIndex) {
        TopFilesIndex = CurFilesIndex;
      }
      break;
    case SDLK_BUTTONC:
    case SDLK_DOWN:
      if (CurFilesIndex < Files.Length() - 1) {
        CurFilesIndex ++;
      }
      if (CurFilesIndex >= TopFilesIndex + PageLines) {
        TopFilesIndex = CurFilesIndex - PageLines + 1;
      }
      break;
    case SDLK_LEFT:
      CurFilesIndex = TopFilesIndex = 0;
      break;
    case SDLK_RIGHT:
      CurFilesIndex = Files.Length() - 1;
      TopFilesIndex = CurFilesIndex - PageLines + 1;
      break;
    case SDLK_ESCAPE:
      return false;
    case SDLK_BUTTONB:
    case SDLK_RETURN:
      if (!strcmp(Files[CurFilesIndex], "<Cancel>")) {
        return false;
      }
      *filename = Files[CurFilesIndex];
      *isdir = false;
      *index_file = CurFilesIndex;
      return true;
    }
  }
}

#define m5stack
#include "Common.h"
#include "Structs.h"
#include "Disk.h"
#include "Joystick.h"

extern void DoRun();
extern void DoCalibrateJoystick(int sx, int sy);

/*
 * Main Menu
 *   <Cancel>
 *   Reset
 *   Insert Disk #1: title
 *   Insert Disk #2: title
 *   Select Apple Type: AppleII, AppleII+, AppleIIe
 *   Select Joystick: None, Analog
 */

StringList MainMenu;
int CurMainMenuIndex = 0;

enum {
  MM_Cancel, MM_Reset, MM_Disk1, MM_Disk2, MM_AppleType, MM_JoystickType, MM_CalibrateJoystick
};

static void SetupMainMenu()
{
  const char *p;

  MainMenu.Setup(1024, 100);
  MainMenu.Add("<Cancel>", MM_Cancel);
  MainMenu.Add("Reset", MM_Reset);
  MainMenu.Add("Insert Disk #1", MM_Disk1);
  MainMenu.Add("Insert Disk #2", MM_Disk2);
  switch (g_Apple2Type) {
  case A2TYPE_APPLE2:
    p = "Select Apple Type: AppleII";
    break;
  case A2TYPE_APPLE2PLUS:
    p = "Select Apple Type: AppleII+";
    break;
  default:
    p = "Select Apple Type: Apple//e";
    break;
  }
  MainMenu.Add(p, MM_AppleType);
  switch (g_Joystick) {
  case Joystick_Analog:
    p = "Select Joystick: Analog";
    break;
  case Joystick_Acc:
    p = "Select Joystick: Accelerometer";
    break;
  case Joystick_I2C:
    p = "Select Joystick: I2C";
    break;
  default:
    p = "Select Joystick: None";
    break;
  }
  MainMenu.Add(p, MM_JoystickType);
  MainMenu.Add("Calibrate Joystick", MM_CalibrateJoystick);
}

static void NextAppleType()
{
  switch (g_Apple2Type) {
  case A2TYPE_APPLE2:
    g_Apple2Type = A2TYPE_APPLE2PLUS;
    break;
  case A2TYPE_APPLE2PLUS:
    g_Apple2Type = A2TYPE_APPLE2EEHANCED;
    break;
  default:
    g_Apple2Type = A2TYPE_APPLE2;
    break;
  }
}

static void NextJoystickType()
{
  switch (g_Joystick) {
  case Joystick_Analog:
    g_Joystick = Joystick_Acc;
    joytype[0] = 1;
    break;
  case Joystick_Acc:
    g_Joystick = Joystick_I2C;
    joytype[0] = 1;
    break;
  case Joystick_I2C:
    g_Joystick = Joystick_None;
    joytype[0] = 0;
    break;
  default:
    g_Joystick = Joystick_Analog;
    joytype[0] = 1;
    break;
  }
}

static void DisplayMainMenu(int x, int y, int w, int h)
{
  info("DisplayMainMenu");
  char *name;
  int value;
  MainMenu.Rewind();
#ifdef REAL_M5STACK
  DisplayFrame(x, y, w, h);
  M5.Lcd.setTextSize(1);
  for (int i = 0; MainMenu.Iterate(name, value); i ++) {
    M5.Lcd.setCursor(x+2, y+2);
    if (i == CurMainMenuIndex) {
      M5.Lcd.setTextColor(map16(0,0,0), map16(255,255,0));
    } else {
      M5.Lcd.setTextColor(map16(255,255,255), map16(0,0,0));
    }
    M5.Lcd.printf("%s", name);
    y += LineHeight;
  }
#else /* REAL_M5STACK */
  for (int i = 0; MainMenu.Iterate(name, value); i ++) {
    printf("%s%s", i == CurMainMenuIndex ? "*" : " ", name);
  }
#endif /* REAL_M5STACK */
}

int ChooseMainMenu()
{
  info("ChooseMainMenu");
  CurMainMenuIndex = 0;
  while (true) {
    SetupMainMenu();
    DisplayMainMenu(25, 110, 270, 90);
    DisplayFunction(" Up  ", " OK  ", " Down");
    SDL_Event event;
    event.type = SDL_QUIT;
    while(event.type != SDL_KEYDOWN) {  // wait for key pressed
      SDL_Delay(10);
      SDL_PollEvent(&event);
    }
    switch (event.key.keysym.sym) {
    case SDLK_BUTTONA:
    case SDLK_UP:
      if (CurMainMenuIndex > 0) {
        CurMainMenuIndex --;
      }
      break;
    case SDLK_BUTTONC:
    case SDLK_DOWN:
      if (CurMainMenuIndex < MainMenu.Length() - 1) {
        CurMainMenuIndex ++;
      }
      break;
    case SDLK_LEFT:
      CurMainMenuIndex = 0;
      break;
    case SDLK_RIGHT:
      CurMainMenuIndex = MainMenu.Length() - 1;
      break;
    case SDLK_ESCAPE:
      DisplayFunction("BTN A", "Menu", "BTN B");
      return false;
    case SDLK_BUTTONB:
    case SDLK_RETURN:
      switch (MainMenu.value(CurMainMenuIndex)) {
      case MM_Cancel:
        DisplayFunction("BTN A", "Menu", "BTN B");
        return false;
      case MM_Reset:
        DoRun();
        DisplayFunction("BTN A", "Menu", "BTN B");
        return true;
      case MM_Disk1:
        DiskSelect(0);
        break;
      case MM_Disk2:
        DiskSelect(1);
        break;
      case MM_AppleType:
        NextAppleType();
        break;
      case MM_JoystickType:
        NextJoystickType();
        JoyInitialize();
        break;
      case MM_CalibrateJoystick:
        DisplayFrame(25, 150, 270, 44);
        DoCalibrateJoystick(26, 152);
        break;
      }
    }
  }
}

#ifdef REAL_M5STACK
static uint16_t DriveStatusColor(enum Disk_Status_e status)
{
  uint16_t color = map16(200, 200, 200);
  switch (status) {
	case DISK_STATUS_READ:
    color = map16(0, 0, 255);
    break;
	case DISK_STATUS_WRITE:
    color = map16(255, 0, 0);
    break;
	case DISK_STATUS_PROT:
    color = map16(255, 255, 0);
    break;
  default: /* DISK_STATUS_OFF */
    break;
  }
  return color;
}
#endif /* REAL_M5STACK */

/*
 * DrawStatusArea
 */

void DrawStatusArea (int drawflags)
{
#ifdef REAL_M5STACK
  extern void DiskGetLightStatus (int *pDisk1Status_, int *pDisk2Status_);

  M5.Lcd.setTextSize(0);
  DisplayFrame(285, 0, 30, 10);
  enum Disk_Status_e iDrive1Status = DISK_STATUS_OFF;
  enum Disk_Status_e iDrive2Status = DISK_STATUS_OFF;
  DiskGetLightStatus((int*) &iDrive1Status,(int*) &iDrive2Status);
  uint16_t color;
  M5.Lcd.setCursor(287, 1);
  color = DriveStatusColor(iDrive1Status);
  M5.Lcd.setTextColor(color);
  M5.Lcd.print("#1");
  M5.Lcd.setCursor(301, 1);
  color = DriveStatusColor(iDrive2Status);
  M5.Lcd.setTextColor(color);
  M5.Lcd.print("#2");
  M5.Lcd.setTextColor(map16(255,255,255));
#else /* REAL_M5STACK */
  info("DrawStatusArea");
#endif /* REAL_M5STACK */
}
