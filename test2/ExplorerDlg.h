#ifndef EXPLOERDLG_ONE_H
#define EXPLOERDLG_ONE_H

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
		tstring	strName;
		DWORD	dwAttributes;
	}tItemList;

struct ListViewItem
{
	tstring m_fullPath;
	tstring m_currentDir;
	tstring m_fileName;
	tstring m_fileExt;
	tstring m_szfilesize;
	bool    m_bIsDir;
	int     m_iIcon;
	unsigned __int64 m_filesize;
};
class CExplorerDlg: public CNppStaticDialog
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
    bool IsValidFolder(const LPWIN32_FIND_DATA  lpFind);
	bool IsValidFile(const LPWIN32_FIND_DATA  lpFind);
    HTREEITEM IsExistAfter(LPCTSTR lpszText, HTREEITEM hPrevItem);
    void GetFolderFullPath(HTREEITEM hItem, LPTSTR lpszFolderFullPath, LPCTSTR lpszChildName);
    bool HaveChildDir(LPCTSTR lpszPath);
	void UpdateFileListAll(LPCTSTR lpszSelDir, LPCTSTR lpszWildcard = _T("*")/*HTREEITEM hSelectItem*/);
protected:
	HWND m_filterCtrl;
	HWND m_splitterCtrl;
	//
	CNppCombox    m_comBoFilter;
	CNppCombox    m_comBoFilter2;
	CNppTreeView  m_treeView2;
	CNppListView  m_listViewAll;
	CNppListView  m_listViewFiles;

	CNppImageList m_imgLst;
	CNppButton    m_btnAdd;
	CNppButton    m_btnAddAll;
	CNppButton    m_btnDel;
	CNppButton    m_btnDelAll;
	int           m_nlvAllFolders;
	int           m_nlvAllItems;
	HIMAGELIST	m_hImageListSmall;
	HANDLE	m_hExploreVolumeThread;
	std::vector<ListViewItem> m_vListViewAll;
	std::vector<ListViewItem> m_vListViewFiles;
};

#endif
