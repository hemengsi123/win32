

#ifndef TREE_VIEW_H
#define  TREE_VIEW_H

#include <windows.h>
#include "Window.h"

#ifndef _WIN32_IE
#define _WIN32_IE	0x0600
#endif //_WIN32_IE

#include <commctrl.h>

class TreeView : public Window
{
public:
	typedef HTREEITEM iterator;
public :
	TreeView(): _bIsCreate(false){};
	~TreeView(){};
	virtual void init(HINSTANCE hInst, HWND hPare, HWND hSelf=NULL);
	virtual void destroy() {
		if(_bIsCreate)
			::DestroyWindow(_hSelf);
	};
	void createImageList(int nWidth, int nHight, UINT flags=(ILC_COLOR32 | ILC_MASK), int cInitial=5, int cGrow=10);
	int addImageIcon(int iconId);
	void setImageList(bool bIsSysImageList);
	bool getItemText(HTREEITEM hItem, LPTSTR szBuf, int bufSize = MAX_PATH);
	HTREEITEM getNextItem(HTREEITEM hitem);
	HTREEITEM getSpecItem(HTREEITEM hitem, UINT flag);
	HTREEITEM addFolderItem(LPTSTR childFolderName, HTREEITEM parentItem, HTREEITEM insertAfter, bool bChildrenTest = TRUE);
	BOOL getItemIcon(HTREEITEM hItem, LPINT piIcon, LPINT piSelected, LPINT piOverlay);
	void getFileIcon(LPCTSTR lpszFile, LPINT iIconNormal, LPINT iIconSelected = NULL, LPINT iIconOverlayed = NULL);
	HTREEITEM addRootItem(LPTSTR lpszName, int nImage = 0, int haveChildren = true);
	HTREEITEM addItem(HTREEITEM hParentItem, LPTSTR lpszName, int nImage = 0, int haveChildren = true);
	BOOL delItem(HTREEITEM hItem);
	void delChildren(HTREEITEM hParentItem);
	HTREEITEM getChild(HTREEITEM hParentItem);
	HTREEITEM getParent(HTREEITEM hChildItem);
	HTREEITEM getPrevious(HTREEITEM hCurrItem);
	HTREEITEM getSelect();
	HTREEITEM getRoot();
	BOOL setItem(HTREEITEM hItem, LPTSTR lpszName, int nImage = 0, int haveChildren = true);
	BOOL isItemExpand(HTREEITEM hItem)const;
	void setOverlayIcon(HTREEITEM hItem, INT iOverlayIcon);
	LPARAM getParam(HTREEITEM hItem);
	void setParam(HTREEITEM hItem, LPARAM lParam);
    int getItemPath(HTREEITEM hItem, LPTSTR lpszItemPath);
// protected:
	HTREEITEM insertItem(LPTSTR lpszItem, HTREEITEM hParent, HTREEITEM hInsertAfter = TVI_LAST, int haveChildren = true, bool bHidden = false, int nImage = 0, int nSelectedImage = 0, int nOverlayedImage = 0, LPARAM lParam = NULL);
	BOOL updateItem(HTREEITEM hUpdateItem, LPTSTR lpszItem, int haveChildren = true, bool bHidden = false, int nImage = 0, int nSelectedImage = 0, int nOverlayedImage = -1, LPARAM lParam = NULL, bool delChildren = true); 
private :
	HIMAGELIST _hImglst;
	BOOL _bIsCreate;
	
};
	
#endif
