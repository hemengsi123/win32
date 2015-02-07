
#include "StdAfx.h"

HANDLE g_hEvent[EID_MAX]	= {NULL};
HANDLE g_hThread			= NULL;

NPP_BEGIN_MESSAGE_MAP(CExplorerDlg)
//NPP_ON_CTRL_MSGMAP(WM_KEYUP, OnClick) WM_LBUTTONDOWN  BN_CLICKED IDC_BTN_DEL
NPP_ON_CTRL_MSGMAP_ID(IDC_CBO_FILTER, WM_KEYUP, OnComboxEdit) 
//NPP_ON_CTRL_MSGMAP_NAME(_T("btnAddAll"), WM_LBUTTONDOWN, OnBtnAddAll)
 NPP_ON_MSGMAP_CMD(-1, BN_CLICKED, OnBtnAddAll)
//NPP_ON_MSGMAP_CMD(IDC_CBO_FILTER, -1, OnComboxList)
//NPP_ON_MSGMAP_NOTIFY(IDC_LIST_ALL, -1, );
NPP_END_MESSAGE_MAP()

DWORD WINAPI UpdateThread(LPVOID lpParam)
{
	BOOL			bRun			= TRUE;
    DWORD			dwWaitResult    = EID_INIT;
	CExplorerDlg*	dlg             = (CExplorerDlg*)lpParam;

	dlg->NotifyEvent(dwWaitResult);

	while (bRun)
	{
		dwWaitResult = ::WaitForMultipleObjects(EID_MAX_THREAD, g_hEvent, FALSE, INFINITE);

		if (dwWaitResult == EID_THREAD_END)
		{
			bRun = FALSE;
		}
		else if (dwWaitResult < EID_MAX)
		{
			dlg->NotifyEvent(dwWaitResult);
		}
	}
	return 0;
}

DWORD WINAPI GetVolumeInformationTimeoutThread(LPVOID lpParam)
{
	DWORD			serialNr		= 0;
	DWORD			space			= 0;
	DWORD			flags			= 0;
	tGetVolumeInfo*	volInfo         = (tGetVolumeInfo*)lpParam;

	if (volInfo->maxSize < MAX_PATH+1)
		*volInfo->pIsValidDrive = GetVolumeInformation(volInfo->pszDrivePathName,
			volInfo->pszVolumeName, volInfo->maxSize, &serialNr, &space, &flags, NULL, 0);

	::SetEvent(g_hEvent[EID_GET_VOLINFO]);
	::ExitThread(0);
	return 0;
}

void CExplorerDlg::initCtrl()
{
	m_treeView2.init(m_hInst, m_hSelf, IDC_TREE_FOLDER);
	m_treeView2.create();
	m_listViewAll.init(m_hInst, m_hSelf, IDC_LIST_ALL);
	m_listViewAll.create();
	m_listViewFiles.init(m_hInst, m_hSelf, IDC_LIST_FILES);
	m_listViewFiles.create();
//	m_listViewAll.alignTo(m_treeView2.getHSelf(), RIGHTALIGN);
	// filter combox
	m_comBoFilter.init(m_hInst, m_hSelf, IDC_CBO_FILTER);
	m_comBoFilter.create();
	
	m_comBoFilter.addText(_T("*.*"));
	m_comBoFilter.addText(_T("*.txt"));
	m_comBoFilter.setText(_T("*.*"), 1);
	
	m_comBoFilter2.init(m_hInst, m_hSelf, IDC_CBO_FILTER +30);
	m_comBoFilter2.create(CBS_DROPDOWN | CBS_AUTOHSCROLL, 0, 0, 200, 30);
	m_comBoFilter2.alignTo(m_treeView2.getHSelf(), TOPALIGN, LEFTALIGN, 0, 10);
	// spliter
	m_splitterCtrl  = ::GetDlgItem(m_hSelf, IDC_BUTTON_SPLITTER);

	// button
	m_btnAdd.init(m_hInst, m_hSelf, IDC_BTN_ADD, _T("btnAdd"));
	m_btnAdd.create();
	m_btnAddAll.init(m_hInst, m_hSelf, IDC_BTN_ADDALL, _T("btnAddAll"));
	m_btnAddAll.create();
	if( !m_listViewAll.isFocus() )
		m_btnAddAll.setEnable(FALSE);
	m_btnDel.init(m_hInst, m_hSelf, IDC_BTN_DEL);
	m_btnDel.create();
	m_btnDelAll.init(m_hInst, m_hSelf, IDC_BTN_DELALL);
	m_btnDelAll.create();
	if( !m_listViewFiles.isFocus() )
		m_btnDel.setEnable(FALSE);
	//m_btnAdd.alignTo(m_hSelf, TOPALIGN, RIGHTALIGN, 10, 10);
	// checkbox
	m_chkRecurse.init(m_hInst, m_hSelf, IDC_CHK_RECURCE);
	m_chkRecurse.create(_T("Recursive"), 0, 0, 0, 70, 20);
	m_chkRecurse.alignTo(m_btnAdd.getHSelf(), TOPALIGN, LEFTALIGN, 0, 20);
	
	m_rd_test.init(m_hInst, m_hSelf, IDC_RDI_TEST);
	m_rd_test.create(_T("Recursive"), 0, 0, 0, 70, 20);
	m_rd_test.alignTo(m_comBoFilter2.getHSelf(), RIGHTALIGN, DEFALIGN, 0, 0);
	
	CNppFont nppFont;
	nppFont.setFont(m_btnAdd.getHSelf(), m_btnAddAll.getHSelf());
	nppFont.setFont(m_chkRecurse.getHSelf(), m_btnAddAll.getHSelf());
	//m_treeView2.setImageList(true);
	m_treeView2.setImageList(m_imgLst.getSysImgLst());

    UpdateDevices();
	UpdateFolders();
	
	m_listViewAll.setExtStyle(m_listViewAll.getExtStyle() | LVS_EX_FULLROWSELECT);
	int errRet = 0;
	m_listViewAll.addColumn(_T("Name"), 100);
	m_listViewAll.addColumn(_T("Ext."), 50, CNppListView::ColTextCenter);
	m_listViewAll.addColumn(_T("Size"), 80, CNppListView::ColTextCenter);
	m_listViewAll.setItemImgList(m_imgLst.getSysImgLst());
	
	m_listViewFiles.hiddenHeader();
	//ListView_SetItemCountEx(m_listViewFiles.getHSelf(), 2, LVSICF_NOSCROLL);
	m_listViewFiles.addColumn(_T("File"), 535);
//	m_listViewFiles.addItem(_T("test1.txt"), 0);
//	m_listViewFiles.addItem(_T("test2.txt"), 1);
//	static ctrl
	m_stCount.init(m_hInst, m_hSelf, 0);
	m_stCount.create("No: 0", 0, 0, 0, 50, 20);
	nppFont.setFont(m_stCount.getHSelf(), m_btnAddAll.getHSelf());
	m_stCount.alignTo(m_listViewFiles.getHSelf(), TOPALIGN, LEFTALIGN, 0, 2);
	
}
LRESULT CExplorerDlg::handleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bDone = TRUE;
	switch(uMsg)
	{
	// OnInitDlg
	case WM_INITDIALOG:
	{
		initCtrl();
		dbg_log(_T("CtrlCount = %u"), CNppCtrlWnd::getCtrlCount());
		DWORD			dwThreadId		= 0;
		// for (int i = 0; i < EID_MAX; i++)
			// g_hEvent[i] = ::CreateEvent(NULL, FALSE, FALSE, NULL);

		/* create thread */
		// g_hThread = ::CreateThread(NULL, 0, UpdateThread, this, 0, &dwThreadId);
		return TRUE;
	}
	case EXM_CHANGECOMBO:
	{
		TCHAR	temp[MAX_PATH];
		m_comBoFilter.getSelText(temp);
		dbg_log(_T("combo input: %s"), temp);
		return TRUE;
	}
	// 关闭窗口
	case WM_CLOSE:
	{
		OnClose();
		// dbg_log(_T("wm_close"));
		return TRUE;
	}
	// 关闭应用程序
	case WM_DESTROY:
	{
		//::PostQuitMessage(0);
		OnDestroy();
		return TRUE;
	}
	case EXM_LISTVIEW_UPDATE:
	{
		printf(_T("EXM_LISTVIEW_UPDATE recv\n"));
		UpdateListViews();
		break;
	}
	default:
		bDone = FALSE;
		break;
	}
	if(bDone)
		return TRUE;
		
	return CNppDlg::handleMessage(hwnd, uMsg, wParam, lParam);
}

