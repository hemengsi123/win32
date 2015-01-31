#include "NppBase.h"
#include "NppString.h"

CNppString::CNppString() : m_pstr(m_szBuffer)
{
	m_szBuffer[0] = '\0';
}

CNppString::CNppString(const TCHAR ch) : m_pstr(m_szBuffer)
{
	m_szBuffer[0] = ch;
	m_szBuffer[1] = '\0';
}

CNppString::CNppString(LPCTSTR lpsz, int nLen) : m_pstr(m_szBuffer)
{      
	ASSERT(!::IsBadStringPtr(lpsz,-1) || lpsz==NULL);
	m_szBuffer[0] = '\0';
	assign(lpsz, nLen);
}

CNppString::CNppString(const CNppString& src) : m_pstr(m_szBuffer)
{
	m_szBuffer[0] = '\0';
	assign(src.m_pstr);
}

CNppString::~CNppString()
{
	if( m_pstr != m_szBuffer ) free(m_pstr);
}

int CNppString::getLength() const
{ 
	return (int) _tcslen(m_pstr); 
}

CNppString::operator LPCTSTR() const 
{ 
	return m_pstr; 
}

void CNppString::append(LPCTSTR pstr)
{
	int nNewLength = getLength() + (int) _tcslen(pstr);
	if( nNewLength >= MAX_LOCAL_STRING_LEN ) {
		if( m_pstr == m_szBuffer ) {
			m_pstr = static_cast<LPTSTR>(malloc((nNewLength + 1) * sizeof(TCHAR)));
			_tcscpy(m_pstr, m_szBuffer);
			_tcscat(m_pstr, pstr);
		}
		else {
			m_pstr = static_cast<LPTSTR>(realloc(m_pstr, (nNewLength + 1) * sizeof(TCHAR)));
			_tcscat(m_pstr, pstr);
		}
	}
	else {
		if( m_pstr != m_szBuffer ) {
			free(m_pstr);
			m_pstr = m_szBuffer;
		}
		_tcscat(m_szBuffer, pstr);
	}
}

void CNppString::assign(LPCTSTR pstr, int cchMax)
{
	if( pstr == NULL ) pstr = _T("");
	cchMax = (cchMax < 0 ? (int) _tcslen(pstr) : cchMax);
	if( cchMax < MAX_LOCAL_STRING_LEN ) {
		if( m_pstr != m_szBuffer ) {
			free(m_pstr);
			m_pstr = m_szBuffer;
		}
	}
	else if( cchMax > getLength() || m_pstr == m_szBuffer ) {
		if( m_pstr == m_szBuffer ) m_pstr = NULL;
		m_pstr = static_cast<LPTSTR>(realloc(m_pstr, (cchMax + 1) * sizeof(TCHAR)));
	}
	_tcsncpy(m_pstr, pstr, cchMax);
	m_pstr[cchMax] = '\0';
}

bool CNppString::isEmpty() const 
{ 
	return m_pstr[0] == '\0'; 
}

void CNppString::empty() 
{ 
	if( m_pstr != m_szBuffer ) free(m_pstr);
	m_pstr = m_szBuffer;
	m_szBuffer[0] = '\0'; 
}

LPCTSTR CNppString::getData() const
{
	return m_pstr;
}

TCHAR CNppString::getAt(int nIndex) const
{
	return m_pstr[nIndex];
}

TCHAR CNppString::operator[] (int nIndex) const
{ 
	return m_pstr[nIndex];
}   

const CNppString& CNppString::operator=(const CNppString& src)
{      
	assign(src);
	return *this;
}

const CNppString& CNppString::operator=(LPCTSTR lpStr)
{      
	if ( lpStr )
	{
		ASSERT(!::IsBadStringPtr(lpStr,-1));
		assign(lpStr);
	}
	else
	{
		empty();
	}
	return *this;
}

#ifdef _UNICODE

