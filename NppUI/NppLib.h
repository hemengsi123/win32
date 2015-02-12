#ifndef _NPPLIB_H_
#define _NPPLIB_H_

#ifdef  MAKEDLL
	#ifdef  NPPIMPORT_DLL
		#define NPPLIB_API __declspec(dllimport)
	#else
		#define NPPLIB_API __declspec(dllexport)
	#endif
#else
#define NPPLIB_API 
#endif

#include "../common/Defines.h"
#include "NppBase.h"
#include "NppString.h"
#include "NppFile.h"
#include "NppWnd.h"
#include "NppTreeView.h"
#include "NppStaticDialog.h"
#include "NppImageList.h"
#include "NppListView.h"
#include "NppCombox.h"
#include "NppButton.h"
#include "NppFont.h"
#include "NppStatic.h"

#include "../utils/CLog.h"

//typedef std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> > tstring;

extern int g_log_level;

#endif