BOOL CExplorerDlg::OnCommand(UINT iCtrlID, UINT uMsg, HWND hwndFrom)
{
	if(uMsg == BN_CLICKED)
	{
		HTREEITEM hSelTreeItem = NULL;
		if( iCtrlID== IDC_BTN_ADD )
		{
			UINT hotItem = this->m_listViewAll.getMarkItem();
			if( hotItem >= 0 && hotItem < m_vListViewAll.size() )
			{
				TCHAR lpszText[MAX_PATH] = {0};
				m_listViewAll.getItemText(hotItem, 0, lpszText, MAX_PATH);
				printf(_T("hotItem = %d, size = %d\n"), hotItem, m_vListViewAll.size());
				ListViewItem lstItem = m_vListViewAll.at(hotItem);
				printf(_T("BN_DBLCLK %d, txt = %s, "), hotItem, lpszText);
				printf(_T("fullpath = %s\n"), lstItem.m_fullPath.c_str());
				//m_vListViewFiles.push_back(lstItem);
				AddFiles2FileList(&lstItem, m_chkRecurse.isCheck());
				m_vListViewAll.erase(m_vListViewAll.begin()+hotItem);
				::SendMessage(m_hSelf, EXM_LISTVIEW_UPDATE, 0, 0);
			}
			else if( hotItem == -1 && (hSelTreeItem = m_treeView2.getSelect() ) != NULL)
			{
				TCHAR szItemPath[MAX_PATH] = {0};
				m_treeView2.getItemPath(hSelTreeItem, szItemPath);
				GetFolderFullPath(hSelTreeItem, szItemPath, _T(""));
				ListViewItem lvItem;
				int iIconNormal   = 0;
				int iIconSelected = 0;
				int iIconOverlayed = 0;
				lvItem.m_bIsDir     = true;
				lvItem.m_fileExt.clear();
				lvItem.m_fileExt    = _T("<DIR>");
				lvItem.m_currentDir = szItemPath;
				lvItem.m_fileName   = _T("");
				lvItem.m_filesize   = 0;//searchFile.findGetSize(lpfindData);
				lvItem.m_szfilesize = _T("");
				lvItem.m_fullPath   = szItemPath;
				AddFiles2FileList(&lvItem, m_chkRecurse.isCheck());
				m_vListViewAll.clear();
				::SendMessage(m_hSelf, EXM_LISTVIEW_UPDATE, 0, 0);
			}
		}
		else if( iCtrlID== IDC_BTN_ADDALL)
		{
			if(m_vListViewAll.size() > 0 )
			{
				std::vector<ListViewItem>::iterator iter = m_vListViewAll.begin();
				for(; iter != m_vListViewAll.end(); ++iter)
				{
					//m_vListViewFiles.push_back(*iter);
					AddFiles2FileList(&(*iter), m_chkRecurse.isCheck());
				}
				m_vListViewAll.clear();
				::SendMessage(m_hSelf, EXM_LISTVIEW_UPDATE, 0, 0);
			}
		}
		else if( iCtrlID == IDC_BTN_DEL )
		{
			UINT hotItem = this->m_listViewFiles.getMarkItem();
			if( hotItem >= 0 && hotItem < m_vListViewFiles.size() )
			{
				TCHAR lpszText[MAX_PATH] = {0};
				m_listViewFiles.getItemText(hotItem, 0, lpszText, MAX_PATH);
				printf(_T("BN_DBLCLK %d, txt = %s\n"), hotItem, lpszText);
				m_mFileLists.erase(tstring(lpszText));
				::SendMessage(m_hSelf, EXM_LISTVIEW_UPDATE, 0, 0);
			}
		}
		else if(iCtrlID == IDC_BTN_DELALL)
		{
			if(m_vListViewFiles.size() > 0 )
			{
//				std::vector<ListViewItem>::iterator iter = m_vListViewFiles.begin();
//				for(; iter != m_vListViewFiles.end(); ++iter)
//				{
//					m_vListViewAll.push_back(*iter);
//				}
//				m_vListViewFiles.clear();
				m_mFileLists.clear();
				::SendMessage(m_hSelf, EXM_LISTVIEW_UPDATE, 0, 0);
			}
		}

	}
	if(iCtrlID == IDC_CBO_FILTER)
	{
		if( uMsg == CBN_EDITUPDATE)
		{

		}
		else if( uMsg == CBN_EDITCHANGE)
		{

		}
	
	}

	return TRUE;
}
void CExplorerDlg::OnDestroy()
{
	::PostQuitMessage(0);
}
BOOL CExplorerDlg::OnNotify(UINT iCtrlID, UINT uMsg, LPNMHDR lpNmhdr)
{
	BOOL bDone = FALSE;
	if(iCtrlID == IDC_LIST_ALL)
	{
		if( m_listViewAll.isFocus() )
		{
			m_btnAddAll.setEnable();
			m_btnDel.setEnable(FALSE);
		}
		if(uMsg == NM_CLICK)
		{
			LPNMCLICK lpNmclick = (LPNMCLICK)lpNmhdr;
			int hotItem = this->m_listViewAll.getMarkItem();
			dbg_log("IDC_LIST_ALL => hotItem: %d", hotItem);
		}
		else if(uMsg == LVN_KEYDOWN)
		{
			dbg_log("LVN_KEYDOWN");
		}
		else if(uMsg == LVN_COLUMNCLICK)
		{
			dbg_log("LVN_COLUMNCLICK");
		}
	}
	else if(iCtrlID == IDC_LIST_FILES)
	{
		if( m_listViewFiles.isFocus() )
		{
			m_btnDel.setEnable();
			m_btnAddAll.setEnable(FALSE);
		}
	}
	else if (lpNmhdr->hwndFrom == m_treeView2.getHSelf())
	{
		bDone = TRUE;
		switch (lpNmhdr->code)
		{
			case NM_RCLICK:
			{
				dbg_log(_T("NM_RCLICK"));
				return TRUE;
			}
			case TVN_SELCHANGED:
			{
				dbg_log(_T("TVN_SELCHANGED"));
				TVITEM	tvi	= (TVITEM)((LPNMTREEVIEW)lpNmhdr)->itemNew;
				TCHAR szItemPath[MAX_PATH] = {0};
				if( m_treeView2.isSelected(tvi.hItem) )
				{
					m_treeView2.getItemPath(tvi.hItem, szItemPath);
					GetFolderFullPath(tvi.hItem, szItemPath, _T(""));
					UpdateFileListAll(szItemPath);
				}
				break;
			}
			case TVN_ITEMEXPANDING:
			{
				
				TVITEM	tvi	= (TVITEM)((LPNMTREEVIEW)lpNmhdr)->itemNew;
				if (!(tvi.state & TVIS_EXPANDED))
				{
					dbg_log(_T("TVN_ITEMEXPANDING not "));
					TCHAR szItemPath[MAX_PATH] = {0};
					m_treeView2.getItemPath(tvi.hItem, szItemPath);
//							dbg_log("ItemPath = %s", szItemPath);				
					GetFolderFullPath(tvi.hItem, szItemPath, _T(""));
//							dbg_log("ItemPath = %s", szItemPath);
					UpDateChildren(szItemPath, tvi.hItem, true);

					//UpdateFileListAll(szItemPath);
				}
				else
				{
					dbg_log(_T("TVN_ITEMEXPANDING"));
				}
				//UpdateFolders();
				break;
			}
			case TVN_KEYDOWN:
			{
				if (((LPNMTVKEYDOWN)lpNmhdr)->wVKey == VK_RIGHT)
				{
					dbg_log(_T("TVN_KEYDOWN"));
				}
				
				return TRUE;
			}
			default:
				bDone = FALSE;
				break;
		}
	}
	else if(lpNmhdr->hwndFrom == m_filterCtrl)
	{
		bDone = TRUE;
		dbg_log(_T("m_filterCtrl NM_RCLICK"));
	}
	return bDone;
}
BOOL CALLBACK CExplorerDlg::run_dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_INITDIALOG:
		{
			initCtrl();
			dbg_log(_T("CtrlCount = %u"), CNppCtrlWnd::getCtrlCount());
			DWORD			dwThreadId		= 0;
			// for (int i = 0; i < EID_MAX; i++)
				// g_hEvent[i] = ::CreateEvent(NULL, FALSE, FALSE, NULL);

			/* create thread */
			// g_hThread = ::CreateThread(NULL, 0, UpdateThread, this, 0, &dwThreadId);
			return TRUE;
		}
		case WM_COMMAND:
		{
			if(LOWORD(wParam) == IDC_CBO_FILTER)
			{
				// dbg_log(_T("H: %04x"), HIWORD(wParam))
				if(HIWORD(wParam) == CBN_EDITUPDATE)
				{
//					dbg_log(_T("CBN_EDITUPDATE"));
				}
				else if(HIWORD(wParam) == CBN_EDITCHANGE)
				{
//					dbg_log(_T("CBN_EDITCHANGE."));
				}
			}
			else if(LOWORD(wParam) == IDC_BTN_ADD)
			{
			//	dbg_log(_T("BN_DBLCLK"));
				if(HIWORD(wParam) == BN_CLICKED)
				{
					dbg_log(_T("BN_DBLCLK"));
					static CNppDlg nppDlg;
					nppDlg.init(m_hInst, m_hSelf);
					nppDlg.create(_T("Hello"), (WS_VISIBLE|WS_SYSMENU|WS_CAPTION|WS_BORDER), 0, 0, 40, 32);
					nppDlg.gotoCenter();
					nppDlg.doModal();
				}
			}
			break;
			// return TRUE;
		}
		case WM_KEYUP:
		{
			dbg_log(_T("WM_KEYUP hwnd = 0x%08X"), hwnd);
			break;
		}
		case WM_NOTIFY:
		{
			LPNMHDR		nmhdr = (LPNMHDR)lParam;
			
			if (nmhdr->hwndFrom == m_treeView2.getHSelf())
			{
				switch (nmhdr->code)
				{
					case NM_RCLICK:
					{
						dbg_log(_T("NM_RCLICK"));
						return TRUE;
					}
					case TVN_SELCHANGED:
					{
						dbg_log(_T("TVN_SELCHANGED"));
						TVITEM	tvi	= (TVITEM)((LPNMTREEVIEW)lParam)->itemNew;
						TCHAR szItemPath[MAX_PATH] = {0};
						if( m_treeView2.isSelected(tvi.hItem) )
						{
							m_treeView2.getItemPath(tvi.hItem, szItemPath);
							GetFolderFullPath(tvi.hItem, szItemPath, _T(""));
							UpdateFileListAll(szItemPath);
						}
						break;
					}
					case TVN_ITEMEXPANDING:
					{
						
						TVITEM	tvi	= (TVITEM)((LPNMTREEVIEW)lParam)->itemNew;
						if (!(tvi.state & TVIS_EXPANDED))
						{
							dbg_log(_T("TVN_ITEMEXPANDING not "));
							TCHAR szItemPath[MAX_PATH] = {0};
							m_treeView2.getItemPath(tvi.hItem, szItemPath);
//							dbg_log("ItemPath = %s", szItemPath);				
							GetFolderFullPath(tvi.hItem, szItemPath, _T(""));
//							dbg_log("ItemPath = %s", szItemPath);
							UpDateChildren(szItemPath, tvi.hItem, true);

							//UpdateFileListAll(szItemPath);
						}
						else
						{
							dbg_log(_T("TVN_ITEMEXPANDING"));
						}
						//UpdateFolders();
						break;
					}
					case TVN_KEYDOWN:
					{
						if (((LPNMTVKEYDOWN)lParam)->wVKey == VK_RIGHT)
						{
							dbg_log(_T("TVN_KEYDOWN"));
						}
						
						return TRUE;
					}
					default:
						break;
				}
			}
			else if(nmhdr->hwndFrom == m_filterCtrl)
			{
				dbg_log(_T("m_filterCtrl NM_RCLICK"));
			}
			
			return TRUE;
		}
		case EXM_CHANGECOMBO:
		{
			TCHAR	temp[MAX_PATH];
			m_comBoFilter.getSelText(temp);
			dbg_log(_T("combo input: %s"), temp);
			return TRUE;
		}
		// 关闭窗口
		case WM_CLOSE:
		{
			OnClose();
			// dbg_log(_T("wm_close"));
			return TRUE;
		}
		// 关闭应用程序
		case WM_DESTROY:
		{
			OnDestroy();
			return TRUE;
		}
		default:
			break;
	}
	return false;
}
void CExplorerDlg::UpdateDevices(void)
{
	BOOL			bDefaultDevice  = FALSE;
	DWORD			driveList		= ::GetLogicalDrives();
	BOOL			isValidDrive	= FALSE;
	BOOL			haveChildren	= FALSE;
	
	DWORD			serialNr		= 0;
	DWORD			space			= 0;
	DWORD			flags			= 0;
	
	HTREEITEM		hCurrentItem	= m_treeView2.getRoot();

	TCHAR			drivePathName[]	= _T(" :\\\0\0");	// it is longer for function 'HaveChildren()'
	TCHAR			TEMP[MAX_PATH]	= {0};
	TCHAR			volumeName[MAX_PATH];
	CNppFile        tmpFile;
	for (int i = 1; i < 32; i++)
	{
		drivePathName[0] = 'A' + i;

		if (0x01 & (driveList >> i))
		{
			/* create volume name */
			isValidDrive =  ::GetVolumeInformation(drivePathName, TEMP, MAX_PATH, &serialNr, &space, &flags, NULL, 0);
			_stprintf(volumeName, _T("%c: []"), 'A' + i);
			
			if (isValidDrive == TRUE)
			{
				_stprintf(volumeName, _T("%c: [%s]"), 'A' + i, TEMP);
				/* correct modified drivePathName */
				drivePathName[3]			= '\0';
			}
			tmpFile.setFullPath(drivePathName);
			/* have children */
//			dbg_log(_T("path = %s"), drivePathName);
			haveChildren = HaveChildDir(drivePathName);
//			dbg_log(_T("haveChildren = %d"), haveChildren);
			// dbg_log(_T("%s chidren = %d"), drivePathName, haveChildren);
			if (hCurrentItem != NULL)
			{
				int			iIconNormal		= 0;
				int			iIconSelected	= 0;
				int			iIconOverlayed	= 0;

				/* get current volume name in list and test if name is changed */
				m_treeView2.getText(hCurrentItem, TEMP, MAX_PATH);

				if (_tcscmp(volumeName, TEMP) == 0)
				{
					/* if names are equal, go to next item in tree */
					m_treeView2.getFileIcon(drivePathName, &iIconNormal, &iIconSelected, &iIconOverlayed);
					m_treeView2.setItem(hCurrentItem, volumeName, iIconNormal, haveChildren);
					hCurrentItem = m_treeView2.getNext(hCurrentItem);
				}
				else if (volumeName[0] == TEMP[0])
				{
					/* if names are not the same but the drive letter are equal, rename item */

					/* get icons */
					m_treeView2.getFileIcon(drivePathName, &iIconNormal, &iIconSelected, &iIconOverlayed);
					m_treeView2.setItem(hCurrentItem, volumeName, iIconNormal, haveChildren);
					// hCurrentItem = TreeView_GetNextItem(m_hTreeCtrl, hCurrentItem, TVGN_NEXT);
					hCurrentItem = m_treeView2.getNext(hCurrentItem);
				}
				else
				{
					/* insert the device when new and not present before */
					// HTREEITEM	hItem	= TreeView_GetNextItem(m_hTreeCtrl, hCurrentItem, TVGN_PREVIOUS);
					HTREEITEM	hItem	= m_treeView2.getPrevious(hCurrentItem);
					m_treeView2.insertItem(volumeName, TVI_ROOT, hItem, haveChildren, iIconNormal, iIconNormal);
				}
			}
			else
			{
				int nIconNormal = 0;
				m_treeView2.getFileIcon(drivePathName, &nIconNormal);
				// dbg_log(_T("%s\t nIcon = %d\t volume = %s"), drivePathName, nIconNormal, volumeName);
				m_treeView2.addRoot(volumeName, nIconNormal, haveChildren);
				// m_treeView2.insertItem(volumeName, TVI_ROOT, TVI_LAST, true, false, nIconNormal, nIconNormal);
			}
		}
		else
		{
			if (hCurrentItem != NULL)
			{
				/* get current volume name in list and test if name is changed */
				m_treeView2.getText(hCurrentItem, TEMP, MAX_PATH);
				if (drivePathName[0] == TEMP[0])
				{
					HTREEITEM	pPrevItem	= hCurrentItem;
					// hCurrentItem = TreeView_GetNextItem(m_hTreeCtrl, hCurrentItem, TVGN_NEXT);
					hCurrentItem = m_treeView2.getNext(hCurrentItem);
					// TreeView_DeleteItem(m_hTreeCtrl, pPrevItem);
					m_treeView2.delItem(pPrevItem);
				}
			}
		}
	}
//	dbg_log(_T("init ctrl finish"));
}

