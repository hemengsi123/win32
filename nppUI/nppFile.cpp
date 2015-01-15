
#include <Shlwapi.h>
// #include "nppFile.h"
#include "nppLib.h"

CNppFile::CNppFile(LPCTSTR lpszFilePath): m_lpszPath(NULL), m_lpszName(NULL), m_lpszExtension(NULL), m_hFind(NULL)
{
	setFullPath(lpszFilePath);
}
CNppFile::~CNppFile()
{
	SAFE_DELETE_ARRAY(m_lpszName);
	SAFE_DELETE_ARRAY(m_lpszPath);
	SAFE_DELETE_ARRAY(m_lpszExtension);
}

void CNppFile::setFullPath(LPCTSTR lpszFilePath)
{
	if(lpszFilePath)
		_tcscpy(m_szFilePath, lpszFilePath);
}
LPCTSTR CNppFile::getFullPath() const
{
	return m_szFilePath;
}
BOOL CNppFile::isExist(LPCTSTR lpszFilePath) const
{
	if( lpszFilePath )
		return ::PathFileExists(lpszFilePath);
	else
		return ::PathFileExists(m_szFilePath);
}
BOOL CNppFile::isFile(LPCTSTR lpszFilePath) const
{
	if( lpszFilePath )
		return !::PathIsDirectory(lpszFilePath);
	else
		return !isDir(m_szFilePath);
}
BOOL CNppFile::isDir(LPCTSTR lpszFilePath) const
{
	if( lpszFilePath )
		return ::PathIsDirectory(lpszFilePath);
	else
		return ::PathIsDirectory(m_szFilePath);
}
BOOL CNppFile::isEmptyDir(LPCTSTR lpszFilePath) const
{
	if( lpszFilePath )
	{
		if( isDir(lpszFilePath) )
		{
			return ::PathIsDirectoryEmpty(lpszFilePath);
		}
		else
			return TRUE;
	}
	else
	{
		if( isDir(m_szFilePath) )
		{
			return ::PathIsDirectoryEmpty(m_szFilePath);
		}
		else
			return TRUE;
	}
}
LPCTSTR CNppFile::getName(LPCTSTR lpszFilePath)
{
	if( lpszFilePath )
	{
		return setName(::PathFindFileName(lpszFilePath));
	}
	else
	{
		return setName(::PathFindFileName(m_szFilePath));
	}
}
LPCTSTR CNppFile::setName(LPCTSTR lpszName)
{
	// if( lpszName == NULL)
		// return NULL;
	// m_strName = lpszName;
	if( m_lpszName == NULL)
		m_lpszName = new TCHAR[MAX_PATH];

	_tcscpy(m_lpszName, lpszName);
	
	return m_lpszName;
}

LPCTSTR CNppFile::getPath(LPCTSTR lpszFilePath)
{
	if( isDir(m_szFilePath) )
	{
		return m_szFilePath;
	}
	else
	{
		setPath(m_szFilePath);
		if(::PathRemoveFileSpec(m_lpszPath) )
		{
			return m_lpszPath;
		}
		else
			return NULL;
	}
}
LPCTSTR CNppFile::setPath(LPCTSTR lpszPath)
{
	if( m_lpszPath == NULL)
		m_lpszPath = new TCHAR[MAX_PATH];
	
	_tcscpy(m_lpszPath, lpszPath);
	return m_lpszPath;
}
LPCTSTR CNppFile::getNameExt(LPCTSTR lpszFilePath)const
{
	if( lpszFilePath )
		return NULL;
	else
		return NULL;
}

LPCTSTR CNppFile::getExtension(LPCTSTR lpszFilePath)const
{
	if( lpszFilePath )
		return ::PathFindExtension(lpszFilePath);
	else
		return ::PathFindExtension(m_szFilePath);
}
// add '\'
LPTSTR CNppFile::addBackslash(LPTSTR lpszFilePath)
{
	if( lpszFilePath )
		return ::PathAddBackslash(lpszFilePath);
	else
		return ::PathAddBackslash(m_szFilePath);
}

void CNppFile::rmExtension(LPTSTR lpszFilePath)
{
	if( lpszFilePath )
		::PathRemoveExtension(lpszFilePath);
	else
		::PathRemoveExtension(m_szFilePath);
}

LPCTSTR CNppFile::append(LPCTSTR pszMore, LPTSTR  pszPath)
{
	if( pszMore == NULL)
		return NULL;
	if( pszPath )
	{
		if(::PathAppend(pszPath, pszMore) )
			return pszPath;
		else
			return  NULL;
	}
	else
	{
		if(::PathAppend(m_szFilePath, pszMore) )
			return pszPath;
		else
			return  NULL;
	}
}
bool CNppFile::isValidFolder(const LPWIN32_FIND_DATA lpfindData) const
{
	if ((lpfindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
		(!(lpfindData->dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) ) &&
		 (_tcscmp(lpfindData->cFileName, _T(".")) != 0) && 
		 (_tcscmp(lpfindData->cFileName, _T("..")) != 0) &&
		 (lpfindData->cFileName[0] != '?'))
		return true;

	return false;
}
LPWIN32_FIND_DATA CNppFile::findFirstFile()
{
	::ZeroMemory(&m_findData, sizeof(WIN32_FIND_DATA));
	findClose();
	m_hFind = ::FindFirstFile(m_szFilePath, &m_findData);
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
bool CNppFile::findIsHidden() const
{
	if(m_hFind)
	{
		return (m_findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);
	}
	else
		return false;
}

LPCTSTR CNppFile::findGetName() const
{
	if(m_hFind)
	{
		return m_findData.cFileName;
	}
	else
		return NULL;
}
DWORD CNppFile::findGetAttri() const
{
	if(m_hFind)
	{
		return m_findData.dwFileAttributes;
	}
	else
		return 0;
}
DWORD CNppFile::getLogicalDrives() const
{
	return ::GetLogicalDrives();
}