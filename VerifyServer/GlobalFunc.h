#ifndef __H_CC500_GLOBALFUNC_H__
#define __H_CC500_GLOBALFUNC_H__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <tchar.h>
#include "Shlwapi.h"
#include <vector>
#include <string>

//#ifndef _DEFILE_STD_STRING
//#define _DEFILE_STD_STRING

#ifdef _UNICODE
#define MyString std::wstring
#else
#define MyString std::string
#endif //_UNICODE

//#endif //_DEFILE_STD_STRING
#define BUFSIZE 256

typedef enum {
	UNKOWN = 0,
	X86,
	X64
}WIN_OS_BIT_TYPE;

MyString GetModuleFilePath();
TCHAR* GetLocalIP(TCHAR* pIp);
int GlobalUnicodeToUtf8(const TCHAR *unicode, char *uft8, int len);
TCHAR* GlobalUtf8ToUnicode( const char *uft8, TCHAR *unicode, int len);

BOOL CreateDirectoryMulti(TCHAR* path);


#endif //__H_CC500_GLOBALFUNC_H__