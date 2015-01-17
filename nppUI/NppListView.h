
#ifndef LISTVIEW_H
#define LISTVIEW_H

#include "NppWnd.h"
#include "NppImageList.h"

class CNppListView : public CNppWnd
{
public:
	CNppListView() : _bIsCreate(false), _iColumnIndx(0){};
	virtual ~CNppListView() {};
	virtual void init(HINSTANCE hInst, HWND hParent, HWND hSelf=NULL);
	virtual void destroy();
	int addColumn(LPTSTR pszText, int cx, UINT mask = (LVCF_TEXT | LVCF_WIDTH));
//	void setValues(int codepage = 0);
//	void resetValues(int codepage);

//	generic_string getAscii(unsigned char value);

protected:
    bool _bIsCreate;
	int _iColumnIndx;
	CNppImageList _hImglst;
	HWND _hHeader;
	WNDPROC _sysHeaderWndProc;
protected:
	LRESULT runListProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	
	LRESULT runHeaderProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK headerWndProcWrap(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return (((CNppListView *)(::GetWindowLongPtr(hwnd, GWL_USERDATA)))->runHeaderProc(hwnd, uMsg, wParam, lParam));
	}
//	LRESULT runWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
};


#endif // LISTVIEW_H
