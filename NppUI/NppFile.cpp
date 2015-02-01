
#include "NppBase.h"
#include "NppFile.h"

CNppFile::CNppFile(LPCTSTR lpszFilePath): m_hFind(NULL)
{
	setFullPath(lpszFilePath);
}
CNppFile::~CNppFile()
{
	findClose();
}

void CNppFile::setFullPath(LPCTSTR lpszFilePath)
{
	if(lpszFilePath)
	{
		::ZeroMemory(m_szFilePath, sizeof(m_szFilePath));
//		_stprintf(m_szFilePath, _T("%s\0\0"), lpszFilePath);
		_tcscpy(m_szFilePath, lpszFilePath);
	}
}
LPCTSTR CNppFile::getFullPath() const
{
	return m_szFilePath;
}
BOOL CNppFile::isExist(LPCTSTR lpszFilePath) const
{
	if( !lpszFilePath )
		lpszFilePath = m_szFilePath;
		
	return ::PathFileExists(lpszFilePath);
}
BOOL CNppFile::isFile(LPCTSTR lpszFilePath) const
{
	if( !lpszFilePath )
		lpszFilePath = m_szFilePath;
		
	return !::PathIsDirectory(lpszFilePath);
}
BOOL CNppFile::isDir(LPCTSTR lpszFilePath) const
{
	if( !lpszFilePath )
		lpszFilePath = m_szFilePath;
		
	return ::PathIsDirectory(lpszFilePath);
}
BOOL CNppFile::isEmptyDir(LPCTSTR lpszFilePath) const
{
	if( !lpszFilePath )
		lpszFilePath = m_szFilePath;
	
	if( isDir(lpszFilePath) )
	{
		return ::PathIsDirectoryEmpty(lpszFilePath);
	}
	return true;
}
LPCTSTR CNppFile::getName(LPCTSTR lpszFilePath)
{
	if( lpszFilePath )
	{
		return ::PathFindFileName(lpszFilePath);
	}
	else
	{
		return ::PathFindFileName(m_szFilePath);
	}
}
LPCTSTR CNppFile::setName(LPCTSTR lpszName)
{
	_tcscpy(m_szBuff, lpszName);
	
	return m_szBuff;
}

LPCTSTR CNppFile::getPath(LPCTSTR lpszFilePath)
{
	if( !lpszFilePath )
		lpszFilePath = m_szFilePath;
		
	if( isDir(lpszFilePath) )
	{
		return lpszFilePath;
	}
	else
	{
		setBuff(lpszFilePath);
		if(::PathRemoveFileSpec(m_szBuff) )
		{
			return m_szBuff;
		}
	}
	return NULL;
}
LPTSTR CNppFile::setBuff(LPCTSTR lpszPath)
{

	_tcscpy(m_szBuff, lpszPath);
	return m_szBuff;
}
LPCTSTR CNppFile::getBuff()const
{
	return m_szBuff;
}
LPCTSTR CNppFile::getNameExt(LPCTSTR lpszFilePath)const
{
	if( !lpszFilePath )
		lpszFilePath = m_szFilePath;

	return NULL;
}

LPCTSTR CNppFile::getExtension(LPCTSTR lpszFilePath)const
{
	if( !lpszFilePath )
		lpszFilePath = m_szFilePath;
    return _tcsrchr(lpszFilePath, _T('.'));
}
// add '\'
LPTSTR CNppFile::addBackslash(LPTSTR lpszFilePath)
{
	if( !lpszFilePath )
		lpszFilePath = m_szFilePath;
	
	return ::PathAddBackslash(lpszFilePath);
}

LPCTSTR CNppFile::rmExtension(LPCTSTR lpszFilePath)
{
	if( !lpszFilePath )
		lpszFilePath = m_szFilePath;
	
	LPTSTR lpDotPos = _tcsrchr(setBuff(lpszFilePath), _T('.'));
	if(lpDotPos != NULL)
	{
		*lpDotPos = _T('\0');
		// 排除 .gitconfig
		if(_tcslen(m_szBuff) == 0)
			return lpszFilePath;
		else
			return m_szBuff;
	}
	return lpszFilePath;
}

