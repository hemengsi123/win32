
#include "NppLib.h"
#include "NppTreeView.h"

LPCTSTR CNppTreeView::getWndClassName()const
{
	return _T("SysTreeView32");// WC_TREEVIEW;
}
void CNppTreeView::init(HINSTANCE hInst, HWND hParent, UINT iCtrlIDs)
{
	CNppCtrlWnd::init(hInst, hParent, iCtrlIDs);
}
HWND CNppTreeView::create(DWORD dwStyle, DWORD dwExStyle)
{
	CNppCtrlWnd::create(dwStyle, dwExStyle, NULL);
	setWndProc();
	return m_hSelf;
}
HWND CNppTreeView::create(DWORD dwStyle, int x, int y, int cx, int cy, DWORD dwExStyle)
{
	return CNppCtrlWnd::create(NULL, dwStyle, x, y, cx, cy, dwExStyle);
}
LRESULT CNppTreeView::runCtrlProc(UINT uMsg, WPARAM wParam, LPARAM lParam, bool & bDone)
{
	bDone = false;
	
	return CNppCtrlWnd::runCtrlProc(uMsg, wParam, lParam, bDone);
}

HIMAGELIST CNppTreeView::setImageList(HIMAGELIST himl, int iImgLstType)
{
//	::SendMessage(m_hSelf, TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)himl);
	return TreeView_SetImageList(m_hSelf, himl, iImgLstType);
}
bool CNppTreeView::getText(HTREEITEM hItem, LPTSTR szBuf, int bufSize)
{
	TVITEM			tvi;
	tvi.mask		= TVIF_TEXT;
	tvi.hItem		= hItem;
	tvi.pszText		= szBuf;
	tvi.cchTextMax	= bufSize;
	
//	return TreeView_GetItem(m_hSelf, &tvi);
	return static_cast<bool>(::SendMessage(m_hSelf, TVM_GETITEM, 0, (LPARAM)(TV_ITEM *)&tvi));
}
HTREEITEM CNppTreeView::getSpecItem(HTREEITEM hitem, UINT flag)
{
	return (HTREEITEM)::SendMessage(m_hSelf, TVM_GETNEXTITEM, (WPARAM)flag, (LPARAM)(HTREEITEM)(hitem));
}
HTREEITEM CNppTreeView::getNext(HTREEITEM hitem)
{
	return getSpecItem(hitem, TVGN_NEXT);
}

HTREEITEM CNppTreeView::insertItem(LPCTSTR lpszItem, HTREEITEM hParent, HTREEITEM hInsertAfter, int haveChildren, int nImage, int nSelectedImage, int nOverlayedImage, LPARAM lParam, bool bHidden)
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

//	return TreeView_InsertItem(m_hSelf, &tvis);
	return (HTREEITEM)::SendMessage(m_hSelf, TVM_INSERTITEM, 0, (LPARAM)(LPTV_INSERTSTRUCT)(&tvis));
}

BOOL CNppTreeView::updateItem(HTREEITEM hUpdateItem, LPTSTR lpszItem, int haveChildren, int nImage, int nSelectedImage, int nOverlayedImage, LPARAM lParam, bool bHidden, bool bDelChildren)
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
	if ((haveChildren == true) && bDelChildren && TreeView_GetChild(m_hSelf, hUpdateItem))	
	{
		delChildren(hUpdateItem);
	}

	// return TreeView_SetItem(m_hSelf, &item);
	return (BOOL)::SendMessage(m_hSelf, TVM_SETITEM, 0, (LPARAM)(const TV_ITEM *)(&item));
}

BOOL CNppTreeView::getItemIcon(HTREEITEM hItem, LPINT piIcon, LPINT piSelected, LPINT piOverlay)
{
	if ((piIcon == NULL) || (piSelected == NULL) || (piOverlay == NULL))
		return FALSE;

	TVITEM			tvi;
	tvi.mask		= TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvi.hItem		= hItem;
	tvi.stateMask	= TVIS_OVERLAYMASK;

	BOOL bRet = TreeView_GetItem(m_hSelf, &tvi);

	if (bRet) {
		*piIcon			= tvi.iImage;
		*piSelected		= tvi.iSelectedImage;
		*piOverlay		= (tvi.state >> 8) & 0xFF;
	}

	return bRet;
}
void CNppTreeView::getFileIcon(LPCTSTR lpszFile, LPINT iIconNormal, LPINT iIconSelected, LPINT iIconOverlayed)
{
	if( iIconNormal == NULL)
		return;
	SHFILEINFO sfi = {0};
	bool isDir     = false;
	int tmpIconOverlayed = iIconOverlayed ? SHGFI_OVERLAYINDEX : 0;
	// TCHAR			TEMP[MAX_PATH];
	CNppFile tmpFile(lpszFile);
	
	if(tmpFile.isDir())
	{
		tmpFile.addBackslash();
		isDir = true;
		::SHGetFileInfo(tmpFile.getFullPath(), -1, &sfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_SMALLICON | tmpIconOverlayed);
	}
	else
	{
		::SHGetFileInfo(tmpFile.getFullPath(), FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_SMALLICON | tmpIconOverlayed | SHGFI_USEFILEATTRIBUTES);
	}
	*iIconNormal	= sfi.iIcon & 0x000000ff;
	if( iIconOverlayed )
		*iIconOverlayed = sfi.iIcon >> 24;
	::DestroyIcon(sfi.hIcon);
	if( isDir && iIconSelected)
	{
		::ZeroMemory(&sfi, sizeof(SHFILEINFO));
		::SHGetFileInfo(lpszFile, 0, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON);
		::DestroyIcon(sfi.hIcon);
		*iIconSelected = sfi.iIcon;
	}
	else if(iIconSelected)
	{
		*iIconSelected = *iIconNormal;
	}
}

