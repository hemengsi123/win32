#ifndef EXPLOERDLG_ONE_H
#define EXPLOERDLG_ONE_H
#include <map>
/* Explorer messages */
enum eExpMessage
{
	EXX_MESSAGES		= WM_USER + 20,
	EXM_CHANGECOMBO,
	EXM_LISTVIEW_UPDATE,
	EXM_OPENDIR,
	EXM_OPENFILE,
	EXM_RIGHTCLICK,
	EXM_TOOLTIP,
	EXM_UPDATE_PATH,

};

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

struct FunctoriString
{
    bool operator()(const tstring& lhs, const tstring& rhs)const
    {
		if(_tcsicmp(lhs.c_str(), rhs.c_str()) < 0)
			return true;
		else
			return false;
    }
};

typedef struct{
		tstring	strName;
		DWORD	dwAttributes;
	}tItemList;

struct ListViewItem
{
	int     m_markIndex;
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
	virtual BOOL OnCommand(UINT iCtrlID, UINT uMsg, HWND hwndFrom);
	// The return value is ignored except for notification messages that specify otherwise. 
	virtual BOOL OnNotify(UINT iCtrlID, UINT uMsg, LPNMHDR lpNmhdr);
	virtual void OnDestroy();
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
	BOOL OnComboxEdit(NppMsgParams & msg);
	void UpdateListViews();
	UINT __stdcall AddFiles2FileList(ListViewItem * plvItem, BOOL bRecurse = FALSE);
	static unsigned int __stdcall UpdateFileListThread(void *param);
	static unsigned int __stdcall CyptoDataThread(void *param);
	void EncyptoData(uint8 *data, const uint64 data_len, rc4_key * rc4Key);
	void DecyptoData(uint8 *data, const uint64 data_len, rc4_key * rc4Key);
	void ReadFileList();
	INT CmdFileList(LPCTSTR lpszList);
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
	CNppButton    m_btnCypto;
	CNppCheckbox  m_chkRecurse;
	CNppRadioButton m_rd_test;
	CNppStatic      m_stCount;
	int           m_nlvAllFolders;
	int           m_nlvAllItems;
	HIMAGELIST	  m_hImageListSmall;
	HANDLE	      m_hExploreVolumeThread;
	std::vector<ListViewItem> m_vListViewAll;
	std::vector<ListViewItem> m_vListViewFiles;
	std::map<tstring, bool/*, FunctoriString*/>   m_mFileLists; // 1-œ‘ æ£¨0-≤ªœ‘ æ
	ListViewItem  m_selLVItem;
	CWaitEvent    m_event_selLVItem;
	CWaitEvent    m_event_encypt;
};

#endif