LPCTSTR CNppFile::append(LPCTSTR pszMore, LPTSTR  pszPath)
{
	if( !pszPath )
		pszPath = m_szFilePath;
	if( pszMore )
	{
		::PathAppend(pszPath, pszMore);
	}
	return pszPath;
}
BOOL CNppFile::isValidFolder(const LPWIN32_FIND_DATA lpfindData) const
{
	if ((lpfindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
		(!(lpfindData->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) ) &&
		 (_tcscmp(lpfindData->cFileName, _T(".")) != 0) && 
		 (_tcscmp(lpfindData->cFileName, _T("..")) != 0) &&
		 (lpfindData->cFileName[0] != '?'))
		return true;

	return false;
}
LPWIN32_FIND_DATA CNppFile::findFirstFile(LPCTSTR lpszDirPath, LPCTSTR lpszwildcard)
{
	::ZeroMemory(&m_findData, sizeof(WIN32_FIND_DATA));
	findClose();
	TCHAR tmpPath[MAX_PATH] = {0};
	if( lpszDirPath )
		_tcscpy(tmpPath, lpszDirPath);
	else
		_tcscpy(tmpPath, m_szFilePath);
	::PathAddBackslash(tmpPath);
	::PathAppend(tmpPath, lpszwildcard);
	
	m_hFind = ::FindFirstFile(tmpPath, &m_findData);
	if ( m_hFind== INVALID_HANDLE_VALUE)
	{
		m_hFind = NULL;
		return NULL;
	}
	else
	{
		return &m_findData;
	}
}
LPWIN32_FIND_DATA CNppFile::findNextFile()
{
	if(m_hFind == NULL)
		return NULL;
	if( !::FindNextFile(m_hFind, &m_findData) )
	{
		findClose();
		return NULL;
	}
	else
		return &m_findData;
}
void CNppFile::findClose()
{
	if(m_hFind)
	{
		::FindClose(m_hFind);
		m_hFind = NULL;
		::ZeroMemory(&m_findData, sizeof(WIN32_FIND_DATA));
	}
}
BOOL CNppFile::findIsHidden(const WIN32_FIND_DATA * lpfindData) const
{
	if( !lpfindData )
		lpfindData = &m_findData;
		
	if(m_hFind)
	{
		return (lpfindData->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);
	}
	return false;
}

LPCTSTR CNppFile::findGetName(const WIN32_FIND_DATA * lpfindData) const
{
	if( !lpfindData )
		lpfindData = &m_findData;
		
	if(m_hFind)
	{
		return lpfindData->cFileName;
	}
	return NULL;
}
DWORD CNppFile::findGetAttri(const WIN32_FIND_DATA * lpfindData) const
{
	if( !lpfindData )
		lpfindData = &m_findData;
		
	if(m_hFind)
	{
		return lpfindData->dwFileAttributes;
	}
	return 0;
}
unsigned __int64 CNppFile::findGetSize(const WIN32_FIND_DATA* lpfindData)const
{
	if( !lpfindData )
		lpfindData = &m_findData;
	
	unsigned __int64 lFilesize = 0;
	lFilesize = (((unsigned __int64)lpfindData->nFileSizeHigh) << 32) + lpfindData->nFileSizeLow;
	
	return lFilesize;
}
BOOL CNppFile::findIsDir(const WIN32_FIND_DATA* lpfindData) const
{
	if( !lpfindData )
		lpfindData = &m_findData;
	return (lpfindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}
BOOL CNppFile::findIsFile(const WIN32_FIND_DATA* lpfindData) const
{
	if( !lpfindData )
		lpfindData = &m_findData;
		
	return !(lpfindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY); 
}
LPSYSTEMTIME CNppFile::getSysTime(LPFILETIME lpftTime)const
{
	static SYSTEMTIME sysTime;
	FILETIME          ftLocalTime;
	::FileTimeToLocalFileTime(lpftTime, &ftLocalTime);
	::FileTimeToSystemTime(&ftLocalTime, &sysTime);
	
	return &sysTime;
}
DWORD CNppFile::getLogicalDrives() const
{
	return ::GetLogicalDrives();
}
/*
 * @brief SHFileOperation支持中同时删除多个文件，
 * 在pFrom中加入多个文件路径，每个路径之间用'\0'分隔，最后用"\0\0"结尾
 *eg. delFile(_T("D:\\1.txt\0D:\\2.txt\0\0"));
*/
int CNppFile::delFile(LPCTSTR lpszFile, int fFlags, HWND hwnd)const
{
	if( lpszFile == NULL)
	{
		lpszFile = m_szFilePath;
	}
	SHFILEOPSTRUCT	fileOp	= {0};
	fileOp.hwnd				= hwnd;
	fileOp.pFrom			= lpszFile;
//	fileOp.pTo              = NULL;
	fileOp.fFlags			= fFlags;
	fileOp.wFunc			= FO_DELETE;
	return ::SHFileOperation(&fileOp);
}
int CNppFile::copyFile(LPCTSTR lpszTo, LPCTSTR lpszFrom, int fFlags, HWND hwnd)const
{
	if( lpszFrom == NULL)
	{
		lpszFrom = m_szFilePath;
	}
	SHFILEOPSTRUCT	fileOp	= {0};
	fileOp.hwnd				= hwnd;
	fileOp.pFrom			= lpszFrom;
	fileOp.pTo              = lpszTo;
	fileOp.fFlags			= fFlags;
	fileOp.wFunc			= FO_COPY;
	
	return ::SHFileOperation(&fileOp);
}

int CNppFile::moveFile(LPCTSTR lpszTo, LPCTSTR lpszFrom, int fFlags, HWND hwnd)const
{
	if( lpszFrom == NULL)
	{
		lpszFrom = m_szFilePath;
	}
	SHFILEOPSTRUCT	fileOp	= {0};
	fileOp.hwnd				= hwnd;
	fileOp.pFrom			= lpszFrom;
	fileOp.pTo              = lpszTo;
	fileOp.fFlags			= fFlags;
	fileOp.wFunc			= FO_MOVE;
	
	return ::SHFileOperation(&fileOp);
}
int CNppFile::renameFile(LPCTSTR lpszTo, LPCTSTR lpszFrom, int fFlags, HWND hwnd)
{
	if( lpszFrom == NULL)
	{
		lpszFrom = m_szFilePath;
	}
	SHFILEOPSTRUCT	fileOp	= {0};
	fileOp.hwnd				= hwnd;
	fileOp.pFrom			= lpszFrom;
	fileOp.pTo              = lpszTo;
	fileOp.fFlags			= fFlags;
	fileOp.wFunc			= FO_RENAME;
	
	return ::SHFileOperation(&fileOp);
}