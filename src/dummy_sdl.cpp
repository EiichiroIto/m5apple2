#ifndef REAL_M5STACK
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define m5stack
#include "wwrapper.h"
#include "sdlcompat.h"
#include "Common.h"

extern int _main(int argc, char * lpCmdLine[]);

int main(int argc, char * lpCmdLine[])
{
  _main(argc, lpCmdLine);
}

#define r16(col) ((col >> 10) & 0x1f)
#define g16(col) ((col >> 5)  & 0x1f)
#define b16(col) (col & 0x1f)

// rrrrrggggggbbbbb
#define map16(r,g,b) ((((r)>>3)<<11) | (((g)>>2)<<5) | ((b)>>3))

void error(const char *str)
{
  printf("error: %s\n", str);
  exit(0);
}

void info(const char *str)
{
  printf("info: %s\n", str);
}

int SDL_Init(int p1)
{
  printf("p1=%d\n", p1);
  info("SDL_Init");
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

  printf("SDL_LoadBMP: p1=%s\n", p1);
  if (!strcmp(p1, "icon.bmp")) {
    error("icon.bmp not supported");
  } else if (!strcmp(p1, "charset40.bmp")) {
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
    printf("charset40.bmp = %x\n", surface);
  } else {
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
  printf("SDL_putenv: p1=\"%s\"\n", p1);
}

SDL_Surface *SDL_SetVideoMode(int w, int h, int bpp, int p4)
{
  printf("SDL_SetVideoMode: w=%d, h=%d, bpp=%d, p4=%d\n", w, h, bpp, p4);
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
  printf("SDL_SetVideoMode=%x\n", ret);
  return ret;
}

void SDL_EnableKeyRepeat(int p1, int p2)
{
  printf("SDL_EnableKeyRepeat: p1=%d, p2=%d\n", p1, p2);
}

int SDL_OpenAudio(SDL_AudioSpec *p1,SDL_AudioSpec *p2)
{
  info("SDL_OpenAudio");
  return -1;
}

SDL_Surface *SDL_CreateRGBSurface(int p1, int w, int h, int bpp, int p5, int p6, int p7, int p8)
{
  printf("SDL_CreateRGBSurface: p1=%d, w=%d, h=%d, bpp=%d, p5=%d, p6=%d, p7=%d, p8=%d\n", p1, w, h, bpp, p5, p6, p7, p8);
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
  printf("SDL_CreateRGBSurface=%x\n");
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
  printf("SDL_SetColors: p1=%x, colors=%x, p3=%d, p4=%d\n", p1, colors, firstcolor, p4);
  printf("p1->format=%x\n", p1->format);
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
  printf("SDL_MapRGB: p1=%x, p2=%d, p3=%d, p4=%d\n", p1, r, g, b);
  return map16(r, g, b);
}

static int indexOf(SDL_Surface *p, int x, int y)
{
  if (x < 0 || x >= p->w) {
    printf("x=%d, w=%d\n", x, p->w);
    error("indexOf: invalid x");
  }
  if (y < 0 || y >= p->h) {
    printf("y=%d, h=%d\n", x, p->h);
    error("indexOf: invalid y");
  }
  return x + (p->w * y);
}

void SDL_FillRect(SDL_Surface *p1, SDL_Rect *rect, int color)
{
  printf("SDL_FillRect: p1=%x, rect(%d,%d,%d,%d), color=%x\n", p1, rect->x, rect->y, rect->w, rect->h, color);
  for (int y = 0; y < rect->h; y ++) {
    int index = indexOf(p1, rect->x, rect->y + y);
    for (int x = 0; x < rect->w; x ++) {
      p1->pixels[index++] = color;
    }
  }
}

int SDL_SoftOr(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect)
{
  printf("SDL_SoftOr: src=%x, (%d,%d,%d,%d), dst=%x, (%d,%d,%d,%d)\n", src, srcrect->x,srcrect->y,srcrect->w,srcrect->h,
         dst, dstrect->x,dstrect->y,dstrect->w,dstrect->h );
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
  printf("SDL_SoftStretch: src=%x, (%d,%d,%d,%d), dst=%x, (%d,%d,%d,%d)\n", src, srcrect->x,srcrect->y,srcrect->w,srcrect->h,
         dst, dstrect->x,dstrect->y,dstrect->w,dstrect->h );
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
  info("SDL_MUSTLOCK");
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
bool event_init = false;
int keyStateInit = false;
Uint8 keyState[256];

void SDL_PushEvent(SDL_Event *p1)
{
  events[event_push] = *p1;
  event_push ++;
  event_push %= MAX_EVENTS;
}

static void setupEvents()
{
  info("setupEvents");
  SDL_Event e;

  e.type = SDL_KEYDOWN;
  e.key.keysym.sym = SDLK_BUTTONB;
  SDL_PushEvent(&e);

  e.type = SDL_KEYDOWN;
  e.key.keysym.sym = SDLK_BUTTONC;
  SDL_PushEvent(&e);

  e.type = SDL_KEYDOWN;
  e.key.keysym.sym = SDLK_BUTTONC;
  SDL_PushEvent(&e);

  e.type = SDL_KEYDOWN;
  e.key.keysym.sym = SDLK_BUTTONC;
  SDL_PushEvent(&e);

  e.type = SDL_KEYDOWN;
  e.key.keysym.sym = SDLK_RETURN;
  SDL_PushEvent(&e);

  e.type = SDL_KEYDOWN;
  e.key.keysym.sym = SDLK_BUTTONC;
  SDL_PushEvent(&e);

  e.type = SDL_KEYDOWN;
  e.key.keysym.sym = SDLK_BUTTONC;
  SDL_PushEvent(&e);

  e.type = SDL_KEYDOWN;
  e.key.keysym.sym = SDLK_RETURN;
  SDL_PushEvent(&e);

  e.type = SDL_KEYDOWN;
  e.key.keysym.sym = SDLK_BUTTONA;
  SDL_PushEvent(&e);

  e.type = SDL_KEYDOWN;
  e.key.keysym.sym = SDLK_BUTTONA;
  SDL_PushEvent(&e);

  e.type = SDL_KEYDOWN;
  e.key.keysym.sym = SDLK_RETURN;
  SDL_PushEvent(&e);
}

int SDL_PollEvent(SDL_Event *p1)
{
  info("SDL_PollEvent");
  if (!event_init) {
    event_init = true;
    setupEvents();
  }
  if (event_pop == event_push) {
    return 0;
  }
  *p1 = events[event_pop];
  event_pop ++;
  event_pop %= MAX_EVENTS;
  return 1;
}

static void setupKeyState()
{
  info("setupKeyState");
  ZeroMemory(keyState, sizeof keyState);
}

Uint8 *SDL_GetKeyState(const void *p1)
{
  info("SDL_GetKeyState");
  if (!keyStateInit) {
    keyStateInit = true;
    setupKeyState();
  }
  static Uint8 tmpKeyState[sizeof keyState];
  CopyMemory(tmpKeyState, keyState, sizeof keyState);
  ZeroMemory(keyState, sizeof keyState);
  return tmpKeyState;
}

void SDL_Flip(SDL_Surface *p1)
{
  info("SDL_Flip");
  printf("w=%d, h=%d, colors=%x\n", p1->w, p1->h, p1->format->palette->colors);
  int w = p1->w > 280 ? 280 : p1->w;
  int h = p1->h > 192 ? 192 : p1->h;
  for (int y = 0; y < h; y ++) {
    int pos = indexOf(p1, 0, y);
    for (int x = 0; x < w; x ++) {
      printf("%02x", p1->pixels[pos++]);
    }
    printf("\n");
  }
}

void SDL_PauseAudio(int p1)
{
  info("SDL_PauseAudio");
  printf("p1=%d\n", p1);
}

void SDL_BlitSurface(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect)
{
  int sx, sy, dx, dy;

  printf("SDL_BlitSurface: src=%x, ", src);
  if (srcrect != NULL) {
    printf("(%d,%d,%d,%d), ", srcrect->x,srcrect->y,srcrect->w,srcrect->h);
    sx = srcrect->x;
    sy = srcrect->y;
  } else {
    sx = sy = 0;
  }
  printf("dst=%x, ", dst);
  if (dstrect != NULL) {
    printf("(%d,%d,%d,%d), ", dstrect->x,dstrect->y,dstrect->w,dstrect->h );
    dx = dstrect->x;
    dy = dstrect->y;
  } else {
    dx = dy = 0;
  }
  printf("\n");
  if (src->w == dst->w && src->h == dst->h) {
    info("SDL_BlitSurface fullcopy");
    memcpy(dst->pixels, src->pixels, src->w * src->h);
  } else {
    info("SDL_BlitSurface partialcopy");
    printf("w=%d, h=%d\n", src->w, src->h);
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
  info("SDL_Delay");
  printf("p1=%d\n", p1);
}

int SDL_GetTicks(void)
{
  struct timespec spec;

  info("SDL_GetTicks");
  clock_gettime(CLOCK_REALTIME, &spec);
  long ms = spec.tv_nsec / 1.0e6;
  printf("ms=%ld\n", ms);
  return ms;
}

void SDL_UpdateRect(SDL_Surface *p1, int x, int y, int w, int h)
{
  info("SDL_UpdateRect");
  printf("SDL_UpdateRect: p1=%x, x=%d, y=%d, w=%d, h=%d\n", p1, x, y, w, h);
  w = w > 90 ? 90 : w;
  h = h > 40 ? 40 : h;
  for (int y = 0; y < h; y ++) {
    int pos = indexOf(p1, 0, y);
    for (int x = 0; x < w; x ++) {
      printf("%02x", p1->pixels[pos++]);
    }
    printf("\n");
  }
}

enum M5Joystick g_Joystick = Joystick_Analog; //Joystick_None;

int SDL_NumJoysticks(void)
{
  info("SDL_NumJoysticks");
  return g_Joystick == Joystick_None ? 0 : 1;
}

SDL_Joystick *SDL_JoystickOpen(int p1)
{
  info("SDL_JoystickOpen");
  return NULL;
}

void SDL_JoystickClose(SDL_Joystick *p1)
{
  error("SDL_JoystickClose");
}

void SDL_JoystickUpdate(void)
{
  error("SDL_JoystickUpdate");
}

int SDL_JoystickGetButton(SDL_Joystick *p1, int button)
{
  printf("SDL_JoystickGetButton: button=%d\n", button);
  return 0;
}

int SDL_JoystickGetAxis(SDL_Joystick *p1, int p2)
{
  error("SDL_JoystickGetAxis");
}

/* --- */

void SDL_WM_SetCaption(const char *p1, const char *p2)
{
  info("SDL_WM_SetCaption");
  printf("p1=%s, p2=%s\n", p1, p2);
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
  printf("p1=%s\n", p1);
  error("SDL_SetError");
}

void SDL_SetColorKey(SDL_Surface *surface, int flag, Uint32 key)
{
  info("SDL_SetColorKey");
  printf("surface=%x, flag=%d key=%x\n", surface, flag, key);
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

void DoCalibrateJoystick(int sx, int sy)
{
}

#endif /* REAL_M5STACK */

