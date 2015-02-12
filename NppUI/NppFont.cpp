#include "NppLib.h"
#include "NppFont.h"

CNppFont::CNppFont():m_hFont(NULL), m_lpLogFont(NULL)
{

}
CNppFont::~CNppFont()
{

}
void CNppFont::destroy()
{
	if(m_hFont)
	{
		::DeleteObject(m_hFont);
		m_hFont = NULL;
	}
	if(m_lpLogFont)
	{
		::DeleteObject(m_lpLogFont);
		m_lpLogFont = NULL;
	}
}
HFONT CNppFont::getFont(HWND hwnd)
{
	return reinterpret_cast<HFONT>(::SendMessage(hwnd, WM_GETFONT, 0, 0));
}

void  CNppFont::setFont(HWND hctrl, HFONT hfont, BOOL redraw)
{
	::SendMessage(hctrl, WM_SETFONT, reinterpret_cast<WPARAM>(hfont), MAKELPARAM(redraw, 0));
}
void CNppFont::setFont(HWND hTo, HWND hFrom)
{
	HFONT hFont = reinterpret_cast<HFONT>(::SendMessage(hFrom, WM_GETFONT, 0, 0));
	::SendMessage(hTo, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), MAKELPARAM(TRUE, 0));
}
void CNppFont::setFont(HWND hctrl, LPLOGFONT lpLogFont, BOOL redraw)
{
//	if( m_hFont )
//	{
//		::DeleteObject(m_hFont);
//		m_hFont = NULL;
//	}
	HFONT hFont = ::CreateFontIndirect(lpLogFont);
	::SendMessage(hctrl, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), MAKELPARAM(redraw, 0));
}
LOGFONT& CNppFont::getLogFont(HWND hctrl)
{
	HFONT hFont = reinterpret_cast<HFONT>(::SendMessage(hctrl, WM_GETFONT, 0, 0));
	static LOGFONT	logFont  = {0};
	::GetObject(hFont, sizeof(LOGFONT), &logFont);
	return logFont;
}