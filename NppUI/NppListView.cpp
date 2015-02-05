
#include "NppLib.h"
#include "NppListView.h"

LRESULT CALLBACK CNppListView::headerWndProcWrap(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return (((CNppListView *)(::GetWindowLongPtr(hwnd, GWL_USERDATA)))->runHeaderProc(hwnd, uMsg, wParam, lParam));
}
LPCTSTR CNppListView::getWndClassName()const
{
	return _T("SysListView32");
}
void CNppListView::init(HINSTANCE hInst, HWND hParent, UINT iCtrlID, LPCTSTR sCtrlName)
{
	CNppCtrlWnd::init(hInst, hParent, iCtrlID, sCtrlName);
}
HWND CNppListView::create(DWORD dwStyle, DWORD dwExStyle)
{
	if( CNppCtrlWnd::create(dwStyle, 0, NULL) )
	{
		setWndProc();
		setHeaderWndProc(headerWndProcWrap);
		if(dwExStyle > 0)
		{
			setExtStyle(dwExStyle);
		}
	}
	return m_hSelf;//getHSelf();
}
HWND CNppListView::create(DWORD dwStyle, int x, int y, int cx, int cy, DWORD dwExStyle)
{
	if( CNppCtrlWnd::create(NULL, dwStyle, x, y, cx, cy, 0) )
	{
		setWndProc();
		setHeaderWndProc(headerWndProcWrap);
		if(dwExStyle > 0)
		{
			setExtStyle(dwExStyle);
		}
	}
	return m_hSelf;
}
int CNppListView::addColumn(LPTSTR pszText, int cWidth, int fmt)
{
	LVCOLUMN lvColumn;
	
	lvColumn.mask    = (LVCF_TEXT | LVCF_WIDTH | LVCF_FMT | LVCF_SUBITEM);
	lvColumn.cx      = cWidth;
	lvColumn.fmt     = fmt;
	lvColumn.pszText = pszText;
	lvColumn.cchTextMax = _tcslen(pszText);
//	::ListView_InsertColumn(m_hSelf, 2, &lvColumn);
	int index = (int)::SendMessage(m_hSelf, LVM_INSERTCOLUMN, (WPARAM)(int)m_iColumnCount, (LPARAM)(const LV_COLUMN *)&lvColumn);
//	int index = ListView_InsertColumn(m_hSelf, 1, &lvColumn);
	if( index != -1)
	{
		++m_iColumnCount;
	}
	return index;
}
BOOL CNppListView::setColumn(int iCol, LPTSTR pszText, int cWidth, int fmt)
{
	LVCOLUMN lvColumn;
	lvColumn.mask    = (LVCF_TEXT | LVCF_WIDTH | LVCF_FMT);
	lvColumn.fmt     = fmt;
	lvColumn.cx      = cWidth;
	lvColumn.pszText = pszText;
	lvColumn.cchTextMax = _tcslen(pszText);
	return static_cast<BOOL>(ListView_SetColumn(m_hSelf, iCol, &lvColumn));
}
BOOL CNppListView::setScroll(int cWidth, int cHight)
{

	return static_cast<BOOL>(ListView_Scroll(m_hSelf, cWidth, cHight));
}
void CNppListView::setExtStyle(DWORD dwExStyle)
{
	ListView_SetExtendedListViewStyle(m_hSelf, dwExStyle);
}
DWORD CNppListView::getExtStyle()const
{
	return ListView_GetExtendedListViewStyle(m_hSelf);
}
int CNppListView::getColumnWidth(int iCol)const
{
	return ListView_GetColumnWidth(m_hSelf, iCol);
}
BOOL CNppListView::setColumnWidth(int iCol, int cWidth)const
{
	return static_cast<BOOL>(ListView_SetColumnWidth(m_hSelf, iCol, cWidth));
}
int CNppListView::getheaderHight()const
{
	RECT rc = {0};
	Header_GetItemRect(m_hHeader, 0, &rc);
	return rc.bottom;
}
BOOL CNppListView::isSelect(int iItem) const
{
	return (ListView_GetItemState(m_hSelf, iItem, LVIS_SELECTED) == LVIS_SELECTED);
}
void CNppListView::setFocusItem(int iItem)
{
	/* at first unselect all */
	for (int i = 0; i < m_iItemIndx+1; i++)
		ListView_SetItemState(m_hSelf, iItem, 0, 0xFF);
	// to select the iItem
	ListView_SetItemState(m_hSelf, iItem, LVIS_SELECTED|LVIS_FOCUSED, 0xFF);
	ListView_EnsureVisible(m_hSelf, iItem, TRUE);
	ListView_SetSelectionMark(m_hSelf, iItem);
}
void CNppListView::hiddenHeader()
{
	CNppBaseWnd::setWndStyle(getWndStyle() | LVS_NOCOLUMNHEADER);
}
int CNppListView::addItem(LPCTSTR  lpszText, int iItem, int iIcon)
{
	LVITEM item   = {0};
	
	item.mask     = LVIF_TEXT;
	item.iItem    = iItem;
	item.iSubItem = 0;
	item.pszText  = const_cast<LPTSTR>(lpszText);
	m_iItemIndx    = iItem;
	if( iIcon >= 0 )
	{
		item.mask  |= LVIF_IMAGE;
		item.iImage = iIcon;
	}
	return (int)::SendMessage(m_hSelf, LVM_INSERTITEM, 0, (LPARAM)&item);
}
BOOL CNppListView::addSubItem(LPCTSTR lpszText, int iCol, int iItem)
{
	if( iItem == -1 )
		iItem = m_iItemIndx;
	
	LVITEM item   = {0};
	item.mask     = LVIF_TEXT;
	item.iItem    = iItem;
	item.iSubItem = iCol;
	item.pszText  = const_cast<LPTSTR>(lpszText);

	return (BOOL)::SendMessage(m_hSelf, LVM_SETITEM, 0, (LPARAM)&item);
}