HTREEITEM CNppTreeView::addRoot(LPCTSTR lpszName, int nImage, int haveChildren)
{
	return insertItem(lpszName, TVI_ROOT, TVI_LAST, haveChildren, nImage, nImage);
}
HTREEITEM CNppTreeView::addLast(HTREEITEM hParentItem, LPCTSTR lpszName, int nImage, int haveChildren)
{
	return insertItem(lpszName, hParentItem, TVI_LAST, haveChildren, nImage, nImage);
}
HTREEITEM CNppTreeView::addFirst(HTREEITEM hParentItem, LPCTSTR lpszName, int nImage, int haveChildren)
{
	return insertItem(lpszName, hParentItem, TVI_FIRST, haveChildren, nImage, nImage);
}
HTREEITEM CNppTreeView::delItem(HTREEITEM hItem)
{
	HTREEITEM hNextItem = TreeView_GetNextItem(m_hSelf, hItem, TVGN_NEXT);
	if(TreeView_DeleteItem(m_hSelf, hItem) == TRUE)
		return hNextItem;
	else
		return NULL;
}
HTREEITEM CNppTreeView::getChild(HTREEITEM hParentItem)
{
	return getSpecItem(hParentItem, TVGN_CHILD);
}
HTREEITEM CNppTreeView::getParent(HTREEITEM hChildItem)
{
	return getSpecItem(hChildItem, TVGN_PARENT);
}
HTREEITEM CNppTreeView::getPrevious(HTREEITEM hCurrItem)
{
	return getSpecItem(hCurrItem, TVGN_PREVIOUS);
}
HTREEITEM CNppTreeView::getSelect()
{
	return getSpecItem(NULL,  TVGN_CARET);
}
HTREEITEM CNppTreeView::getRoot()
{
	return getSpecItem(NULL, TVGN_ROOT);
}
BOOL CNppTreeView::clearItem()
{
	return TreeView_DeleteAllItems(m_hSelf);
}
BOOL CNppTreeView::selectItem(HTREEITEM  hSelItem)
{
	return TreeView_SelectItem(m_hSelf, hSelItem);
}
void CNppTreeView::delChildren(HTREEITEM hParentItem)
{
	HTREEITEM pCurrentItem = TreeView_GetNextItem(m_hSelf, hParentItem, TVGN_CHILD);
	// HTREEITEM	pCurrentItem = getChild(hParentItem);
	while (pCurrentItem != NULL)
	{
		TreeView_DeleteItem(m_hSelf, pCurrentItem);
		// delItem(pCurrentItem);
		pCurrentItem = TreeView_GetNextItem(m_hSelf, hParentItem, TVGN_CHILD);
	}
}

BOOL CNppTreeView::setItem(HTREEITEM hItem, LPTSTR lpszName, int nImage, int haveChildren)
{
	return updateItem(hItem, lpszName, haveChildren, nImage, nImage);
}
BOOL CNppTreeView::isExpanded(HTREEITEM hItem)const
{
	return (BOOL)(TreeView_GetItemState(m_hSelf, hItem, TVIS_EXPANDED) & TVIS_EXPANDED);
}
BOOL CNppTreeView::isSelected(HTREEITEM hItem)const
{
	return (BOOL)(TreeView_GetItemState(m_hSelf, hItem, TVIS_SELECTED) & TVIS_SELECTED);
}
void CNppTreeView::setOverlayIcon(HTREEITEM hItem, INT iOverlayIcon)
{
	/* Note: No LOCK */
	TVITEM		item;

	ZeroMemory(&item, sizeof(TVITEM));
	item.hItem			 = hItem;
	item.mask			 = TVIF_STATE;
	item.state			|= INDEXTOOVERLAYMASK(iOverlayIcon);
	item.stateMask		|= TVIS_OVERLAYMASK;

	TreeView_SetItem(m_hSelf, &item);
}

LPARAM CNppTreeView::getParam(HTREEITEM hItem)
{
	TVITEM			tvi;
	tvi.mask		= TVIF_PARAM;
	tvi.hItem		= hItem;
	tvi.lParam		= 0;
	
	TreeView_GetItem(m_hSelf, &tvi);

	return tvi.lParam;
}

void CNppTreeView::setParam(HTREEITEM hItem, LPARAM lParam)
{
	TVITEM		item;

	ZeroMemory(&item, sizeof(TVITEM));
	item.hItem			 = hItem;
	item.mask			 = TVIF_PARAM;
	item.lParam			 = lParam;

	TreeView_SetItem(m_hSelf, &item);
}
int CNppTreeView::getItemPath(HTREEITEM hItem, LPTSTR lpszItemPath)
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
        _sntprintf(szTmp, MAX_PATH, _T("%s\\%s"), szCurItemText, lpszItemPath);
        _tcscpy(lpszItemPath, szTmp);
        hItem = getParent(hItem);
    }

    nPathLen = _tcslen(lpszItemPath);
    // remove last '\'
//    if( nPathLen > 0)
//        lpszItemPath[--nPathLen] = '\0';
    
    return nPathLen;
}

void CNppTreeView::destroy() 
{
	if(isCreated() && m_hSelf)
		::DestroyWindow(m_hSelf);
}
