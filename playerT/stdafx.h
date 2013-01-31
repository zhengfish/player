// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER		0x0500
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0200
#include "resource.h"
#define _WTL_USE_CSTRING
#define  _WTYPES_NS
#define _WTL_NO_WTYPES 
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>
//#include "SafeWinImpl.h"
#include <atlframe.h>
#include <atlmisc.h>
#include <atlimage.h>
#include <atldlgs.h>
#include <atlctrls.h>
#include <atlctrlw.h>
#include <atlddx.h>
#include <atlcrack.h>

//split file has changed something
#include "atlsplit.h"
#include "dmusici.h"
#include "dxerr8.h"


//#undef BEGIN_MSG_MAP
//#define BEGIN_MSG_MAP() BEGIN_MSG_MAP_EX()

#ifdef _UNICODE
#define tstring   wstring
#define tifstream wifstream
#define tofstream wofstream
#define tfilebuf wfilebuf
#else
#define tstring        string
#define tifstream ifstream
#define tofstream ofstream
#define tfilebuf filebuf
#endif

#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"dxerr8.lib")
#pragma comment(lib,"dxguid.lib")
 

#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
