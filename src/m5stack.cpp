#ifdef REAL_M5STACK
#include <M5Core2.h>

extern int _main(int argc, char * lpCmdLine[]);
extern void CheckI2CKeyboard();

bool bSuppressMessage = false;
bool bDebugMode = false;
#define CARDKB_ADDR 0x5F
const int cs = 4;

void setup()
{
  delay(500);
  M5.begin(true, true, false);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("Initializing ...");
  Wire.begin();
  Wire.setClock(400000UL);  // Set I2C frequency to 400kHz
  pinMode(5, INPUT);
  digitalWrite(5, HIGH);
  // for Analog Joystick
  dacWrite(25, 0);
#ifdef USE_I2CKeyboard
  // check for I2CKeyboard
  CheckI2CKeyboard();
#endif /* USE_I2CKeyboard */
  M5.Lcd.println("Starting m5apple...");
  _main(1,NULL);
}

void loop()
{
}

#define m5stack
#include "wwrapper.h"
#include "sdlcompat.h"
#include "Common.h"

// rrrrrggggggbbbbb
#define map16(r,g,b) ((((r)>>3)<<11) | (((g)>>2)<<5) | ((b)>>3))

void SuppressMessages()
{
  bSuppressMessage = true;
}

void StartDebug()
{
  bSuppressMessage = false;
  bDebugMode = true;
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.clearDisplay();
}

void error(const char *str)
{
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("error: %s\n", str);
  while (true)
    ;
}

void info(const char *str)
{
  static int count = 0;

  if (bSuppressMessage) {
    return;
  }
  M5.Lcd.printf("info: %s\n", str);
  if (count ++ > 10) {
    M5.Lcd.clearDisplay();
    M5.Lcd.setCursor(0, 0);
    count = 0;
  }
  if (bDebugMode) {
    while (true) {
      M5.BtnA.read();
      if (M5.BtnA.wasPressed()) {
        break;
      }
    }
  }
}

int SDL_Init(int p1)
{
  M5.Lcd.printf("SDL_Init: p1=%d\n", p1);
  return 0;
}

const char *SDL_GetError(void)
{
  return "SDL_GetError\n";
}

#include "charset40.h"

SDL_Surface *SDL_LoadBMP(const char *p1)
{
  SDL_Surface *surface;

  if (!strcmp(p1, "charset40.bmp")) {
    surface = new SDL_Surface;
    surface->pixels = (Uint8*) charset40_pixels;
    surface->w = charset40_width;
    surface->h = charset40_height;
    surface->pitch = surface->w;
    surface->format = new SDL_Surface::_format;
    surface->format->BitsPerPixel = 8;
    surface->format->BytesPerPixel = 1;
    surface->format->palette = new SDL_Surface::_format::_palette;
    surface->format->palette->colors = new SDL_Color[256];
    surface->format->palette->ncolors = 256;
  } else {
    M5.Lcd.printf("SDL_LoadBMP: p1=%s\n", p1);
    error("unknown bmp");
  }
  return surface;
}

void SDL_SaveBMP(SDL_Surface *p1, const char *p2)
{
  error("SDL_SaveBMP");
}

void SDL_putenv(const char *p1)
{
  M5.Lcd.printf("SDL_putenv: p1=\"%s\"\n", p1);
}

SDL_Surface *SDL_SetVideoMode(int w, int h, int bpp, int p4)
{
  M5.Lcd.printf("SDL_SetVideoMode: w=%d, h=%d, bpp=%d, p4=%d\n", w, h, bpp, p4);
  SDL_Surface *ret = new SDL_Surface;
  ret->pixels = new Uint8[w * h];
  ret->w = w;
  ret->h = h;
  ret->pitch = ret->w;
  ret->format = new SDL_Surface::_format;
  ret->format->BitsPerPixel = bpp;
  ret->format->BytesPerPixel = 1;
  ret->format->palette = new SDL_Surface::_format::_palette;
  ret->format->palette->colors = new SDL_Color[256];
  ret->format->palette->ncolors = 256;
  return ret;
}

