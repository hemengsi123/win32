#ifndef EXPLOERDLG_ONE_H
#define EXPLOERDLG_ONE_H

#include "../nppUI/StaticDialog.h"
#include "../nppUI/ComboOrgi.h"
#include "../nppUI/TreeHelperClass.h"
#include "../nppUI/TreeView.h"


typedef enum {
	EID_INIT = 0,
	EID_UPDATE_USER,
	EID_UPDATE_DEVICE,
	EID_UPDATE_ACTIVATE,
	EID_UPDATE_ACTIVATEPATH,
	EID_EXPAND_ITEM,
	EID_THREAD_END,
	EID_MAX_THREAD,
	EID_GET_VOLINFO,
	EID_MAX
} eEventID;

typedef struct {
	LPCTSTR		pszDrivePathName;
	LPTSTR		pszVolumeName;
	UINT		maxSize;
	LPBOOL		pIsValidDrive;
} tGetVolumeInfo;

// typedef enum {
	// DEVT_DRIVE,
	// DEVT_DIRECTORY,
	// DEVT_FILE
// } eDevType;
typedef struct{
		string	strName;
		DWORD	dwAttributes;
	}tItemList;
	
class CExplorerDlg: public StaticDialog
{
public:
	void create(HINSTANCE hInst, int dialogId);
	BOOL CALLBACK run_dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void initCtrl();
	void UpdateDevices();
	void UpdateFolders(void);
	void UpdateFolderRecursive(LPTSTR pszParentPath, HTREEITEM hParentItem);
	BOOL ExploreVolumeInformation(LPCTSTR pszDrivePathName, LPTSTR pszVolumeName, UINT maxSize);
	void NotifyEvent(DWORD event);
	BOOL FindFolderAfter(LPTSTR itemName, HTREEITEM pAfterItem);
	void UpDateChildren(LPTSTR pszParentPath, HTREEITEM hParentItem, BOOL doRecursive = TRUE);
    bool IsValidFolder(const LPWIN32_FIND_DATA  Find);
    bool IsExistAfter(LPCTSTR lpszText, HTREEITEM hPrevItem);
    void GetFolderFullPath(HTREEITEM hItem, LPTSTR lpszFolderFullPath, LPCTSTR lpszChildName);
protected:
	HWND m_hTreeCtrl;
	HWND m_listCtrlAll;
	HWND m_listCtrlFiles;
	HWND m_filterCtrl;
	HWND m_splitterCtrl;
	//
	ComboOrgi m_comBoFilter;
	TreeHelper m_treeView;
	TreeView  m_treeView2;
	//
	HIMAGELIST	m_hImageListSmall;
	//
	HANDLE	m_hExploreVolumeThread;
};

#endif
