
#include "NppLib.h"
#include "NppImageList.h"

bool CNppImageList::create(int nWidth, int nHight, UINT flags, int cInitial, int cGrow)
{
	_hImglst = ::ImageList_Create(nWidth, nHight, flags, cInitial, cGrow);
	if (!_hImglst)
		return false;
	else
		return true;
}

int CNppImageList::addIcon(HINSTANCE hInst, int iconID)
{
	HICON hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(iconID));
	int index = -1;
	if( hIcon )
	{
		index = ::ImageList_AddIcon(_hImglst, hIcon);
		::DestroyIcon(hIcon);
	}
	return index;
}
bool CNppImageList::delIcon(int iconID)
{
	bool bSuccuss = true;
	if(_hImglst)
	{
		bSuccuss = ::ImageList_Remove(_hImglst, iconID);
	}
	return bSuccuss;
}
int CNppImageList::addImage(HBITMAP hbmImage,HBITMAP hbmMask)
{
	int index = -1;
	if(_hImglst)
		index = ::ImageList_Add(_hImglst, hbmImage, hbmMask);
	return index;
}
int CNppImageList::getCount()const
{
	return ::ImageList_GetImageCount(_hImglst);
}
void CNppImageList::clear()
{
	::ImageList_RemoveAll(_hImglst);
}
HICON CNppImageList::getHIcon(int indx,UINT uFlags)
{
	return ::ImageList_GetIcon(_hImglst,indx, uFlags);
}
HIMAGELIST CNppImageList::getSysImgLst(UINT uFlags)
{
	SHFILEINFO sfi  = {0};
	DWORD_PTR  hr   = 0;
	HIMAGELIST himl = NULL;
	hr = ::SHGetFileInfo(_T("C:\\"), 0, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | uFlags);
	if(SUCCEEDED(hr))
	{
		himl = (HIMAGELIST)hr;
	}
	return himl;
}

bool CNppImageList::destroy() 
{
	bool bSuccess = true;
	if(_hImglst )
	{
		bSuccess = ::ImageList_Destroy(_hImglst);
		_hImglst = NULL;
	}
	return bSuccess;
}

HIMAGELIST CNppImageList::getImglst()const
{
	return _hImglst;
}
void CNppImageList::setImglst(HIMAGELIST hImglst, bool bDestroy)
{
	if( bDestroy )
	{
		destroy();
	}
	_hImglst = hImglst;
}
void CNppImageList::getFileIcon(LPCTSTR lpszFile, LPINT iIconNormal, LPINT iIconSelected, LPINT iIconOverlayed)
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