const CNppString& CNppString::operator=(LPCSTR lpStr)
{
	if ( lpStr )
	{
		ASSERT(!::IsBadStringPtrA(lpStr,-1));
		int cchStr = (int) strlen(lpStr) + 1;
		LPWSTR pwstr = (LPWSTR) _alloca(cchStr);
		if( pwstr != NULL ) ::MultiByteToWideChar(::GetACP(), 0, lpStr, -1, pwstr, cchStr) ;
		assign(pwstr);
	}
	else
	{
		empty();
	}
	return *this;
}

const CNppString& CNppString::operator+=(LPCSTR lpStr)
{
	if ( lpStr )
	{
		ASSERT(!::IsBadStringPtrA(lpStr,-1));
		int cchStr = (int) strlen(lpStr) + 1;
		LPWSTR pwstr = (LPWSTR) _alloca(cchStr);
		if( pwstr != NULL ) ::MultiByteToWideChar(::GetACP(), 0, lpStr, -1, pwstr, cchStr) ;
		append(pwstr);
	}
	
	return *this;
}

#else

const CNppString& CNppString::operator=(LPCWSTR lpwStr)
{      
	if ( lpwStr )
	{
		ASSERT(!::IsBadStringPtrW(lpwStr,-1));
		int cchStr = ((int) wcslen(lpwStr) * 2) + 1;
		LPSTR pstr = (LPSTR) _alloca(cchStr);
		if( pstr != NULL ) ::WideCharToMultiByte(::GetACP(), 0, lpwStr, -1, pstr, cchStr, NULL, NULL);
		assign(pstr);
	}
	else
	{
		empty();
	}
	
	return *this;
}

const CNppString& CNppString::operator+=(LPCWSTR lpwStr)
{
	if ( lpwStr )
	{
		ASSERT(!::IsBadStringPtrW(lpwStr,-1));
		int cchStr = ((int) wcslen(lpwStr) * 2) + 1;
		LPSTR pstr = (LPSTR) _alloca(cchStr);
		if( pstr != NULL ) ::WideCharToMultiByte(::GetACP(), 0, lpwStr, -1, pstr, cchStr, NULL, NULL);
		append(pstr);
	}
	
	return *this;
}

#endif // _UNICODE

const CNppString& CNppString::operator=(const TCHAR ch)
{
	empty();
	m_szBuffer[0] = ch;
	m_szBuffer[1] = '\0';
	return *this;
}

CNppString CNppString::operator+(const CNppString& src) const
{
	CNppString sTemp = *this;
	sTemp.append(src);
	return sTemp;
}

CNppString CNppString::operator+(LPCTSTR lpStr) const
{
	if ( lpStr )
	{
		ASSERT(!::IsBadStringPtr(lpStr,-1));
		CNppString sTemp = *this;
		sTemp.append(lpStr);
		return sTemp;
	}

	return *this;
}

const CNppString& CNppString::operator+=(const CNppString& src)
{      
	append(src);
	return *this;
}

const CNppString& CNppString::operator+=(LPCTSTR lpStr)
{      
	if ( lpStr )
	{
		ASSERT(!::IsBadStringPtr(lpStr,-1));
		append(lpStr);
	}
	
	return *this;
}

const CNppString& CNppString::operator+=(const TCHAR ch)
{      
	TCHAR str[] = { ch, '\0' };
	append(str);
	return *this;
}

bool CNppString::operator == (LPCTSTR str) const { return (compare(str) == 0); };
bool CNppString::operator != (LPCTSTR str) const { return (compare(str) != 0); };
bool CNppString::operator <= (LPCTSTR str) const { return (compare(str) <= 0); };
bool CNppString::operator <  (LPCTSTR str) const { return (compare(str) <  0); };
bool CNppString::operator >= (LPCTSTR str) const { return (compare(str) >= 0); };
bool CNppString::operator >  (LPCTSTR str) const { return (compare(str) >  0); };

void CNppString::setAt(int nIndex, TCHAR ch)
{
	ASSERT(nIndex>=0 && nIndex<getLength());
	m_pstr[nIndex] = ch;
}

