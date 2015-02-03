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
	int     m_iIconOverlay;
	
	unsigned __int64 m_filesize;
};
class CExplorerDlg: public CNppDlg//public CNppStaticDialog
{
public:
	BOOL CALLBACK run_dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT handleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	// If an application processes this message, it should return zero.
	virtual BOOL handleCommand(UINT iCtrlID, UINT uMsg, HWND hwndFrom);
	// The return value is ignored except for notification messages that specify otherwise. 
	virtual BOOL handleNotify(UINT iCtrlID, UINT uMsg, LPNMHDR lpNmhdr);
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
	BOOL OnClick(NppMsgParams & msg);
	BOOL OnBtnAddAll(NppMsgParams & msg);
	BOOL OnComboxList(NppMsgParams & msg);
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
