#ifndef NPPSTRING_H
#define NPPSTRING_H

#pragma once

// copy from DuiLib 
class CNppString
{
public:
	enum { MAX_LOCAL_STRING_LEN = 63 };
	
	CNppString();
	CNppString(const TCHAR ch);
	CNppString(const CNppString& src);
	CNppString(LPCTSTR lpsz, int nLen = -1);
	~CNppString();

	void empty();
	int getLength() const;
	bool isEmpty() const;
	TCHAR getAt(int nIndex) const;
	void append(LPCTSTR pstr);
	void assign(LPCTSTR pstr, int nLength = -1);
	LPCTSTR getData() const;

	void setAt(int nIndex, TCHAR ch);
	operator LPCTSTR() const;

	TCHAR operator[] (int nIndex) const;
	const CNppString& operator=(const CNppString& src);
	const CNppString& operator=(const TCHAR ch);
	const CNppString& operator=(LPCTSTR pstr);
#ifdef _UNICODE
	const CNppString& CNppString::operator=(LPCSTR lpStr);
	const CNppString& CNppString::operator+=(LPCSTR lpStr);
#else
	const CNppString& CNppString::operator=(LPCWSTR lpwStr);
	const CNppString& CNppString::operator+=(LPCWSTR lpwStr);
#endif
	CNppString operator+(const CNppString& src) const;
	CNppString operator+(LPCTSTR pstr) const;
	const CNppString& operator+=(const CNppString& src);
	const CNppString& operator+=(LPCTSTR pstr);
	const CNppString& operator+=(const TCHAR ch);

	bool operator == (LPCTSTR str) const;
	bool operator != (LPCTSTR str) const;
	bool operator <= (LPCTSTR str) const;
	bool operator <  (LPCTSTR str) const;
	bool operator >= (LPCTSTR str) const;
	bool operator >  (LPCTSTR str) const;

	int compare(LPCTSTR pstr) const;
	int compareNoCase(LPCTSTR pstr) const;

	void makeUpper();
	void makeLower();

	CNppString left(int nLength) const;
	CNppString mid(int iPos, int nLength = -1) const;
	CNppString right(int nLength) const;

	int find(TCHAR ch, int iPos = 0) const;
	int find(LPCTSTR pstr, int iPos = 0) const;
	int reverseFind(TCHAR ch) const;
	int replace(LPCTSTR pstrFrom, LPCTSTR pstrTo);

	int __cdecl format(LPCTSTR pstrFormat, ...);
	int __cdecl smallFormat(LPCTSTR pstrFormat, ...);

protected:
	// @brief: str > MAX_LOCAL_STRING_LEN  ±£¨∑÷≈‰ƒ⁄¥Ê m_pstr = realloc();
	LPTSTR m_pstr;
	TCHAR  m_szBuffer[MAX_LOCAL_STRING_LEN + 1];

};
#endif

