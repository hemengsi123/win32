
#ifndef IMAGE_LIST_H
#define IMAGE_LIST_H

#include <CommCtrl.h>

class NPPLIB_API CNppImageList
{
public :
	CNppImageList() : m_hImglst(NULL){};
	virtual ~CNppImageList() 
	{
		destroy();
	}
	virtual BOOL create(int cx,int cy,UINT flags = (ILC_COLOR32 | ILC_MASK), int cInitial = 3, int cGrow = 6);
	virtual BOOL destroy();
	HIMAGELIST   getSysImgLst(UINT uFlags = SHGFI_SMALLICON);
	HIMAGELIST   getImglst()const;
	int          addIcon(HINSTANCE hInst, int iconID);
	BOOL         delIcon(int iconID);
	int          addImage(HBITMAP hbmImage,HBITMAP hbmMask=NULL);
	void         setIconSize(int size) const;
	void         setImglst(HIMAGELIST hImglst, bool bDestroy=true);
	int          getCount()const;
	void         clear();
	HICON        getHIcon(int indx,UINT uFlags);
	int          replaceIcon(int iIcon, HICON hicon);
	void getFileIcon(IN LPCTSTR lpszFile, OUT LPINT iIconNormal, OUT LPINT iIconSelected = NULL, OUT LPINT iIconOverlayed = NULL);
	BOOL  drawTo(int indx, HDC hdcDst, int x, int y, UINT fStyle = ILD_NORMAL);
private :
	HIMAGELIST m_hImglst;
};

#endif //IMAGE_LIST_H
