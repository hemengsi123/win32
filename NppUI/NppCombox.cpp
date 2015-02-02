

#include "NppLib.h"
#include "SysMsg.h"
#include "NppCombox.h"

#include "StdAfx.h"

using std::vector;

CNppCombox::CNppCombox()
{
	m_comboItems.clear();
}

CNppCombox::~CNppCombox()
{
}
LPCTSTR CNppCombox::getWndClassName()const
{
	return WC_COMBOBOX;
}
void CNppCombox::init(HINSTANCE hInst, HWND hParent, UINT iCtrlID, LPCTSTR sCtrlName)
{
	CNppCtrlWnd::init(hInst, hParent, iCtrlID, sCtrlName);
}
HWND CNppCombox::create(DWORD dwStyle, DWORD dwExStyle)
{
	HWND hWnd = CNppCtrlWnd::create(dwStyle, dwExStyle, NULL);
	if( hWnd )
	{
		COMBOBOXINFO	comboBoxInfo;
		comboBoxInfo.cbSize = sizeof(COMBOBOXINFO);
		::SendMessage(hWnd, CB_GETCOMBOBOXINFO, 0, (LPARAM)&comboBoxInfo);
		CNppBaseWnd::setWndProc(comboBoxInfo.hwndItem);
	}
	return hWnd;
}
HWND CNppCombox::create(DWORD dwStyle, int x, int y, int cx, int cy, DWORD dwExStyle)
{
	if( CNppCtrlWnd::create(NULL, dwStyle, x, y, cx, cy, dwExStyle) )
	{
		COMBOBOXINFO	comboBoxInfo;
		comboBoxInfo.cbSize = sizeof(COMBOBOXINFO);
		::SendMessage(m_hSelf, CB_GETCOMBOBOXINFO, 0, (LPARAM)&comboBoxInfo);
		CNppBaseWnd::setWndProc(comboBoxInfo.hwndItem);
	}
	return m_hSelf;
}
LRESULT CNppCombox::runCtrlProc(UINT uMsg, WPARAM wParam, LPARAM lParam, OUT bool & bDone)
{
	bDone = false;
	dbg_log("bad done");
	switch( uMsg )
	{
		case WM_KEYUP:
		{
			dbg_log(_T("uMsg = 0x%04X"), uMsg);
			// 13 ENTER
			if (wParam == 13)
			{
				LPTSTR	pszText	= (LPTSTR)new TCHAR[MAX_PATH];

				getText(pszText);
				addText(pszText);
				::SendMessage(::GetParent(getHSelf()), EXM_CHANGECOMBO, 0, 0);
				delete [] pszText;
				bDone = true;
				return TRUE;
			}
//			dbg_log(_T("combo hwnd = 0x%08X"), getHSelf());
//			::SendMessage(::GetParent(getHSelf()), uMsg, wParam, lParam);
			bDone = true;
			break;
		}
		case WM_DESTROY:
		{
			m_comboItems.clear();
			break;
		}
		default :
			bDone = false;
			break;
	}
	return bDone;
}
LRESULT CNppCombox::handleMessage(struct NppMsgParams & msgParams)
{
/*
	switch( msgParams.uMsg )
	{
		case WM_KEYUP:
		{
			// 13 ENTER
			if (msgParams.wParam == 13)
			{
				LPTSTR	pszText	= (LPTSTR)new TCHAR[MAX_PATH];
				dbg_log(_T("uMsg = %04x, ctrlID = %d"), msgParams.uMsg, msgParams.ctrlID);
				getText(pszText);
				addText(pszText);
				::SendMessage(::GetParent(getHSelf()), EXM_CHANGECOMBO, 0, 0);
				delete [] pszText;
				return TRUE;
			}
//			dbg_log(_T("combo hwnd = 0x%08X"), getHSelf());
//			::SendMessage(::GetParent(getHSelf()), uMsg, wParam, lParam);
			break;
		}
		case WM_DESTROY:
		{
			m_comboItems.clear();
			break;
		}
		default :
			break;
	}*/
	return CNppCtrlWnd::handleMessage(msgParams);
}

void CNppCombox::addText(LPCTSTR pszText)
{
	/* find item */
	INT		count		= m_comboItems.size();
	INT		i			= 0;
	INT		hasFoundOn	= -1;

	for (; i < count; i++)
	{
		if (_tcscmp(pszText, m_comboItems[i].c_str()) == 0)
		{
			hasFoundOn = count - i - 1;
		}
	}

	/* item missed create new and select it correct */
	if (hasFoundOn == -1)
	{
		m_comboItems.push_back(pszText);

		::SendMessage(getHSelf(), CB_RESETCONTENT, 0, 0);
		for (i = count; i >= 0 ; --i)
		{
			::SendMessage( getHSelf(), CB_ADDSTRING, MAX_PATH, (LPARAM)m_comboItems[i].c_str());
		}
	}
	selectComboText(pszText);
}


void CNppCombox::setText(LPCTSTR pszText, UINT size)
{
	::SendMessage(getHSelf(), WM_SETTEXT, size, (LPARAM)pszText);
}

void CNppCombox::getText(LPTSTR pszText, UINT size)
{
	::SendMessage(getHSelf(), WM_GETTEXT, size, (LPARAM)pszText);
}

bool CNppCombox::getSelText(LPTSTR pszText)
{
	INT		curSel = ::SendMessage(getHSelf(), CB_GETCURSEL, 0, 0);

	if (curSel != CB_ERR)
	{
		if (MAX_PATH > ::SendMessage(getHSelf(), CB_GETLBTEXTLEN, curSel, 0))
		{
			::SendMessage(getHSelf(), CB_GETLBTEXT, curSel, (LPARAM)pszText);
			return true;
		}
	}
	return false;
}

void CNppCombox::selectComboText(LPCTSTR pszText)
{
	LRESULT			lResult	= -1;

	lResult = ::SendMessage(getHSelf(), CB_FINDSTRINGEXACT, -1, (LPARAM)pszText);
	::SendMessage(getHSelf(), CB_SETCURSEL, lResult, 0);
}

void CNppCombox::setComboList(vector<tstring> vStrList)
{
	size_t	iCnt	= vStrList.size();

	::SendMessage(getHSelf(), CB_RESETCONTENT, 0, 0);
	for (size_t i = 0; i < iCnt; i++)
	{
		addText((LPCTSTR)vStrList[i].c_str());
	}
}

void CNppCombox::getComboList(vector<tstring> & vStrList)
{
	LPTSTR	pszTemp	= (LPTSTR)new TCHAR[MAX_PATH];
	size_t	iCnt	= ::SendMessage(getHSelf(), CB_GETCOUNT, 0, 0);

	vStrList.clear();

	for (size_t i = 0; i < iCnt; i++)
	{
		if (MAX_PATH > ::SendMessage(getHSelf(), CB_GETLBTEXTLEN, i, 0))
		{
			::SendMessage(getHSelf(), CB_GETLBTEXT, i, (LPARAM)pszTemp);
			vStrList.push_back(pszTemp);
		}
	}
	delete [] pszTemp;
}