BOOL CNppListView::getItemText(int iItem, int iCol, LPTSTR lpszText, int cMaxLen)
{
	LVITEM item = {0};
	
	item.mask     = LVIF_TEXT;
	item.iItem    = iItem;
	item.iSubItem = iCol;
	item.pszText  = lpszText;
	item.cchTextMax = cMaxLen;
	return (BOOL)::SendMessage(m_hSelf, LVM_GETITEMTEXT, (WPARAM)iItem, (LPARAM)&item);
}
BOOL CNppListView::setItemText(LPTSTR lpszText, int iItem, int iCol)
{
	LVITEM item = {0};
	item.iSubItem = iCol;
	item.pszText  = lpszText;
//	ListView_SetItemText(m_hSelf, iItem, iCol, lpszText);
	return (BOOL)::SendMessage(m_hSelf, LVM_SETITEMTEXT, (WPARAM)iItem, (LPARAM)&item);
}
int CNppListView::getHotItem()
{
	return ListView_GetHotItem(m_hSelf);
}
int CNppListView::getMarkItem()
{
	return ListView_GetSelectionMark(m_hSelf);
}
BOOL CNppListView::clearItem()
{
	return ListView_DeleteAllItems(m_hSelf);
}
BOOL CNppListView::delItem(int iItem)
{
	return ListView_DeleteItem(m_hSelf, iItem);
}
BOOL CNppListView::delColumn(int iCol)
{
	return ListView_DeleteColumn(m_hSelf, iCol);
}
/*
void CNppListView::resetValues(int codepage)
{
	if (codepage == -1)
		codepage = 0;

	if (_codepage == codepage)
		return;

	ListView_DeleteAllItems(m_hSelf);
	setValues(codepage);
}

generic_string CNppListView::getAscii(unsigned char value)
{
	switch (value)
	{
		case 0:
			return TEXT("NULL");
		case 1:
			return TEXT("SOH");
		case 2:
			return TEXT("STX");
		case 3:
			return TEXT("ETX");
		case 4:
			return TEXT("EOT");
		case 5:
			return TEXT("ENQ");
		case 6:
			return TEXT("ACK");
		case 7:
			return TEXT("BEL");
		case 8:
			return TEXT("BS");
		case 9:
			return TEXT("TAB");
		case 10:
			return TEXT("LF");
		case 11:
			return TEXT("VT");
		case 12:
			return TEXT("FF");
		case 13:
			return TEXT("CR");
		case 14:
			return TEXT("SO");
		case 15:
			return TEXT("SI");
		case 16:
			return TEXT("DLE");
		case 17:
			return TEXT("DC1");
		case 18:
			return TEXT("DC2");
		case 19:
			return TEXT("DC3");
		case 20:
			return TEXT("DC4");
		case 21:
			return TEXT("NAK");
		case 22:
			return TEXT("SYN");
		case 23:
			return TEXT("ETB");
		case 24:
			return TEXT("CAN");
		case 25:
			return TEXT("EM");
		case 26:
			return TEXT("SUB");
		case 27:
			return TEXT("ESC");
		case 28:
			return TEXT("FS");
		case 29:
			return TEXT("GS");
		case 30:
			return TEXT("RS");
		case 31:
			return TEXT("US");
		case 32:
			return TEXT("Space");
		case 127:
			return TEXT("DEL");
		default:
		{
			TCHAR charStr[10];
			char ascii[2];
			ascii[0] = value;
			ascii[1] = '\0';
			MultiByteToWideChar(_codepage, 0, ascii, -1, charStr, sizeof(charStr));
			return charStr;
		}

	}
}

void CNppListView::setValues(int codepage)
{
	_codepage = codepage;
	
	for (int i = 0 ; i < 256 ; ++i)
	{
		LVITEM item;
		item.mask = LVIF_TEXT;
		TCHAR dec[8];
		TCHAR hex[8];
		generic_sprintf(dec, _T("%d"), i);
		generic_sprintf(hex, _T("%02X"), i);
		item.pszText = dec;
		item.iItem = i;
		item.iSubItem = 0;
		ListView_InsertItem(m_hSelf, &item);

		ListView_SetItemText(m_hSelf, i, 1, (LPTSTR)hex);

		generic_string s = getAscii((unsigned char)i);
		ListView_SetItemText(m_hSelf, i, 2, (LPTSTR)s.c_str());
	}
}
*/