int CNppString::compare(LPCTSTR lpsz) const 
{ 
	return _tcscmp(m_pstr, lpsz); 
}

int CNppString::compareNoCase(LPCTSTR lpsz) const 
{ 
	return _tcsicmp(m_pstr, lpsz); 
}

void CNppString::makeUpper() 
{ 
	_tcsupr(m_pstr); 
}

void CNppString::makeLower() 
{ 
	_tcslwr(m_pstr); 
}

CNppString CNppString::left(int iLength) const
{
	if( iLength < 0 ) iLength = 0;
	if( iLength > getLength() ) iLength = getLength();
	return CNppString(m_pstr, iLength);
}

CNppString CNppString::mid(int iPos, int iLength) const
{
	if( iLength < 0 ) iLength = getLength() - iPos;
	if( iPos + iLength > getLength() ) iLength = getLength() - iPos;
	if( iLength <= 0 ) return CNppString();
	return CNppString(m_pstr + iPos, iLength);
}

CNppString CNppString::right(int iLength) const
{
	int iPos = getLength() - iLength;
	if( iPos < 0 ) {
		iPos = 0;
		iLength = getLength();
	}
	return CNppString(m_pstr + iPos, iLength);
}

int CNppString::find(TCHAR ch, int iPos /*= 0*/) const
{
	ASSERT(iPos>=0 && iPos<=getLength());
	if( iPos != 0 && (iPos < 0 || iPos >= getLength()) ) return -1;
	LPCTSTR p = _tcschr(m_pstr + iPos, ch);
	if( p == NULL ) return -1;
	return (int)(p - m_pstr);
}

int CNppString::find(LPCTSTR pstrSub, int iPos /*= 0*/) const
{
	ASSERT(!::IsBadStringPtr(pstrSub,-1));
	ASSERT(iPos>=0 && iPos<=getLength());
	if( iPos != 0 && (iPos < 0 || iPos > getLength()) ) return -1;
	LPCTSTR p = _tcsstr(m_pstr + iPos, pstrSub);
	if( p == NULL ) return -1;
	return (int)(p - m_pstr);
}

int CNppString::reverseFind(TCHAR ch) const
{
	LPCTSTR p = _tcsrchr(m_pstr, ch);
	if( p == NULL ) return -1;
	return (int)(p - m_pstr);
}

int CNppString::replace(LPCTSTR pstrFrom, LPCTSTR pstrTo)
{
	CNppString sTemp;
	int nCount = 0;
	int iPos = find(pstrFrom);
	if( iPos < 0 ) return 0;
	int cchFrom = (int) _tcslen(pstrFrom);
	int cchTo = (int) _tcslen(pstrTo);
	while( iPos >= 0 ) {
		sTemp = left(iPos);
		sTemp += pstrTo;
		sTemp += mid(iPos + cchFrom);
		assign(sTemp);
		iPos = find(pstrFrom, iPos + cchTo);
		nCount++;
	}
	return nCount;
}

int CNppString::format(LPCTSTR pstrFormat, ...)
{
	LPTSTR szSprintf = NULL;
	va_list argList;
    int nLen;
	va_start(argList, pstrFormat);
    nLen = ::_vsntprintf(NULL, 0, pstrFormat, argList);
    szSprintf = (TCHAR*)malloc((nLen + 1) * sizeof(TCHAR));
    ZeroMemory(szSprintf, (nLen + 1) * sizeof(TCHAR));
	int iRet = ::_vsntprintf(szSprintf, nLen + 1, pstrFormat, argList);
	va_end(argList);
	assign(szSprintf);
    free(szSprintf);
	return iRet;
}

int CNppString::smallFormat(LPCTSTR pstrFormat, ...)
{
	CNppString sFormat = pstrFormat;
	TCHAR szBuffer[64] = { 0 };
	va_list argList;
	va_start(argList, pstrFormat);
	int iRet = ::wvsprintf(szBuffer, sFormat, argList);
	va_end(argList);
	assign(szBuffer);
	return iRet;
}