void SDL_EnableKeyRepeat(int p1, int p2)
{
  M5.Lcd.printf("SDL_EnableKeyRepeat: p1=%d, p2=%d\n", p1, p2);
}

int SDL_OpenAudio(SDL_AudioSpec *p1,SDL_AudioSpec *p2)
{
  info("SDL_OpenAudio");
  return -1;
}

SDL_Surface *SDL_CreateRGBSurface(int p1, int w, int h, int bpp, int p5, int p6, int p7, int p8)
{
  if (p1 != SDL_SWSURFACE) {
    error("SDL_CreateRGBSurface");
  }
  SDL_Surface *ret = new SDL_Surface;
  ret->pixels = (Uint8*) VirtualAlloc(0, w * h * sizeof (Uint8), 0, 0);
  ret->w = w;
  ret->h = h;
  ret->pitch = ret->w;
  ret->format = new SDL_Surface::_format;
  ret->format->BitsPerPixel = bpp;
  ret->format->BytesPerPixel = 1;
  ret->format->palette = new SDL_Surface::_format::_palette;
  ret->format->palette->colors = new SDL_Color[256];
  ret->format->palette->ncolors = 256;
  return ret;
}

void SDL_FreeSurface(SDL_Surface *surface)
{
  info("SDL_FreeSurface");
  if (surface->format->palette->colors) {
    delete[] surface->format->palette->colors;
  }
  if (surface->format->palette) {
    delete surface->format->palette;
  }
  if (surface->format) {
    delete surface->format;
  }
  if (surface->pixels) {
    VirtualFree(surface->pixels, 0, MEM_RELEASE);
  }
}

int SDL_SetColors(SDL_Surface *p1, SDL_Color *colors, int firstcolor, int p4)
{
  if (p1->format == 0) {
    error("SDL_SetColors format is NULL");
  } 
  if (p1->format->palette == 0) {
    error("SDL_SetColors palette is NULL");
  }
  if (firstcolor != 0) {
    error("SDL_SetColors firstcolors is not 0");
  }
  if (colors == 0) {
    error("SDL_SetColors colors is NULL");
  }
  memcpy(p1->format->palette->colors, colors, p1->format->palette->ncolors);
  return 0;
}

Uint32 SDL_MapRGB(void *p1, int r, int g, int b)
{
  return map16(r, g, b);
}

static int indexOf(SDL_Surface *p, int x, int y)
{
  if (x < 0 || x >= p->w) {
    M5.Lcd.printf("x=%d, w=%d\n", x, p->w);
    error("indexOf: invalid x");
  }
  if (y < 0 || y >= p->h) {
    M5.Lcd.printf("y=%d, h=%d\n", x, p->h);
    error("indexOf: invalid y");
  }
  return x + (p->w * y);
}

void SDL_FillRect(SDL_Surface *p1, SDL_Rect *rect, int color)
{
  for (int y = 0; y < rect->h; y ++) {
    int index = indexOf(p1, rect->x, rect->y + y);
    for (int x = 0; x < rect->w; x ++) {
      p1->pixels[index++] = color;
    }
  }
}

int SDL_SoftOr(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect)
{
  if (srcrect->w != dstrect->w || srcrect->h != dstrect->h) {
    error("invalid w or h");
  }
  for (int y = 0; y < srcrect->h; y ++) {
    int srcIndex = indexOf(src, srcrect->x, srcrect->y + y);
    int dstIndex = indexOf(dst, dstrect->x, dstrect->y + y);
    for (int x = 0; x < srcrect->w; x ++) {
      Uint8 c = src->pixels[srcIndex++];
      if (c > 0) {
        dst->pixels[dstIndex++] = c;
      }
    }
  }
}

