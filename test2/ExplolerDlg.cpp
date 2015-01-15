
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
	
	CNppFile tmpFile(_T("G:\\"));
	dbg_log(_T("dir = %d"), tmpFile.isEmptyDir());
	// dbg_log(_T("%s"), tmpFile.getFullPath());
	// tmpFile.addBackslash();
	// tmpFile.append(_T("*"));
	// dbg_log(_T("%s"), tmpFile.getFullPath());
	// m_treeView.init(m_hTreeCtrl);
	m_treeView2.init(_hInst, _hSelf, m_hTreeCtrl);
	m_treeView2.setImageList(true);
	
	UpdateDevices();
	/*
	HTREEITEM himl2 = NULL;
	HTREEITEM himl3 = NULL;
	himl2 = m_treeView2.addFolderItem(_T("C:\\"), TVI_ROOT, TVI_LAST, true);
	dbg_log(_T("%x"), himl2);
	m_treeView2.addFolderItem(_T("E:\\Simon\\projects\\win32\\common\\"), himl2, TVI_LAST, false);
	m_treeView2.addFolderItem(_T("D:\\"), TVI_ROOT, TVI_LAST, true);
	// TreeView_Expand(m_treeView2.getHSelf(), himl2, TVE_EXPAND);
	// TreeView_EnsureVisible(m_treeView2.getHSelf(), himl2);
	TCHAR szTmp[MAX_PATH] = {0};
	HTREEITEM hItem = m_treeView2.getRootItem();
	m_treeView2.getSpecItem(hItem, TVGN_CHILD);
	TreeView_SelectItem(m_treeView2.getHSelf(), himl2);
	if(hItem == NULL)
	{
		HTREEITEM		hCurrentItem	= TreeView_GetNextItem(m_hTreeCtrl, TVI_ROOT, TVGN_CHILD);
		dbg_log(_T("hItem == NULL\t%x\tCurr: %x"), m_treeView2.getHSelf(), hCurrentItem);
	}
	else
	{
		m_treeView2.getItemText(hItem, szTmp, MAX_PATH);
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
void CExplorerDlg::upDateFolder()
{
	
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
						dbg_log(_T("TVN_ITEMEXPANDING"));
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
	
	HTREEITEM		hCurrentItem	= m_treeView2.getRootItem();
	dbg_log(_T("rootItem = %x"), hCurrentItem);
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
			haveChildren = (!tmpFile.isEmptyDir());
			// dbg_log(_T("%s chidren = %d"), drivePathName, haveChildren);
			if (hCurrentItem != NULL)
			{
				int			iIconNormal		= 0;
				int			iIconSelected	= 0;
				int			iIconOverlayed	= 0;

				/* get current volume name in list and test if name is changed */
				m_treeView2.getItemText(hCurrentItem, TEMP, MAX_PATH);

				if (_tcscmp(volumeName, TEMP) == 0)
				{
					/* if names are equal, go to next item in tree */
					//m_treeView.ExtractIcons(drivePathName, NULL, DEVT_DRIVE, &iIconNormal, &iIconSelected, &iIconOverlayed);
					m_treeView2.getFileIcon(drivePathName, &iIconNormal, &iIconSelected, &iIconOverlayed);
					m_treeView.UpdateItem(hCurrentItem, volumeName, iIconNormal, iIconSelected, iIconOverlayed, 0, haveChildren);
					hCurrentItem = m_treeView2.getNextItem(hCurrentItem);
				}
				else if (volumeName[0] == TEMP[0])
				{
					/* if names are not the same but the drive letter are equal, rename item */

					/* get icons */
					m_treeView.ExtractIcons(drivePathName, NULL, DEVT_DRIVE, &iIconNormal, &iIconSelected, &iIconOverlayed);
					m_treeView.UpdateItem(hCurrentItem, volumeName, iIconNormal, iIconSelected, iIconOverlayed, 0, haveChildren);
					m_treeView.DeleteChildren(hCurrentItem);
					hCurrentItem = TreeView_GetNextItem(m_hTreeCtrl, hCurrentItem, TVGN_NEXT);
				}
				else
				{
					/* insert the device when new and not present before */
					HTREEITEM	hItem	= TreeView_GetNextItem(m_hTreeCtrl, hCurrentItem, TVGN_PREVIOUS);
					m_treeView.InsertChildFolder(volumeName, TVI_ROOT, hItem, isValidDrive);
				}
			}
			else
			{
				int nIconNormal = 0;
				m_treeView2.getFileIcon(drivePathName, &nIconNormal);
				// dbg_log(_T("%s\t nIcon = %d\t volume = %s"), drivePathName, nIconNormal, volumeName);
				m_treeView2.addRootItem(volumeName, nIconNormal, haveChildren);
				// m_treeView2.insertItem(volumeName, TVI_ROOT, TVI_LAST, true, false, nIconNormal, nIconNormal);
				//m_treeView.InsertChildFolder(volumeName, TVI_ROOT, TVI_LAST, isValidDrive);
			}
		}
		else
		{
			if (hCurrentItem != NULL)
			{
				/* get current volume name in list and test if name is changed */
				m_treeView2.getItemText(hCurrentItem, TEMP, MAX_PATH);
				if (drivePathName[0] == TEMP[0])
				{
					HTREEITEM	pPrevItem	= hCurrentItem;
					// hCurrentItem = TreeView_GetNextItem(m_hTreeCtrl, hCurrentItem, TVGN_NEXT);
					hCurrentItem = m_treeView2.getNextItem(hCurrentItem);
					// TreeView_DeleteItem(m_hTreeCtrl, pPrevItem);
					m_treeView2.delItem(pPrevItem);
				}
			}
		}
	}

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


