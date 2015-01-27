
#ifndef LISTVIEW_H
#define LISTVIEW_H

class NPPLIB_API CNppListView : public CNppCtrlWnd
{
public:
	typedef enum {
		ColTextLeft   = LVCFMT_LEFT, // column
		ColTextRight  = LVCFMT_RIGHT,
		ColTextCenter = LVCFMT_CENTER,
	}eColumnAlign;
public:
	CNppListView() : m_iColumnCount(0){};
	virtual ~CNppListView() {};
	virtual void destroy();
	virtual LPCTSTR getWndClassName()const;
	void     init(HINSTANCE hInst, HWND hParent, UINT iCtrlIDs);
	HWND     create(DWORD dwStyle = 0, DWORD dwExStyle = 0);
	HWND     create(DWORD dwStyle /*=  LVS_REPORT*/, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, DWORD dwExStyle = 0);
	LRESULT  runListProc(UINT Message, WPARAM wParam, LPARAM lParam, BOOL & bDone);
	int   addColumn(LPTSTR pszText, int cWidth, int fmt = LVCFMT_CENTER);
	BOOL  setColumn(int iCol, LPTSTR pszText, int cWidth, int fmt = LVCFMT_CENTER);
	BOOL  setScroll(int cWidth, int cHight);
	void  setExtStyle(DWORD dwExStyle);
	DWORD getExtStyle()const;
	int   addItem();
	BOOL  setColumnWidth(int iCol, int cWidth)const;
	int   getColumnWidth(int iCol)const;
	int   getheaderHight()const;
	BOOL  isSelect(int iItem)const;
	void  setFocusItem(int iItem);
	void  hiddenHeader();
	/*@retrn: if successful return index, or return -1*/
	int   addItem(LPCTSTR lpszText, int iItem, int iIcon = -1);
	/*@retrn: if successful return true, or false*/
	BOOL  addSubItem(LPCTSTR lpszText, int iCol, int iItem = -1);
	BOOL  getItemText(int iItem, int iCol, LPTSTR lpszText, int cMaxLen);
	BOOL  setItemText(LPTSTR lpszText, int iItem, int iCol);
	BOOL  clearItem();
	BOOL  delItem(int iItem);
	BOOL  delColumn(int iCol);
	HIMAGELIST setItemImgList(HIMAGELIST himl, int iImgLstType = LVSIL_SMALL);
	HIMAGELIST getItemImgList(int iImgLstType = LVSIL_SMALL)const;
	HIMAGELIST setHeaderImgList(HIMAGELIST himl);
	HIMAGELIST getHeaderImgList()const;
//	void setValues(int codepage = 0);
//	void resetValues(int codepage);

//	generic_string getAscii(unsigned char value);

protected:
	int m_iColumnCount;
	int m_iItemIndx;
	HWND m_hHeader;
	WNDPROC m_sysHeaderWndProc;
protected:
	WNDPROC setHeaderWndProc(WNDPROC headerProc = headerWndProcWrap);
	static LRESULT CALLBACK headerWndProcWrap(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT runHeaderProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	LRESULT runCtrlProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bDone);
};


#endif // LISTVIEW_H
