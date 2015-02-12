#ifndef _MAIN_ONE_
#define _MAIN_ONE_



#include <windows.h>
#include <vector>
#include <algorithm>
#include <Wingdi.h>
#include "ole2.h"

#include "resource.h"

#include "../utils/winVersion.h"
#include "../nppUI/NppLib.h"

#include "../utils/CLog.h"
#include "../utils/Lock.h"
#include "../utils/rc4.h"
#include "ExplorerDlg.h"
// #pragma warning(disable:4006)

const char * const g_szDefRc4Key = "how to open file";

typedef enum {
	DEVT_DRIVE,
	DEVT_DIRECTORY,
	DEVT_FILE
} eDevType;


#endif
