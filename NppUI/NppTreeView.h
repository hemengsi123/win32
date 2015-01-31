
#ifndef TREE_VIEW_H
#define TREE_VIEW_H

#ifndef _WIN32_IE
#define _WIN32_IE	0x0600
#endif //_WIN32_IE

#include <commctrl.h>
#include "NppWnd.h"

class CNppTreeView : public CNppCtrlWnd
{
public:
	typedef HTREEITEM iterator;
public :
	CNppTreeView(): _hImglst(NULL){};
	virtual ~CNppTreeView(){};
	virtual void destroy();
	virtual LPCTSTR getWndClassName()const;
	LRESULT runCtrlProc(UINT uMsg, WPARAM wParam, LPARAM lParam, bool & bDone);
	void init(HINSTANCE hInst, HWND hParent, UINT iCtrlID, LPCTSTR sCtrlName = NULL);
	HWND create(DWORD dwStyle =  TVS_HASLINES, DWORD dwExStyle = 0);
	HWND create(DWORD dwStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, DWORD dwExStyle = 0);
	bool getText(HTREEITEM hItem, LPTSTR szBuf, int bufSize = MAX_PATH);
	BOOL getItemIcon(HTREEITEM hItem, LPINT piIcon, LPINT piSelected, LPINT piOverlay);
	void getFileIcon(LPCTSTR lpszFile, LPINT iIconNormal, LPINT iIconSelected = NULL, LPINT iIconOverlayed = NULL);
	HIMAGELIST setImageList(HIMAGELIST himl, int iImgLstType = TVSIL_NORMAL);
	HTREEITEM getNext(HTREEITEM hitem);
	HTREEITEM getSpecItem(HTREEITEM hitem, UINT flag);
	HTREEITEM addRoot(LPCTSTR lpszName, int nImage = 0, int haveChildren = true);
	HTREEITEM addLast(HTREEITEM hParentItem, LPCTSTR lpszName, int nImage = 0, int haveChildren = true);
	HTREEITEM addFirst(HTREEITEM hParentItem, LPCTSTR lpszName, int nImage = 0, int haveChildren = true);
	HTREEITEM delItem(HTREEITEM hItem);
	HTREEITEM getChild(HTREEITEM hParentItem);
	HTREEITEM getParent(HTREEITEM hChildItem);
	HTREEITEM getPrevious(HTREEITEM hCurrItem);
	HTREEITEM getSelect();
	HTREEITEM getRoot();
	void delChildren(HTREEITEM hParentItem);
	BOOL clearItem();
	BOOL selectItem(HTREEITEM  hSelItem);
	BOOL setItem(HTREEITEM hItem, LPTSTR lpszName, int nImage = 0, int haveChildren = true);
	BOOL isExpanded(HTREEITEM hItem)const;
	BOOL isSelected(HTREEITEM hItem)const;
	void setOverlayIcon(HTREEITEM hItem, INT iOverlayIcon);
	LPARAM getParam(HTREEITEM hItem);
	void setParam(HTREEITEM hItem, LPARAM lParam);
    int getItemPath(HTREEITEM hItem, LPTSTR lpszItemPath);
// protected:
	HTREEITEM insertItem(LPCTSTR lpszItem, HTREEITEM hParent, HTREEITEM hInsertAfter = TVI_LAST, int haveChildren = true, int nImage = 0, int nSelectedImage = 0, int nOverlayedImage = 0, LPARAM lParam = NULL, bool bHidden = false);
	BOOL updateItem(HTREEITEM hUpdateItem, LPTSTR lpszItem, int haveChildren = true, int nImage = 0, int nSelectedImage = 0, int nOverlayedImage = 0, LPARAM lParam = NULL, bool bHidden = false, bool delChildren = true); 
private :
	HIMAGELIST _hImglst;
	
};
	
#endif
