
#include "StdAfx.h"
#include "TreeView.h"
// #include "nppLib.h"


void TreeView::init(HINSTANCE hInst, HWND hPare, HWND hSelf)
{
	Window::init(hInst, hPare);
	dbg_log(_T("TreeVies this = %x"), this);
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
bool TreeView::getText(HTREEITEM hItem, LPTSTR szBuf, int bufSize)
{
	TVITEM			tvi;
	tvi.mask		= TVIF_TEXT;
	tvi.hItem		= hItem;
	tvi.pszText		= szBuf;
	tvi.cchTextMax	= bufSize;
	
//	return TreeView_GetItem(_hSelf, &tvi);
	return (bool)::SendMessage(_hSelf, TVM_GETITEM, 0, (LPARAM)(TV_ITEM *)&tvi);
}
HTREEITEM TreeView::getSpecItem(HTREEITEM hitem, UINT flag)
{
	return (HTREEITEM)::SendMessage(_hSelf, TVM_GETNEXTITEM, (WPARAM)flag, (LPARAM)(HTREEITEM)(hitem));
}
HTREEITEM TreeView::getNext(HTREEITEM hitem)
{
	return getSpecItem(hitem, TVGN_NEXT);
}

HTREEITEM TreeView::insertItem(LPCTSTR lpszItem, HTREEITEM hParent, HTREEITEM hInsertAfter, int haveChildren, int nImage, int nSelectedImage, int nOverlayedImage, LPARAM lParam, bool bHidden)
{
	TV_INSERTSTRUCT tvis;

	::ZeroMemory(&tvis, sizeof(TV_INSERTSTRUCT));
	tvis.hParent			 = hParent;
	tvis.hInsertAfter		 = hInsertAfter;
	tvis.item.mask			 = TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_CHILDREN;
	tvis.item.pszText		 = (LPTSTR)lpszItem;
	tvis.item.cchTextMax	 = sizeof(tvis.item.pszText)/sizeof(TCHAR); //sizeof(tvis.item.pszText[0]); 
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

//	return TreeView_InsertItem(_hSelf, &tvis);
	return (HTREEITEM)::SendMessage(_hSelf, TVM_INSERTITEM, 0, (LPARAM)(LPTV_INSERTSTRUCT)(&tvis));
}

BOOL TreeView::updateItem(HTREEITEM hUpdateItem, LPTSTR lpszItem, int haveChildren, int nImage, int nSelectedImage, int nOverlayedImage, LPARAM lParam, bool bHidden, bool bDelChildren)
{
	TVITEM		item;
	::ZeroMemory(&item, sizeof(TVITEM));
	
	item.hItem			 = hUpdateItem;
	item.mask			 = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_CHILDREN;
	item.pszText		 = lpszItem;
	item.cchTextMax	     = sizeof(item.pszText)/sizeof(TCHAR);
	item.iImage			 = nImage;
	item.iSelectedImage	 = nSelectedImage;
	item.cChildren		 = haveChildren;
	
	if( lParam != NULL)
	{
		item.mask			|= TVIF_PARAM;
		item.lParam			 = lParam;
	}
	/* update overlay icon in any case */
	if( nOverlayedImage != 0)
	{
		item.mask			|= TVIF_STATE;
		item.state			 = INDEXTOOVERLAYMASK(nOverlayedImage);
		item.stateMask		 = TVIS_OVERLAYMASK;
	}

	/* mark as cut if the icon is hidden */
	if (bHidden == true)
	{
	    item.mask		|= LVIF_STATE;
		item.state		|= LVIS_CUT;
		item.stateMask  |= LVIS_CUT;
	}

	/* delete children items when available but not needed */
	if ((haveChildren == true) && bDelChildren && TreeView_GetChild(_hSelf, hUpdateItem))	
	{
		delChildren(hUpdateItem);
	}

	// return TreeView_SetItem(_hSelf, &item);
	return (BOOL)::SendMessage(_hSelf, TVM_SETITEM, 0, (LPARAM)(const TV_ITEM *)(&item));
}

BOOL TreeView::getItemIcon(HTREEITEM hItem, LPINT piIcon, LPINT piSelected, LPINT piOverlay)
{
	if ((piIcon == NULL) || (piSelected == NULL) || (piOverlay == NULL))
		return FALSE;

	TVITEM			tvi;
	tvi.mask		= TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvi.hItem		= hItem;
	tvi.stateMask	= TVIS_OVERLAYMASK;

	BOOL bRet = TreeView_GetItem(_hSelf, &tvi);

	if (bRet) {
		*piIcon			= tvi.iImage;
		*piSelected		= tvi.iSelectedImage;
		*piOverlay		= (tvi.state >> 8) & 0xFF;
	}

	return bRet;
}
void TreeView::getFileIcon(LPCTSTR lpszFile, LPINT iIconNormal, LPINT iIconSelected, LPINT iIconOverlayed)
{
	if( iIconNormal == NULL)
		return;
	SHFILEINFO		sfi	= {0};
	bool isDir = false;
	// TCHAR			TEMP[MAX_PATH];
	CNppFile tmpFile(lpszFile);
	if(tmpFile.isDir())
	{
		tmpFile.addBackslash();
		isDir = true;
		SHGetFileInfo(tmpFile.getFullPath(), 0, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON);
	}
	else
	{
		::SHGetFileInfo(tmpFile.getFullPath(), FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_SMALLICON | SHGFI_OVERLAYINDEX | SHGFI_USEFILEATTRIBUTES);
	}
	*iIconNormal	= sfi.iIcon & 0x000000ff;
	if( isDir && iIconSelected)
	{
		*iIconSelected = sfi.iIcon;
	}
	else if(iIconSelected)
	{
		*iIconSelected = *iIconNormal;
	}
	if( iIconOverlayed )
		*iIconOverlayed = sfi.iIcon >> 24;
	::DestroyIcon(sfi.hIcon);
}

HTREEITEM TreeView::addRoot(LPCTSTR lpszName, int nImage, int haveChildren)
{
	return insertItem(lpszName, TVI_ROOT, TVI_LAST, haveChildren, nImage, nImage);
}
HTREEITEM TreeView::addLast(HTREEITEM hParentItem, LPCTSTR lpszName, int nImage, int haveChildren)
{
	return insertItem(lpszName, hParentItem, TVI_LAST, haveChildren, nImage, nImage);
}
HTREEITEM TreeView::addFirst(HTREEITEM hParentItem, LPCTSTR lpszName, int nImage, int haveChildren)
{
	return insertItem(lpszName, hParentItem, TVI_FIRST, haveChildren, nImage, nImage);
}
HTREEITEM TreeView::delItem(HTREEITEM hItem)
{
	HTREEITEM hNextItem = TreeView_GetNextItem(_hSelf, hItem, TVGN_NEXT);
	if(TreeView_DeleteItem(_hSelf, hItem) == TRUE)
		return hNextItem;
	else
		return NULL;
}
HTREEITEM TreeView::getChild(HTREEITEM hParentItem)
{
	return getSpecItem(hParentItem, TVGN_CHILD);
}
HTREEITEM TreeView::getParent(HTREEITEM hChildItem)
{
	return getSpecItem(hChildItem, TVGN_PARENT);
}
HTREEITEM TreeView::getPrevious(HTREEITEM hCurrItem)
{
	return getSpecItem(hCurrItem, TVGN_PREVIOUS);
}
HTREEITEM TreeView::getSelect()
{
	return getSpecItem(NULL,  TVGN_CARET);
}
HTREEITEM TreeView::getRoot()
{
	return getSpecItem(NULL, TVGN_ROOT);
}
void TreeView::delChildren(HTREEITEM hParentItem)
{
	HTREEITEM pCurrentItem = TreeView_GetNextItem(_hSelf, hParentItem, TVGN_CHILD);
	// HTREEITEM	pCurrentItem = getChild(hParentItem);
	while (pCurrentItem != NULL)
	{
		TreeView_DeleteItem(_hSelf, pCurrentItem);
		// delItem(pCurrentItem);
		pCurrentItem = TreeView_GetNextItem(_hSelf, hParentItem, TVGN_CHILD);
	}
}

BOOL TreeView::setItem(HTREEITEM hItem, LPTSTR lpszName, int nImage, int haveChildren)
{
	return updateItem(hItem, lpszName, haveChildren, nImage, nImage);
}
BOOL TreeView::isItemExpand(HTREEITEM hItem)const
{
	return (BOOL)(TreeView_GetItemState(_hSelf, hItem, TVIS_EXPANDED) & TVIS_EXPANDED);
}
void TreeView::setOverlayIcon(HTREEITEM hItem, INT iOverlayIcon)
{
	/* Note: No LOCK */
	TVITEM		item;

	ZeroMemory(&item, sizeof(TVITEM));
	item.hItem			 = hItem;
	item.mask			 = TVIF_STATE;
	item.state			|= INDEXTOOVERLAYMASK(iOverlayIcon);
	item.stateMask		|= TVIS_OVERLAYMASK;

	TreeView_SetItem(_hSelf, &item);
}

LPARAM TreeView::getParam(HTREEITEM hItem)
{
	TVITEM			tvi;
	tvi.mask		= TVIF_PARAM;
	tvi.hItem		= hItem;
	tvi.lParam		= 0;
	
	TreeView_GetItem(_hSelf, &tvi);

	return tvi.lParam;
}

void TreeView::setParam(HTREEITEM hItem, LPARAM lParam)
{
	TVITEM		item;

	ZeroMemory(&item, sizeof(TVITEM));
	item.hItem			 = hItem;
	item.mask			 = TVIF_PARAM;
	item.lParam			 = lParam;

	TreeView_SetItem(_hSelf, &item);
}
int TreeView::getItemPath(HTREEITEM hItem, LPTSTR lpszItemPath)
{
    int nPathLen = 0;
    if( hItem == TVI_ROOT)
        return nPathLen;
    
    lpszItemPath[0] = '\0';
    TCHAR szTmp[MAX_PATH]         = {0};
    TCHAR szCurItemText[MAX_PATH] = {0};
    while( hItem != NULL)
    {
        getText(hItem, szCurItemText, MAX_PATH);
        _stprintf(szTmp, _T("%s\\%s"), szCurItemText, lpszItemPath);
        _tcscpy(lpszItemPath, szTmp);
        hItem = getParent(hItem);
    }

    nPathLen = _tcslen(lpszItemPath);
    // remove last '\'
//    if( nPathLen > 0)
//        lpszItemPath[--nPathLen] = '\0';
    
    return nPathLen;
}
