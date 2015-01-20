
#include "NppLib.h"
#include "NppListView.h"

void CNppListView::init(HINSTANCE hInst, HWND hParent, HWND hSelf, DWORD dwStyle)
{
	CNppWnd::init(hInst, hParent);
    if( hSelf == NULL )
    {
    	int listViewStyles = LVS_REPORT | LVS_NOSORTHEADER\
    						| LVS_SINGLESEL | LVS_AUTOARRANGE\
    						| LVS_SHAREIMAGELISTS | LVS_SHOWSELALWAYS;

    	m_hSelf = ::CreateWindow(WC_LISTVIEW, 
                                    _T(""), 
                                    WS_CHILD | listViewStyles,
                                    0,
                                    0, 
                                    0,
                                    0,
                                    hParent, 
                                    (HMENU) NULL, 
                                    hInst,
                                    NULL);
		if (!m_hSelf)
		{
			throw int(SYSERRm_hSelf_NULL);
		}
		_bIsCreate = true;
      
      }
      else
      {
      	m_hSelf = hSelf;
      }
	CNppWnd::setWndProc();
	setHeaderWndProc(headerWndProcWrap);
	

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
	int index = (int)::SendMessage(m_hSelf, LVM_INSERTCOLUMN, (WPARAM)(int)_iColumnCount, (LPARAM)(const LV_COLUMN *)&lvColumn);
//	int index = ListView_InsertColumn(m_hSelf, 1, &lvColumn);
	if( index != -1)
	{
		++_iColumnCount;
	}
	return index;
}
bool CNppListView::setColumn(int iCol, LPTSTR pszText, int cWidth, int fmt)
{
	LVCOLUMN lvColumn;
	lvColumn.mask    = (LVCF_TEXT | LVCF_WIDTH | LVCF_FMT);
	lvColumn.fmt     = fmt;
	lvColumn.cx      = cWidth;
	lvColumn.pszText = pszText;
	lvColumn.cchTextMax = _tcslen(pszText);
	return static_cast<bool>(ListView_SetColumn(m_hSelf, iCol, &lvColumn));
}
bool CNppListView::setScroll(int cWidth, int cHight)
{

	return static_cast<bool>(ListView_Scroll(m_hSelf, cWidth, cHight));
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
bool CNppListView::setColumnWidth(int iCol, int cWidth)const
{
	return static_cast<bool>(ListView_SetColumnWidth(m_hSelf, iCol, cWidth));
}
int CNppListView::getheaderHight()const
{
	RECT rc = {0};
	Header_GetItemRect(_hHeader, 0, &rc);
	return rc.bottom;
}
bool CNppListView::isSelect(int iItem) const
{
	return (ListView_GetItemState(m_hSelf, iItem, LVIS_SELECTED) == LVIS_SELECTED);
}
void CNppListView::setFocusItem(int iItem)
{
	/* at first unselect all */
	for (int i = 0; i < _iItemIndx+1; i++)
		ListView_SetItemState(m_hSelf, iItem, 0, 0xFF);
	// to select the iItem
	ListView_SetItemState(m_hSelf, iItem, LVIS_SELECTED|LVIS_FOCUSED, 0xFF);
	ListView_EnsureVisible(m_hSelf, iItem, TRUE);
	ListView_SetSelectionMark(m_hSelf, iItem);
}
void CNppListView::hiddenHeader()
{
	CNppWnd::setWndStyle(getWndStyle() | LVS_NOCOLUMNHEADER);
}
int CNppListView::addItem(LPCTSTR  lpszText, int iItem)
{
	LVITEM item   = {0};
	
	item.mask     = LVIF_TEXT;
	item.iItem    = iItem;
	item.iSubItem = 0;
	item.pszText  = const_cast<LPTSTR>(lpszText);
	_iItemIndx    = iItem;
	
	return (int)::SendMessage(m_hSelf, LVM_INSERTITEM, 0, (LPARAM)&item);
}
bool CNppListView::addSubItem(LPCTSTR lpszText, int iCol, int iItem)
{
	if( iItem == -1 )
		iItem = _iItemIndx;
	
	LVITEM item   = {0};
	item.mask     = LVIF_TEXT;
	item.iItem    = iItem;
	item.iSubItem = iCol;
	item.pszText  = const_cast<LPTSTR>(lpszText);

	return (bool)::SendMessage(m_hSelf, LVM_SETITEM, 0, (LPARAM)&item);
}

bool CNppListView::getItemText(int iItem, int iCol, LPTSTR lpszText, int cMaxLen)
{
	LVITEM item = {0};
	
	item.mask     = LVIF_TEXT;
	item.iItem    = iItem;
	item.iSubItem = iCol;
	item.pszText  = lpszText;
	item.cchTextMax = cMaxLen;
	return (bool)::SendMessage(m_hSelf, LVM_GETITEMTEXT, (WPARAM)iItem, (LPARAM)&item);
}
bool CNppListView::setItemText(LPTSTR lpszText, int iItem, int iCol)
{
	LVITEM item = {0};
	item.iSubItem = iCol;
	item.pszText  = lpszText;
//	ListView_SetItemText(m_hSelf, iItem, iCol, lpszText);
	return (bool)::SendMessage(m_hSelf, LVM_SETITEMTEXT, (WPARAM)iItem, (LPARAM)&item);
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
void CNppListView::destroy()
{
	if(_bIsCreate && m_hSelf )
	{
		::DestroyWindow(m_hSelf);
		m_hSelf = NULL;
	}
}

LRESULT CNppListView::runListProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	
	return runWndProc(hwnd, Message, wParam, lParam);
}
LRESULT CNppListView::runHeaderProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	
	return ::CallWindowProc(_sysHeaderWndProc, hwnd, Message, wParam, lParam);
}
WNDPROC CNppListView::setHeaderWndProc(WNDPROC headerProc)
{
	_hHeader = ListView_GetHeader(m_hSelf);
	::SetWindowLongPtr(_hHeader, GWLP_USERDATA, LONG_PTR(this));
	_sysHeaderWndProc = reinterpret_cast<WNDPROC>(::SetWindowLongPtr(_hHeader, GWL_WNDPROC, reinterpret_cast<LONG>(headerProc)));
	return _sysHeaderWndProc;
}
