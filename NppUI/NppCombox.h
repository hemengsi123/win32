#ifndef COMBOX_DEFINE_H
#define COMBOX_DEFINE_H


#ifndef CB_GETCOMBOBOXINFO
#define	CB_GETCOMBOBOXINFO	0x0164
#endif

#if(WINVER <= 0x0400)
struct COMBOBOXINFO 
{
    int cbSize;
    RECT rcItem;
    RECT rcButton;
    DWORD stateButton;
    HWND hwndCombo;
    HWND hwndItem;
    HWND hwndList; 
};
#endif 

class CNppCombox: public CNppCtrlWnd
{
public :
	CNppCombox();
    ~CNppCombox();
	LPCTSTR getWndClassName()const;
	void init(HINSTANCE hInst, HWND hParent, UINT iCtrlIDs);
	HWND create(DWORD dwStyle =  CBS_DROPDOWN, DWORD dwExStyle = 0, LPCTSTR lpszCaption = NULL);
	void addText(LPCTSTR pszText);
	void setText(LPCTSTR pszText, UINT size = MAX_PATH);
	void getText(LPTSTR pszText, UINT size = MAX_PATH);
	bool getSelText(LPTSTR pszText);

	void setComboList(std::vector<tstring> vStrList);
	void getComboList(std::vector<tstring> & vStrList);
	
	virtual LRESULT runCtrlProc(UINT uMsg, WPARAM wParam, LPARAM lParam, OUT bool & bDone);
private:
	void selectComboText(LPCTSTR pszText);

private :
	tstring					m_currData;
	std::vector<tstring>	m_comboItems;
};

#endif // COMBOORGI_DEFINE_H
