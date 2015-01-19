#ifndef _MAIN_ONE_
#define _MAIN_ONE_



#include <windows.h>
#include <vector>
#include <algorithm>
#include <Wingdi.h>
#include "ole2.h"

#include "resource.h"

#include "../utils/winVersion.h"
#include "../nppUI/ComboOrgi.h"
#include "../nppUI/NppLib.h"

#include "ExplorerDlg.h"
// #pragma warning(disable:4006)

/* Explorer messages */
#define	EXX_MESSAGES		 30800
#define EXM_CHANGECOMBO					(EXX_MESSAGES + 1)
#define EXM_OPENDIR						(EXX_MESSAGES + 2)
#define EXM_OPENFILE					(EXX_MESSAGES + 3)
#define EXM_RIGHTCLICK					(EXX_MESSAGES + 4)
#define EXM_TOOLTIP						(EXX_MESSAGES + 5)
#define EXM_UPDATE_PATH					(EXX_MESSAGES + 6)

typedef enum {
	DEVT_DRIVE,
	DEVT_DIRECTORY,
	DEVT_FILE
} eDevType;


#endif
