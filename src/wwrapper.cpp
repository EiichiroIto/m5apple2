/*
 * Wrappers for some common Microsoft file and memory functions used in AppleWin
 *	by beom beotiger, Nov 2007AD
*/

#include "wwrapper.h"

#ifdef REAL_M5STACK
#include <M5Core2.h>
#endif /* REAL_M5STACK */

extern "C" {
  void error(const char *str);
  void info(const char *str);
};

bool M5OpenFile(const char *imagefilename, HANDLE *lpHandle, LPDWORD lpSize, LPBYTE *lpImage)
{
#ifdef REAL_M5STACK
  String path = "/m5apple2/";
  path += imagefilename;
  File f = SD.open(path.c_str());
  int size = f.size();
  if (size == 0) {
    return false;
  }
  LPBYTE pImage = (LPBYTE) VirtualAlloc(NULL, size, MEM_COMMIT,PAGE_READWRITE);
  if (pImage == 0) {
    return false;
  }
  int ret = f.readBytes((char *) pImage, size);
  if (ret != size) {
    VirtualFree(pImage, 0, MEM_RELEASE);
    f.close();
    return false;
  }
  *lpHandle = (HANDLE) &f;
#else /* REAL_M5STACK */
  FILE *fp = fopen(imagefilename, "rb");
  if (fp == NULL) {
    return false;
  }
  fseek(fp, 0, SEEK_END);
  int size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  LPBYTE pImage = (LPBYTE) VirtualAlloc(NULL, size, MEM_COMMIT,PAGE_READWRITE);
  int ret = fread(pImage, 1, size, fp);
  if (ret != size) {
    VirtualFree(pImage, 0, MEM_RELEASE);
    fclose(fp);
    return false;
  }
  *lpHandle = (HANDLE) fp;
#endif /* REAL_M5STACK */
  *lpSize = size;
  *lpImage = pImage;
  return true;
}

bool M5WriteFile(const char *imagefilename, DWORD pos, const LPBYTE src, DWORD size)
{
#ifdef REAL_M5STACK
  String path = "/m5apple2/";
  path += imagefilename;
  File f = SD.open(path.c_str(), "r+");
  if (!f.seek(pos)) {
    f.close();
    return false;
  }
  int ret = f.write((uint8_t*) src, (size_t) size);
  if (ret != size) {
    f.close();
    return false;
  }
  f.close();
#else /* REAL_M5STACK */
  FILE *fp = fopen(imagefilename, "rb");
  if (fp == NULL) {
    return false;
  }
  fseek(fp, pos, SEEK_SET);
  int ret = fwrite(src, 1, size, fp);
  if (ret != size) {
    fclose(fp);
    return false;
  }
  fclose(fp);
#endif /* REAL_M5STACK */
  return true;
}

DWORD SetFilePointer(HANDLE hFile,
       LONG lDistanceToMove,
       PLONG lpDistanceToMoveHigh,
       DWORD dwMoveMethod)	{
#ifdef REAL_M5STACK
  if (dwMoveMethod != FILE_BEGIN) {
    error("SetFilePointer: invalid parameter");
  }
  File *f = (File*) hFile;
  f->seek(lDistanceToMove);
  return (DWORD) f->position();
#else /* REAL_M5STACK */
	       /* ummm,fseek in Russian */
	       fseek((FILE*)hFile, lDistanceToMove, dwMoveMethod);
	       return ftell((FILE*)hFile);
#endif /* REAL_M5STACK */
}

BOOL ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead,
       		LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)	{

#ifdef REAL_M5STACK
  File *f = (File*) hFile;
  DWORD bytesread = f->readBytes((char*) lpBuffer, (size_t) nNumberOfBytesToRead);
#else /* REAL_M5STACK */
	/* read something from file */
	DWORD bytesread = fread(lpBuffer, 1, nNumberOfBytesToRead, (FILE*)hFile);
#endif /* REAL_M5STACK */
	*lpNumberOfBytesRead = bytesread;
	return (nNumberOfBytesToRead == bytesread);
}

BOOL WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite,
		LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped) {
#ifdef REAL_M5STACK
  File *f = (File*) hFile;
  DWORD byteswritten = f->write((uint8_t*) lpBuffer, (size_t) nNumberOfBytesToWrite);
#else /* REAL_M5STACK */
	/* write something to file */
	DWORD byteswritten = fwrite(lpBuffer, 1, nNumberOfBytesToWrite, (FILE*)hFile);
#endif /* REAL_M5STACK */
	*lpNumberOfBytesWritten = byteswritten;
	return (nNumberOfBytesToWrite == byteswritten);
}

/* close handle whatever it has been .... hmmmmm. I just love Microsoft! */
BOOL CloseHandle(HANDLE hObject) {
#ifdef REAL_M5STACK
  File *f = (File*) hObject;
  f->close();
  return TRUE;
#else /* REAL_M5STACK */
	return(!fclose((FILE*) hObject));
#endif /* REAL_M5STACK */
}

BOOL DeleteFile(LPCTSTR lpFileName) {
#ifdef REAL_M5STACK
  bool ret = SD.remove(lpFileName);
  return (BOOL) ret;
#else /* REAL_M5STACK */
	if(remove(lpFileName) == 0) return TRUE;
	else return FALSE;
#endif /* REAL_M5STACK */
}

DWORD GetFileSize(HANDLE hFile, LPDWORD lpFileSizeHigh) {
#ifdef REAL_M5STACK
  File *f = (File*) hFile;
  DWORD lfilesize = f->size();
  f->seek(0);
#else /* REAL_M5STACK */
	/* what is the size of the specified file??? Hmmm, really I donna. ^_^ */
	long lcurset = ftell((FILE*)hFile); // remember current file position

	fseek((FILE*)hFile, 0, FILE_END);	// go to the end of file
	DWORD lfilesize = ftell((FILE*)hFile); // that is the real size of file, isn't it??
	fseek((FILE*)hFile, lcurset, FILE_BEGIN); // let the file position be the same as before
#endif /* REAL_M5STACK */
	return lfilesize;
}

LPVOID VirtualAlloc(LPVOID lpAddress, size_t dwSize,
      DWORD flAllocationType, DWORD flProtect) {
#ifdef REAL_M5STACK
  static int total = 0;
  total += dwSize;
  int free = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
  //M5.Lcd.printf("VirtualAlloc: size=%d, total=%d, free=%d\n", dwSize, total, free);
  void* mymemory = ps_malloc(dwSize);
  if (flAllocationType & 0x1000) {
    ZeroMemory(mymemory, dwSize);
  }
  return mymemory;
#else /* REAL_M5STACK */
  static int total = 0;
  total += dwSize;
  printf("VirtualAlloc: %d (%d)\n", dwSize, total);
  void* mymemory = malloc(dwSize);
  if (flAllocationType & 0x1000) {
    ZeroMemory(mymemory, dwSize);
  }
  return mymemory;
#endif /* REAL_M5STACK */
}

BOOL VirtualFree(LPVOID lpAddress, size_t dwSize,
			DWORD dwFreeType) {
#ifdef REAL_M5STACK
  info("VirtualFree");
  heap_caps_free(lpAddress);
#else /* REAL_M5STACK */
  info("VirtualFree");
  free(lpAddress);
#endif /* REAL_M5STACK */
	return TRUE;
}

// make all chars in buffer lowercase
DWORD CharLowerBuff(LPTSTR lpsz, DWORD cchLength)
{
//		char *s;
	if (lpsz)
		for (lpsz; *lpsz; lpsz++)
			*lpsz = tolower(*lpsz);
	return 1;

}
