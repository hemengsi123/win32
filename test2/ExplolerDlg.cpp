
#include "StdAfx.h"

HANDLE g_hEvent[EID_MAX]	= {NULL};
HANDLE g_hThread			= NULL;


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

void CExplorerDlg::create(HINSTANCE hInst, int dialogId)
{
	Window::init(hInst, NULL);
	dbg_log(_T("Explore this = %x"), this);
	StaticDialog::create(dialogId, false);
}
void CExplorerDlg::initCtrl()
{
	m_hTreeCtrl = ::GetDlgItem(_hSelf, IDC_TREE_FOLDER);
	m_listCtrlAll  = ::GetDlgItem(_hSelf, IDC_LIST_ALL);
	m_listCtrlFiles = ::GetDlgItem(_hSelf, IDC_LIST_FILES);
	m_filterCtrl    = ::GetDlgItem(_hSelf, IDC_CBO_FILTER);
	m_splitterCtrl  = ::GetDlgItem(_hSelf, IDC_BUTTON_SPLITTER);
	
	m_comBoFilter.init(m_filterCtrl);
	m_comBoFilter.addText(_T("*.*"));
	m_comBoFilter.addText(_T("*.txt"));
	m_comBoFilter.setText(_T("*.*"), 1);
	
//	CNppFile tmpFile(_T("G:\\"));
//	dbg_log(_T("dir = %d"), tmpFile.isEmptyDir());
	// dbg_log(_T("%s"), tmpFile.getFullPath());
	// tmpFile.addBackslash();
	// tmpFile.append(_T("*"));
	// dbg_log(_T("%s"), tmpFile.getFullPath());
	m_treeView2.init(_hInst, _hSelf, m_hTreeCtrl);
	m_treeView2.setImageList(true);
	
    UpdateDevices();
	UpdateFolders();
    /*
	TCHAR szItemPath[MAX_PATH] = {0};
	HTREEITEM himl2 = NULL;
	HTREEITEM himl3 = NULL;
	himl2 = m_treeView2.addFolderItem(_T("C:\\"), TVI_ROOT, TVI_LAST, true);
	dbg_log(_T("child = %x"), m_treeView2.getChild(himl2));
	himl2 = m_treeView2.addFolderItem(_T("Simon"), himl2, TVI_LAST, false);
	m_treeView2.getItemPath(himl2, szItemPath);
	dbg_log(_T("itemPath = %s"), szItemPath);
	himl2 = m_treeView2.getNext(himl2);
	m_treeView2.getItemPath(himl2, szItemPath);
	dbg_log(_T("itemPath = %s\tp= %x"), szItemPath, himl2);
	m_treeView2.addFolderItem(_T("D:\\"), TVI_ROOT, TVI_LAST, true);
	// TreeView_Expand(m_treeView2.getHSelf(), himl2, TVE_EXPAND);
	// TreeView_EnsureVisible(m_treeView2.getHSelf(), himl2);
	TCHAR szTmp[MAX_PATH] = {0};
	HTREEITEM hItem = m_treeView2.getRoot();
	dbg_log(_T("root = %x"), hItem);
	m_treeView2.getSpecItem(hItem, TVGN_CHILD);
	TreeView_SelectItem(m_treeView2.getHSelf(), himl2);
	if(hItem == NULL)
	{
		HTREEITEM		hCurrentItem	= TreeView_GetNextItem(m_hTreeCtrl, TVI_ROOT, TVGN_CHILD);
		dbg_log(_T("hItem == NULL\t%x\tCurr: %x"), m_treeView2.getHSelf(), hCurrentItem);
	}
	else
	{
		m_treeView2.getText(hItem, szTmp, MAX_PATH);
		dbg_log(_T("%s"), szTmp);
	}
	*/

	// m_treeView2.display();
	//
	// HIMAGELIST himlTmp = ::ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 6, 30);
	// ::ImageList_AddIcon(himlTmp, ::LoadIcon(_hInst, MAKEINTRESOURCE(IDI_PARENTFOLDER)));
	// ::ImageList_AddIcon(himlTmp, ::LoadIcon(_hInst, MAKEINTRESOURCE(IDI_FOLDER)));
	// ::ImageList_AddIcon(himlTmp, ::LoadIcon(_hInst, MAKEINTRESOURCE(IDI_FILE)));
	
	// m_hImageListSmall = GetSmallImageList(true);
	// ::SendMessage(m_hTreeCtrl, TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)m_hImageListSmall/*m_hImageListSmall*/);
	
}

