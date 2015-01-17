
#ifndef IMAGE_LIST_H
#define IMAGE_LIST_H

#include <CommCtrl.h>

class CNppImageList
{
public :
	CNppImageList() : _hImglst(NULL){};
	virtual ~CNppImageList() 
	{
		destroy();
	}
	virtual bool create(int cx,int cy,UINT flags = (ILC_COLOR32 | ILC_MASK), int cInitial = 3, int cGrow = 6);
	virtual bool destroy();
	HIMAGELIST getSysImgLst(UINT uFlags = SHGFI_SMALLICON);
	int addIcon(HINSTANCE hInst, int iconID);
	bool delIcon(int iconID);
	int addImage(HBITMAP hbmImage,HBITMAP hbmMask=NULL);
	void setIconSize(int size) const;
	HIMAGELIST getImglst()const;
	int getCount()const;
	void clear();
	HICON getHIcon(int indx,UINT uFlags);
private :
	HIMAGELIST _hImglst;
};

#endif //IMAGE_LIST_H