void SDL_SoftStretch(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect)
{
  if (srcrect->w == dstrect->w && srcrect->h == dstrect->h) {
    for (int y = 0; y < srcrect->h; y ++) {
      int srcIndex = indexOf(src, srcrect->x, srcrect->y + y);
      int dstIndex = indexOf(dst, dstrect->x, dstrect->y + y);
      memcpy(&dst->pixels[dstIndex], &src->pixels[srcIndex], srcrect->w);
    }
  } else if (srcrect->w == dstrect->w * 2 && srcrect->h == dstrect->h) {
    for (int y = 0; y < srcrect->h; y ++) {
      int srcIndex = indexOf(src, srcrect->x, srcrect->y + y);
      int dstIndex = indexOf(dst, dstrect->x, dstrect->y + y);
      for (int x = 0; x < srcrect->w; x += 2) {
        dst->pixels[dstIndex] = src->pixels[srcIndex];
        srcIndex += 2;
        dstIndex ++;
      }
    }
  } else if (srcrect->w == dstrect->w * 2 && srcrect->h == dstrect->h * 2) {
    for (int y = 0; y < srcrect->h; y += 2) {
      int srcIndex = indexOf(src, srcrect->x, srcrect->y + y);
      int dstIndex = indexOf(dst, dstrect->x, dstrect->y + (y / 2));
      for (int x = 0; x < srcrect->w; x += 2) {
        dst->pixels[dstIndex] = src->pixels[srcIndex];
        srcIndex += 2;
        dstIndex ++;
      }
    }
  } else {
    error("invalid w or h");
  }
}

int SDL_MUSTLOCK(void *p1)
{
  //info("SDL_MUSTLOCK");
  return 0;
}

void SDL_ShowCursor(int p1)
{
  info("SDL_ShowCursor");
  return;
}

void SDL_WM_GrabInput(int p1)
{
  info("SDL_WM_GrabInput");
  return;
}

#define MAX_EVENTS 100
SDL_Event events[MAX_EVENTS];
int event_push = 0;
int event_pop = 0;
int keyStateInit = false;
Uint8 keyState[256];

static void clearKeyState()
{
  info("clearKeyState");
  ZeroMemory(keyState, sizeof keyState);
}

Uint8 *SDL_GetKeyState(const void *p1)
{
  info("SDL_GetKeyState");
  if (!keyStateInit) {
    keyStateInit = true;
    clearKeyState();
  }
  return keyState;
}

void SDL_PushEvent(SDL_Event *p1)
{
  events[event_push] = *p1;
  event_push ++;
  event_push %= MAX_EVENTS;
  if (p1->type == SDL_KEYDOWN) {
    int c = p1->key.keysym.sym;
    if (keyState[c]) {
      keyState[c] = 0;
    } else {
      clearKeyState();
      keyState[c] = 1;
    }
  }
  if (!bSuppressMessage) {
    M5.Lcd.printf("SDL_PushEvent: %d %c\n", event_push, p1->key.keysym.sym);
  }
}

extern void JoySetButton (eBUTTON number, eBUTTONSTATE down);

void PushButtonEvents()
{
  M5.update();
  JoySetButton(BUTTON0, (eBUTTONSTATE) M5.BtnA.isPressed());
  JoySetButton(BUTTON1, (eBUTTONSTATE) M5.BtnC.isPressed());
  if (M5.BtnA.wasPressed()) {
    SDL_Event e;
    e.type = SDL_KEYDOWN;
    e.key.keysym.sym = SDLK_BUTTONA;
    SDL_PushEvent(&e);
  }
  if (M5.BtnA.wasReleased()) {

  }
  if (M5.BtnB.wasPressed()) {
    SDL_Event e;
    e.type = SDL_KEYDOWN;
    e.key.keysym.sym = SDLK_BUTTONB;
    SDL_PushEvent(&e);
  }
  if (M5.BtnB.wasReleased()) {

  }
  if (M5.BtnC.wasPressed()) {
    SDL_Event e;
    e.type = SDL_KEYDOWN;
    e.key.keysym.sym = SDLK_BUTTONC;
    SDL_PushEvent(&e);
  }
  if (M5.BtnC.wasReleased()) {

  }
}

#ifdef USE_I2CKeyboard
static bool I2CKeyboardAvailable = false;

void CheckI2CKeyboard()
{
  Wire.beginTransmission(CARDKB_ADDR);
  I2CKeyboardAvailable = Wire.endTransmission() == 0;
}

