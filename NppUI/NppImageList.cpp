
#include "NppLib.h"
#include "NppImageList.h"

bool CNppImageList::create(int nWidth, int nHight, UINT flags, int cInitial, int cGrow)
{
	m_hImglst = ::ImageList_Create(nWidth, nHight, flags, cInitial, cGrow);
	if (!m_hImglst)
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
		index = ::ImageList_AddIcon(m_hImglst, hIcon);
		::DestroyIcon(hIcon);
	}
	return index;
}
bool CNppImageList::delIcon(int iconID)
{
	bool bSuccuss = true;
	if(m_hImglst)
	{
		bSuccuss = ::ImageList_Remove(m_hImglst, iconID);
	}
	return bSuccuss;
}
int CNppImageList::addImage(HBITMAP hbmImage,HBITMAP hbmMask)
{
	int index = -1;
	if(m_hImglst)
		index = ::ImageList_Add(m_hImglst, hbmImage, hbmMask);
	return index;
}
int CNppImageList::getCount()const
{
	return ::ImageList_GetImageCount(m_hImglst);
}
void CNppImageList::clear()
{
	::ImageList_RemoveAll(m_hImglst);
}
HICON CNppImageList::getHIcon(int indx,UINT uFlags)
{
	return ::ImageList_GetIcon(m_hImglst,indx, uFlags);
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
	if(m_hImglst )
	{
		bSuccess = ::ImageList_Destroy(m_hImglst);
		m_hImglst = NULL;
	}
	return bSuccess;
}

HIMAGELIST CNppImageList::getImglst()const
{
	return m_hImglst;
}
void CNppImageList::setImglst(HIMAGELIST hImglst, bool bDestroy)
{
	if( bDestroy )
	{
		destroy();
	}
	m_hImglst = hImglst;
}
void CNppImageList::getFileIcon(LPCTSTR lpszFile, LPINT iIconNormal, LPINT iIconSelected, LPINT iIconOverlayed)
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

int CNppImageList::replaceIcon(int iIcon, HICON hicon)
{
	return ::ImageList_ReplaceIcon(m_hImglst, iIcon, hicon);
}

BOOL CNppImageList::drawTo(int indx, HDC hdcDst, int x, int y, UINT fStyle)
{
	return ImageList_Draw(m_hImglst, indx, hdcDst, x, y, fStyle);
}