void CExplorerDlg::UpdateFolders(void)
{
	LPTSTR			pszPath			= (LPTSTR)new TCHAR[MAX_PATH];
	HTREEITEM		hCurrentItem	= TreeView_GetChild(m_hTreeCtrl, TVI_ROOT);
	DWORD			serialNr		= 0;
	DWORD			space			= 0;
	DWORD			flags			= 0;

	while (hCurrentItem != NULL)
	{
		m_treeView.GetItemText(hCurrentItem, pszPath, MAX_PATH);
		pszPath[2] = '\\';
		pszPath[3] = '\0';

		if (GetVolumeInformation(pszPath, NULL, 0, &serialNr, &space, &flags, NULL, 0))
		{
			pszPath[2] = '\0';
			UpdateFolderRecursive(pszPath, hCurrentItem);
		}
		hCurrentItem = TreeView_GetNextItem(m_hTreeCtrl, hCurrentItem, TVGN_NEXT);
	}

	delete [] pszPath;
}

void CExplorerDlg::UpdateFolderRecursive(LPTSTR pszParentPath, HTREEITEM hParentItem)
{
	WIN32_FIND_DATA		Find			= {0};
	HANDLE				hFind			= NULL;
	TVITEM				item			= {0};
	
	vector<tItemList>	vFolderList;
	tItemList			listElement;
	LPTSTR				pszItem			= (LPTSTR) new TCHAR[MAX_PATH];
	LPTSTR				pszPath			= (LPTSTR) new TCHAR[MAX_PATH];
	LPTSTR				pszSearch		= (LPTSTR) new TCHAR[MAX_PATH];
	HTREEITEM			hCurrentItem	= TreeView_GetNextItem(m_hTreeCtrl, hParentItem, TVGN_CHILD);

	strcpy(pszSearch, pszParentPath);

	/* add wildcard */
	strcat(pszSearch, "\\*");

	if ((hFind = ::FindFirstFile(pszSearch, &Find)) == INVALID_HANDLE_VALUE)
	{
		delete [] pszItem;
		delete [] pszPath;
		delete [] pszSearch;
		return;
	}

	/* find folders */
	do
	{
		if (IsValidFolder(Find) == TRUE)
		{
			listElement.strName			= Find.cFileName;
			listElement.dwAttributes	= Find.dwFileAttributes;
			vFolderList.push_back(listElement);
		}
	} while (FindNextFile(hFind, &Find));

	::FindClose(hFind);

	/* sort data */
	// m_treeView.QuickSortItems(&vFolderList, 0, vFolderList.size()-1);

	/* update tree */
	for (size_t i = 0; i < vFolderList.size(); i++)
	{
		if (m_treeView.GetItemText(hCurrentItem, pszItem, MAX_PATH) == TRUE)
		{
			/* compare current item and the current folder name */
			while ((strcmp(pszItem, vFolderList[i].strName.c_str()) != 0) && (hCurrentItem != NULL))
			{
				HTREEITEM	pPrevItem = NULL;

				/* if it's not equal delete or add new item */
				if (FindFolderAfter((LPTSTR)vFolderList[i].strName.c_str(), hCurrentItem) == TRUE)
				{
					pPrevItem		= hCurrentItem;
					hCurrentItem	= TreeView_GetNextItem(m_hTreeCtrl, hCurrentItem, TVGN_NEXT);
					TreeView_DeleteItem(m_hTreeCtrl, pPrevItem);
				}
				else
				{
					pPrevItem = TreeView_GetNextItem(m_hTreeCtrl, hCurrentItem, TVGN_PREVIOUS);

					/* Note: If hCurrentItem is the first item in the list pPrevItem is NULL */
					if (pPrevItem == NULL)
						hCurrentItem = m_treeView.InsertChildFolder((LPTSTR)vFolderList[i].strName.c_str(), hParentItem, TVI_FIRST);
					else
						hCurrentItem = m_treeView.InsertChildFolder((LPTSTR)vFolderList[i].strName.c_str(), hParentItem, pPrevItem);
				}

				if (hCurrentItem != NULL)
					m_treeView.GetItemText(hCurrentItem, pszItem, MAX_PATH);
			}

			/* get current path */
			sprintf(pszPath, "%s\\%s", pszParentPath, pszItem);

			/* update icons and expandable information */
			int					iIconNormal		= 0;
			int					iIconSelected	= 0;
			int					iIconOverlayed	= 0;
			BOOL				haveChildren	= m_treeView.HaveChildren(pszPath);
			BOOL				bHidden			= FALSE;

			/* correct by HaveChildren() modified pszPath */
			pszPath[strlen(pszPath) - 2] = '\0';

			/* get icons and update item */
			m_treeView.ExtractIcons(pszPath, NULL, true, &iIconNormal, &iIconSelected, &iIconOverlayed);
			bHidden = ((vFolderList[i].dwAttributes & FILE_ATTRIBUTE_HIDDEN) != 0);
			m_treeView.UpdateItem(hCurrentItem, pszItem, iIconNormal, iIconSelected, iIconOverlayed, bHidden, haveChildren);

			/* update recursive */
			if (TreeView_GetChild(m_hTreeCtrl, hCurrentItem) != NULL)
			{
				UpdateFolderRecursive(pszPath, hCurrentItem);
			}

			/* select next item */
			hCurrentItem = TreeView_GetNextItem(m_hTreeCtrl, hCurrentItem, TVGN_NEXT);
		}
		else
		{
			hCurrentItem = m_treeView.InsertChildFolder((LPTSTR)vFolderList[i].strName.c_str(), hParentItem);
			hCurrentItem = TreeView_GetNextItem(m_hTreeCtrl, hCurrentItem, TVGN_NEXT);
		}
	}

	/* delete possible not existed items */
	while (hCurrentItem != NULL)
	{
		HTREEITEM	pPrevItem	= hCurrentItem;
		hCurrentItem			= TreeView_GetNextItem(m_hTreeCtrl, hCurrentItem, TVGN_NEXT);
		TreeView_DeleteItem(m_hTreeCtrl, pPrevItem);
	}

	delete [] pszItem;
	delete [] pszPath;
	delete [] pszSearch;
}

BOOL CExplorerDlg::FindFolderAfter(LPTSTR itemName, HTREEITEM pAfterItem)
{
	BOOL		isFound			= FALSE;
	LPTSTR		pszItem			= (LPTSTR) new TCHAR[MAX_PATH];
	HTREEITEM	hCurrentItem	= TreeView_GetNextItem(m_hTreeCtrl, pAfterItem, TVGN_NEXT);

	while (hCurrentItem != NULL)
	{
		m_treeView.GetItemText(hCurrentItem, pszItem, MAX_PATH);
		if (strcmp(itemName, pszItem) == 0)
		{
			isFound = TRUE;
			hCurrentItem = NULL;
		}
		else
		{
			hCurrentItem = TreeView_GetNextItem(m_hTreeCtrl, hCurrentItem, TVGN_NEXT);
		}
	}

	return isFound;
}