void PushI2CKeyboardEvents()
{
  if (!I2CKeyboardAvailable) {
    return;
  }
  Wire.requestFrom(CARDKB_ADDR, 1);
  while (Wire.available()) {
    int c = Wire.read();
    if (c != 0) {
      SDL_Event e;
      if (c == 0xA8) {
        c = 3;
      }
      e.type = SDL_KEYDOWN;
      e.key.keysym.sym = c;
      SDL_PushEvent(&e);
      e.type = SDL_KEYUP;
      SDL_PushEvent(&e);
      e.type = SDL_DUMMY;
      SDL_PushEvent(&e);
    }
  }
}
#endif /* USE_I2CKeyboard */

int SDL_PollEvent(SDL_Event *p1)
{
  //info("SDL_PollEvent");
  if (event_pop == event_push) {
    PushButtonEvents();
    PushI2CKeyboardEvents();
  }
  if (event_pop == event_push) {
    return 0;
  }
  *p1 = events[event_pop];
  event_pop ++;
  event_pop %= MAX_EVENTS;
  if (!bSuppressMessage) {
    M5.Lcd.printf("SDL_PollEvent: %d type=%d %c\n", event_pop, p1->type, p1->key.keysym.sym);
  }
  return p1->type == SDL_DUMMY ? 0 : 1;
}

#define BUFF_SIZE 64
#define SCANLINE_SIZE 2048

static uint16_t scanline[SCANLINE_SIZE];

void PushScanline(int offset, int size)
{
  uint16_t nb = size / BUFF_SIZE;
  uint16_t *ptr = &scanline[offset];
  for (int i = 0; i < nb; i++) {
          M5.Lcd.pushColors(ptr, BUFF_SIZE);
          ptr += BUFF_SIZE;
  }
  uint16_t np = size % BUFF_SIZE;
  if (np) {
          M5.Lcd.pushColors(ptr, np);
  }
}

extern SDL_Color  framebufferinfo[256];
const int screenTopX = 24;
const int screenTopY = 20;

void SDL_Flip(SDL_Surface *p1)
{
  info("SDL_Flip");
  if (bDebugMode) {
    return;
  }
  //SuppressMessages();
  M5.Lcd.setWindow(screenTopX, screenTopY, screenTopX + p1->w - 1, screenTopY + p1->h - 1);
  for (int y = 0; y < p1->h; y ++) {
    Uint8 *src = p1->pixels + p1->pitch * y;
    uint16_t *dst = scanline;
    for (int x = 0; x < p1->w; x ++) {
      int v = *src++;
      SDL_Color c = framebufferinfo[v];
      *dst++ = map16(c.r, c.g, c.b);
    }
    PushScanline(0, p1->w);
  }
}

void SDL_PauseAudio(int p1)
{
  M5.Lcd.printf("SDL_PauseAudio: p1=%d\n", p1);
}

void SDL_BlitSurface(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect)
{
  int sx, sy, dx, dy;

  if (srcrect != NULL) {
    sx = srcrect->x;
    sy = srcrect->y;
  } else {
    sx = sy = 0;
  }
  if (dstrect != NULL) {
    dx = dstrect->x;
    dy = dstrect->y;
  } else {
    dx = dy = 0;
  }
  if (src->w == dst->w && src->h == dst->h) {
    info("SDL_BlitSurface fullcopy");
    memcpy(dst->pixels, src->pixels, src->w * src->h);
  } else {
    info("SDL_BlitSurface partialcopy");
    if (sx + src->w >= dx + dst->w) {
      error("SDL_BlitSurface x exceed");
    }
    if (sy + src->h >= dy + dst->h) {
      error("SDL_BlitSurface y exceed");
    }
    for (int y = 0; y < src->h; y ++) {
      int srcOffset = indexOf(src, sx, sy + y);
      int dstOffset = indexOf(dst, dx, dy + y);
      memcpy(&dst->pixels[dstOffset], &src->pixels[srcOffset], src->w);
    }
  }
}