BOOL CExplorerDlg::ExploreVolumeInformation(LPCTSTR pszDrivePathName, LPTSTR pszVolumeName, UINT maxSize)
{
	tGetVolumeInfo	volInfo;
	DWORD			dwThreadId		= 0;
	BOOL			isValidDrive	= FALSE;
	HANDLE			hThread			= NULL;

	volInfo.pszDrivePathName	= pszDrivePathName;
	volInfo.pszVolumeName		= pszVolumeName;
	volInfo.maxSize				= maxSize;
	volInfo.pIsValidDrive		= &isValidDrive;


	m_hExploreVolumeThread = ::CreateThread(NULL, 0, GetVolumeInformationTimeoutThread, &volInfo, 0, &dwThreadId);

	if (WAIT_OBJECT_0 != ::WaitForSingleObject(g_hEvent[EID_GET_VOLINFO], 3))
	{
		::TerminateThread(m_hExploreVolumeThread, 0);
		isValidDrive = FALSE;
	}
	::CloseHandle(m_hExploreVolumeThread);
	m_hExploreVolumeThread = NULL;

	return isValidDrive;
}


void CExplorerDlg::NotifyEvent(DWORD event)
{
	switch(event)	
	{
		case EID_INIT :
		{
			/* initial tree */
			UpdateDevices();
			break;
		}
		case EID_UPDATE_DEVICE :
		{
			UpdateDevices();
			break;
		}
		case EID_UPDATE_USER :
		{
			UpdateDevices();
			UpdateFolders();
			::SendMessage(m_hSelf, EXM_UPDATE_PATH, 0, 0);
			break;
		}
		default:
			break;
	}
}

