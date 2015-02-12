#ifndef NPPFONT_H
#define NPPFONT_H

class CNppFont
{
public:
	CNppFont();
	~CNppFont();
	void destroy();
	static HFONT getFont(HWND hctrl);
	// @brief: dialog ²»Ö§³Ö
	static void setFont(HWND hctrl, HFONT hfont, BOOL redraw = TRUE);
	static void setFont(HWND hTo, HWND hFrom);
	static void setFont(HWND hctrl, LPLOGFONT lpLogFont, BOOL redraw = TRUE);
	static LOGFONT& getLogFont(HWND hctrl);
private:
	LPLOGFONT m_lpLogFont;
	HFONT     m_hFont;
};
#endif
