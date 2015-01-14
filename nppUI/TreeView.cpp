// This file is part of Notepad++ project
// Copyright (C)2003 Don HO <don.h@free.fr>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// Note that the GPL places important restrictions on "derived works", yet
// it does not provide a detailed definition of that term.  To avoid      
// misunderstandings, we consider an application to constitute a          
// "derivative work" for the purpose of this license if it does any of the
// following:                                                             
// 1. Integrates source code from Notepad++.
// 2. Integrates/includes/aggregates Notepad++ into a proprietary executable
//    installer, such as those produced by InstallShield.
// 3. Links to a library or executes a program that does any of the above.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#include "TreeView.h"
#include "StdAfx.h"

HTREEITEM TreeView::insertTo(HTREEITEM parent, TCHAR *itemStr, int imgIndex)
{
	TV_INSERTSTRUCT tvinsert;
	tvinsert.hParent=parent;
	tvinsert.hInsertAfter=parent?TVI_LAST:TVI_ROOT;
	tvinsert.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	tvinsert.item.pszText=itemStr;
	tvinsert.item.iImage=imgIndex;
	tvinsert.item.iSelectedImage=0;
	return (HTREEITEM)::SendMessage(_hSelf, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
}

void TreeView::init(HINSTANCE hInst, HWND hPare, HWND hSelf)
{
	Window::init(hInst, hPare);
	if( hSelf == NULL)
	{
		_hSelf = CreateWindowEx(0,
                            WC_TREEVIEW,
                            TEXT("Tree View"),
                            WS_VISIBLE | WS_CHILD | WS_BORDER | 
							TVS_HASLINES | TVS_HASBUTTONS | TVS_SHOWSELALWAYS , 
                            0,  0,  0, 0,
                            _hParent, 
                            NULL, 
                            _hInst, 
                            NULL);
		_bIsCreate = true;
	}
	else
		_hSelf = hSelf;
	if (!_hSelf)
		throw int(56);
	
	// HTREEITEM Parent;           // Tree item handle
	// HTREEITEM Before;           // .......
	// HTREEITEM Root; 
	
    // Get the dimensions of the parent window's client area, and create 
    // the tree-view control. 
    

	// Parent = insertTo(NULL, TEXT("MAOCS30 Command"), 0);
	// Root=Parent;
	// Before=Parent;

	// Parent = insertTo(Parent, TEXT("Native command"), 0);
	// insertTo(Parent, TEXT("Power On"), 0);
	// insertTo(Parent, TEXT("Power off"), 0);
	// insertTo(Parent, TEXT("Entrant"), 0);
	// insertTo(Parent, TEXT("Sortant"), 0);
	// Parent = insertTo(Before, TEXT("Macro"), 0);
	// insertTo(Parent, TEXT("ChangeCode"), 0);
	// insertTo(Parent, TEXT("CipherData"), 0);
	
	// insertTo(NULL, TEXT("Bla bla bla bla..."), 0);
	//display();
}
/*
 * @param: cInitial list初始化大小；cGrow 超过时增大
*/
void TreeView::createImageList(int nWidth, int nHight, UINT flags, int cInitial, int cGrow)
{
	HIMAGELIST himl;
	himl = ::ImageList_Create(nWidth, nHight, flags, cInitial, cGrow);
	if (himl== NULL)
	{
		DbgLog(_T("ImageList_Create failed"));
		return;
	}
	_hImglst = himl;
}
/*@retrn: if failed return -1, success return imagelist index*/

int TreeView::addImageIcon(int iconId)
{
	if(_hImglst == NULL)
	{
		DbgLog(_T("_hImglst == NULL"));
		return -1;
	}
	HICON hIcon = ::LoadIcon(_hInst, MAKEINTRESOURCE(iconId));
	return ::ImageList_AddIcon(_hImglst, hIcon);
}
void TreeView::setImageList(bool bIsSysImageList)
{
	HIMAGELIST himl = NULL;
	if(bIsSysImageList || _hImglst == NULL)
	{
		SHFILEINFO		sfi		= {0};
		himl = (HIMAGELIST)SHGetFileInfo(_T("C:\\"), 0, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	}
	else
	{
		himl = _hImglst;
	}
	::SendMessage(_hSelf, TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)himl);

}
bool TreeView::getItemText(HTREEITEM hItem, LPTSTR szBuf, int bufSize)
{
	TVITEM			tvi;
	tvi.mask		= TVIF_TEXT;
	tvi.hItem		= hItem;
	tvi.pszText		= szBuf;
	tvi.cchTextMax	= bufSize;
	
//	return TreeView_GetItem(_hTreeCtrl, &tvi);
	return (bool)::SendMessage(_hSelf, TVM_GETITEM, 0, (LPARAM)(TV_ITEM *)&tvi);
}
HTREEITEM TreeView::getRootItem()
{
	return getSpecItem(NULL, TVGN_ROOT);
}
HTREEITEM TreeView::getSpecItem(HTREEITEM hitem, UINT flag)
{
	return (HTREEITEM)::SendMessage(_hSelf, TVM_GETNEXTITEM, (WPARAM)flag, (LPARAM)(HTREEITEM)(hitem));
}
HTREEITEM TreeView::getNextItem(HTREEITEM hitem)
{
	return getSpecItem(hitem, TVGN_NEXT);
}
HTREEITEM TreeView::addFolderItem(LPTSTR childFolderName, HTREEITEM parentItem, HTREEITEM insertAfter, bool bChildrenTest)
{
	HTREEITEM pCurrentItem = getSpecItem(parentItem, TVGN_CHILD);
	bool				bHidden			= FALSE;
	WIN32_FIND_DATA		Find			= {0};
	HANDLE				hFind			= NULL;
	
	/* insert item */
	int					iIconNormal		= 0;
	int					iIconSelected	= 0;
	int					iIconOverlayed	= 0;

	extractIcons(childFolderName, NULL, FALSE, &iIconNormal, &iIconSelected, &iIconOverlayed);
	return insertItem(childFolderName, iIconNormal, iIconSelected, iIconOverlayed, bHidden, parentItem, insertAfter, false);
}

HTREEITEM TreeView::insertItem(LPTSTR lpszItem, int nImage, int nSelectedImage, int nOverlayedImage, bool bHidden,	 HTREEITEM hParent, 
								 HTREEITEM hInsertAfter, bool haveChildren, LPARAM lParam)
{
	TV_INSERTSTRUCT tvis;

	ZeroMemory(&tvis, sizeof(TV_INSERTSTRUCT));
	tvis.hParent			 = hParent;
	tvis.hInsertAfter		 = hInsertAfter;
	tvis.item.mask			 = TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_CHILDREN;
	tvis.item.pszText		 = lpszItem;
	tvis.item.iImage		 = nImage;
	tvis.item.iSelectedImage = nSelectedImage;
	tvis.item.cChildren		 = haveChildren;
	tvis.item.lParam		 = lParam;

	if (nOverlayedImage != 0)
	{
		tvis.item.mask		|= TVIF_STATE;
		tvis.item.state		|= INDEXTOOVERLAYMASK(nOverlayedImage);
		tvis.item.stateMask	|= TVIS_OVERLAYMASK;
	}

	if (bHidden == TRUE)
	{
		tvis.item.mask		|= LVIF_STATE;
		tvis.item.state		|= LVIS_CUT;
		tvis.item.stateMask |= LVIS_CUT;
	}

//	return TreeView_InsertItem(_hTreeCtrl, &tvis);
	return (HTREEITEM)::SendMessage(_hSelf, TVM_INSERTITEM, 0, (LPARAM)(LPTV_INSERTSTRUCT)(&tvis));
}

void TreeView::extractIcons(LPCTSTR currentPath, LPCTSTR volumeName, bool isDir, LPINT iIconNormal, LPINT iIconSelected, LPINT iIconOverlayed)
{
	SHFILEINFO		sfi	= {0};
	TCHAR			TEMP[MAX_PATH];

	_tcscpy(TEMP, currentPath);
	if (TEMP[_tcslen(TEMP) - 1] == _T('*'))
	{
		TEMP[_tcslen(TEMP) - 1] = _T('\0');
	}
	else if (TEMP[_tcslen(TEMP) - 1] != _T('\\'))
	{
		_tcscat(TEMP, _T("\\"));
	}

	if (volumeName != NULL)
	{
		_tcscat(TEMP, volumeName);
	}


	/* get normal and overlayed icon */
	if (isDir)
	{
		SHGetFileInfo(TEMP, 0, &sfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_SMALLICON | SHGFI_OVERLAYINDEX);
		if (TEMP[4] == '\0')
		{
			::DestroyIcon(sfi.hIcon);
			SHGetFileInfo(TEMP, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_SMALLICON | SHGFI_OVERLAYINDEX | SHGFI_USEFILEATTRIBUTES);
		}
	}
	else
	{
		SHGetFileInfo(TEMP, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_SMALLICON | SHGFI_OVERLAYINDEX | SHGFI_USEFILEATTRIBUTES);
	}

	*iIconNormal	= sfi.iIcon & 0x000000ff;
	*iIconOverlayed = sfi.iIcon >> 24;
	::DestroyIcon(sfi.hIcon);

	/* get selected icon */
	if (isDir)
	{
		SHGetFileInfo(TEMP, 0, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON);
		*iIconSelected = sfi.iIcon;
	}
	else
	{
		*iIconSelected = 0;
	}
}

