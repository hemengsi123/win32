

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
	bool getItemText(HTREEITEM hItem, LPTSTR szBuf, int bufSize);
	HTREEITEM getRootItem();
	HTREEITEM getNextItem(HTREEITEM hitem);
	HTREEITEM getSpecItem(HTREEITEM hitem, UINT flag);
	HTREEITEM addFolderItem(LPTSTR childFolderName, HTREEITEM parentItem, HTREEITEM insertAfter, bool bChildrenTest = TRUE);
protected:
		HTREEITEM insertTo(HTREEITEM parent, TCHAR *itemStr, int imgIndex);
		HTREEITEM insertItem(LPTSTR lpszItem, int nImage, int nSelectedImage, int nOverlayedImage, bool bHidden, 
							HTREEITEM hParent, HTREEITEM hInsertAfter = TVI_LAST, bool haveChildren = FALSE, LPARAM lParam = NULL);
		void extractIcons(LPCTSTR currentPath, LPCTSTR volumeName, bool isDir, LPINT iIconNormal, LPINT iIconSelected, LPINT iIconOverlayed);
private :
	HIMAGELIST _hImglst;
	bool _bIsCreate;
	
};
	
#endif
