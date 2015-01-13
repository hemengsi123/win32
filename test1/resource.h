
#include <windows.h>

////
#define IDR_M10_MENU          100
#define IDR_M10_ACCELERATORS  101
#define IDI_MAIN              102

////////////////////////////////////////////
// Icon
#define IDI_MAIN_ICON            200
///////////////////////////////////////////
// Control
#define IDC_BTN_CRYTO            300
#define IDC_EDT_SHOW             301
#define IDC_CBO_BROWSE           302
////////////////////////////////////////////
// Menu
#define IDM 1000

#define IDM_FILE                 IDM + 500
	#define IDM_FILE_NEW         IDM_FILE + 1
	#define IDM_FILE_OPEN        IDM_FILE + 2

#define IDM_EDIT                 IDM + 1000
	#define IDM_EDIT_CUT         IDM_EDIT + 1
	#define IDM_EDIT_COPY        IDM_EDIT + 2
/////////////////////////////////////////////////////////
//
// Dialog
//
#define IDD_EXPLORER_DLG 3000
#define IDC_TREE_FOLDER     IDD_EXPLORER_DLG + 1
#define IDC_LIST_ALL        IDD_EXPLORER_DLG + 2
#define IDC_LIST_FILES      IDD_EXPLORER_DLG + 3
#define IDC_CBO_FILTER      IDD_EXPLORER_DLG + 4
#define IDC_STC_FILTER      IDD_EXPLORER_DLG + 5
#define IDC_BTN_ADD         IDD_EXPLORER_DLG + 6
#define IDC_BTN_ADDALL      IDD_EXPLORER_DLG + 7
#define IDC_BTN_DEL 	    IDD_EXPLORER_DLG + 8
#define IDC_BTN_DELALL      IDD_EXPLORER_DLG + 9
#define IDC_BTN_CRYPTO      IDD_EXPLORER_DLG + 10
#define IDC_BUTTON_SPLITTER IDD_EXPLORER_DLG + 11
