
#include "NppLib.h"
#include "NppListView.h"


void CNppListView::init(HINSTANCE hInst, HWND hParent, HWND hSelf)
{
	CNppWnd::init(hInst, hParent);
    if( hSelf == NULL )
    {
    	int listViewStyles = LVS_REPORT | LVS_NOSORTHEADER\
    						| LVS_SINGLESEL | LVS_AUTOARRANGE\
    						| LVS_SHAREIMAGELISTS | LVS_SHOWSELALWAYS;

    	_hSelf = ::CreateWindow(WC_LISTVIEW, 
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
		if (!_hSelf)
		{
			throw int(SYSERR_HSELF_NULL);
		}
		_bIsCreate = true;
      
      }
      else
      {
      	_hSelf = hSelf;
      }
	CNppWnd::setWndProc();

	DWORD exStyle = ListView_GetExtendedListViewStyle(_hSelf);
	exStyle |= LVS_EX_FULLROWSELECT | LVS_EX_BORDERSELECT ;
	ListView_SetExtendedListViewStyle(_hSelf, exStyle);

}

int CNppListView::addColumn(LPTSTR pszText, int cx, UINT mask)
{
	LVCOLUMN lvColumn;
	
	lvColumn.mask    = mask;
	lvColumn.cx      = cx;
	lvColumn.pszText = pszText;
//	::ListView_InsertColumn(_hSelf, 2, &lvColumn);
	int index = (int)::SendMessage(_hSelf, LVM_INSERTCOLUMN, (WPARAM)(int)_iColumnIndx, (LPARAM)(const LV_COLUMN *)&lvColumn);
//	int index = ListView_InsertColumn(_hSelf, 1, &lvColumn);
	if( index != -1)
	{
		++_iColumnIndx;
	}
	return index;
}
/*
void CNppListView::resetValues(int codepage)
{
	if (codepage == -1)
		codepage = 0;

	if (_codepage == codepage)
		return;

	ListView_DeleteAllItems(_hSelf);
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
		ListView_InsertItem(_hSelf, &item);

		ListView_SetItemText(_hSelf, i, 1, (LPTSTR)hex);

		generic_string s = getAscii((unsigned char)i);
		ListView_SetItemText(_hSelf, i, 2, (LPTSTR)s.c_str());
	}
}
*/
void CNppListView::destroy()
{
	if(_bIsCreate && _hSelf )
	{
		::DestroyWindow(_hSelf);
		_hSelf = NULL;
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