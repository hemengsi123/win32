
#ifndef LISTVIEW_H
#define LISTVIEW_H

class NPPLIB_API CNppListView : public CNppCtrlWnd
{
public:
	CNppListView() : m_iColumnCount(0){};
	virtual ~CNppListView() {};
	virtual void destroy();
	virtual LPCTSTR getWndClassName()const;
	void     init(HINSTANCE hInst, HWND hParent, UINT iCtrlIDs);
	HWND     create(DWORD dwStyle = 0, DWORD dwExStyle = 0);
	HWND     create(DWORD dwStyle /*=  LVS_REPORT*/, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, DWORD dwExStyle = 0);
	LRESULT  runListProc(UINT Message, WPARAM wParam, LPARAM lParam, bool & bDone);
	int   addColumn(LPTSTR pszText, int cWidth, int fmt = LVCFMT_CENTER);
	bool  setColumn(int iCol, LPTSTR pszText, int cWidth, int fmt = LVCFMT_CENTER);
	bool  setScroll(int cWidth, int cHight);
	void  setExtStyle(DWORD dwExStyle);
	DWORD getExtStyle()const;
	int   addItem();
	bool  setColumnWidth(int iCol, int cWidth)const;
	int   getColumnWidth(int iCol)const;
	int   getheaderHight()const;
	bool  isSelect(int iItem)const;
	void  setFocusItem(int iItem);
	void  hiddenHeader();
	/*@retrn: if successful return index, or return -1*/
	int   addItem(LPCTSTR lpszText, int iItem, int iIcon = -1);
	/*@retrn: if successful return true, or false*/
	bool  addSubItem(LPCTSTR lpszText, int iCol, int iItem = -1);
	bool  getItemText(int iItem, int iCol, LPTSTR lpszText, int cMaxLen);
	bool  setItemText(LPTSTR lpszText, int iItem, int iCol);
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
	LRESULT runCtrlProc(UINT uMsg, WPARAM wParam, LPARAM lParam, bool & bDone);
};


#endif // LISTVIEW_H
