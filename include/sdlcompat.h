#define SDLK_a 'a'
#define SDLK_z 'z'
#define SDLK_0 '0'
#define SDLK_9 '9'
#define SDLK_RETURN 0x0D
#define SDLK_ESCAPE 0x1B
#define SDLK_LEFT 0xB4
#define SDLK_UP 0xB5
#define SDLK_DOWN 0xB6
#define SDLK_RIGHT 0xB7

enum {
  SDLK_LSHIFT = 128,
  SDLK_RSHIFT,
  SDLK_LCTRL,
  SDLK_RCTRL,
  SDLK_LALT,
  SDLK_RALT,
  SDLK_DELETE,
  SDLK_PAGEUP,
  SDLK_PAGEDOWN,
  SDLK_HOME,
  SDLK_END,
  SDLK_PAUSE,
  SDLK_KP_PLUS,
  SDLK_KP_MINUS,
  SDLK_KP_MULTIPLY,
  SDLK_CAPSLOCK,
  SDLK_SCROLLOCK,
  SDLK_KP0,
  SDLK_KP1,
  SDLK_KP9,
  SDLK_CLEAR,
  SDLK_INSERT,
  SDLK_KP_PERIOD,
  SDLK_F1,
  SDLK_F2,
  SDLK_F3,
  SDLK_F4,
  SDLK_F5,
  SDLK_F6,
  SDLK_F7,
  SDLK_F8,
  SDLK_F9,
  SDLK_F10,
  SDLK_F11,
  SDLK_F12,
  SDLK_BUTTONA,
  SDLK_BUTTONB,
  SDLK_BUTTONC,
};

enum {
  SDL_QUIT = 0,
  SDL_KEYDOWN,
  SDL_KEYUP,
  SDL_ACTIVEEVENT,
  SDL_MOUSEBUTTONDOWN,
  SDL_MOUSEBUTTONUP,
  SDL_BUTTON_LEFT,
  SDL_BUTTON_RIGHT,
  SDL_MOUSEMOTION,
  SDL_USEREVENT,
  SDL_DUMMY,
};

#define SDL_SWSURFACE 0
#define SDL_SRCCOLORKEY 0
#define SDL_BYTEORDER 0
#define SDL_BIG_ENDIAN 0
#define SDL_DISABLE 0
#define SDL_ENABLE 0
#define SDL_GRAB_ON 0
#define SDL_GRAB_OFF 0
#define SDL_HWPALETTE 0
#define SDL_DEFAULT_REPEAT_DELAY 0
#define SDL_DEFAULT_REPEAT_INTERVAL 0
#define SDL_INIT_EVERYTHING 0

#define KMOD_SHIFT 0
#define KMOD_CTRL 0
#define KMOD_ALT 0

#define AUDIO_S16MSB 0
#define SDL_MIX_MAXVOLUME 0

#define CURL_GLOBAL_DEFAULT 0
#define CURLE_OK 0
enum {
  CURLOPT_USERPWD,
  CURLOPT_URL,
  CURLOPT_WRITEFUNCTION,
  CURLOPT_WRITEDATA,
};

#define CRTSCTS 0
#define O_NDELAY 0
#define MEM_COMMIT 0x1000
#define PAGE_READWRITE 0
#define MEM_RELEASE 0

typedef unsigned char Uint8;
typedef unsigned short Uint16;
typedef unsigned int Uint32;

typedef struct {
  int r, g, b;
} SDL_Color;

typedef struct {
  int w, h;
  struct _format {
    struct _palette {
      SDL_Color *colors;
      int ncolors;
    } *palette;
    int BitsPerPixel;
    int BytesPerPixel;
  } *format;
  int pitch;
  Uint8 *pixels;
  const unsigned char *cpixels;
} SDL_Surface;

typedef struct {
  int x, y, w, h;
} SDL_Rect;

typedef struct {
  int type;
  union {
    struct {
      struct {
        int mod;
        int sym;
      } keysym;
    } key;
    struct {
      int button;
      int x, y;
    } button;
    struct {
      int x, y;
    } motion;
    struct {
      int gain;
    } active;
    struct {
      int code;
    } user;
  };
} SDL_Event;

typedef struct {
  int freq;
  int format;
  int samples;
  int channels;
  void (*callback)(void*, BYTE*, int);
  void *userdata;
  int size;
  int silence;
} SDL_AudioSpec;

typedef int SDL_Joystick;

typedef void CURL;
typedef int CURLcode;
typedef int gzFile;
struct { int unused; } zip;
struct { int unused; } zip_file;