struct FunctorLVItem
{
	bool operator()(const ListViewItem& lhs, const ListViewItem& rhs )
	{
		return FunctoriString()(lhs.m_fileName, rhs.m_fileName);
	}
};
void CExplorerDlg::UpdateFolders(void)
{
	LPTSTR			pszPath			= (LPTSTR)new TCHAR[MAX_PATH];
//	HTREEITEM		hCurrentItem	= TreeView_GetChild(m_hTreeCtrl, TVI_ROOT);
	HTREEITEM		hCurrentItem	= m_treeView2.getChild(TVI_ROOT);
	
	DWORD			serialNr		= 0;
	DWORD			space			= 0;
	DWORD			flags			= 0;

	while (hCurrentItem != NULL)
	{
		m_treeView2.getText(hCurrentItem, pszPath, MAX_PATH);
		
		pszPath[2] = '\\';
		pszPath[3] = '\0';

		//if (m_treeView2.isItemExpand(hCurrentItem))
		{
			//pszPath[2] = '\0';
//			UpdateFolderRecursive(pszPath, hCurrentItem);
            UpDateChildren(pszPath, hCurrentItem, true);
		}
//		hCurrentItem = TreeView_GetNextItem(m_hTreeCtrl, hCurrentItem, TVGN_NEXT);
		hCurrentItem = m_treeView2.getNext(hCurrentItem);
	}

	delete [] pszPath;
}
void CExplorerDlg::UpDateChildren(LPTSTR pszParentPath, HTREEITEM hParentItem, BOOL doRecursive)
{
	CNppFile searchPath(pszParentPath);
    LPWIN32_FIND_DATA lpfindData = NULL;
    std::vector<tstring> vFolderList;
    searchPath.addBackslash();
    lpfindData = searchPath.findFirstFile();
    if( lpfindData != NULL)
    {
        do
        {
            if(IsValidFolder(lpfindData) && !searchPath.findIsHidden() )
            {
                vFolderList.push_back(searchPath.findGetName());
            }
            
            lpfindData = searchPath.findNextFile();
            
        }while(lpfindData != NULL);
    }
    searchPath.findClose();
    std::sort(vFolderList.begin(), vFolderList.end(), FunctoriString());
    // get child item
    HTREEITEM hCurrItem = m_treeView2.getChild(hParentItem);
    TCHAR lpszItem[MAX_PATH] = {0};
    TCHAR szItemPath[MAX_PATH] = {0};
    CNppFile fileTmp;
    
    for(unsigned int i=0; i < vFolderList.size(); ++i)
    {
    	
    	int iIconNormal = 0;
    	int iIconSelect = 0;
    	int iOverloadIcon   = 0;
        if(m_treeView2.getText(hCurrItem, lpszItem, MAX_PATH))
        {
        	// if exist to update or add 
            if(_tcscmp(lpszItem, vFolderList[i].c_str()) == 0)
            {
				// /*
				_stprintf(szItemPath, _T("%s%s"), searchPath.getPath(), lpszItem);
				bool haveChildren = HaveChildDir(szItemPath);
				m_treeView2.getFileIcon(szItemPath, &iIconNormal);
				m_treeView2.setItem(hCurrItem, lpszItem, iIconNormal, haveChildren);
				
				// */
            }
            else
            {
				HTREEITEM hfindItem = IsExistAfter(vFolderList[i].c_str(), hCurrItem);
				if( hfindItem != NULL)
				{
					m_treeView2.delChildren(hfindItem);
					m_treeView2.delItem(hfindItem);
				}
				_stprintf(szItemPath, _T("%s%s"), searchPath.getPath(), lpszItem);
				bool haveChildren = HaveChildDir(szItemPath);
				m_treeView2.getFileIcon(szItemPath, &iIconNormal, &iIconSelect);
				hCurrItem = m_treeView2.addLast(hCurrItem, vFolderList[i].c_str(), iIconNormal, haveChildren);
            }
            /* update recursive */
			if(doRecursive && m_treeView2.isExpanded(hCurrItem) )
			{
				UpDateChildren(szItemPath, hCurrItem, true);
			}
			hCurrItem = m_treeView2.getNext(hCurrItem);
        }
        else
        {
            GetFolderFullPath(hParentItem, szItemPath, vFolderList[i].c_str());
            m_treeView2.getFileIcon(szItemPath, &iIconNormal, &iIconSelect);
            bool haveChildren = HaveChildDir(szItemPath);
            hCurrItem = m_treeView2.addLast(hParentItem, vFolderList[i].c_str(), iIconNormal, haveChildren);
            hCurrItem = m_treeView2.getNext(hCurrItem);
        }
    }
}
bool CExplorerDlg::IsValidFolder(const LPWIN32_FIND_DATA Find)
{
	if ((Find->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
		(!(Find->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) /*|| exProp.bShowHidden*/) &&
		 (_tcscmp(Find->cFileName, _T(".")) != 0) && 
		 (_tcscmp(Find->cFileName, _T("..")) != 0) &&
		 (Find->cFileName[0] != _T('?')))
		return true;

	return false;
}
bool CExplorerDlg::IsValidFile(const LPWIN32_FIND_DATA  lpFind)
{
	if (!(lpFind->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
		(!(lpFind->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)))
		return true;

	return false;
}
HTREEITEM CExplorerDlg::IsExistAfter(LPCTSTR lpszText, HTREEITEM hPrevItem)
{
    HTREEITEM hCurrItem = m_treeView2.getNext(hPrevItem);
    
    TCHAR szCurText[MAX_PATH] = {0};
    bool isFind = false;
    
    while( hCurrItem != NULL)
    {
        if( m_treeView2.getText(hCurrItem, szCurText, MAX_PATH) && (_tcscmp(lpszText, szCurText) == 0) )
        {
            isFind = true;
            //hCurrItem = NULL;
            break;
        }
        else
        {
            hCurrItem = m_treeView2.getNext(hCurrItem);
        }
            
    }
    if(isFind)
    	return hCurrItem;
    else
    	return NULL;
}
void CExplorerDlg::GetFolderFullPath(HTREEITEM hParentItem, LPTSTR lpszFolderFullPath, LPCTSTR lpszChildName)
{
//    lpszFolderFullPath[0] = '\0';
    TCHAR szTmp[MAX_PATH] = {0};
    int nLen = m_treeView2.getItemPath(hParentItem, szTmp);
 
    // "C: [...]"
    if(nLen > 3)
    {
        LPTSTR lpSlash = _tcschr(szTmp, '\\');
        if( lpSlash )
        {
            _stprintf(lpszFolderFullPath, _T("%c:%s"), szTmp[0], lpSlash);
        }
         _tcscat(lpszFolderFullPath, lpszChildName);
        // dbgLog(_T("ItemPath = %s"), lpszFolderFullPath);
    }
   
}

bool CExplorerDlg::HaveChildDir(LPCTSTR lpszPath)
{
	CNppFile fileSearch(lpszPath);
	LPWIN32_FIND_DATA lpFindData = NULL;
	lpFindData = fileSearch.findFirstFile();
	bool bHaveChildDir = false;
	while(lpFindData != NULL)
	{
		if(IsValidFolder(lpFindData))
		{
//			dbg_log("validdir = %s", fileSearch.findGetName());
			bHaveChildDir = true;
			break;
		}
		lpFindData = fileSearch.findNextFile();
	}
	return bHaveChildDir;
}
void CExplorerDlg::UpdateFileListAll(LPCTSTR lpszSelDir, LPCTSTR lpszWildcard)
{
	LPWIN32_FIND_DATA lpfindData = NULL;
	
	ListViewItem lvItem;
	std::vector<ListViewItem> vTmpFolders;
	std::vector<ListViewItem> vTmpFiles;
	
	CNppFile searchFile(lpszSelDir);
	lpfindData = searchFile.findFirstFile(NULL, lpszWildcard);
	while( lpfindData != NULL)
	{
		if( IsValidFile(lpfindData) )
		{
			LPCTSTR lpszExt = NULL;
			int iIconNormal    = 0;
			int iIconSelected  = 0;
			int iIconOverlayed = 0;
			TCHAR szTmpFile[MAX_PATH] = {0};
			_stprintf(szTmpFile, _T("%s%s"), lpszSelDir, searchFile.findGetName());
			lvItem.m_fullPath  = szTmpFile;

			std::map<tstring, bool/*, FunctoriString*/>::iterator iter2 = m_mFileLists.find(lvItem.m_fullPath);
			if( iter2 == m_mFileLists.end() )
			{
				lvItem.m_bIsDir     = false;
				lvItem.m_currentDir = tstring(lpszSelDir);
				lvItem.m_fileName  = searchFile.rmExtension(searchFile.findGetName());
				lpszExt = searchFile.getExtension(searchFile.findGetName());
				if( lpszExt != NULL)
				{
					lvItem.m_fileExt = ++lpszExt;
				}
				else
					lvItem.m_fileExt = _T("");
				
				lvItem.m_filesize   = searchFile.findGetSize(lpfindData);
				m_imgLst.getFileIcon(szTmpFile, &iIconNormal, &iIconSelected, &iIconOverlayed);
				lvItem.m_iIcon      = iIconNormal;
				lvItem.m_iIconOverlay = iIconOverlayed;
				lvItem.m_szfilesize = GetFileSizeFmtStr(lvItem.m_filesize, SFMT_DYNAMIC);
				vTmpFiles.push_back(lvItem);
			}
			
		}
		else if( IsValidFolder(lpfindData) )
		{
			TCHAR szTmpFile[MAX_PATH] = {0};
			int iIconNormal   = 0;
			int iIconSelected = 0;
			int iIconOverlayed = 0;
			lvItem.m_fileName   = tstring(searchFile.findGetName());
			_stprintf(szTmpFile, _T("%s%s\\"), lpszSelDir, lvItem.m_fileName.c_str());
			lvItem.m_fullPath   = szTmpFile;
			
			std::map<tstring, bool/*, FunctoriString*/>::iterator iter2 = m_mFileLists.find(lvItem.m_fullPath);
			if( iter2 == m_mFileLists.end() )
			{
				lvItem.m_bIsDir     = true;
				lvItem.m_fileExt.clear();
				lvItem.m_fileExt    = _T("<DIR>");
				lvItem.m_currentDir = tstring(lpszSelDir);
				lvItem.m_filesize   = 0;//searchFile.findGetSize(lpfindData);
				lvItem.m_szfilesize = _T("");
				m_imgLst.getFileIcon(szTmpFile, &iIconNormal, &iIconSelected, &iIconOverlayed);
				lvItem.m_iIcon      = iIconNormal;
				lvItem.m_iIconOverlay = iIconOverlayed;
				
				vTmpFolders.push_back(lvItem);
			}
			
		}
		
		lpfindData = searchFile.findNextFile();
	}
	std::sort(vTmpFiles.begin(), vTmpFiles.end(), FunctorLVItem());
	std::sort(vTmpFolders.begin(), vTmpFolders.end(), FunctorLVItem());
	m_nlvAllFolders = vTmpFolders.size();
	m_nlvAllItems   = m_nlvAllFolders + vTmpFiles.size();
	m_vListViewAll.clear();
	m_vListViewAll.assign(vTmpFolders.begin(), vTmpFolders.end());
	std::vector<ListViewItem>::iterator iter = vTmpFiles.begin();
	for(; iter != vTmpFiles.end(); ++iter)
	{
		m_vListViewAll.push_back(*iter);
	}
	vTmpFiles.clear();
	vTmpFolders.clear();
	m_listViewAll.clearItem();
	iter = m_vListViewAll.begin();
	for(int i=0; iter != m_vListViewAll.end(); ++iter,++i)
	{
		m_listViewAll.addItem(iter->m_fileName.c_str(), i, iter->m_iIcon);
		m_listViewAll.addSubItem(iter->m_fileExt.c_str(), 1);
		m_listViewAll.addSubItem(iter->m_szfilesize.c_str(), 2);
	}
	
}
BOOL CExplorerDlg::OnClick(NppMsgParams & msg)
{
	if(msg.iCtrlID == IDC_CBO_FILTER)
	{
		
	}
	dbg_log(_T("good done %s"), msg.sCtrlName.getData());
	msg.lResult = TRUE;
	return msg.lResult;
}

BOOL CExplorerDlg::OnBtnAddAll(NppMsgParams & msg)
{
	dbg_log(_T("uMsg = %04X, ctrlName = %s, id = %d"), msg.uMsg, msg.sCtrlName.getData(), msg.iCtrlID);

	msg.lResult = TRUE;
	return msg.lResult;
}
BOOL CExplorerDlg::OnComboxList(NppMsgParams & msg)
{
	dbg_log("combox");
	dbg_log("uMsg = %04X, ctrlName = %s, id = %d", msg.uMsg, msg.sCtrlName.getData(), msg.iCtrlID);
	
	msg.lResult = TRUE;
	return msg.lResult;
}

BOOL CExplorerDlg::OnComboxEdit(NppMsgParams & msg)
{
	dbg_log(_T("OnComboxEdit uMsg = 0x%04X"), msg.uMsg);
	// 13 ENTER
	if (msg.wParam == 13)
	{
		CNppCombox * combox = (CNppCombox * )msg.pSender;
		LPTSTR	pszText	= (LPTSTR)new TCHAR[MAX_PATH];

		combox->getText(pszText);
		combox->addText(pszText);
		::SendMessage(combox->getParent(), EXM_CHANGECOMBO, 0, 0);
		delete [] pszText;
	}
	
	msg.lResult = TRUE;
	return msg.lResult;
}

void CExplorerDlg::UpdateListViews()
{
	m_listViewAll.clearItem();
	//std::sort(m_vListViewAll.begin(), m_vListViewAll.end(), FunctorLVItem());
	std::vector<ListViewItem>::iterator iter = m_vListViewAll.begin();
	for(int i=0; iter != m_vListViewAll.end(); ++iter,++i)
	{
		m_listViewAll.addItem(iter->m_fileName.c_str(), i, iter->m_iIcon);
		m_listViewAll.addSubItem(iter->m_fileExt.c_str(), 1);
		m_listViewAll.addSubItem(iter->m_szfilesize.c_str(), 2);
	}
	m_listViewFiles.clearItem();
	int iCount = 0;
	std::map<tstring, bool/*, FunctoriString*/>::iterator iter2 = m_mFileLists.begin();
	for(int i=0; iter2 != m_mFileLists.end(); ++iter2, ++i)
	{
//		printf("%s\n", iter2->first.c_str());
		if( iter2->second == true)
		{
			m_listViewFiles.addItem(iter2->first.c_str(), i);
			++iCount;
		}
	}
	TCHAR szBuff[32] = {0};
	_sntprintf(szBuff, sizeof(szBuff), _T("No: %d"), iCount);
	m_stCount.setWndText(szBuff);
}
UINT CExplorerDlg::AddFiles2FileList(ListViewItem * plvItem, BOOL bRecurse)
{
	CNppFile fileOp(plvItem->m_fullPath.c_str());
	BOOL bIsDir = fileOp.isDir();
	if( bIsDir && bRecurse)
	{
		m_mFileLists[plvItem->m_fullPath] = false;
		LPWIN32_FIND_DATA lpFindData = fileOp.findFirstFile();
		while(lpFindData != NULL)
		{
			ListViewItem lvItem;
			
			if(IsValidFile(lpFindData) )
			{
				LPCTSTR lpszExt = NULL;
				int iIconNormal    = 0;
				int iIconSelected  = 0;
				int iIconOverlayed = 0;
				TCHAR szTmpFile[MAX_PATH] = {0};
				lvItem.m_bIsDir     = false;
				lvItem.m_currentDir = tstring(fileOp.getFullPath());
				lvItem.m_fileName  = fileOp.rmExtension(fileOp.findGetName());
				_stprintf(szTmpFile, _T("%s%s"), fileOp.getFullPath(), fileOp.findGetName());
				lvItem.m_fullPath  = szTmpFile;
				lpszExt = fileOp.getExtension(fileOp.findGetName());
				if( lpszExt != NULL)
				{
					lvItem.m_fileExt = ++lpszExt;
				}
				else
					lvItem.m_fileExt = _T("");
				
				lvItem.m_filesize   = fileOp.findGetSize(lpFindData);
				m_imgLst.getFileIcon(szTmpFile, &iIconNormal, &iIconSelected, &iIconOverlayed);
				lvItem.m_iIcon      = iIconNormal;
				lvItem.m_iIconOverlay = iIconOverlayed;
				lvItem.m_szfilesize = GetFileSizeFmtStr(lvItem.m_filesize, SFMT_DYNAMIC);
				m_vListViewFiles.push_back(lvItem);
				m_mFileLists[lvItem.m_fullPath] = true;
			}
			else if( IsValidFolder(lpFindData) )
			{
				TCHAR szTmpFile[MAX_PATH] = {0};
				int iIconNormal   = 0;
				int iIconSelected = 0;
				int iIconOverlayed = 0;
				lvItem.m_bIsDir     = true;
				lvItem.m_fileExt.clear();
				lvItem.m_fileExt    = _T("<DIR>");
				lvItem.m_currentDir = fileOp.getFullPath();
				lvItem.m_fileName   = tstring(fileOp.findGetName());
				lvItem.m_filesize   = 0;//searchFile.findGetSize(lpfindData);
				lvItem.m_szfilesize = _T("");
				_stprintf(szTmpFile, _T("%s%s\\"), fileOp.getFullPath(), lvItem.m_fileName.c_str());
				lvItem.m_fullPath   = szTmpFile;
				m_imgLst.getFileIcon(szTmpFile, &iIconNormal, &iIconSelected, &iIconOverlayed);
				lvItem.m_iIcon      = iIconNormal;
				lvItem.m_iIconOverlay = iIconOverlayed;
				AddFiles2FileList(&lvItem, bRecurse);
			}
			lpFindData = fileOp.findNextFile();
			
		}
	}
	else if( bIsDir )
	{
		LPWIN32_FIND_DATA lpFindData = fileOp.findFirstFile();
		while(lpFindData != NULL)
		{
			printf("good jobs: %s\n", fileOp.findGetName());
			if( IsValidFile(lpFindData) )
			{
				ListViewItem lvItem;
				LPCTSTR lpszExt = NULL;
				int iIconNormal    = 0;
				int iIconSelected  = 0;
				int iIconOverlayed = 0;
				TCHAR szTmpFile[MAX_PATH] = {0};
				lvItem.m_bIsDir     = false;
				lvItem.m_currentDir = tstring(fileOp.getFullPath());
				lvItem.m_fileName  = fileOp.rmExtension(fileOp.findGetName());
				_stprintf(szTmpFile, _T("%s%s"), fileOp.getFullPath(), fileOp.findGetName());
				lvItem.m_fullPath  = szTmpFile;
				lpszExt = fileOp.getExtension(fileOp.findGetName());
				if( lpszExt != NULL)
				{
					lvItem.m_fileExt = ++lpszExt;
				}
				else
					lvItem.m_fileExt = _T("");
				
				lvItem.m_filesize   = fileOp.findGetSize(lpFindData);
				m_imgLst.getFileIcon(szTmpFile, &iIconNormal, &iIconSelected, &iIconOverlayed);
				lvItem.m_iIcon      = iIconNormal;
				lvItem.m_iIconOverlay = iIconOverlayed;
				lvItem.m_szfilesize = GetFileSizeFmtStr(lvItem.m_filesize, SFMT_DYNAMIC);
				m_vListViewFiles.push_back(lvItem);
				m_mFileLists[lvItem.m_fullPath] = true;
			}
			lpFindData = fileOp.findNextFile();
		}
	}
	else
	{
		m_vListViewFiles.push_back(*plvItem);
		m_mFileLists[plvItem->m_fullPath] = true;
	}
	return 0;
}