BOOL CALLBACK CExplorerDlg::run_dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_INITDIALOG:
		{
			dbg_log(_T("dialog init"));
			initCtrl();
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
					dbg_log(_T("CBN_EDITUPDATE"));
				}
				else if(HIWORD(wParam) == CBN_EDITCHANGE)
				{
					dbg_log(_T("CBN_EDITCHANGE."));
				}
			}
			return TRUE;
		}
		case WM_NOTIFY:
		{
			LPNMHDR		nmhdr = (LPNMHDR)lParam;
			
			if (nmhdr->hwndFrom == m_hTreeCtrl)
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
							HTREEITEM	hItem = TreeView_GetSelection(m_hTreeCtrl);
							// DrawChildren(hItem);
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
			destroy();
			// dbg_log(_T("wm_close"));
			return TRUE;
		}
		// 关闭应用程序
		case WM_DESTROY:
		{
			::PostQuitMessage(0);
			dbg_log(_T("wm_destroy"));
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
	POINT	pt;

    // LONG oldCur = ::SetClassLong(_hSelf, GCL_HCURSOR, (LONG)_hCurWait);
    // ::EnableWindow(_hSelf, FALSE);
	// ::GetCursorPos(&pt);
	// ::SetCursorPos(pt.x, pt.y);

	switch(event)	
	{
		case EID_INIT :
		{
			
			/* initial tree */
			UpdateDevices();

			// /* initilize file list */
			// _FileList.SetToolBarInfo(&_ToolBar , IDM_EX_UNDO, IDM_EX_REDO);

			// /* set data */
			// SelectItem(_pExProp->szCurrentPath);

			// /* initilize combo */
			// _ComboFilter.setComboList(_pExProp->vStrFilterHistory);
			// _ComboFilter.addText("*.*");
			// _ComboFilter.setText((LPTSTR)_pExProp->strLastFilter.c_str());
			// _FileList.filterFiles((LPTSTR)_pExProp->strLastFilter.c_str());

			// /* Update "Go to Folder" icon */
			// NotifyNewFile();

			// /* resize to remove splitter problems */
			// ::SendMessage(_hSelf, WM_SIZE, 0, 0);
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
			::SendMessage(_hSelf, EXM_UPDATE_PATH, 0, 0);
			break;
		}
		default:
			break;
	}

    // ::SetClassLong(_hSelf, GCL_HCURSOR, oldCur);
	// ::EnableWindow(_hSelf, TRUE);
	// ::GetCursorPos(&pt);
	// ::SetCursorPos(pt.x, pt.y);
}

struct StringCompare
{
    bool operator()(const tstring &lhs, const tstring &rhs)const
    {
        return _tcsicmp(lhs.c_str(), rhs.c_str());
    }
} compareFunctor;
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
    std::sort(vFolderList.begin(), vFolderList.end()/*, compareFunctor*/);
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
//        	while(_tcscmp(lpszItem, vFolderList[i].c_str()) != 0 && hCurrItem != NULL)
//        	{
//				// if not exist then add it
//	            if(IsExistAfter(vFolderList[i].c_str(), hCurrItem))
//	            {
//					hCurrItem = m_treeView2.delItem(hCurrItem);
//	            }
//	            else
//	            {
//	                
//	            }
//        	}
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
					dbg_log(_T("test done"))
				}
				_stprintf(szItemPath, _T("%s%s"), searchPath.getPath(), lpszItem);
				bool haveChildren = HaveChildDir(szItemPath);
				m_treeView2.getFileIcon(szItemPath, &iIconNormal);
				hCurrItem = m_treeView2.addLast(hCurrItem, vFolderList[i].c_str(), iIconNormal, haveChildren);
            }
            /* update recursive */
			if(doRecursive && m_treeView2.isItemExpand(hCurrItem) )
			{
				UpDateChildren(szItemPath, hCurrItem, true);
			}
			hCurrItem = m_treeView2.getNext(hCurrItem);
        }
        else
        {
            GetFolderFullPath(hParentItem, szItemPath, vFolderList[i].c_str());
            m_treeView2.getFileIcon(szItemPath, &iIconNormal);
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