extern "C" {
  void SuppressMessages();
  void StartDebug();
  void error(const char *str);
  void info(const char *str);
  void SDL_WM_SetCaption(const char *, const char *);
  Uint8 *SDL_GetKeyState(const void *);
  SDL_Surface *SDL_CreateRGBSurface(int, int, int, int, int, int, int, int);
  void SDL_FreeSurface(SDL_Surface *surface);
  int SDL_SetColors(SDL_Surface *, SDL_Color *, int, int);
  Uint32 SDL_MapRGB(void *, int, int, int);
  void SDL_FillRect(SDL_Surface *, SDL_Rect *, int);
  int SDL_MUSTLOCK(void *);
  int SDL_LockSurface(void *);
  void SDL_UnlockSurface(void *);
  void SDL_SoftStretch(SDL_Surface * , SDL_Rect *, SDL_Surface *, SDL_Rect *);
  void SDL_Delay(int);
  int SDL_GetTicks(void);
  void SDL_Flip(SDL_Surface *);
  SDL_Surface *SDL_LoadBMP(const char *);
  void SDL_SaveBMP(SDL_Surface *, const char *);
  SDL_Surface *SDL_DisplayFormat(SDL_Surface *);
  void SDL_BlitSurface(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect);
  void SDL_UpdateRect(SDL_Surface *, int, int, int, int);
  //SDL_Surface *SDL_CreateRGBSurface(SDL_Surface *, int, int, int, int, int, int, int);
  int SDL_PollEvent(SDL_Event *);
  void SDL_SetError(const char *);
  void SDL_SetColorKey(SDL_Surface *, int, Uint32);
  //void SDL_GetClipRect(SDL_Surface *, void *);
  void SDL_PushEvent(SDL_Event *);
  void SDL_WM_ToggleFullScreen(SDL_Surface *);
  void SDL_ShowCursor(int);
  void SDL_WM_GrabInput(int);
  void SDL_putenv(const char *);
  SDL_Surface *SDL_SetVideoMode(int, int, int, int);
  const char *SDL_GetError(void);
  void SDL_Quit(void);
  void SDL_EnableKeyRepeat(int, int);
  int SDL_Init(int);
  void SDL_WM_SetIcon(SDL_Surface *, void *);
  void SDL_PauseAudio(int);
  int SDL_OpenAudio(SDL_AudioSpec *,SDL_AudioSpec *);
  void SDL_CloseAudio(void);
  void SDL_LockAudio(void);
  void SDL_UnlockAudio(void);
  void SDL_JoystickUpdate(void);
  int SDL_JoystickGetButton(SDL_Joystick *, int);
  int SDL_JoystickGetAxis(SDL_Joystick *, int);
  void SDL_JoystickClose(SDL_Joystick *);
  int SDL_NumJoysticks(void);
  SDL_Joystick *SDL_JoystickOpen(int);
  int SDL_SoftOr(SDL_Surface *src, SDL_Rect *srcrect, SDL_Surface *dst, SDL_Rect *dstrect);
  int SDL_SoftStretchOr(SDL_Surface * , SDL_Rect *, SDL_Surface *, SDL_Rect *);
  void SDL_GetClipRect(SDL_Surface *, SDL_Rect *);

  void curl_global_init(int p);
  void *curl_easy_init(void);
  void curl_easy_setopt(void *p1, int p2, const void *p3);
  void curl_easy_cleanup(void *p);
  void curl_global_cleanup(void);
  CURLcode ftp_get(const char *ftp_path, const char *local_path);
  gzFile gzopen(const char *p1, const char *p2);
  void gzclose(gzFile p);
  int gzeof(gzFile p);
  int gzread(gzFile p1, void *p2, int p3);
  struct zip *zip_open(const char *p1, int p2, int p3);
  void zip_close(struct zip *p);
  struct zip_file *zip_fopen_index(struct zip *p1, int p2, int p3);
  int zip_fread(struct zip_file *p1, void *p2, int p3);
  void zip_fclose(struct zip_file *p1);
  void mlock(void *p1, int p2);
  void munlock(void *p1, int p2);
  LPVOID VirtualAlloc(LPVOID lpAddress, size_t dwSize, DWORD flAllocationType, DWORD flProtect);
  BOOL VirtualFree(LPVOID lpAddress, size_t dwSize, DWORD dwFreeType);
  DWORD CharLowerBuff(LPTSTR lpsz, DWORD cchLength);
  double DSUploadBuffer(short* buffer, unsigned len);
  bool ReadImageFile(const char *filename, unsigned char **pBuffer, int *pSize, char **pExt);
};

#define IsCharLower(ch) ('a' <= (ch) && (ch) <= 'z')

extern bool g_bDSAvailable;