void SDL_Delay(int p1)
{
  delay(p1);
}

int SDL_GetTicks(void)
{
  return millis();
}

void SDL_UpdateRect(SDL_Surface *surface, int x0, int y0, int w, int h)
{
  info("SDL_UpdateRect");
  if (bDebugMode) {
    return;
  }
  int x1 = x0 + w - 1;
  int y1 = y0 + h - 1;
  M5.Lcd.setWindow(x0, y0, x1, y1);
  for (int y = y0; y <= y1; y ++) {
    Uint8 *src = surface->pixels + surface->pitch * y;
    uint16_t *dst = scanline;
    for (int x = x0; x <= x1; x ++) {
      int v = *src++;
      SDL_Color c = framebufferinfo[v];
      *dst++ = map16(c.r, c.g, c.b);
    }
    PushScanline(0, w);
  }
}

#include "m5joystick.h"

SDL_Joystick joystick1;
enum M5Joystick g_Joystick = Joystick_None;

#ifdef USE_AnalogJoystick
const int Joystick_pinA = 26;
const int Joystick_pinB = 36;
AnalogJoystick analogJoystick(3);
#endif /* USE_AnalogJoystick */
#ifdef USE_AccJoystick
AccJoystick accJoystick(3);
#endif /* USE_AccJoystick */
#ifdef USE_I2CJoystick
I2CJoystick i2cJoystick(1);
#endif /* USE_I2CJoystick */

char vhKeys[] = "IJKL";

#define VHKEYS_UP ((int) vhKeys[0])
#define VHKEYS_LEFT ((int) vhKeys[1])
#define VHKEYS_DOWN ((int) vhKeys[2])
#define VHKEYS_RIGHT ((int) vhKeys[3])

int SDL_NumJoysticks(void)
{
  info("SDL_NumJoysticks");
  return g_Joystick == Joystick_None ? 0 : 1;
}

SDL_Joystick *SDL_JoystickOpen(int p1)
{
  info("SDL_JoystickOpen");
#ifdef USE_AnalogJoystick
  if (g_Joystick == Joystick_Analog) {
    pinMode(Joystick_pinA, INPUT);
    pinMode(Joystick_pinB, INPUT);
    analogJoystick.Setup(Joystick_pinA, Joystick_pinB);
  }
#endif /* USE_AnalogJoystick */
#ifdef USE_AccJoystick
  if (g_Joystick == Joystick_Acc) {
    accJoystick.Setup(Wire1);
  }
#endif /* USE_AccJoystick */
#ifdef USE_I2CJoystick
  if (g_Joystick == Joystick_I2C) {
    i2cJoystick.Setup(Wire);
  }
#endif /* USE_I2CJoystick */
  return &joystick1;
}

void SDL_JoystickClose(SDL_Joystick *p1)
{
  info("SDL_JoystickClose");
}

void SDL_JoystickUpdate(void)
{
  //info("SDL_JoystickUpdate");
}

int SDL_JoystickGetButton(SDL_Joystick *p1, int button)
{
  int v = 0;
  if (button == 0) {
    v = M5.BtnA.isPressed();
  } else if (button == 1) {
    v = M5.BtnC.isPressed();
  }
  return 0;
}

static void updateKeyStateFromJoystick(int axis, int v)
{
  if (axis == 0) {
    keyState[VHKEYS_LEFT] = 0;
    keyState[VHKEYS_RIGHT] = 0;
    if (v < -16384) {
      keyState[VHKEYS_LEFT] = 1;
    } else if (v > 16383) {
      keyState[VHKEYS_RIGHT] = 1;
    }
  } else {
    keyState[VHKEYS_UP] = 0;
    keyState[VHKEYS_DOWN] = 0;
    if (v < -16384) {
      keyState[VHKEYS_UP] = 1;
    } else if (v > 16383) {
      keyState[VHKEYS_DOWN] = 1;
    }
  }
}

