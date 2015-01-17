
#ifndef LISTVIEW_H
#define LISTVIEW_H

#include "window.h"

class ListView : public Window
{
public:
	ListView() : _bIsCreate(false){};

	virtual ~ListView() {};
	virtual void init(HINSTANCE hInst, HWND hParent, HWND hSelf=NULL);
	virtual void destroy();

	void setValues(int codepage = 0);
	void resetValues(int codepage);

	generic_string getAscii(unsigned char value);

protected:
    bool _bIsCreate;
	int _codepage;
	WNDPROC _defaultProc;
	LRESULT runProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK staticProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		return (((ListView *)(::GetWindowLongPtr(hwnd, GWL_USERDATA)))->runProc(hwnd, Message, wParam, lParam));
	};
};


#endif // LISTVIEW_H