LRESULT CNppListView::runListProc(UINT Message, WPARAM wParam, LPARAM lParam, BOOL & bDone)
{
	bDone = false;
	
	return 0;
}

LRESULT CNppListView::runHeaderProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_GETDLGCODE:
		{
			return DLGC_WANTALLKEYS | ::CallWindowProc(m_sysWndProc, hwnd, uMsg, wParam, lParam);
		}
	}
	return ::CallWindowProc(m_sysHeaderWndProc, hwnd, uMsg, wParam, lParam);
}
WNDPROC CNppListView::setHeaderWndProc(WNDPROC headerProc)
{
	m_hHeader = ListView_GetHeader(m_hSelf);
	::SetWindowLongPtr(m_hHeader, GWLP_USERDATA, LONG_PTR(this));
	m_sysHeaderWndProc = reinterpret_cast<WNDPROC>(::SetWindowLongPtr(m_hHeader, GWL_WNDPROC, reinterpret_cast<LONG>(headerProc)));
	return m_sysHeaderWndProc;
}
HIMAGELIST CNppListView::setItemImgList(HIMAGELIST himl, int iImgLstType)
{
	return ListView_SetImageList(m_hSelf, himl, iImgLstType);
}
HIMAGELIST CNppListView::getItemImgList(int iImgLstType)const
{
	return ListView_GetImageList(m_hSelf, iImgLstType);
}
HIMAGELIST CNppListView::setHeaderImgList(HIMAGELIST himl)
{
	return Header_SetImageList(m_hHeader, himl);
}
HIMAGELIST CNppListView::getHeaderImgList()const
{
	return Header_GetImageList(m_hHeader);
}
BOOL  CNppListView::getSubItemRect(int iItem, int iSubItem, LPRECT lpRect, int tCode)
{
	return ListView_GetSubItemRect(m_hSelf, iItem, iSubItem, tCode, lpRect);
}
int CNppListView::getStringWidth(LPCTSTR lpszItemText)
{
	return ListView_GetStringWidth(m_hSelf, lpszItemText);
}