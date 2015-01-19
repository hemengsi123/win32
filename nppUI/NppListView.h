
#ifndef LISTVIEW_H
#define LISTVIEW_H

#include "NppWnd.h"
#include "NppImageList.h"

class CNppListView : public CNppWnd
{
public:
	CNppListView() : _bIsCreate(false), _iColumnCount(0){};
	virtual ~CNppListView() {};
	virtual void init(HINSTANCE hInst, HWND hParent, HWND hSelf=NULL, DWORD dwStyle = LVS_REPORT);
	virtual void destroy();
	int addColumn(LPTSTR pszText, int cWidth, int fmt = LVCFMT_CENTER);
	bool setColumn(int iCol, LPTSTR pszText, int cWidth, int fmt = LVCFMT_CENTER);
	bool setScroll(int cWidth, int cHight);
	void setExtStyle(DWORD dwExStyle);
	DWORD getExtStyle()const;
	WNDPROC setHeaderWndProc(WNDPROC headerProc = headerWndProcWrap);
	int addItem();
	bool setColumnWidth(int iCol, int cWidth)const;
	int getColumnWidth(int iCol)const;
	int getheaderHight()const;
	bool isSelect(int iItem)const;
	void setFocusItem(int iItem);
	void hiddenHeader();
	/*@retrn: if successful return index, or return -1*/
	int addItem(LPCTSTR lpszText, int iItem/*, int iCol*/);
	/*@retrn: if successful return true, or false*/
	bool addSubItem(LPCTSTR lpszText, int iCol, int iItem = -1);
	bool getItemText(int iItem, int iCol, LPTSTR lpszText, int cMaxLen);
	bool setItemText(LPTSTR lpszText, int iItem, int iCol);
	
//	void setValues(int codepage = 0);
//	void resetValues(int codepage);

//	generic_string getAscii(unsigned char value);

protected:
    bool _bIsCreate;
	int _iColumnCount;
	int _iItemIndx;
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