int SDL_JoystickGetAxis(SDL_Joystick *joystick, int axis)
{
  int v = 0;
#ifdef USE_AnalogJoystick
  if (g_Joystick == Joystick_Analog) {
    analogJoystick.Update();
    v = axis == 0 ? analogJoystick.GetX() : analogJoystick.GetY();
  }
#endif /* USE_AnalogJoystick */
#ifdef USE_AccJoystick
  if (g_Joystick == Joystick_Acc) {
    accJoystick.Update();
    v = axis == 0 ? accJoystick.GetX() : accJoystick.GetY();
  }
#endif /* USE_AccJoystick */
#ifdef USE_I2CJoystick
  if (g_Joystick == Joystick_I2C) {
    i2cJoystick.Update();
    v = axis == 0 ? i2cJoystick.GetX() : i2cJoystick.GetY();
  }
#endif /* USE_I2CJoystick */
  updateKeyStateFromJoystick(axis, v);
  return v;
}

void DoCalibrateJoystick(int sx, int sy)
{
  M5.Lcd.setCursor(sx, sy);
#ifdef USE_AnalogJoystick
  if (g_Joystick == Joystick_Analog) {
    M5.Lcd.println("Calibrating Analog Joystick...");
    analogJoystick.CalibrateCenter(500);
    M5.Lcd.setCursor(sx, sy += 10);
    M5.Lcd.println("Move stick vertically and holizontally.");
    analogJoystick.CalibrateMinMax(5000);
  }
#endif /* USE_AnalogJoystick */
#ifdef USE_AccJoystick
  if (g_Joystick == Joystick_Acc) {
    M5.Lcd.println("Calibrating Accelerometer...");
    accJoystick.CalibrateCenter(500);
  }
#endif /* USE_AccJoystick */
#ifdef USE_I2CJoystick
  if (g_Joystick == Joystick_I2C) {
    M5.Lcd.println("Calibrating I2C Joystick...");
    i2cJoystick.CalibrateCenter(500);
    M5.Lcd.setCursor(sx, sy += 10);
    M5.Lcd.println("Move stick vertically and holizontally.");
    i2cJoystick.CalibrateMinMax(5000);
  }
#endif /* USE_I2CJoystick */
  M5.Lcd.setCursor(sx, sy+10);
  M5.Lcd.println("Done.");
  delay(1000);
}

/* --- */

void SDL_WM_SetCaption(const char *p1, const char *p2)
{
  M5.Lcd.setCursor(0,0);
  M5.Lcd.printf("SDL_WM_SetCaption: p1=%s, p2=%s\n", p1, p2);
}

int SDL_LockSurface(void *p1)
{
  error("SDL_LockSurface");
}

void SDL_UnlockSurface(void *p1)
{
  error("SDL_UnlockSurface");
}

SDL_Surface *SDL_DisplayFormat(SDL_Surface *p1)
{
  info("SDL_DisplayFormat");
  return p1;
}

void SDL_SetError(const char *p1)
{
  M5.Lcd.printf("p1=%s\n", p1);
  error("SDL_SetError");
}

void SDL_SetColorKey(SDL_Surface *surface, int flag, Uint32 key)
{
  info("SDL_SetColorKey");
}

void SDL_WM_ToggleFullScreen(SDL_Surface *p1)
{
  error("SDL_WM_ToggleFullScreen");
}

void SDL_Quit(void)
{
  error("SDL_Quit");
}

void SDL_WM_SetIcon(SDL_Surface *p1, void *p2)
{
  error("SDL_WM_SetIcon");
}

void SDL_CloseAudio(void)
{
  error("SDL_CloseAudio");
}

void SDL_LockAudio(void)
{
  error("SDL_LockAudio");
}

void SDL_UnlockAudio(void)
{
  error("SDL_UnlockAudio");
}

void SDL_GetClipRect(SDL_Surface *surface, SDL_Rect *rect)
{
  //info("SDL_GetClipRect");
  rect->x = 0;
  rect->y = 0;
  rect->w = surface->w;
  rect->h = surface->h;
}

double DSUploadBuffer(short* buffer, unsigned len)
{
  info("DSUploadBuffer");
}

bool g_bDSAvailable = false;

#endif /* REAL_M5STACK */
