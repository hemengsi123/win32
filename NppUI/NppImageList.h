
#ifndef IMAGE_LIST_H
#define IMAGE_LIST_H

#include <CommCtrl.h>

class NPPLIB_API CNppImageList
{
public :
	CNppImageList() : _hImglst(NULL){};
	virtual ~CNppImageList() 
	{
		destroy();
	}
	virtual bool create(int cx,int cy,UINT flags = (ILC_COLOR32 | ILC_MASK), int cInitial = 3, int cGrow = 6);
	virtual bool destroy();
	HIMAGELIST   getSysImgLst(UINT uFlags = SHGFI_SMALLICON);
	HIMAGELIST   getImglst()const;
	int          addIcon(HINSTANCE hInst, int iconID);
	bool         delIcon(int iconID);
	int          addImage(HBITMAP hbmImage,HBITMAP hbmMask=NULL);
	void         setIconSize(int size) const;
	void         setImglst(HIMAGELIST hImglst, bool bDestroy=true);
	int          getCount()const;
	void         clear();
	HICON        getHIcon(int indx,UINT uFlags);
	void getFileIcon(IN LPCTSTR lpszFile, OUT LPINT iIconNormal, OUT LPINT iIconSelected = NULL, OUT LPINT iIconOverlayed = NULL);
private :
	HIMAGELIST _hImglst;
};

#endif //IMAGE_LIST_H
